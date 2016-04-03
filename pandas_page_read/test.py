#!/usr/bin/env python

"""
simple test of the page_read.pyx and c_array.c test code
"""



import page_read

pyf = page_read.create_file_reader()

df = pyf.read_page(); print df
while len(df) != 0:
    df = pyf.read_page(); print df

