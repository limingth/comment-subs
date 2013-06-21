all: 
	make new
	make test-kmod

f figures:
	make -C figures
	
%: 
	echo making $@.md to $@.pdf
	rm -rf $@.pdf
	rm -rf $@.aux
	pandoc -s $@.md -o $@.tex --number-sections
	pandoc $@.md -o $@.html
	sed -i '/setmainfont/d' $@.tex
	sed -i '2 i\\\usepackage[BoldFont,SlantFont,CJKchecksingle]{xeCJK}' $@.tex
	sed -i '4 i\\\setmainfont{Courier New}' $@.tex
	sed -i '6 i\\\setCJKmainfont{WenQuanYi Micro Hei}' $@.tex
	doconce subst '\\begin{verbatim}' '{\\begin{shaded}\\begin{verbatim}' $@.tex
	doconce subst '\\end{verbatim}' '\\end{verbatim}\\end{shaded}}' $@.tex
	doconce subst '\\begin{document}' '\\usepackage{framed,color}\n\\definecolor{shadecolor}{gray}{0.95}\n\\begin{document}' $@.tex
	xelatex $@.tex
	xelatex $@.tex
	gnome-open $@.pdf

new: figures
	pandoc kmod.md -o kmod-new.tex --chapters --number-sections
	doconce subst '\\begin{verbatim}' '{\\begin{shaded}\\begin{verbatim}' kmod-new.tex
	doconce subst '\\end{verbatim}' '\\end{verbatim}\\end{shaded}}' kmod-new.tex
	doconce subst '\\begin{document}' '\\usepackage{framed,color}\n\\definecolor{shadecolor}{gray}{0.95}\n\\begin{document}' kmod-new.tex
	xelatex kmod-main.tex
	xelatex kmod-main.tex
	gnome-open kmod-main.pdf

doc: figures
	pandoc kmod.tex -o kmod.doc

clean:
	rm *.html *.tex *.pdf *.aux *.log *.aux *.out *.old~
