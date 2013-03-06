#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""构建脚本存放在build/buildtool目录中。build.py是脚本的入口"""

import sys
import os
import platform

# 将build目录添加python路径中
sys.path.append("build")

from buildtool import alltasks

def main():

    # 不支持cygwin环境
    if sys.platform != "win32":
        print "the build script requires win32, \
but your platform is %s" % sys.platform
        exit(-1)

    # 必须是python 2.7.x或更高的版本
    (major, minor, patchlevel) = platform.python_version_tuple()
    if major != '2' or \
        (major == '2' and minor < '7'):
        print "require python 2.7.x or above, but your python version is %s" % platform.python_version()
        exit(-1)

    alltasks.build()

if __name__ == '__main__':
    main()
