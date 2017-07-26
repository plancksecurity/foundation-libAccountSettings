.PHONY: all clear isp_db test

CXX=c++ -Wall -O3  -std=c++11 -g -I../include/

all: libAccountSettings.a test

test: accountSettings_test
	./accountSettings_test

isp_db: isp_db.cc

isp_db.cc: xml2cxx
	./xml2cxx ../../autoconfig/* > $@

xml2cxx: xml2cxx.o tinyxml2.o account_settings_internal.o stringpool.o
	${CXX}  -o $@ $^

accountSettings_test: accountSettings_test.o
	${CXX}  -o $@ $^ ./libAccountSettings.a

libAccountSettings.a: account_settings_internal.o implementation.o \
      isp_db.o \
      from_srv.o
	ar rcs $@ $^

%.o : %.cc %.hh
	${CXX} -o $@ -c $<

clean:
	rm -vf *.o \
		*.a \
		xml2cxx

