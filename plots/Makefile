YEAR_MONTH := $(shell date +%Y-%m)
YEAR_MONTH_PREV := $(shell date -v-1m +%Y-%m)

DATAFILES = ../data/gvh507x_A4C138258E2F-${YEAR_MONTH}.txt \
	    ../data/gvh507x_A4C138E4544D-${YEAR_MONTH}.txt

FILES = "'../data/gvh507x_A4C138258E2F-${YEAR_MONTH_PREV}.txt' '../data/gvh507x_A4C138258E2F-${YEAR_MONTH}.txt'" \
	"'../data/gvh507x_A4C138E4544D-${YEAR_MONTH_PREV}.txt' '../data/gvh507x_A4C138E4544D-${YEAR_MONTH}.txt'"

all:    govee-${YEAR_MONTH}.pdf govee-${YEAR_MONTH_PREV}-${YEAR_MONTH}.pdf

govee-${YEAR_MONTH}.tex: govee-single.gnuplot ${DATAFILES} sync
	gnuplot -c $< $@  ${DATAFILES}

govee-${YEAR_MONTH_PREV}-${YEAR_MONTH}.tex: govee.gnuplot sync
	gnuplot -c $< $@  ${FILES}

sync:
	${MAKE} -C ../

clean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux

veryclean:
	-rm -rf *-eps-converted-to.pdf *-inc.eps *.tex *.eps *~ *.log *.aux *.pdf

%.pdf: %.tex ; pdflatex $<

.PHONY: clean sync
