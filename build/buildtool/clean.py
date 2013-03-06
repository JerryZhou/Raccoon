#! /usr/bin/env python
# -*- coding: utf-8 -*-

u"""清理构建产生的文件"""

import butil

def clean(build_prop):
    u"""清理构建产生的文件"""
    butil.delete_dir(build_prop.path.dist_dir)
