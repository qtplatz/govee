YEAR_MONTH := $(shell date +%Y-%m)

DATAFILES = ../data/gvh507x_A4C138258E2F-${YEAR_MONTH}.txt \
	    ../data/gvh507x_A4C138E4544D-${YEAR_MONTH}.txt

all:    gobee-log-${YEAR_MONTH}.pdf

gobee-log-${YEAR_MONTH}.tex: gobee-log.gnuplot ${DATAFILES} sync
	gnuplot -c $< $@  ${DATAFILES}

sync:
	${MAKE} -C ../

clean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux

veryclean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux *.pdf

%.pdf: %.tex ; pdflatex $<

.PHONY: clean sync
