#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""构建所需要的属性，以dict存储，以便直接读成BuildProperties对象"""

import os
from os.path import join
import locale

import version

build_config = {

    "version" : version.version_config,

    "phase_list" : [
        "configure",
        "clean",
        "compile",
        "test",
        "package",
        "cppcheck",
    ],

    "phase_run_order" : [],

    # 脚本中可能用到的目录，一般只放一级目录
    # 一级目录以下的目录交友各个phase自己管理
    "path" : {
        # 存放构建结果
        "dist_dir" : "dist",
        "depends_dir" : "depends",
        "framework_dir" : "framework",
        "app_dir" : "app",
        "ext_dir" : "ext",
        "mw_dir" : "mw",
        "tools_dir" : "tools",
        "setup_dir" : join("tools", "setup4.0"),
        "crash_report_dir" : join("tools", "crashreport"),
    },

    "configure" : {
        # 各个依赖库的版本配置
        "depends_config" : "build/buildtool/config/depend.list",

        # 远程repo的url
        "repo_url_root" : "http://repo.yypm.com/dwintegrate",
    },

    "cppcheck" : {
        # CppCheck 是否打开 --enable-all选项
        "cppcheck_all" : False,
        "cppcheck_xml" : False,
    },

    "compile" : {
        # 是否重新编译
        "recompile" : False,
    },

    "package" : {
        # 是否设置打包密码
        "password" : False,

        # 是否打上数字签名
        "signature" : False,

        # 是否在打数字签名上加上时间戳
        # 正式版本必须有时间戳
        # 由于时间戳服务在美国，平时可以不打
        "sign_with_timestamp" : False,

        # 是否进行版本号注入，将会影响productinfo和protocol的version
        "inject_version" : False,
    },

    "share" : {
        "build_exe" : "",

        # official_build 为True时，不带accessible，不带开发用的私钥
        "official_build" : False,

        # 多CPU build
        "multi_cpu_build" : False,

        # code coverage
        "coverage" : False
    },

    # 对于文件路径的日志输出，需要转成unicode来处理
    # 对于环境变量中与路径相关的值，使用操作系统默认的encoding
    #   命令行传入参数是 gbk
    #   直接在环境变量中设置，也是gbk
    #   可以直接只用中文的环境变量值读写文件
    "os_encoding" : locale.getpreferredencoding(),

}
