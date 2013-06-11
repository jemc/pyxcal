#
# pyxcal
#
#   Python extension module for manipulating 
#   X gamma ramps with libXxf86vm.
#
# 2012, Joe McIlvain
#   All rights temporarily reserved 
#   until permanent license is applied
#

from distutils.core import setup, Extension
setup(name='xcal', version='1.0',  \
    ext_modules=[Extension( \
        'xcal', \
        ['pyxcal.c'], \
        libraries=['Xxf86vm'] \
        )])
