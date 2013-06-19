all:
	make kmod

f figures:
	make -C figures
	
%: 
	echo making $@.md to $@.pdf
	pandoc -s $@.md -o $@.tex
	pandoc $@.md -o $@.html
	doconce subst '\\begin{verbatim}' '{\\begin{shaded}\\begin{verbatim}' $@.tex
	doconce subst '\\end{verbatim}' '\\end{verbatim}\\end{shaded}}' $@.tex
	doconce subst '\\begin{document}' '\\usepackage{framed,color}\n\\definecolor{shadecolor}{gray}{0.95}\n\\begin{document}' $@.tex
	xelatex $@.tex
	xelatex $@.tex
	gnome-open $@.pdf

clean:
	rm *.html *.tex *.pdf *.aux *.log *.aux *.out
