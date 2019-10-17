.PHONY: all clear isp_db test

CXX=c++ -Wall -O0  -std=c++11 -g -I../include/ -I/usr/local/include -L/usr/local/lib

all: libAccountSettings.a test

test: accountSettings_test
	./accountSettings_test

isp_db: isp_db.cc

isp_db.cc: xml2cxx
	./xml2cxx ../../autoconfig/* ../example/* > $@

xml2cxx: xml2cxx.o tinyxml2.o account_settings_internal.o stringpool.o
	${CXX}  -o $@ $^

accountSettings_test: accountSettings_test.o libAccountSettings.a
	${CXX}  -o $@ $^  -lldns

libAccountSettings.a: account_settings_internal.o implementation.o \
      isp_db.o http_client.o \
      from_srv.o
	${AR} rcs $@ $^

%.o : %.cc %.hh
	${CXX} -o $@ -c $<

clean:
	rm -vf *.o \
		*.a \
		xml2cxx

