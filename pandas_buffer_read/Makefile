CC=g++
CFLAGS=-g -O3 -std=c99 --float-store -fPIC
LDFLAGS=-shared
MAIN=build page_read.cpp page_read.so
SRC=page_read.pyx interface.cpp setup.py
FILE_READER_SRC=src/file_reader.cpp
FILE_READER_OBJ=src/file_reader.o
FILE_READER_TGT=lib/libfilereader.so

all: $(MAIN)

$(FILE_READER_OBJ) : $(FILE_READER_SRC)
	$(CC) $(CFLAGS) -o $@ -c $< 

$(FILE_READER_TGT) : $(FILE_READER_OBJ)
	$(CC) $(LDFLAGS) $< -o $@

$(MAIN) : $(SRC) $(FILE_READER_TGT)
	rm -r build; python setup.py build_ext --inplace

.PHONY: clean

clean:
	rm -rf $(MAIN) $(FILE_READER_TGT) $(FILE_READER_OBJ)
