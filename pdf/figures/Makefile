SRC=$(wildcard *.dot)
JPG=$(SRC:%.dot=%.jpg) 
dotname=$(SRC:%.dot=%) 

RE_SRC=sys_sim.dot kmod_module_new.dot kmod_module_new_from_path.dot kmod_module_new_from_name.dot kmod_module_get_probe_list.dot 0-overview.dot 1-app.dot kmod_module_probe_insert_module.dot cmd.dot 3-syscall.dot
re_dotname=$(RE_SRC:%.dot=%) 

all: $(JPG) rebuild
	@echo all jpgs is generated.

rebuild: $(re_dotname)
	@for i in $(re_dotname); do dot -Tjpg $$i.dot -o $$i.jpg; done

re:
	dot -Tjpg sys_sim.dot -o sys_sim.jpg
	dot -Tjpg kmod_module_new.dot -o kmod_module_new.jpg
	dot -Tjpg kmod_module_new_from_path.dot -o kmod_module_new_from_path.jpg
	dot -Tjpg kmod_module_new_from_name.dot -o kmod_module_new_from_name.jpg
	dot -Tjpg kmod_module_get_probe_list.dot -o kmod_module_get_probe_list.jpg
	dot -Tjpg 0-overview.dot -o 0-overview.jpg
	dot -Tjpg 1-app.dot -o 1-app.jpg
	dot -Tjpg kmod_module_probe_insert_module.dot -o kmod_module_probe_insert_module.jpg
	dot -Tjpg cmd.dot -o cmd.jpg
	dot -Tjpg 3-syscall.dot -o 3-syscall.jpg

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
	rm $(JPG)
