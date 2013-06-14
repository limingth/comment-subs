SRC=$(wildcard *.dot)
JPG=$(SRC:%.dot=%.jpg) 
dotname=$(SRC:%.dot=%) 


all: $(JPG)
	

%.jpg : %.dot
	dot -Grankdir=LR -Tjpg $< -o $@

loop:
	@for i in $(dotname) ;\
	do \
		echo + $$i.jpg		;\
		make $$i.jpg	;\
	done

%:
	dot -Tjpg $@.dot -o $@.jpg
#	gnome-open $@.jpg
	
c clean:
	rm *.jpg
