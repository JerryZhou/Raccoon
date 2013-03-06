# -*- coding: utf-8 -*-

u"""编译阶段"""

import subprocess
import logging
import glob
import os
import sys
import string
import codecs

from buildexp import BuildException
import butil

class CompilePhase(object):

    def __init__(self, build_prop):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop
        self._back_dir = os.path.join(self._build_prop.path.dist_dir, "bak")

        self._tasks = []
        self._determine_task()

    def _determine_task(self):
        build_type = self._build_prop.share.build_type

        if "debug" in build_type:
            self._tasks.append(DebugCompileTask(self._build_prop))

        if "release" in build_type:
            self._tasks.append(ReleaseCompileTask(self._build_prop))

    def compile_(self):
        u"""编译阶段"""
        self._init_dist_dir()
        self._init_backup_dir()
        self._generate_string_translator()
        self._generate_qm()

        if self._build_prop.package.inject_version:
            self._backup_version_relative_files()
            try:
                self._generate_version_relative_files()
                self._execute_all_task()
            except:
                raise
            finally:
                self._restore_version_relative_files()
        else:
            self._execute_all_task()

    def _init_dist_dir(self):
        u"""初始化dist目录。这个目录用于存放构建中生成中间文件（除了编译所生成的）
        这个目录需要在编译开始前创建，不能早也不能晚
        """
        butil.mkdir(self._build_prop.path.dist_dir)

    def _init_backup_dir(self):
        butil.mkdir(self._back_dir)

    def _generate_string_translator(self):
        u"""生成与国际化相关的代码文件"""
        returncode = subprocess.call(["python",
            "script/xmlstringtranslator.py",
            "bin/release/layout",
            "bin/release/addons/addonsmanager/resource",
            "src/yyapp/xml_string_translator.h",
            "src/yyapp/yyapp.pro",
            "HEADERS += xml_string_translator.h"],
            cwd=self._build_prop.path.app_dir)
        if returncode != 0:
            raise BuildException("String translatation failed")

    def _generate_qm(self):
        process = subprocess.Popen([
            "lrelease",
            "-compress",
            "../framework/src/duifw/duifw_zh.ts",
            "src/yycommon/yycommon_zh.ts",
            "src/yyapp/yyapp_zh.ts",
            "src/yymainframe/yymainframe_zh.ts",
            "src/yychannel/yychannel_zh.ts",
            "src/yyim/yyim_zh.ts",
            "src/yyhistroymsg/yymsg_zh.ts",
            "src/yygroup/yygroup_zh.ts",
            "src/yggroup/yggroup_zh.ts",
            "src/yyplugin/yyplugin_zh.ts",
            "src/addonfw/addonfw_zh.ts",
            "-qm",
            "src/yyapp/yyapp_zh.qm",
            ],
            cwd=self._build_prop.path.app_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
        process.wait()

        if process.returncode != 0:
            output_normalize = process.communicate()[1].replace("\r\n", "\n")
            self._logger.debug("\n%s" % output_normalize)
            sys.stderr.write(output_normalize)
            raise BuildException("Generate .qm file failed")

        butil.copy_file_to_dir("app/bin/release/lang", "app/src/yyapp",
            ["yyapp_zh.qm"])

    def _backup_version_relative_files(self):
        self._backup_productinfo()
        self._backup_protocol_module_version()

    def _backup_productinfo(self):
        butil.copy_file_to_dir(self._back_dir,
            "version",
            ["productinfo.cpp"])

    def _backup_protocol_module_version(self):
        butil.copy_file_to_dir(self._back_dir,
            "mw/protocol4/protocol",
            ["ProtocolModuleVersion.h"])

    def _generate_version_relative_files(self):
        self._generate_productinfo()
        self._generate_protocol_module_version()

    def _generate_productinfo(self):
        file_path = "version/productinfo.cpp"
        file_ = codecs.open(file_path, mode="r", encoding="GBK")

        template = string.Template(file_.read())
        result = template.substitute(self._build_prop.version.about.__dict__)
        file_.close()

        file_ = codecs.open(file_path, mode="w", encoding="GBK")
        file_.write(result)
        file_.close()

        butil.copy_file(file_path,
            os.path.join(self._back_dir, "%s.modify" % os.path.basename(file_path)))

    def _generate_protocol_module_version(self):
        file_path = "mw/protocol4/protocol/ProtocolModuleVersion.h"

        rev_string = self._build_prop.version.about.last_commit_rev
        if rev_string != '[rev]' and len(rev_string) > 1:
            rev = int(rev_string[1:])
        else:
            rev = 0

        if rev != 0:
            lines = []
            with codecs.open(file_path, mode="r", encoding="GBK") as f_read:
                lines = f_read.readlines()
                for index, line in enumerate(lines):
                    if line.startswith('#define REVISION_VERSION'):
                        lines[index] = '#define REVISION_VERSION %d\r\n' % rev
            with codecs.open(file_path, mode="w", encoding="GBK") as f_write:
                f_write.write(''.join(lines))

        butil.copy_file(file_path,
            os.path.join(self._back_dir, "%s.modify" % os.path.basename(file_path)))

    def _execute_all_task(self):
        for task in self._tasks:
            task.execute()

    def _restore_version_relative_files(self):
        self._restore_productinfo()
        self._restore_protocol_module_version()

    def _restore_productinfo(self):
        butil.copy_file_to_dir("version",
            self._back_dir,
            ["productinfo.cpp"])

    def _restore_protocol_module_version(self):
        butil.copy_file_to_dir("mw/protocol4/protocol",
            self._back_dir,
            ["ProtocolModuleVersion.h"])

class AbstractCompileTask(object):
    u"""提供编译任务的模板方法"""

    def __init__(self, build_prop):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop
        self._projects = [
            butil.Project("misc",
                "yymisc.sln",
                os.path.join(self._build_prop.path.tools_dir, "yymisc"),
                True),
            butil.Project("yy",
                "yy.sln",
                ".",
                True),
            butil.Project("crashreport",
                "crashreport.sln",
                self._build_prop.path.crash_report_dir,
                True),
            butil.Project("yy_checker",
                "yy_checker.sln",
                os.path.join(self._build_prop.path.tools_dir, "yy_checker"),
                True),
            butil.Project("duospeak",
                "duospeak.sln",
                os.path.join(self._build_prop.path.tools_dir, "duospeak"),
                True),
            butil.Project("yylauncher",
                "yylauncher.sln",
                self._build_prop.path.tools_dir,
                True),
        ]

    def execute(self):
        self._logger.info("execute compile task [%s]" % self._get_task_name())

        self._generate_pro_files()

        if self._build_prop.compile.recompile:
            self._clean_by_devenv(self._projects, self._get_vc_config_name())

        self._compile_by_devenv(self._projects, self._get_vc_config_name())
        self._unit_test()

    def _get_task_name(self):
        u"""供子类使用，定义用于标识task的名字，如Debug"""
        raise BuildException("_get_task_name must be overrided")

    def _generate_pro_files(self):
        u"""调用qmake，生成.sln和.vcproj文件"""
        command_list = ["qmake", "-tp", "vc", "-r"]

        if self._build_prop.share.official_build:
            command_list.append("DEFINES+=OFFICIAL_BUILD")

        if self._build_prop.share.coverage:
            command_list.append("DEFINES+=CODE_COVERAGE")
            command_list.append("QMAKE_LFLAGS_RELEASE+=/debugtype:cv,fixup")

        self._get_addtional_qmake_arguments(command_list)

        returncode = subprocess.call(command_list)

        if returncode != 0:
            raise BuildException("Invoke qmake failed")

    def _get_addtional_qmake_arguments(self, qmake_command_list):
        u"""供子类使用，添加附加的qmake参数"""
        pass

    def _get_vc_config_name(self):
        u"""供子类使用，定义vc的config名，用于调用devenv时使用"""
        raise BuildException("_get_vc_config_name must be overrided")

    def _clean_by_devenv(self, project_list, config):
        for project in project_list:
            devenv = butil.Devenv(self._build_prop)
            devenv(project,
                "clean",
                config)

    def _compile_by_devenv(self, project_list, config):
        for project in project_list:
            devenv = butil.Devenv(self._build_prop)
            devenv(project,
                "build",
                config)

    def _unit_test(self):
        u"""供子类使用，单元测试"""
        raise BuildException("_unit_test must be overrided")

class DebugCompileTask(AbstractCompileTask):
    u"""编译Debug版"""
    def _get_task_name(self):
        return "Debug"

    def _get_vc_config_name(self):
        return "Debug|Win32"

    def _unit_test(self):
        unittest_dir = "/app/bin/debug"
        returncode = subprocess.call([
            "app/bin/debug/dwutest.exe",
            unittest_dir,
            "--gtest_output=xml:log/unittest/dwutestd.xml",
        ])

        if returncode == 0:
            self._logger.info("%s: unittest success" % unittest_dir)
        else:
            raise BuildException("%s: unittest failed, returncode: %d"
                % (unittest_dir, returncode))

class ReleaseCompileTask(AbstractCompileTask):
    u"""编译Release版"""
    def _get_task_name(self):
        return "Release"

    def _get_vc_config_name(self):
        return "Release|Win32"

    def _unit_test(self):
        unittest_dir = "/app/bin/release"
        returncode = subprocess.call([
            "app/bin/release/dwutest.exe",
            unittest_dir,
            "--gtest_output=xml:log/unittest/dwutest.xml",
        ])

        if returncode == 0:
            self._logger.info("%s: unittest success" % unittest_dir)
        else:
            raise BuildException("%s: unittest failed, returncode: %d"
                % (unittest_dir, returncode))

