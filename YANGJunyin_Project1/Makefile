all: 
	g++ -o Main Main.cpp -std=c++14 -ljsoncpp
	g++ -o test test.cpp -std=c++14 -ljsoncpp

report: report.tex
	xelatex report.tex
	rm -f report.out
	rm -f report.log
	rm -f report.aux
	rm -f report.fdb_latexmk
	rm -f report.fls
	
clean:
	rm -r test
	rm -r report.pdf