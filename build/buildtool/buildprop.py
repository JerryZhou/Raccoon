#! /usr/bin/env python
# -*- coding: utf-8 -*-

import StringIO

u"""将一个dict转成BuildProperties对象"""

class BuildProperties(object):

    def __init__(self, dict_data):
        for k, v in dict_data.iteritems():
            setattr(self, k, BuildProperties(v) if isinstance(v, dict) else v)

    def __str__(self):
        result = StringIO.StringIO()
        self._dump(result, self.__dict__, 0)
        return result.getvalue()

    def _dump(self, output, data, indent):
        for k, v in data.iteritems():
            if isinstance(v, BuildProperties):
                output.write("%s%s =>\n" % (" " * indent, k))
                self._dump(output, v.__dict__, indent + 4)
            else:
                output.write("%s%s => %s\n" % (" " * indent, k, v))
