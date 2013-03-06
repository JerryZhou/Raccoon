#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""构建异常。构建过程中遇到不可恢复的错误时，通过抛出异常来使构建正确退出"""

class BuildException(Exception):
    pass
