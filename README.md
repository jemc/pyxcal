# pyxcal

Python extension module for manipulating X gamma ramps with libXxf86vm.

(Inspired by [xcalib](http://xcalib.sourceforge.net/))

## Status

The pyxcal module is functional, but currently feature-incomplete.  Documentation of the features is yet-to-come.  Stand by for updates, and if the progress of this project is important to you, please let me know and I can make it a higher priority.

## Installing

Build and install this extension module in the usual way:
```
$ hg clone https://bitbucket.org/jemc/pyxcal
$ cd pyxcal
$ su -c "python setup.py install"
```
Note that if you have multiple versions of Python installed, the module is only built for the version you use to call setup.py.  The module is developed and tested with Python 3.x and 2.7.

You'll need gcc to build it, as well as the python and libXxf86vm headers.  
For me, on Fedora, this can be done with:
```
$ su -c "yum install gcc python-devel python3-devel libXxf86vm-devel"
```
Of course, you only need to install the python headers for the version of python you intend to use.

