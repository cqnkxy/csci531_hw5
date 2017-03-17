OBJS = hw5.o tablecheck.o crypt.o utility.o modprod.o keyexpand.o \
	config.o inverse.o
CXXFLAGS = -g -W
CXX = g++

hw5: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o hw5

tablecheck.o: tablecheck.h tablecheck.cpp
	$(CXX) $(CXXFLAGS) -c tablecheck.cpp
crypt.o: crypt.h crypt.cpp
	$(CXX) $(CXXFLAGS) -c crypt.cpp
utility.o: utility.h utility.cpp
	$(CXX) $(CXXFLAGS) -c utility.cpp
modprod.o: modprod.h modprod.cpp
	$(CXX) $(CXXFLAGS) -c modprod.cpp
keyexpand.o: keyexpand.h keyexpand.cpp
	$(CXX) $(CXXFLAGS) -c keyexpand.cpp
config.o: config.h config.cpp
	$(CXX) $(CXXFLAGS) -c config.cpp
inverse.o: inverse.h inverse.cpp
	$(CXX) $(CXXFLAGS) -c inverse.cpp
hw5.o: hw5.cpp
	$(CXX) $(CXXFLAGS) -c hw5.cpp

clean:
	rm -f *.o *.gch hw5
