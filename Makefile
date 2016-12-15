CC=gcc
CFLAGS=-l
CXXFLAGS=-Wall -ldb
srv: srv.c ed.c querydb.c addtodb.c 
	$(CC) -o srv srv.c ed.c querydb.c addtodb.c $(CXXFLAGS)
