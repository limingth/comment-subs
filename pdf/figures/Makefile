SRC=$(wildcard *.dot)
dotname=$(SRC:%.dot=%) 

all:
	@for i in $(dotname) ;\
	do \
		echo + $$i.jpg		;\
		dot -Grankdir=LR -Tjpg $$i.dot -o $$i.jpg	;\
	done

%:
	dot -Tjpg $@.dot -o $@.jpg
#	gnome-open $@.jpg
	
c clean:
	rm *.jpg
