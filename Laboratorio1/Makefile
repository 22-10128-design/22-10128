all:
	g++ -Dmain=lab1_main lab1.cpp -c -o lab1.o
	g++ test.cpp lab1.o -o test

clean:
	rm -f *.o test
	rm lab1.hpp.gch

run: all
	./test
