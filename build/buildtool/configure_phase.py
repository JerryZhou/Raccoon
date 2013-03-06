# -*- coding: utf-8 -*-

import os
import logging
import ConfigParser
import urllib2
import tarfile
import shutil

from buildexp import BuildException
import butil

REPO_CACHE_DIR = os.path.join(os.environ['APPDATA'], 'dw_repo')

class ConfigurePhase(object):

    def __init__(self, build_prop):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop

    def configure(self):
        self._config_artifact()
        #self._copy_audio_file()

    def _config_artifact(self):
        butil.mkdir(REPO_CACHE_DIR)
        butil.mkdir(self._build_prop.path.depends_dir)

        depends = ConfigParser.SafeConfigParser()
        # depends中，必须允许大写的key
        depends.optionxform = str
        depends.read(self._build_prop.configure.depends_config)

        # 整个依赖库，称为repository，简称repo
        # repo下面，分repo_section，如dw3rd、internal等
        # 每个repo_session，包含若干个artifact

        for repo_section in depends.sections():
            artifacts = {}

            for artifact_name, artifact_version in depends.items(repo_section):
                artifacts[artifact_name] = Artifact(self._build_prop, repo_section, artifact_name, artifact_version)

            self._tidy_outofdate_artifact(repo_section, artifacts)

            for artifact in artifacts.itervalues():
                artifact.config()

    def _tidy_outofdate_artifact(self, repo_section, artifacts):
        repo_section_dir = os.path.join(self._build_prop.path.depends_dir, repo_section)
        if os.path.isdir(repo_section_dir):
            for d in os.listdir(repo_section_dir):
                if not artifacts.has_key(d):
                    butil.delete_dir(os.path.join(repo_section_dir, d))

    def _copy_audio_file(self):
        butil.copy_file_to_dir("app/bin/debug", "mw/audio/bin",
            [
            "AudioCommon_D.dll",
            "AudioDec_D.dll",
            "AudioEnc_D.dll",
            "AudioEngine_D.dll",
            "AudioUtil_D.dll",
            "HookAudio_D.dll",
            ])

        butil.copy_file_to_dir("app/bin/release", "mw/audio/bin",
            [
            "AudioCommon.dll",
            "AudioDec.dll",
            "AudioEnc.dll",
            "AudioEngine.dll",
            "AudioUtil.dll",
            "HookAudio.dll",
            ])

class Artifact(object):

    def __init__(self, build_prop, section, name, version):
        self._logger = logging.getLogger(__name__)
        self._build_prop = build_prop

        self._section = section
        self._name = name
        self._version = version

        # 每个artifact都在3个地方存在：
        #   repo：http服务器上，放着version.info和若干tar.gz
        #   cache：本地的%AppData%/dw_repo里，作为http服务器的缓存，同样有version.info和tar.gz
        #   local：working copy内，被实际使用到的文件。只有tar.gz和解压后的内容，不包含version.info

        self._repo_path = "%s/%s/%s/%s" % (self._build_prop.configure.repo_url_root,
            self._section,
            self._name,
            self._version)

        self._cache_path = os.path.join(REPO_CACHE_DIR,
            self._section,
            self._name,
            self._version)

        self._local_path = os.path.join(self._build_prop.path.depends_dir,
            self._section,
            self._name)
        # 注意，local在存放时，并不需要self._version

        self._logger.debug('''artifact paths:
     repo: %s
    cache: %s
    local: %s''' % (self._repo_path,
        self._cache_path,
        self._local_path))

        self._cache_version_info_path = os.path.join(self._cache_path, 'version.info')

    def config(self):
        self._logger.info('config [%s.%s] start', self._name, self._version)
        self._config()
        self._logger.info('config [%s.%s] finish', self._name, self._version)

    def _config(self):
        # version为latestbuild的artifact，不作缓存
        if self._version == 'latestbuild':
            self._sync_local_with_repo()
            return

        if not self._cache_ok():
            self._sync_cache_with_repo()
            if not self._cache_ok():
                raise BuildException('cache still not ok')

        version_config = self._read_version_config()

        if not self._local_ok(version_config):
            self._sync_local_with_cache(version_config)

        self._transfer_to_app_bin()

    def _read_version_config(self):
        version_config = ConfigParser.SafeConfigParser()
        version_config.read(self._cache_version_info_path)

        return version_config

    def _sync_local_with_repo(self):
        u"""直接从repo下载到local，由于没有version.info，只能尝试去下载"""
        butil.delete_dir(self._local_path)
        butil.mkdir(self._local_path)

        file_list = [
            'bin.tar.gz',
            'dev.tar.gz',
            'pdb.tar.gz',
        ]

        for f in file_list:
            self._download_ignore_404(f, self._local_path)

        self._unpack_all_tar()
        self._transfer_to_app_bin()

    def _cache_ok(self):
        self._logger.info('check cache integrity of [%s] ...', self._cache_path)

        if not os.path.isdir(self._cache_path):
            self._logger.info('dir [%s] is not exist', self._cache_path)
            return False

        if not os.path.isfile(self._cache_version_info_path):
            self._logger.info('file [%s] is not exist', self._cache_version_info_path)
            return False

        version_config = self._read_version_config()

        if not version_config.has_section('md5'):
            self._logger.info('config section [md5] is not exist')

        result = self._check_files_md5(version_config, self._cache_path)
        if result:
            self._logger.info('cache integrity OK')

        return result

    def _sync_cache_with_repo(self):
        butil.delete_dir(self._cache_path)
        butil.mkdir(self._cache_path)

        self._download_respect_404('version.info', self._cache_path)

        version_config = self._read_version_config()

        for key in version_config.options('md5'):
            self._download_respect_404(key, self._cache_path)

    def _local_ok(self, version_config):
        self._logger.info('check local integrity of [%s] ...', self._local_path)
        if not os.path.isdir(self._local_path):
            self._logger.info('dir [%s] is not exist', self._local_path)
            return False

        result = self._check_files_md5(version_config, self._local_path)
        if result:
            self._logger.info('cache integrity OK')

        return result

    def _sync_local_with_cache(self, version_config):
        butil.delete_dir(self._local_path)
        butil.mkdir(self._local_path)

        for key in version_config.options('md5'):
            butil.copy_file(os.path.join(self._cache_path, key), self._local_path)

        self._unpack_all_tar()

    def _check_files_md5(self, version_config, target_dir):
        result = True
        for key in version_config.options('md5'):
            file_path = os.path.join(target_dir, key)

            if os.path.isfile(file_path):
                expect_md5 = version_config.get('md5', key)
                actual_md5 = butil.md5sum(file_path)
                if expect_md5 != actual_md5:
                    self._logger.info('md5 of [%s] is wrong', file_path)
                    result = False
            else:
                self._logger.info('file [%s] is not exist', file_path)
                result = False

        return result

    def _download_respect_404(self, file_name, dest_dir):
        self._download(file_name, dest_dir, True)

    def _download_ignore_404(self, file_name, dest_dir):
        self._download(file_name, dest_dir, False)

    def _download(self, file_name, dest_dir, report_404):
        url = "%s/%s" % (self._repo_path, file_name)
        dest = os.path.join(dest_dir, file_name)

        ret = None
        try:
            ret = urllib2.urlopen(url, timeout=30)
        except urllib2.HTTPError as e:
            if report_404:
                raise BuildException('download fail [%s]' % url)
            else:
                self._logger.debug('[%s] is not exist', url)

        if ret != None:
            with open(dest, 'wb') as w:
                w.write(ret.read())
            self._logger.info('download success [%s]', url)

    def _unpack_all_tar(self):
        # unpack 只会发生在local
        for f in os.listdir(self._local_path):
            if f.endswith('.tar.gz'):
                gz_path = os.path.join(self._local_path, f)

                tar = tarfile.open(gz_path, 'r:gz')
                tar.extractall(self._local_path)
                tar.close()


        artifact_pdb_dir = os.path.join(self._local_path, "pdb")
        artifact_lib_release_dir = os.path.join(self._local_path, "lib", "release")

        if os.path.isdir(artifact_pdb_dir):
            butil.copy_file_to_dir(artifact_lib_release_dir,
                artifact_pdb_dir,
                ["*.pdb"])

    def _transfer_to_app_bin(self):
        artifact_bin_debug_dir = os.path.join(self._local_path, "bin", "debug")
        artifact_bin_release_dir = os.path.join(self._local_path, "bin", "release")

        if os.path.isdir(artifact_bin_debug_dir):
            self._copy_binary_dir(os.path.join(self._build_prop.path.app_dir, "bin/debug"),
                artifact_bin_debug_dir)

        if os.path.isdir(artifact_bin_release_dir):
            self._copy_binary_dir(os.path.join(self._build_prop.path.app_dir, "bin/release"),
                artifact_bin_release_dir)
            # 存在release目录，但是缺少debug目录的artifact，把它的release当作debug来用
            if not os.path.isdir(artifact_bin_debug_dir):
                self._copy_binary_dir(os.path.join(self._build_prop.path.app_dir, "bin/debug"),
                    artifact_bin_release_dir)

    def _copy_binary_dir(self, to_dir, base_dir):
        for item in os.listdir(base_dir):
            item_path = os.path.join(base_dir, item)
            if os.path.isdir(item_path):
                dest_dir_path = os.path.join(to_dir, item)
                if os.path.exists(dest_dir_path):
                    # shutil.copytree 调用前，必须确保目标目录不存在
                    shutil.rmtree(dest_dir_path)
                shutil.copytree(item_path,
                    dest_dir_path,
                    ignore=shutil.ignore_patterns(".svn"))
            else:
                shutil.copy(item_path, to_dir)

