# -*- coding: utf-8 -*-

import os

# 注意：
#   使用windows换行符
#   使用4空格缩进
#   逗号可以多加，不要误删

package_list = {

    # 打包规则：
    #   把basedir中，被include的目录或文件列出来形成列表，再从这个列表中把exclude列出的目录或文件剔除，剩下的打包到安装目录下
    #       如果指定了todir，则打包到安装目录下的todir中
    #   basedir 和 include 必须指定
    #   basedir 的路径相对于代码根目录
    #   include 中不允许带有 /
    #   include 不允许为空
    #   include 中的目录或文件名，将作为安装目录下的目录或文件名
    #   include支持简单的通配，glob语法，exclude不支持
    #   exclude 可选，
    #   .svn目录默认会被过滤，与是否写了exclude无关

    # 只在Release版中包含
    'Release' : [

        {
            'basedir' : 'app/bin/release',
            'include' : [
                'yygame_files',
                'yygame_image',
                '*.dll',
                '*.exe',
                'gamebox.zip',
            ],
            'exclude' : [
                # 如果你是开发人员，通常只需要在这里指定不想被打包进去的文件或目录
                'dwutest.exe',
                'qtdlldemo.dll',
                'yyrobot.exe',
                'yyapitest.exe',
                'addon_sign_tool.exe',
                'tinyClient.exe',
            ],
        },

        {
            'basedir' : 'app/bin/dist/files',
            'include' : [
                'imageformats',
                'QtCore4.dll',
                'QtGui4.dll',
                'QtScript4.dll',
            ],
        },

        {
            'basedir' : 'depends/dw3rd/Microsoft.VC90.CRT-9.0/bin/release',
            'include' : [
                'Microsoft.VC90.CRT',
            ],
        },

        {
            'basedir' : 'depends/dw3rd/Microsoft.VC90.CRT-9.0/bin/release',
            'todir' : 'imageformats',
            'include' : [
                'Microsoft.VC90.CRT',
            ],
        },

    ],


    # 只在Debug版中包含
    'Debug' : [

        {
            'basedir' : 'app/bin/debug',
            'include' : [
                'imageformats',
                'yygame_files',
                'yygame_image',
                '*.dll',
                '*.exe',
                'gamebox.zip',
            ],
        },

        {
            'basedir' : os.path.join(os.environ['QTDIR'], 'bin'),
            'include' : [
                'QtCored4.dll',
                'QtGuid4.dll',
                'QtScriptd4.dll',
            ],
        },

    ],

    # Debug 和 Release 均需要被打包的内容
    'Debug_and_Release' : [

        {
            'basedir' : 'app/bin/release',
            'include' : [
                'layout',
                'lang',
                'wave',
                'plugins',
            ],
        },

    ],

}

