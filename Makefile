hw5: hw5.o tablecheck.o crypt.o utility.o
	g++ -g -W hw5.o tablecheck.o crypt.o utility.o -o hw5

tablecheck.o: tablecheck.h tablecheck.cpp
	g++ -g -W -c tablecheck.cpp

crypt.o: crypt.h crypt.cpp
	g++ -g -W -c crypt.cpp

utility.o: utility.h utility.cpp
	g++ -g -W -c utility.cpp

hw5.o: hw5.cpp
	g++ -g -W -c hw5.cpp

clean:
	rm -f *.o *.gch hw5
