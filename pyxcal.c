// . 
// . Copyright 2012 Joe McIlvain
// . 
// .    Licensed under the Apache License, Version 2.0 (the "License");
// .    you may not use this file except in compliance with the License.
// .    You may obtain a copy of the License at
// . 
// .        http://www.apache.org/licenses/LICENSE-2.0
// . 
// . pyxcal
// .     Python extension module for manipulating 
// .     X gamma ramps with libXxf86vm.
// .
// .     (Inspired by xcalib (http://xcalib.sourceforge.net/))
// . 
// . pyxcal.c
// .     C extension module, to be built by setup.py
// . 


#include <Python.h>

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>

#ifdef FGLRX
# include <fglrx_gamma.h>
#endif




PyObject *PyList_from_UShorts(unsigned short array[], int length)
{
    PyObject *pylist, *item;
    int i;
    pylist = PyList_New(length);
    if (pylist != NULL) {
        for (i=0; i<length; i++) {
            item = Py_BuildValue("H", array[i]);
            PyList_SET_ITEM(pylist, i, item);
        }
    }
    return pylist;
  }




static PyObject*
xcal_testerror(PyObject *self)
{
    PyErr_SetString(PyExc_ArithmeticError, "Blah, blah, erroneous blah.");
    return NULL;
}


static PyObject*
xcal_setgamma(PyObject *self, PyObject *args)
{
    int sts;
    Display* dpy;
    int screen;
    XF86VidModeGamma gamma;

    // Parse args as three floats into gamma
    PyArg_ParseTuple(args, "fff", 
        &gamma.red, &gamma.green, &gamma.blue);
    
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
        
    sts = XF86VidModeSetGamma(dpy, screen, &gamma);
    
    XCloseDisplay(dpy);
    
    return Py_BuildValue("");
}

static PyObject*
xcal_getrampsize(PyObject *self)
{
    Display* dpy;
    int screen;
    unsigned int ramp_size;
    
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
    
    XF86VidModeGetGammaRampSize(dpy, screen, &ramp_size);
    
    XCloseDisplay(dpy);
    
    return Py_BuildValue("I", ramp_size);
}

static PyObject*
xcal_getramps(PyObject *self)
{
    Display* dpy;
    int screen;
    unsigned int ramp_size;
    unsigned short *r_ramp, *g_ramp, *b_ramp;
    PyObject *pyr_ramp, *pyg_ramp, *pyb_ramp, *pylist;
    
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
    
    XF86VidModeGetGammaRampSize(dpy, screen, &ramp_size);
    
    r_ramp = (unsigned short*) malloc(ramp_size * sizeof(unsigned short));
    g_ramp = (unsigned short*) malloc(ramp_size * sizeof(unsigned short));
    b_ramp = (unsigned short*) malloc(ramp_size * sizeof(unsigned short));
      
    XF86VidModeGetGammaRamp(dpy, screen, ramp_size, r_ramp, g_ramp, b_ramp);
    
    pyr_ramp = PyList_from_UShorts(r_ramp, ramp_size);
    pyg_ramp = PyList_from_UShorts(g_ramp, ramp_size);
    pyb_ramp = PyList_from_UShorts(b_ramp, ramp_size);
    
    pylist = PyList_New(3);
    PyList_SET_ITEM(pylist, 0, pyr_ramp);
    PyList_SET_ITEM(pylist, 1, pyg_ramp);
    PyList_SET_ITEM(pylist, 2, pyb_ramp);
    
    free(r_ramp);
    free(g_ramp);
    free(b_ramp);
    
    XCloseDisplay(dpy);
    
    return pylist;
}

static PyObject*
xcal_setramps(PyObject *self, PyObject *args)
{
    Display* dpy;
    int screen;
    int i = 0;
    Py_ssize_t      r_num,   g_num,   b_num;
    unsigned short *r_ramp, *g_ramp, *b_ramp;
    PyObject *pylist, *pyr_ramp, *pyg_ramp, *pyb_ramp;
    
    if(!PyArg_ParseTuple(args, "O!", 
        &PyList_Type, &pylist))
            return NULL;
    
    if(PyList_Size(pylist) != 3)
        return NULL;
    
    pyr_ramp = PyList_GetItem(pylist, 0);
    pyg_ramp = PyList_GetItem(pylist, 1);
    pyb_ramp = PyList_GetItem(pylist, 2);
    
    r_num = PyList_Size(pyr_ramp);
    g_num = PyList_Size(pyg_ramp);
    b_num = PyList_Size(pyb_ramp);
    
    r_ramp = (unsigned short*) malloc(r_num * sizeof(unsigned short));
    g_ramp = (unsigned short*) malloc(g_num * sizeof(unsigned short));
    b_ramp = (unsigned short*) malloc(b_num * sizeof(unsigned short));
    
    screen = (int)r_num;
    
    for(i=0; i<r_num; i++)
        r_ramp[i] = (short)(PyLong_AsLong(PyList_GetItem(pyr_ramp, i)));
        
    for(i=0; i<g_num; i++)
        g_ramp[i] = (short)(PyLong_AsLong(PyList_GetItem(pyg_ramp, i)));
        
    for(i=0; i<b_num; i++)
        b_ramp[i] = (short)(PyLong_AsLong(PyList_GetItem(pyb_ramp, i)));
    
    dpy = XOpenDisplay(NULL);
    screen = DefaultScreen(dpy);
    
    if((r_num==g_num)&&(r_num==b_num))
        XF86VidModeSetGammaRamp(dpy, screen, r_num, r_ramp, g_ramp, b_ramp);
    
    free(r_ramp);
    free(g_ramp);
    free(b_ramp);

    XCloseDisplay(dpy);
    
    return Py_BuildValue("");
}

static PyMethodDef xcal_methods[] = {
    
    {"setgamma",  xcal_setgamma, METH_VARARGS,
     "Change the screen gamma."},
    {"getrampsize",  xcal_getrampsize, METH_NOARGS,
     "Get the screen gamma ramp size."},
    {"getramps",  xcal_getramps, METH_NOARGS,
     "Get the screen gamma ramps."},
    {"setramps",  xcal_setramps, METH_VARARGS,
     "Set the screen gamma ramps."},
    {"testerror",  xcal_testerror, METH_NOARGS,
     "Throws an example error."},
     
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef xcal_module = {
   PyModuleDef_HEAD_INIT,
   "xcal",   // name of module
   NULL,     // module documentation, may be NULL
   -1,       // size of per-interpreter state of the module,
               // or -1 if the module keeps state in global variables.
   xcal_methods
};

PyMODINIT_FUNC
PyInit_xcal(void)
{
    return PyModule_Create(&xcal_module);
}
