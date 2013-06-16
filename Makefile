
all:
	@echo make s to ./s.sh kmod-11
	@echo make b to make -C kmod-11
	@echo make p to make -C pdf kmod
	@echo make f to make -C pdf/figures 
	@echo make d to execute ../doxygen
	@echo make c to git commit

s subs:
	./s.sh kmod-11
	cp kmod-11-subs ../doxygen/ -R

b build:
	make -C kmod-11

.PHONY: pdf
p pdf: f
	make -C pdf kmod

f figure:
	make -C pdf/figures

d doxy:
	cd ../doxygen && doxygen

c commit: 
	git add .
	git commit -a -m "M kmod.md"
	git push

t test:
	sudo ./kmod-11/tools/modprobe nfs sunrpc
