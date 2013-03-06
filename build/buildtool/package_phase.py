#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""打包，包括普通的7z压缩包、安装包和在线更新包"""

import os
import shutil
import subprocess
import logging
import codecs
import string
import tarfile
import StringIO
import glob

import butil
from buildexp import BuildException
from config.package_list import package_list

class PackagePhase(object):

    def __init__(self, build_prop):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop

        self._tasks = []
        self._determine_task()

    def _determine_task(self):
        build_type = self._build_prop.share.build_type

        if "debug" in build_type:
            self._tasks.append(DebugPackageTask(self._build_prop))

        if "release" in build_type:
            task = ReleasePackageTask(self._build_prop)
            if self._build_prop.package.setup:
                task = SetupPackageTask(self._build_prop, task)
            self._tasks.append(task)

    def package(self):
        u"""完成各种版本的打包工作"""
        for task in self._tasks:
            task.execute()

class IPackageTask(object):
    def execute(self):
        pass

    def get_package_dir(self):
        pass

class AbstractPackageTask(IPackageTask):

    def __init__(self, build_prop, package_dir):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop
        self._package_dir = package_dir
        self._flat_yy40_dir = os.path.join(package_dir, "yy40")

    def execute(self):
        self._clean_for_package()
        self._copy_file_to_dist()
        self._package_7z()

    def get_package_dir(self):
        return self._package_dir

    def _clean_for_package(self):
        butil.delete_dir(self._package_dir)

    def _copy_file_to_dist(self):
        u"""供子类实现"""
        pass

    def _package_7z(self):
        butil.exec_7z("yy40", "yy40.7z", self._package_dir)

        yy40_7z = os.path.join(self._package_dir, "yy40.7z")

        size_byte = os.path.getsize(yy40_7z)
        size_k_byte = float(size_byte) / (1024 * 1024)
        self._logger.info("%s size: %.3f M (%d bytes)"
        % (yy40_7z, size_k_byte, size_byte))

class DebugPackageTask(AbstractPackageTask):

    def __init__(self, build_prop):
        AbstractPackageTask.__init__(self,
            build_prop,
            os.path.join(build_prop.path.dist_dir, "debug"))

    def _copy_file_to_dist(self):
        butil.mkdir(self._flat_yy40_dir)

        for fileset in package_list['Debug_and_Release']:
            butil.copy(fileset, self._flat_yy40_dir)
        for fileset in package_list['Debug']:
            butil.copy(fileset, self._flat_yy40_dir)

class ReleasePackageTask(AbstractPackageTask):

    def __init__(self, build_prop):
        AbstractPackageTask.__init__(self,
            build_prop,
            os.path.join(build_prop.path.dist_dir, "release"))
        self._pdb_dir = os.path.join(self._package_dir, "pdb")
        self._addon_temp_dir = os.path.join(self._package_dir, "addon_temp")

    def _copy_file_to_dist(self):
        butil.mkdir(self._flat_yy40_dir)

        for fileset in package_list['Debug_and_Release']:
            butil.copy(fileset, self._flat_yy40_dir)
        for fileset in package_list['Release']:
            butil.copy(fileset, self._flat_yy40_dir)

        self._sign_addons()

        self._collect_pdb()

    def _sign_addons(self):
        # self._addon_temp_dir 用于临时存放待打包的文件
        butil.delete_dir(self._addon_temp_dir)
        butil.mkdir(self._addon_temp_dir)

        addons_key = "mw/addon_sign/dbkey"
        if self._build_prop.share.official_build:
            addons_key = self._build_prop.package.addons_key_filepath

        addon_src_dir = "app/bin/release/addons"
        for d in os.listdir(addon_src_dir):
            if d != '.svn':
                self._sign_one_addon(addon_src_dir, d, addons_key)

    def _sign_one_addon(self, addon_src_dir, addon_name, addons_key):
        shutil.copytree(os.path.join(addon_src_dir, addon_name),
            os.path.join(self._addon_temp_dir, addon_name),
            ignore=shutil.ignore_patterns(".svn"))

        butil.exec_7z(".\\%s\\*" % addon_name, "%s.7z" % addon_name, self._addon_temp_dir)

        butil.mkdir(os.path.join(self._flat_yy40_dir, "addons", addon_name))
        returncode = subprocess.call([
            "app/bin/release/addon_sign_tool.exe",
            "sign",
            addons_key,
            os.path.join(self._addon_temp_dir, "%s.7z" % addon_name),
            os.path.join(self._flat_yy40_dir, "addons", addon_name, addon_name),
        ])

        if returncode == 0:
            self._logger.info("addon_sign_tool sign [%s] success" % addon_name)
        else:
            raise BuildException("addon_sign_tool sign [%s] failed, returncode: %d"
                % (addon_name, returncode))

    def _collect_pdb(self):
        butil.mkdir(self._pdb_dir)
        butil.copy_file_to_dir(self._pdb_dir,
            "app/bin/release",
            ["*.pdb", "*.exe", "*.dll"])

        logger = logging.getLogger(__name__)
        logger.debug('tar pdb.tar.bz2 start')

        pdb_tar_name = os.path.join(self._package_dir, "pdb.tar")
        pdb_tar_file = tarfile.open(pdb_tar_name, "w")
        pdb_tar_file.add(self._pdb_dir, arcname="")
        pdb_tar_file.close()

        exe_7za = os.path.abspath("build/bin/7za.exe")

        command_list = [exe_7za,
            "a",
            "pdb.tar.bz2",
            "pdb.tar",
        ]

        subprocess.call(command_list, cwd=self._package_dir)

        logger.debug('tar pdb.tar.bz2 finished')

class SetupPackageTask(IPackageTask):

    def __init__(self, build_prop, release_package_task):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop
        self._release_package_task = release_package_task
        self._package_dir = release_package_task.get_package_dir()
        self._back_dir = os.path.join(self._package_dir, "bak")

        about = self._build_prop.version.about
        if self._build_prop.share.official_build:
            self._liveupdate_url_prefix = "%s/%s/liveupdate" \
                % (about.real_download_server_url, about.real_product_version_stat)
            self._liveupdate_url_prefix_v2 = "%s/liveupdate" \
                % about.real_product_version_stat
        else:
            self._liveupdate_url_prefix = "%s/%s/liveupdate" \
                % (about.real_download_server_url, about.real_release_name)
            self._liveupdate_url_prefix_v2 = "%s/liveupdate" \
                % about.real_release_name

        # 收集需要被打包（安装包和在线升级包）的文件的目录
        self._flat_yy40_dir = os.path.join(self._package_dir, "yy40")

        # 用于存放将会被打包到安装包的文件
        self._setupfiles_dir = os.path.join(self._package_dir, "SetupFiles")
        # 用于存放将会安装到安装目录下的文件
        self._install_dir = os.path.join(self._setupfiles_dir, "InstallDir")
        self._internal_install_dir = os.path.join(self._install_dir,
            self._build_prop.version.about.real_product_version_stat)

        # 用于存放将会安装到%AppData%目录下的文件
        self._appdata_dir = os.path.join(self._setupfiles_dir, "AppData")

        self._liveupdate_dir = os.path.join(self._package_dir, "liveupdate")

        if self._build_prop.package.signature:
            self._pfx_filepath = self._build_prop.package.pfx_filepath
            passwd_filepath = self._build_prop.package.passwd_filepath
            self._pfx_password = self._read_pfx_password(passwd_filepath)

    def _read_pfx_password(self, passwd_filepath):
        passwd_file = open(passwd_filepath)
        password = passwd_file.readline().strip()
        passwd_file.close()
        return password

    def execute(self):
        self._release_package_task.execute()

        butil.delete_dir(self._liveupdate_dir)
        butil.mkdir(self._liveupdate_dir)

        butil.delete_dir(self._setupfiles_dir)
        butil.mkdir(self._setupfiles_dir)
        # TODO copytree 需要目的目录不存在，因此不能建立 self._install_dir
        butil.mkdir(self._appdata_dir)
        butil.mkdir(self._back_dir)

        self._package()

    def _package(self):
        self._backup_configs()

        try:
            self._do_package()
        except:
            raise
        finally:
            self._restore_config()

    def _do_package(self):
        self._substitute_config()
        self._build_uninstall_exe()
        self._put_uninstall_exe_to_flat_dir()

        if self._build_prop.package.signature:
            self._do_digital_signature()

        self._copy_flat_to_install_dir()
        self._make_all_cabs()
        self._copy_release_note_for_update_and_setup()
        self._generate_version_xml()
        self._generate_launcher_xml()
        self._create_yy7z()
        self._build_setup_exe()

        if self._build_prop.package.signature:
            self._do_digital_signature_for_setup_exe()

        self._tar_liveupdate()

    def _backup_configs(self):
        butil.copy_file_to_dir(self._back_dir,
            os.path.join(self._build_prop.path.setup_dir, "src/install"),
            ["Config.h", "YYSetup.rc"])
        butil.copy_file_to_dir(self._back_dir,
            os.path.join(self._build_prop.path.setup_dir, "src/uninstall"),
            ["Uninstall.rc"])

    def _substitute_config(self):
        for f in [os.path.join(self._build_prop.path.setup_dir, "src/install/Config.h"),
            os.path.join(self._build_prop.path.setup_dir, "src/install/YYSetup.rc"),
            os.path.join(self._build_prop.path.setup_dir, "src/uninstall/Uninstall.rc")]:
            self._substitute_one_file(f, self._build_prop.version.setup)

    def _substitute_one_file(self, file_path, setup_props):
        file_ = codecs.open(file_path, mode="r", encoding="GBK")
        template = file_.read()
        file_.close()

        file_ = codecs.open(file_path, mode="w", encoding="GBK")
        file_.write(template.format(setup=setup_props))
        file_.close()

        butil.copy_file(file_path,
            os.path.join(self._back_dir, "%s.modify" % os.path.basename(file_path)))

    def _build_uninstall_exe(self):
        uninstall_project = butil.Project("uninstall",
            "Uninstall.sln",
            os.path.join(self._build_prop.path.setup_dir, "src/uninstall"),
            True)
        devenv = butil.Devenv(self._build_prop)
        devenv(uninstall_project,
            "rebuild",
            "Release|Win32")

    def _put_uninstall_exe_to_flat_dir(self):
        butil.copy_file_to_dir(self._flat_yy40_dir,
            os.path.join(self._build_prop.path.setup_dir, "bin"),
            ["Uninstall.exe"])

    def _do_digital_signature(self):
        for root, dirs, files in os.walk(self._flat_yy40_dir):
            if "Microsoft.VC90.CRT" in dirs:
                dirs.remove("Microsoft.VC90.CRT")
            for f in files:
                if f.endswith((".exe", ".dll", ".ocx")):
                    butil.signature(self._pfx_filepath,
                        self._pfx_password,
                        os.path.join(root, f),
                        "YY",
                        self._build_prop.package.sign_with_timestamp)

    def _copy_flat_to_install_dir(self):
        shutil.copytree(self._flat_yy40_dir, self._internal_install_dir)
        butil.copy_file_to_dir(self._install_dir,
            self._internal_install_dir,
            [
                "yylauncher.exe",
                "uninstall.exe",
            ])

        butil.delete_file(os.path.join(self._internal_install_dir, "yylauncher.exe"))
        butil.delete_file(os.path.join(self._internal_install_dir, "uninstall.exe"))

    def _copy_release_note_for_update_and_setup(self):
        butil.copy_file_to_dir(self._internal_install_dir, "build/config", ["ReleaseNotes.txt"])
        butil.copy_file_to_dir(self._appdata_dir, "build/config", ["ReleaseNotes.txt"])
        butil.copy_file_to_dir(self._liveupdate_dir, "build/config", ["ReleaseNotes.txt"])

    def _create_yy7z(self):
        self._create_first_run_tag()
        butil.exec_7z(".\\SetupFiles\\*", "yy.7z", self._package_dir)

    def _create_first_run_tag(self):
        u"""首次安装YY的标记"""
        tag_file = os.path.join(self._appdata_dir, "install.dat")
        open(tag_file, "w").close()

    def _build_setup_exe(self):
        butil.copy_file(os.path.join(self._package_dir, "yy.7z"),
            os.path.join(self._build_prop.path.setup_dir, "src/install/res/yy.7z"))

        setup_project = butil.Project("setup",
            "YYSetup.sln",
            os.path.join(self._build_prop.path.setup_dir, "src/install"),
            True)
        devenv = butil.Devenv(self._build_prop)
        devenv(setup_project,
            "rebuild",
            "Release|Win32")

        butil.copy_file(os.path.join(self._build_prop.path.setup_dir, "bin/YYSetup.exe"),
            self._package_dir)

    def _do_digital_signature_for_setup_exe(self):
        butil.signature(self._pfx_filepath,
                        self._pfx_password,
                        os.path.join(self._package_dir, "YYSetup.exe"),
                        "YY",
                        self._build_prop.package.sign_with_timestamp)

    def _restore_config(self):
        butil.copy_file_to_dir(os.path.join(self._build_prop.path.setup_dir, "src/install"),
            self._back_dir,
            ["Config.h", "YYSetup.rc"])
        butil.copy_file_to_dir(os.path.join(self._build_prop.path.setup_dir, "src/uninstall"),
            self._back_dir,
            ["Uninstall.rc"])

    def _make_all_cabs(self):
        for entity in os.listdir(self._flat_yy40_dir):
            entity_path = os.path.join(self._flat_yy40_dir, entity)
            if os.path.isdir(entity_path):
                pack_name = entity
                cab_path = os.path.join(self._liveupdate_dir, "%s.cab" % pack_name)
                butil.make_cab(self._flat_yy40_dir,
                    cab_path,
                    "%s\\*.*" % entity_path)
            else:
                pack_name = os.path.splitext(entity)[0]
                cab_path = os.path.join(self._liveupdate_dir, "%s.cab" % pack_name)
                butil.make_cab(self._flat_yy40_dir,
                    cab_path,
                    entity_path)

    def _generate_version_xml(self):
        server_config = StringIO.StringIO()
        server_config.write('''<?xml version="1.0"?>
<version>
''')
        client_config = StringIO.StringIO()
        client_config.write('''<?xml version="1.0"?>
<version>
''')

        about = self._build_prop.version.about
        server_config_v2 = StringIO.StringIO()
        server_config_v2.write('''<?xml version="1.0" encoding='utf-8'?>
<Product Product="YY">
    <Version VerNo="%s" BuildNo="%s" Dir="%s" Notes="%s/ReleaseNotes.txt">
'''
        % (about.real_product_version_title,
            self._build_prop.version.liveupdate.base_update_ver,
            self._build_prop.version.about.real_product_version_stat,
            self._liveupdate_url_prefix_v2))

        base_update_ver = self._build_prop.version.liveupdate.base_update_ver
        for cab_path in glob.glob(self._liveupdate_dir + "/*.cab"):
            pack_name = os.path.splitext(os.path.basename(cab_path))[0]
            md5 = butil.md5sum(cab_path)
            server_config.write(
                '%s<pack name="%s" ver="%s" md5="%s" url="%s/%s.cab" type="0" />\n'
                % (" " * 4, pack_name, base_update_ver, md5, self._liveupdate_url_prefix, pack_name) )
            server_config_v2.write(
                '%s<Pack Name="%s" FileVer="%s" PackMD5="%s" FileUrl="%s/%s.cab" />\n'
                % (" " * 8, pack_name, base_update_ver, md5, self._liveupdate_url_prefix_v2, pack_name) )
            client_config.write(
                '%s<pack name="%s" ver="%s" md5="" url="" type="0" />\n'
                % (" " * 4, pack_name, base_update_ver) )
        # 兼容旧版本
        server_config.write('    <log url="%s/liveupdate.txt" />\n' % self._liveupdate_url_prefix)
        server_config.write('    <ReleaseNotes url="%s/ReleaseNotes.txt" />\n' % self._liveupdate_url_prefix)

        server_config.write('</version>')
        server_config_v2.write('''    </Version>
</Product>''')

        client_config.write('</version>')

        with open(os.path.join(self._liveupdate_dir, "update.xml"), "w") as server_config_file:
            server_config_file.write(server_config.getvalue())

        with codecs.open(os.path.join(self._liveupdate_dir, "update_v2.xml"), "w", 'utf-8') as server_config_file_v2:
            server_config_file_v2.write(server_config_v2.getvalue())

        with open(os.path.join(self._internal_install_dir, "localverInBin.xml"), "w") as client_config_file:
            client_config_file.write(client_config.getvalue())

    def _generate_launcher_xml(self):
        with codecs.open(os.path.join(self._install_dir, "Launcher.xml"), "w", 'utf-8') as launcher_xml:
            launcher_xml.write('''<?xml version="1.0"?>
<LauncherInfo>
    <VersionInfo No="%s" yyPath="%s" Enable="1" First="0" Index="1"/>
</LauncherInfo>
''' % (
    self._build_prop.version.about.real_product_version_stat,
    self._build_prop.version.about.real_product_version_stat,
    ))

    def _tar_liveupdate(self):
        livaupdate_tar_file_path = os.path.join(self._package_dir, "liveupdate.tar.gz")
        livaupdate_tar_file = tarfile.open(livaupdate_tar_file_path, "w:gz")

        about = self._build_prop.version.about
        if self._build_prop.share.official_build:
            arc_prefix = about.real_product_version_stat
        else:
            arc_prefix = about.real_release_name

        livaupdate_tar_file.add(self._liveupdate_dir, arcname="%s/liveupdate" % arc_prefix)
        livaupdate_tar_file.close()
