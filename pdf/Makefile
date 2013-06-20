all:
	make kmod

f figures:
	make -C figures
	
%: 
	echo making $@.md to $@.pdf
	rm -rf $@.pdf
	rm -rf $@.aux
	pandoc -s $@.md -o $@.tex
	pandoc $@.md -o $@.html
	sed -i '/setmainfont/d' $@.tex
	sed -i '2 i\\\usepackage[BoldFont,SlantFont,CJKchecksingle]{xeCJK}' $@.tex
	sed -i '4 i\\\setmainfont{Courier New}' $@.tex
	sed -i '6 i\\\setCJKmainfont{WenQuanYi Micro Hei}' $@.tex
	doconce subst '\\begin{verbatim}' '{\\begin{shaded}\\begin{verbatim}' $@.tex
	doconce subst '\\end{verbatim}' '\\end{verbatim}\\end{shaded}}' $@.tex
	doconce subst '\\begin{document}' '\\usepackage{framed,color}\n\\definecolor{shadecolor}{gray}{0.95}\n\\begin{document}' $@.tex
	openout_any=a xelatex $@.tex
	openout_any=a xelatex $@.tex
	gnome-open $@.pdf

doc: figures
	pandoc kmod.md -o kmod.doc

clean:
	rm *.html *.tex *.pdf *.aux *.log *.aux *.out *.old~
