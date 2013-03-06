#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""整个构建流程"""

import logging
import logging.config
import sys
import datetime
import subprocess
import os

import clean
import cli
import compile_phase
import package_phase
import cppcheck_phase
import configure_phase
from buildexp import BuildException
from buildprop import BuildProperties
from config.buildconfig import build_config

def build():
    u"""整个构建流程"""

    begin_time = datetime.datetime.now()
    _init_logging()

    logger = logging.getLogger(__name__)
    try:
        _build()
    except BuildException as e1:
        logger.exception(e1)
        logger.debug("exit with error code: 1")
        sys.exit(1)
    except Exception as e2:
        logger.exception(e2)
        raise
    finally:
        end_time = datetime.datetime.now()
        duration_seconds = _calc_build_time(end_time - begin_time)
        logger.info("build time: %s seconds" % duration_seconds)
        logger.debug("logging is going to shutdown")
        logging.shutdown()

def _init_logging():
    logging.config.fileConfig("build/buildtool/logging.conf")

def _build():
    logger = logging.getLogger(__name__)

    #TODO:
    #每种phase都需要有提示
    build_prop = BuildProperties(build_config)

    cli.parse_user_input(build_prop)
    logger.debug("parsed user input")

    _determine_build_exe(build_prop)

    _dump_build_props(build_prop)

    for phase in build_prop.phase_run_order:
        logger.debug("phase [%s] start ..." % phase)
        if phase == "configure":
            configure_phase_ = configure_phase.ConfigurePhase(build_prop)
            configure_phase_.configure()
        if phase == "clean":
            clean.clean(build_prop)
        if phase == "compile":
            compile_phase_ = compile_phase.CompilePhase(build_prop)
            compile_phase_.compile_()
        if phase == "package":
            package_phase_ = package_phase.PackagePhase(build_prop)
            package_phase_.package()
        if phase == "cppcheck":
            cppcheck_phase_ = cppcheck_phase.CppcheckPhase(build_prop)
            cppcheck_phase_.check()
        logger.debug("phase [%s] finished" % phase)

def _calc_build_time(delta):
    seconds = delta.seconds + delta.days * 24 * 3600
    return (delta.microseconds + seconds * 10**6) / 10**6

def _determine_build_exe(build_prop):
    logger = logging.getLogger(__name__)

    # default is devenv.com and local build
    # 只有在cli完成对用户输入的解析后，DEVENVDIR这个环境才能确保存在
    build_prop.share.build_exe = os.path.join(
        os.environ["DEVENVDIR"],
        "devenv.com")

    if build_prop.share.distribute_build:
        try:
            p = subprocess.Popen(["BuildConsole.exe"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            p.communicate()
            if p.returncode == 0:
                build_prop.share.build_exe = "BuildConsole.exe"
                logger.info("Use BuildConsole.exe for distribute build")
        except WindowsError as e:
            logger.info("""BuildConsole.exe is not found, rollback to devenv.com
Make sure that you have installed "IncrediBuild Agent" and set PATH variable""")
    else:
        logger.info("Use devenv.com for local build")

def _dump_build_props(build_prop):
    logger = logging.getLogger(__name__)
    logger.debug("build_prop:\n%s" % build_prop)
