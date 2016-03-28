MAIN=build array_to_numpy.cpp array_to_numpy.so
SRC=array_to_numpy.pyx cpp_array.cpp setup.py

all: $(MAIN)

$(MAIN) : $(SRC)
	rm -r build; python setup.py build_ext --inplace

.PHONY: clean

clean:
	rm -rf $(MAIN)
