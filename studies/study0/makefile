all: hello hello_workaround
	./hello
	./hello_workaround

hello: hello.cpp
	g++ -std=c++0x -o $@ $< -lOpenCL

hello_workaround: hello_workaround.cpp
	g++ -std=c++0x -o $@ $< -lOpenCL
