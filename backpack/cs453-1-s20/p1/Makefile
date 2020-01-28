all: mydash-project

mydash-project:
	cd mydash-src; make

dox:
	echo "Generating documentation using doxygen..."
	doxygen doxygen-config > doxygen.log
	echo "Use konqueror docs/html/index.html to see docs (or any other browser)"

clean:
	rm mydash
	cd mydash-src; rm -f *.o *.d 



