#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""构建cli命令行界面
读取用户输入，进行分析，进而转换为环境变量和构建属性"""

import optparse
import os
import logging
import pprint
import filecmp
import subprocess
import multiprocessing

import butil
import phase_resolver
from buildexp import BuildException

def parse_user_input(build_prop):
    parser = _create_cli(build_prop)
    (options, args) = parser.parse_args()
    _parse_options(build_prop, options)
    _parse_phases(build_prop, parser, args)
    _check_enviroment_variable(build_prop)

    _check_prerequisites(build_prop)

def _create_cli(build_prop):
    PADDING = '\n' + ' ' * 4
    available_phases = PADDING + PADDING.join(build_prop.phase_list)

    usage = '''python %%prog [phases] [options]

Examples:
    build with [debug] configuration:
        python %%prog compile
    build with [release] configuration:
        python %%prog compile --release
    make YYSetup.exe:
        python %%prog clean package --release --setup
    run cppcheck:
        python %%prog cppcheck
    run configure:
        python %%prog configure
Phases:%s''' % available_phases;

    parser = optparse.OptionParser(usage = usage,
        version = "1.2.0")

    parser.add_option("--debug",
        dest="build_debug",
        help="specify build type to [debug]",
        default=True,
        action="store_true")

    parser.add_option("--release",
        dest="build_debug",
        help="specify build type to [release]",
        default=True,
        action="store_false")

    parser.add_option("--local-build",
        dest="distribute_build",
        help="don't use distribute build",
        default=True,
        action="store_false")

    parser.add_option("--rev",
        dest="last_commit_rev",
        help='specify last commit revision to generate "version string"',
        default="[rev]",
        action="store",
        metavar="REV")

    parser.add_option("--official-build",
        dest="official_build",
        help="make official build, no dev private key",
        default=False,
        action="store_true")

    cppcheck_group = optparse.OptionGroup(parser,
        "Options for Cppcheck phase")
    cppcheck_group.add_option("--cppcheck-all",
        dest="cppcheck_all",
        help='let Cppcheck run with argu "--enable=all"',
        default=True,
        action="store_true")
    cppcheck_group.add_option("--cppcheck-xml",
        dest="cppcheck_xml",
        help='let Cppcheck generate xml output',
        default=False,
        action="store_true")
    parser.add_option_group(cppcheck_group)

    compile_group = optparse.OptionGroup(parser,
        "Options for Compile phase")
    compile_group.add_option("--recompile",
        dest="recompile",
        help="let VC rebuild the whole solution (clean all + build all)",
        default=False,
        action="store_true")
    compile_group.add_option("--coverage",
        dest="coverage",
        help="code coverage:/debugtype:cv,fixup",
        default=False,
        action="store_true")
    parser.add_option_group(compile_group)

    package_group = optparse.OptionGroup(parser,
        "Options for Package phase")
    package_group.add_option("--setup",
        dest="setup",
        help="generate YYSetup.exe",
        default=False,
        action="store_true")
    package_group.add_option("--sign",
        dest="signature",
        help="sign all PE file",
        default=False,
        action="store_true")
    package_group.add_option("--sign-with-timestamp",
        dest="sign_with_timestamp",
        help="cause digital sinature fail when timestamp server is unreachable",
        default=False,
        action="store_true")
    package_group.add_option("--inject-version",
        dest="inject_version",
        help="inject version info",
        default=False,
        action="store_true")

    parser.add_option_group(package_group)

    # 对于日常开发，由于从Qt Prompt开始使用，所以需要指定--qtdir和--vcdir
    # --qtdir E:\Qt\4.7.0
    # --qtdir E:\\Qt\\4.7.0
    # --qtdir E:/Qt/4.7.0
    # 这三种写法都可以接受
    # 如果带有空格，则套上引号，如：
    # --vcdir "E:\Microsoft Visual Studio 9.0"

    ci_group = optparse.OptionGroup(parser,
        "Options for Continuous Integration only")
    ci_group.add_option("--qtdir",
        dest="qtdir",
        help="specify ENV variable: QTDIR",
        metavar="QTDIR")
    ci_group.add_option("--vcdir",
        dest="vcdir",
        help="help to specify ENV variable DEVENVDIR",
        metavar="VCDIR")
    ci_group.add_option("--build-number",
        dest="build_number",
        help="unique build number",
        default=0,
        metavar="NUM")
    parser.add_option_group(ci_group)
    return parser

def _parse_options(build_prop, options):
    logger = logging.getLogger(__name__)

    if options.qtdir != None:
        logger.debug("options.qtdir: %s" %
            options.qtdir.decode(build_prop.os_encoding))
        _read_option_qtdir(options.qtdir)

    if options.vcdir != None:
        logger.debug("options.vcdir: %s" %
            options.vcdir.decode(build_prop.os_encoding))
        _read_option_vcdir(options.vcdir)

    if not os.environ.has_key("QMAKESPEC"):
        os.environ["QMAKESPEC"] = "win32-msvc2008"

    build_prop.cppcheck.cppcheck_all = options.cppcheck_all
    build_prop.cppcheck.cppcheck_xml = options.cppcheck_xml

    build_prop.compile.recompile = options.recompile
    build_prop.share.coverage = options.coverage

    build_prop.package.setup = options.setup
    build_prop.package.signature = options.signature
    build_prop.package.sign_with_timestamp = options.sign_with_timestamp
    build_prop.package.inject_version = options.inject_version

    build_prop.share.official_build = options.official_build
    build_prop.share.distribute_build = options.distribute_build
    if not options.distribute_build and multiprocessing.cpu_count() > 4:
        build_prop.share.multi_cpu_build = True

    build_prop.share.build_type = _determine_build_type(options.build_debug)

    build_prop.version.about.last_commit_rev = options.last_commit_rev

    if options.build_number == 0:
        # 如果外部不指定 --build-number，则构建号和统计版本号使用本地配置
        pass
    else:
        build_prop.version.liveupdate.base_update_ver = options.build_number
        _set_product_version_stat_with_build_number(build_prop, options.build_number)

def _set_product_version_stat_with_build_number(build_prop, build_number):
    template = build_prop.version.about.real_product_version_stat
    template += "%s"
    build_prop.version.about.real_product_version_stat = template % build_number
    build_prop.version.setup.config.product_version_stat = template % build_number

def _parse_phases(build_prop, parser, args):
    logger = logging.getLogger(__name__)

    # TODO: 将phase_list和phase_depend_map作为可配置信息
    phase_depend_map = {"configure" : [],
        "clean" : [],
        "compile" : ["configure"],
        "test" : ["compile"],
        "package" : ["configure", "compile", "test"],
        "cppcheck" : []}

    (invalid_phases, valid_phases) = phase_resolver.resolve(phase_depend_map,
        args)

    if len(invalid_phases) != 0:
        logger.error("phase: [%s] are not supported"
            % ", ".join(invalid_phases))
        parser.print_usage()
        parser.exit()

    phase_run_order = [p for p in build_prop.phase_list
        if valid_phases.count(p) != 0]
    logger.info("phase will run: [%s]" % ", ".join(phase_run_order))
    build_prop.phase_run_order = phase_run_order

def _read_option_qtdir(qtdir):
    os.environ["QTDIR"] = qtdir
    qtbindir = os.path.join(os.environ["QTDIR"], "bin")
    os.environ["PATH"] = qtbindir + ";" + os.environ["PATH"]

def _read_option_vcdir(vcdir):
    devenvdir = os.path.join(vcdir, "common7", "IDE")
    os.environ["DEVENVDIR"] = devenvdir
    vcbindir = os.path.join(vcdir, "VC", "bin")
    os.environ["PATH"] = vcbindir + ";" + os.environ["PATH"]
    vsinstr_dir = os.path.join(vcdir, "Team Tools", "Performance Tools")
    os.environ["PATH"] = vsinstr_dir + ";" + os.environ["PATH"]

def _determine_build_type(build_debug):
    if build_debug:
        return ['debug']
    else:
        return ['release']

def _check_enviroment_variable(build_prop):
    logger = logging.getLogger(__name__)

    build_golbal_env = dict()
    build_golbal_env["QTDIR"] = \
        os.environ["QTDIR"].decode(build_prop.os_encoding)
    build_golbal_env["QMAKESPEC"] = \
        os.environ["QMAKESPEC"]
    build_golbal_env["DEVENVDIR"] = \
        os.environ["DEVENVDIR"].decode(build_prop.os_encoding)
    build_golbal_env["PATH"] = \
        os.environ["PATH"].decode(build_prop.os_encoding).split(";")

    logger.debug("build env:\n%s" % pprint.pformat(build_golbal_env, 4))

def _check_prerequisites(build_prop):
    if build_prop.package.signature:
        _check_pfx_and_password(build_prop)
        _prepare_for_signatrue()

    if build_prop.share.official_build:
        _check_addon_sign_key(build_prop)

def _check_pfx_and_password(build_prop):
    user_profile_path = os.environ["USERPROFILE"]

    pfx_filepath = os.path.join(user_profile_path,
        "YYBuild",
        "VeriSign.pfx")
    if not os.path.isfile(pfx_filepath):
        raise BuildException("signature: .pfx file is not exist")

    passwd_filepath = os.path.join(user_profile_path, "YYBuild", "passwd")
    if not os.path.isfile(passwd_filepath):
        raise BuildException("signature: passwd file is not exist")

    build_prop.package.__dict__["pfx_filepath"] = pfx_filepath
    build_prop.package.__dict__["passwd_filepath"] = passwd_filepath

def _prepare_for_signatrue():
    windows_path = os.environ["SYSTEMROOT"]
    capicom_filepath = os.path.join(windows_path, "system32", "capicom.dll")

    if not _is_capicom_exist(capicom_filepath):
        raise BuildException("signature: capicom.dll is not exist in system32")

def _is_capicom_exist(capicom_filepath):
    return os.path.isfile(capicom_filepath) \
        and filecmp.cmp(capicom_filepath, "build/bin/capicom.dll")

def _check_addon_sign_key(build_prop):
    user_profile_path = os.environ["USERPROFILE"]
    addons_key_filepath = os.path.join(user_profile_path,
        "YYBuild",
        "addons")
    if not os.path.isfile(addons_key_filepath):
        raise BuildException("signature: addons's key file is not exist")
    build_prop.package.__dict__["addons_key_filepath"] = addons_key_filepath
