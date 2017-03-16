OBJS = hw5.o tablecheck.o crypt.o utility.o modprod.o
FLAGS = -g -W
CXX = g++

hw5: $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o hw5

tablecheck.o: tablecheck.h
crypt.o: crypt.h
utility.o: utility.h
modprod.o: modprod.h
hw5.o: hw5.cpp

clean:
	rm -f *.o *.gch hw5
