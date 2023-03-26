all: test.cpp
	g++ -o test test.cpp -std=c++11 -ljsoncpp

report: report.tex
	xelatex report.tex
	rm -f report.out
	rm -f report.log
	rm -f report.aux
	rm -f report.fdb_latexmk
	rm -f report.fls

run:
	
clean:
	rm -r test
	rm -r report.pdf