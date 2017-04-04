.PHONY: all clear

CXX=c++ -Wall -O3  -std=c++11 -g -I../include/

all: xml2cxx libAccountSettings.a

isp_db.cc: xml2cxx
	./xml2cxx ../../autoconfig/* > $@

xml2cxx: xml2cxx.o tinyxml2.o account_settings_internal.o stringpool.o
	${CXX}  -o $@ $^

libAccountSettings.a: account_settings_internal.o implementation.o isp_db.o
	ar rcs $@ $^

%.o : %.cc %.hh
	${CXX} -o $@ -c $<

clean:
	rm -vf *.o \
		*.a \
		isp_db.cc \
		xml2cxx

