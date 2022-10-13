CC=gcc
CXX=g++
INCLUDE = -Ipath
CXXFLAGS=-fno-stack-limit -g -w
#CFLAGS=-g -fno-stack-protector -static-libstdc++ -lstdc++ -w
LD=ld
CFLAGS=-g -fno-stack-protector -lstdc++ -w -lpthread
SOURCE=$(wildcard *.cpp) 
HEADERS=headers

TARGET=main.o

#PATH=fstream/

all: $(TARGET) install 

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) -I $(HEADERS)
install: $(TARGET)
	mkdir -p build
	mv $^ ./build/

run: $(TARGET)
	./build/$^ || /bin/true

clean:
	rm -f *.o 2> /dev/null
	#rm -f *.txt 2> /dev/null
	rm -rf build
	

	
