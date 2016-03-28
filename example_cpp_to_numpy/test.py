#!/usr/bin/env python

"""
simple test of the array_to_numpy.pyx and c_array.c test code
"""


import numpy as np

import array_to_numpy

a = array_to_numpy.get_array(4, 2.0)

print a.astype(float)

