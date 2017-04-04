.PHONY: all clear

CXX=c++ -Wall -O3  -std=c++11 -g -I../include/

all: xml2cxx isp_db.o

isp_db.cc: xml2cxx
	./xml2cxx ../../autoconfig/* > $@

xml2cxx: xml2cxx.o tinyxml2.o account_settings_internal.o stringpool.o
	${CXX}  -o $@ $^

%.o : %.cc %.hh
	${CXX} -o $@ -c $<

clean:
	rm -vf *.o \
		isp_db.cc \
		xml2cxx

