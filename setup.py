# . 
# . Copyright 2012 Joe McIlvain
# . 
# .    Licensed under the Apache License, Version 2.0 (the "License");
# .    you may not use this file except in compliance with the License.
# .    You may obtain a copy of the License at
# . 
# .        http://www.apache.org/licenses/LICENSE-2.0
# . 
# . pyxcal
# .     Python extension module for manipulating 
# .     X gamma ramps with libXxf86vm.
# .
# .     (Inspired by xcalib (http://xcalib.sourceforge.net/))
# . 
# . setup.py
# .     Run 'python setup.py install' to build and install module
# . 

from distutils.core import setup, Extension
setup(name='xcal', version='1.0',  \
    ext_modules=[Extension( \
        'xcal', \
        ['pyxcal.c'], \
        libraries=['Xxf86vm'] \
        )])
