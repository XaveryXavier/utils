CPPFLAGS=-I/. -I/usr/include `root-config --cflags`
LDFLAGS=`root-config --glibs`
LDLIBS=-L/usr/lib64 -lcurl
run: test.o OfflineCalib.o
		g++ $(LDFLAGS) -o run test.o OfflineCalib.o $(LDLIBS)
test.o : test.cxx OfflineCalib.h
		g++ $(CPPFLAGS) -c test.cxx
OfflineCalib.o : OfflineCalib.cxx OfflineCalib.h
		g++ $(CPPFLAGS) -c OfflineCalib.cxx
		
.PHONY: clean
clean:
		rm -f run *.o
