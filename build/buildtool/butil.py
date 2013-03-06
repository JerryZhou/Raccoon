#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""butil = build utilitys
优先使用butil提供的函数，以便获得统一的日志输出
"""

import os
import distutils.dir_util
import logging
import glob
import shutil
import pprint
import subprocess
import collections
import hashlib
import sys
import codecs
import re
import StringIO

from buildexp import BuildException

def delete_dir(dir_path):
    u"""删除目录 dir，及其子目录和文件
    如果目录不存在，则忽略
    """
    if os.path.exists(dir_path):
        distutils.dir_util.remove_tree(dir_path)
        logger = logging.getLogger(__name__)
        logger.info("delete dir [%s]" % dir_path)

def delete_file(file_path):
    u"""删除文件 file
    如果文件不存在，则忽略
    """
    if os.path.exists(file_path):
        os.remove(file_path)
        logger = logging.getLogger(__name__)
        logger.info("delete file [%s]" % file_path)

def mkdir(dir_path):
    _mkdir(dir_path)

def _mkdir(dir_path):
    u"""新建目录dir
    如果父目录还未存在，则自动建立
    """
    dir_created = distutils.dir_util.mkpath(dir_path)
    if len(dir_created) != 0:
        logger = logging.getLogger(__name__)
        logger.info("mkdir [%s]" % dir_path)

def copy_file(src, dst):
    u"""将单个文件复制到dst目录下，或复制为名为dst的文件"""
    logger = logging.getLogger(__name__)
    shutil.copy(src, dst)
    if os.path.isdir(dst):
        logger.info("copy file [%s] to dir [%s]" % (src, dst))
    else:
        logger.info("copy file [%s] to file [%s]" % (src, dst))

def copy_file_to_dir(to_dir, base_dir, includes):
    u"""将base_dir下符合includes中所指定的pattern的文件，复制到to_dir
    includes是一个list，如：["*.dll", "*.png"]
    includes中不允许包含目录
    """
    logger = logging.getLogger(__name__)
    file_to_copy = []
    for include_name in includes:
        if include_name.startswith("/"):
            include_name = include_name[1:]
        if include_name.find("/") != -1:
            raise BuildException("include name [%s] should not contains dir"
                % include_name)
        for f in glob.glob(os.path.join(base_dir, include_name)):
            file_to_copy.append(f)

    for f in file_to_copy:
        shutil.copy(f, to_dir)

    logger.info("copy %d file(s) to dir [%s]" % (len(file_to_copy), to_dir))
    logger.debug("file to copy:\n%s" % pprint.pformat(file_to_copy))

def copy(fileset, target_dir):
    logger = logging.getLogger(__name__)
    file_to_copy = []
    dir_to_copy = []

    basedir = fileset['basedir']
    if fileset.has_key('todir'):
        target_dir = os.path.join(target_dir, fileset['todir'])

    if fileset.has_key('exclude'):
        # windows 下，glob会得到带有 \ 的路径，所以，格式化时采用了"\\"
        exclude_set = set(['%s\\%s' % (basedir, f) for f in fileset['exclude']])
    else:
        exclude_set = set()

    if len(fileset['include']) == 0:
        raise BuildException("include must not be empty")

    for include in fileset['include']:
        include_path = os.path.join(basedir, include)
        for g in glob.glob(include_path):
            if g not in exclude_set:
                if os.path.isdir(g):
                    dir_to_copy.append(g)
                    shutil.copytree(g,
                        os.path.join(target_dir, os.path.basename(g)),
                        ignore=shutil.ignore_patterns(".svn"))
                else:
                    file_to_copy.append(g)
                    shutil.copy(g, target_dir)
    logger.info("copy %d file(s) and %d dir(s) to dir [%s]", len(file_to_copy), len(dir_to_copy), target_dir)
    logger.debug("file to copy:\n%s", pprint.pformat(file_to_copy))
    logger.debug("dir to copy:\n%s", pprint.pformat(dir_to_copy))

Project = collections.namedtuple("Project", "name sln_name work_dir ignore_warning")

class Devenv(object):

    def __init__(self, build_prop):
        self._build_prop = build_prop
        self._logger = logging.getLogger(__name__)

    def __call__(self, project, action, config):
        u"""调用devenv.com对项目进行操作
        action：可以是clean、build、rebuild
        """
        if self._build_prop.share.multi_cpu_build:
            self._exec_with_multi_cpu_build(project, action, config)
        else:
            self._exec_normal(project, action, config)

    def _exec_with_multi_cpu_build(self, project, action, config):
        devenv = self._build_prop.share.build_exe

        self._logger.info("%s [%s:%s] [%s] running" %
            (os.path.split(devenv)[1], action, config, project.name))

        _mkdir("dist/compile")
        output_file_path = "dist/compile/%s-%s-%s.txt" % (project.name, action, config.replace("|", "-"))

        returncode = 0
        with open(output_file_path, "w") as f:
            returncode = subprocess.call([devenv,
                project.sln_name,
                "/" + action,
                config],
                stdout=f,
                stderr=subprocess.STDOUT,
                cwd=project.work_dir)

        self._output_compile_result(output_file_path, project)

        if returncode != 0:
            raise BuildException("%s [%s:%s] [%s] failed, returncode: %d" %
                (os.path.split(devenv)[1], action, config, project.name, returncode))
        else:
            self._logger.info("%s [%s:%s] [%s] successed" %
                (os.path.split(devenv)[1], action, config, project.name))

    def _output_compile_result(self, output_file_path, project):
        pattern_title = re.compile(r""">------
    """, re.VERBOSE)

        pattern_error = re.compile(r""":\ general\ error
    |:\ fatal\ error\ C
    |:\ error\ C
    |:\ error\ RC
    |:\ error\ LNK
    |:\ error\ PRJ
    |:\ warning\ C4715
    |LINK\ :\ fatal\ error
    """, re.VERBOSE)

        pattern_warning = re.compile(r""":\ warning\ C
    """, re.VERBOSE)

        FilterResult = collections.namedtuple("FilterResult", "title errors warnings")
        # result: key => FilterResult
        result = {}

        with codecs.open(output_file_path, "r", "GBK") as f:
            for line in f.readlines():
                if line.find(":") == -1:
                    continue

                line = line.replace("\r\n", "\n")
                if pattern_warning.search(line) != None:
                    key = line[0:line.find(">")]
                    result[key].warnings.append(line)
                    continue

                if pattern_error.search(line) != None:
                    key = line[0:line.find(">")]
                    result[key].errors.append(line)
                    continue

                if pattern_title.search(line) != None:
                    key = line[0:line.find(">")]
                    if not result.has_key(key):
                        result[key] = FilterResult(line, [], [])
                    continue

        for key, filter_result in sorted(result.items(), key=lambda x: int(x[0])):
            has_error = len(filter_result.errors) != 0
            has_warning = len(filter_result.warnings) != 0 and not project.ignore_warning
            if not has_error and not has_warning:
                # 没有error和warning的项目，不输出
                continue

            stderr_output = StringIO.StringIO()
            stderr_output.write(filter_result.title)
            if has_error:
                stderr_output.write("".join(filter_result.errors))
            if not has_error and has_warning:
                # 没有error的情况下，才输出warning
                stderr_output.write("".join(filter_result.warnings))

            sys.stderr.write(stderr_output.getvalue().encode(self._build_prop.os_encoding))
            sys.stderr.flush()

            self._logger.debug(stderr_output.getvalue())

    def _exec_normal(self, project, action, config):
        devenv = self._build_prop.share.build_exe

        returncode = subprocess.call([devenv,
            project.sln_name,
            "/" + action,
            config],
            cwd=project.work_dir)
        if returncode != 0:
            raise BuildException("%s [%s:%s] [%s] failed, returncode: %d" %
                (os.path.split(devenv)[1], action, config, project.name, returncode))
        else:
            self._logger.info("%s [%s:%s] [%s] successed" %
                (os.path.split(devenv)[1], action, config, project.name))

def exec_7z(src_dir, dst_name, working_dir, password=None):
    u"""调用7za.exe进行7z打包"""
    exe_7za = os.path.abspath("build/bin/7za.exe")

    command_list = [exe_7za,
        "a",
        dst_name,
        src_dir,
        "-mx=9",
        "-x!.svn",
        "-x!thumb.db"]

    if password != None:
        command_list.append("-p" + password)

    returncode = subprocess.call(command_list, cwd=working_dir)
    if returncode != 0:
        raise BuildException("compress 7z failed, returncode: %d"
            % returncode)

def md5sum(file_path, block_size=2**20):
    file = open(file_path, mode="rb")
    md5 = hashlib.md5()
    while True:
        data = file.read(block_size)
        if not data:
            break
        md5.update(data)
    file.close()
    result = md5.hexdigest()
    logger = logging.getLogger(__name__)
    logger.debug("[%s] md5: [%s]", file_path, result)
    return result

def make_cab(prefix, cab_path, src_path):
    u"""调用cabsdk的CABARC.exe，打包cab文件"""
    exe_cab = os.path.abspath("build/bin/CABARC.EXE")
    process = subprocess.Popen(
        [
            exe_cab,
            "-m",
            "LZX:21",
            "-r",
            "-p",
            "-P",
            prefix,
            "n",
            cab_path,
            src_path,
        ])
    process.wait()
    # cabarc 的返回值一定为0，这是很古怪的行为
    if process.returncode != 0:
        raise BuildException("compress [%s] failed, returncode: %d"
            % (cab_path, process.returncode))

    logger = logging.getLogger(__name__)
    logger.debug("compress [%s] successed", cab_path)

def signature(pfx_filepath, password, filepath, file_discription, sign_with_timestamp=False):
    logger = logging.getLogger(__name__)
    for i in range(1, 4):
        returncode = _sign_once(pfx_filepath,
            password,
            filepath,
            file_discription,
            sign_with_timestamp)

        if returncode == 0:
            if sign_with_timestamp:
                logger.info("signature [%s] successed", filepath)
            else:
                logger.info("signature [%s] successed without timestamp", filepath)
            return;
        else:
            logger.info("signature [%s] failed, try [%d], returncode: %d"
                % (filepath, i, returncode))

    raise BuildException("signature [%s] failed" % filepath)

def _sign_once(pfx_filepath, password, filepath, file_discription, sign_with_timestamp):
    command_list = [
        "build/bin/signtool.exe",
        "sign",
        "/f",
        pfx_filepath,
        "/p",
        password,
        "/d",
        file_discription,
        "/du",
        "http://www.yy.com",
        ]

    if sign_with_timestamp:
        command_list.append("/t")
        command_list.append("http://timestamp.verisign.com/scripts/timstamp.dll")

    command_list.append(filepath)
    return subprocess.call(command_list)
