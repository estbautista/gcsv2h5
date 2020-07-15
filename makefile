CC = g++
CFLAGS = -std=c++11 -O3 -march=native
VPATH = src include
LFLAGS = -lz -ldl -lm -L/usr/local/hdf5/lib -lhdf5_cpp -lhdf5
HFLAGS = -I./include -I/usr/local/hdf5/include
EXEC_TARGET = gcsv2h5
OBJECTS = main.o gcsv2h5.o H5FileSetup.o parseFeaturesFile.o parseEdgeFile.o

all : $(EXEC_TARGET)

$(EXEC_TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o $@

%.o : %.cpp %.hpp
	$(CC) $(CFLAGS) $(HFLAGS) -c $< -o $@

main.o : main.cpp
	$(CC) $(CFLAGS) $(HFLAGS) -c $< -o $@

clean : 
	rm -f *.o
