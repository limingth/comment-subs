
all:
	./s.sh kmod-11

build:
	make -C kmod-11

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

commit: 
	git add .
	git commit -a -m "M kmod.md"
	git push

