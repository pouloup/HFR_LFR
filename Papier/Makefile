PROJET=article_3
ARGS=-shell-escape
#ARGS=-shell-escape -interaction=batchmode

all: pre_clean pre_pdf bibtex pdf pdf_second_pass clean
		echo Done

pre_clean:
		rm -f *~ *.backup *.aux *.toc *.blg *.log *.out *.bbl *.dvi *.nav *.pyg *.snm *.lof *.acn *.acr *.alg *.glg *.glo *.gls *.ist

clean:
		rm -f *~ *.backup *.aux *.toc *.blg *.log *.out *.bbl *.dvi *.nav *.pyg *.snm *.lof *.acn *.acr *.alg *.glg *.glo *.gls *.ist

pre_pdf:
		pdflatex $(ARGS) $(PROJET)

pdf:
		pdflatex $(ARGS) $(PROJET)

pdf_second_pass:
		pdflatex $(ARGS) $(PROJET)

glossary:
		makeglossaries $(PROJET)

bibtex:
		bibtex $(PROJET)

