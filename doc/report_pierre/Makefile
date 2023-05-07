FIGS =	\
	by-nc.pdf \
	logo-uds.pdf \
	logo-ufr.pdf \
	choc-nobel.jpg \
	crit-eval.pdf \


mem-ter.pdf: mem-ter.tex mem-ter.bib $(FIGS)
	pdflatex mem-ter
	bibtex mem-ter
	pdflatex mem-ter
	bibtex mem-ter
	pdflatex mem-ter


clean:
	rm -f *.aux *.bbl *.blg *.log *.toc *.out
	rm -f mem-ter.pdf
