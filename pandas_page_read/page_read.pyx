from cpython.mem cimport PyMem_Malloc, PyMem_Realloc, PyMem_Free
# Declare the prototype of the C function we are interested in calling
cdef extern from "src/file_reader.h":
    cdef cppclass FileReader:
        void read_page(char* filename, int page_size)    
cdef extern from "interface.cpp":
    void read_sample_page(void* file_reader, int*& a, int*&b, int& size)

from libc.stdlib cimport malloc, free
from cpython cimport PyObject, Py_INCREF

# Import the Python-level symbols of numpy
import numpy as np

# Import the C-level symbols of numpy
cimport numpy as np

import pandas as pd

# Numpy must be initialized. When using numpy from C or Cython you must
# _always_ do that, or you will have segfaults
np.import_array()

# We need to build an array-wrapper class to deallocate our array when
# the Python object is deleted.

cdef class PyFileReader:
    cdef void* data_ptr
    cdef set_data(self, void* data_ptr):
        self.data_ptr = data_ptr
    #def __dealloc__(self):
        # free(<void*>self.data_ptr)
    def read_page(self):
        cdef int *a
        cdef int *b
        cdef int size
        # Call the C function
        read_sample_page(self.data_ptr, a, b, size)
        cdef np.ndarray ndarray_a
        cdef np.ndarray ndarray_b
        ndarray_a = wrap_array(size, a)
        ndarray_b = wrap_array(size, b)
        df = pd.DataFrame({'a':ndarray_a, 'b':ndarray_b})
        del ndarray_a, ndarray_b
        return df

cdef class ArrayWrapper:
    cdef void* data_ptr
    cdef int size

    cdef set_data(self, int size, void* data_ptr):
        """ Set the data of the array
        This cannot be done in the constructor as it must recieve C-level
        arguments.
        Parameters:
        -----------
        size: int
            Length of the array.
        data_ptr: void*
            Pointer to the data            
        """
        self.data_ptr = data_ptr
        self.size = size

    def __array__(self):
        """ Here we use the __array__ method, that is called when numpy
            tries to get an array from the object."""
        cdef np.npy_intp shape[1]
        shape[0] = <np.npy_intp> self.size
        # Create a 1D array, of length 'size'
        ndarray = np.PyArray_SimpleNewFromData(1, shape,
                            np.NPY_INT , self.data_ptr)
        return ndarray

    def __dealloc__(self):
        """ Frees the array. This is called by Python when all the
        references to the object are gone. """
        #free(<void*>self.data_ptr)


cdef wrap_array(int size, void* array):
    cdef np.ndarray ndarray
    array_wrapper = ArrayWrapper()
    array_wrapper.set_data(size, <void*> array) 
    ndarray = np.array(array_wrapper, copy=False)
    # Assign our object to the 'base' of the ndarray object
    ndarray.base = <PyObject*> array_wrapper
    # Increment the reference count, as the above assignement was done in
    # C, and Python does not know that there is this additional reference
    Py_INCREF(array_wrapper)
    return ndarray


def create_file_reader():
    cdef FileReader *fr = new FileReader()
    pyf = PyFileReader()
    pyf.set_data(fr)
    return pyf

