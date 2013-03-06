#! /usr/bin/env python
# -*- coding: utf-8 -*-

import datetime

# 每个发布版本都可能发生改变的数据
# 注意：
#   中文数据需要用 u""
#   使用windows换行符
#   使用4空格缩进

#===============================================================================
# 以下变量只供内部使用，其他module不要直接依赖
_release_name = "dev" # 只用于在线升级
_product_version_full = "dev" # 用于生成完整版本号，如 3.9.0.1
_product_version_title = u"内测" # 只用于界面显示，对于3.9.0.1版本，这个数值应该是 3.9
_product_version_stat = "dev" # 只用于在统计系统中唯一地标识一个版本
_product_version_unique = "39000000" # 只用于在线更新和版本间数据迁移时，唯一地标识一个版本
_download_server = "http://update.office.duowan.local"
#===============================================================================

version_config = {

    "about" : {
        "real_product_version_full" : _product_version_full,
        "build_date" : datetime.datetime.now().strftime("%Y.%m.%d"),
        "last_commit_rev" : "[rev]",
        "real_release_name" : _release_name,
        "real_product_version_title" : _product_version_title,
        "real_product_version_stat" : _product_version_stat,
        "real_product_version_unique" : _product_version_unique,
        "real_download_server_url" : _download_server,
        # 后台升级的域名，正式版是：http://update.yy.duowan.com
        "real_update_server_url" : "http://update.office.duowan.local",
        # 前台升级的域名，正式版是：http://frontupdate.yy.duowan.com
        "real_frontupdate_server_url" : "http://update.office.duowan.local",
        # 强制升级的域名，正式版是：http://forceupdate.yy.duowan.com
        "real_forceupdate_server_url" : "http://update.office.duowan.local",
    },

    "liveupdate" : {
        # 配置update.xml中的ver属性
        # DailyBuild时，使用构建系统的build number自动进行设置
        # 正式发布时，根据需要修改这个数字
        "base_update_ver" : "0",
    },

    "setup" : {
        # 修改安装包的config.h文件
        "config" : {
            "release_name" : _release_name,
            "product_version" : _product_version_full,
            "product_version_stat" : _product_version_stat,
            "register_name" : "YY%s" % _release_name,
            "shortcut_name" : u"YY语音",
            "exe_download_url" :
                "%s/%s/setup/YYSetup.exe" % (_download_server, _release_name),
        },

        # 修改安装包的setup.rc文件
        "install" : {
            "name" : u"YY 安装程序",
            "copyright" : u"版权所有 (C) 2007-2011 多玩游戏网",
        },

        # 修改安装包的uninstall.rc文件
        "uninstall" : {
            "name" : u"YY 卸载程序",
            "copyright" : u"版权所有 (C) 2007-2011 多玩游戏网",
        },
    },

}
