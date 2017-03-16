OBJS = hw5.o tablecheck.o crypt.o utility.o modprod.o keyexpand.o \
	config.o
CXXFLAGS = -g -W
CXX = g++

hw5: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o hw5

tablecheck.o: tablecheck.h
crypt.o: crypt.h
utility.o: utility.h
modprod.o: modprod.h
keyexpand.o: keyexpand.h
config.o: config.h
hw5.o: hw5.cpp

clean:
	rm -f *.o *.gch hw5
