SRC=$(wildcard *.dot)
JPG=$(SRC:%.dot=%.jpg) 
dotname=$(SRC:%.dot=%) 


all: $(JPG)
	dot -Tjpg kmod_module_get_probe_list.dot -o kmod_module_get_probe_list.jpg
	dot -Tjpg 0-overview.dot -o 0-overview.jpg
	dot -Tjpg 1-app.dot -o 1-app.jpg
	dot -Tjpg kmod_module_probe_insert_module.dot -o kmod_module_probe_insert_module.jpg
	dot -Tjpg cmd.dot -o cmd.jpg

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
	am *.jpg
