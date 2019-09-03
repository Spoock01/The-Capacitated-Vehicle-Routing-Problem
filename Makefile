all:
	mkdir -p dist
	g++  include/*.h src/*.cpp -o dist/main.exe
	dist/main.exe

clean:
	rm -r dist
