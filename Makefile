all:
	cd ./proto && make
	cd ./src && make

clean:
	cd ./proto && make clean
	cd ./src && make clean

