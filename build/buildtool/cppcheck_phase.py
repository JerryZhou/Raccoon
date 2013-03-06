# -*- coding: utf-8 -*-

u"""使用CppCheck进行代码静态检查的环节"""

import logging
import subprocess
import sys

from buildexp import BuildException
import butil

class CppcheckPhase(object):

    def __init__(self, build_prop):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop

    def check(self):
        ignore_dirs = [
            "debug",
            "release",
            "Debug",
            "Release",
            "moc",
            ".svn",
            "build",
            "dist",
            "libs",
            "tools",
            "version",
            "app/src/3rd/QtPatch",
            "app/test",
            'ext/3rdparty',
            "framework/src/dwutility/zip/gzip",
            "framework/src/dwutility/zip/7z",
            "framework/src/dwutility/encryption",
            "framework/src/dwutility/xml",
            "mw/addon_sign",
            "mw/plugin/common/pubfunc/7zip",
            "mw/common4",
            ]

        command_list = [
            "cppcheck",
            "--suppressions-list=build/cppcheck_suppressions.txt",
            "-f",
            "--template",
            "{severity} {id}: {file}({line}): {message}",
            "-q",
            ]

        if self._build_prop.cppcheck.cppcheck_all:
            command_list.append("--enable=all")

        if self._build_prop.cppcheck.cppcheck_xml:
            command_list.append("--xml")

        for d in ignore_dirs:
            command_list.append("-i")
            command_list.append(d)
        command_list.append(".")

        if self._build_prop.cppcheck.cppcheck_xml:
            butil.mkdir("log")
            output_file = open("log/cppcheck.xml", "w")
            stderr = output_file
            process = subprocess.Popen(command_list,
                stderr=stderr)
            process.wait()
            output_file.close()
            if process.returncode != 0:
                raise BuildException("cppcheck failed, returncode: %d"
                    % process.returncode)
        else:
            process = subprocess.Popen(command_list,
                stderr=subprocess.PIPE)
            cppcheck_output = process.communicate()[1]
            if process.returncode != 0:
                raise BuildException("cppcheck failed, returncode: %d"
                    % process.returncode)
            else:
                self._logger.info("cppcheck successed")
                cppcheck_output_normalize = cppcheck_output.replace("\r\n", "\n")
                self._logger.debug("\n%s" % cppcheck_output_normalize)
                self._output_to_stderr(command_list, cppcheck_output_normalize)

    def _output_to_stderr(self, command_list,cppcheck_output):
        content = u"""========================================
CppCheck 命令：
%s
========================================
CppCheck 检查结果（共%d条）：
%s
========================================
""" % (" ".join(command_list), cppcheck_output.count("\n"), cppcheck_output)
        sys.stderr.write(content.encode(self._build_prop.os_encoding))
        sys.stderr.flush()
