all:
	g++ -g main_test.cpp -o main_test -lpthread

clean:
	rm -rf main_test 
