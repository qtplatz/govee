
#all: tocopherol+vk1.pdf tocopherol+vk1-2.pdf ARA+C20.pdf ARA-C20.pdf
DATAFILES = ../data/gvh507x_A4C138258E2F-2022-06.txt \
	    ../data/gvh507x_A4C138E4544D-2022-06.txt

all:    gobee-log.pdf

gobee-log.tex: gobee-log.gnuplot ${DATAFILES} sync
	gnuplot -c $< $@  ${DATAFILES}

sync:
	${MAKE} -C ../

clean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux

veryclean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux *.pdf

%.pdf: %.tex ; pdflatex $<

.PHONY: clean sync
