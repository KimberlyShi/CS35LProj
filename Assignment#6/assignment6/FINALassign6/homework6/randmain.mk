randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain

randmain.o: randmain.c randcpuid.h
	$(CC) $(CFLAGS) -c randmain.c

randcpuid.o: randcpuid.h
	$(CC) $(CFLAGS) -c randcpuid.c

randlibhw.o: randlibhw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c

randlibhw.so: randlibhw.o
	$(CC) $(CFLAGS) -fPIC -c randlibhw.o -o randlibhw.so

randlibsw.o: randlibsw.c randlib.h
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c

randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS) -shared randlibsw.o -o randlibsw.so
