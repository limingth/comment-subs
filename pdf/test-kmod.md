# kmod-11 测试

## THU-12-1 下载编译源码包

### 下载源码包
	$ wget https://www.kernel.org/pub/linux/utils/kernel/kmod/kmod-11.tar.gz
	--2013-06-18 01:08:47--  https://www.kernel.org/pub/linux/utils/kernel/kmod/kmod-11.tar.gz
	Resolving www.kernel.org (www.kernel.org)... 149.20.4.69, 198.145.20.140
	Connecting to www.kernel.org (www.kernel.org)|149.20.4.69|:443... connected.
	HTTP request sent, awaiting response... 200 OK
	Length: 3458574 (3.3M) [application/x-gzip]
	Saving to: `kmod-11.tar.gz'

	100%[======================================>] 3,458,574    795K/s   in 5.3s    

	2013-06-18 01:08:54 (641 KB/s) - `kmod-11.tar.gz' saved [3458574/3458574]

查看下载文件大小 3458574字节

	$ ls -l
	total 3380
	-rw-rw-r-- 1 akaedu akaedu 3458574 Nov  8  2012 kmod-11.tar.gz
	$ 

### 解压源码包
	$ tar zxf kmod-11.tar.gz 
	$ ls
	kmod-11  kmod-11.tar.gz

查看解压后文件数量 421个

	$ cd kmod-11
	$ find * | wc  -l
	421
	$ 

### 编译项目源码
	$ ./configure CFLAGS="-g -O2" --prefix=/usr --sysconfdir=/etc --libdir=/usr/lib

查看当前目录下文件数量 确认已经生成 Makefile 

	$ find * | wc  -l
	467
	$ ls -l Makefile
	-rw-rw-r-- 1 akaedu akaedu 91795 Jun 18 01:15 Makefile
	$ make
	
查看tools目录下生成的可执行文件
	$ ls ./tools/ -l |  grep "x"
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 depmod -> kmod-nolib
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 insmod -> kmod-nolib
	-rwxrwxr-x 1 akaedu akaedu   8352 Jun 18 01:17 kmod
	-rwxrwxr-x 1 akaedu akaedu 468079 Jun 18 01:17 kmod-nolib
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 lsmod -> kmod-nolib
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 modinfo -> kmod-nolib
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 modprobe -> kmod-nolib
	lrwxrwxrwx 1 akaedu akaedu     10 Jun 18 01:17 rmmod -> kmod-nolib

安装可执行文件
	$ make install
	权限不够，需要 sudo

	$ sudo make install
	[sudo] password for akaedu: 
	Making install in .
	 /bin/mkdir -p '/usr/lib'
	 /bin/bash ./libtool   --mode=install /usr/bin/install -c   libkmod/libkmod.la '/usr/lib'
	libtool: install: /usr/bin/install -c libkmod/.libs/libkmod.so.2.2.1 /usr/lib/libkmod.so.2.2.1
	libtool: install: (cd /usr/lib && { ln -s -f libkmod.so.2.2.1 libkmod.so.2 || { rm -f libkmod.so.2 && ln -s libkmod.so.2.2.1 libkmod.so.2; }; })
	libtool: install: (cd /usr/lib && { ln -s -f libkmod.so.2.2.1 libkmod.so || { rm -f libkmod.so && ln -s libkmod.so.2.2.1 libkmod.so; }; })
	libtool: install: /usr/bin/install -c libkmod/.libs/libkmod.lai /usr/lib/libkmod.la
	libtool: finish: PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/sbin" ldconfig -n /usr/lib
	----------------------------------------------------------------------
	Libraries have been installed in:
	   /usr/lib

	If you ever happen to want to link against installed libraries
	in a given directory, LIBDIR, you must either use libtool, and
	specify the full pathname of the library, or use the `-LLIBDIR'
	flag during linking and do at least one of the following:
	   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
	     during execution
	   - add LIBDIR to the `LD_RUN_PATH' environment variable
	     during linking
	   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
	   - have your system administrator add LIBDIR to `/etc/ld.so.conf'

	See any operating system documentation about shared libraries for
	more information, such as the ld(1) and ld.so(8) manual pages.
	----------------------------------------------------------------------
	 /bin/mkdir -p '/usr/bin'
	  /bin/bash ./libtool   --mode=install /usr/bin/install -c tools/kmod '/usr/bin'
	libtool: install: /usr/bin/install -c tools/.libs/kmod /usr/bin/kmod
	make --no-print-directory install-exec-hook
	if test "/usr/lib" != "/usr/lib"; then \
			/bin/mkdir -p /usr/lib && \
			so_img_name=$(readlink /usr/lib/libkmod.so) && \
			so_img_rel_target_prefix=$(echo /usr/lib | sed 's,\(^/\|\)[^/][^/]*,..,g') && \
			ln -sf $so_img_rel_target_prefix/usr/lib/$so_img_name /usr/lib/libkmod.so && \
			mv /usr/lib/libkmod.so.* /usr/lib; \
		fi
	 /bin/mkdir -p '/usr/include'
	 /usr/bin/install -c -m 644 libkmod/libkmod.h '/usr/include'
	 /bin/mkdir -p '/usr/lib/pkgconfig'
	 /usr/bin/install -c -m 644 libkmod/libkmod.pc '/usr/lib/pkgconfig'
	Making install in libkmod/docs
	make[2]: Nothing to be done for `install-exec-am'.
	make[2]: Nothing to be done for `install-data-am'.
	Making install in man
	make[2]: Nothing to be done for `install-exec-am'.
	 /bin/mkdir -p '/usr/share/man/man5'
	 /usr/bin/install -c -m 644 depmod.d.5 modprobe.d.5 modules.dep.5 modules.dep.bin.5 '/usr/share/man/man5'
	 /bin/mkdir -p '/usr/share/man/man8'
	 /usr/bin/install -c -m 644 depmod.8 insmod.8 lsmod.8 rmmod.8 modprobe.8 modinfo.8 '/usr/share/man/man8'

## THU-12-2 测试 insmod 命令

### 编写测试用内核模块源码 hello.c 

	$ cat hello.c 

	#include <linux/module.h>
	#include <linux/kernel.h>

	MODULE_AUTHOR("AKAEDU");
	MODULE_DESCRIPTION("module example ");
	MODULE_LICENSE("GPL");

	int global = 100;

	int __init 
	akae_init (void)
	{
		int local = 200;
		printk ("Hello, akaedu\n");

		printk(".text = %p\n", akae_init);
		printk(".data = %p\n", &global);
		printk(".stack = %p\n", &local);
		return 0;
	}

	void __exit
	akae_exit (void)
	{
		int local = 300;
		printk ("module exit\n");

		printk(".text = %p\n", akae_exit);
		printk(".data = %p\n", &global);
		printk(".stack = %p\n", &local);
		return ;
	}

	module_init(akae_init);
	module_exit(akae_exit);
	$ 

### 编写测试用内核模块的 Makefile 文件 

	$ cat Makefile 

	obj-m := hello.o

	KDIR := /usr/src/linux-headers-3.2.0-29-generic-pae/

	all:
		make -C $(KDIR)	SUBDIRS=$(PWD) 	modules

	clean:
		rm -rf *.o *.ko *.mod.* *.cmd 
		rm -rf .*

	$ 

### 编译内核模块 hello.ko

	$ cd hello-module/ 
	$ make
	make -C /usr/src/linux-headers-3.2.0-29-generic-pae/	SUBDIRS=/home/akaedu/Github/comment-subs/hello-module 	modules
	make[1]: Entering directory `/usr/src/linux-headers-3.2.0-29-generic-pae'
	  CC [M]  /home/akaedu/Github/comment-subs/hello-module/hello.o
	  Building modules, stage 2.
	  MODPOST 1 modules
	  CC      /home/akaedu/Github/comment-subs/hello-module/hello.mod.o
	  LD [M]  /home/akaedu/Github/comment-subs/hello-module/hello.ko
	make[1]: Leaving directory `/usr/src/linux-headers-3.2.0-29-generic-pae'
	$ 

### 使用测试用工具 insmod 插入内核模块

	$ sudo ./kmod-11/tools/insmod hello-module/hello.ko 
	
### 查看插入内核模块后的打印结果

	$ lsmod | grep hello
	hello                  12415  0 
	$ dmesg | tail
	[350775.859640] usb 2-2.1: USB disconnect, device number 14
	[350777.611134] Bluetooth: hci0 urb c7304180 submission failed
	[350778.217886] usb 2-2.1: new full-speed USB device number 15 using uhci_hcd
	[352048.604051] usb 2-2.1: USB disconnect, device number 15
	[352048.630829] Bluetooth: hci0 urb dd3d3000 submission failed
	[352049.254135] usb 2-2.1: new full-speed USB device number 16 using uhci_hcd
	[352111.505217] Hello, akaedu
	[352111.505223] .text = e0844000
	[352111.505225] .data = e0c03000
	[352111.505227] .stack = df6e3f54
	$ 

### 重复插入同样的内核模块系统会报错

	$ sudo ./kmod-11/tools/insmod hello-module/hello.ko 
	insmod: ERROR: could not insert module hello-module/hello.ko: File exists
	$ lsmod | grep hello
	hello                  12415  0 
	

## THU-12-3 测试 rmmod 命令

### 使用测试用工具 rmmod 卸载内核模块
	$ sudo ./kmod-11/tools/rmmod hello-module/hello.ko
	$ （rmmod 命令的执行，运行在 hello 的后面加上 .ko 的后缀，这个和以前的命令有所不同）

### 查看卸载内核模块后的打印结果
	$ lsmod | grep hello
	$ （可以看到上面命令的执行结果没有任何输出信息）
	$ dmesg | tail
	[352048.630829] Bluetooth: hci0 urb dd3d3000 submission failed
	[352049.254135] usb 2-2.1: new full-speed USB device number 16 using uhci_hcd
	[352111.505217] Hello, akaedu
	[352111.505223] .text = e0844000
	[352111.505225] .data = e0c03000
	[352111.505227] .stack = df6e3f54
	[352365.795618] module exit
	[352365.795624] .text = e0c01000
	[352365.795626] .data = e0c03000
	[352365.795628] .stack = dd197f40
	$ 

## THU-12-4 测试 lsmod 命令

### lsmod 命令运行
不加参数，直接运行 lsmod ，可以显示出当前在内核中的模块情况。

	$ ./kmod-11/tools/lsmod 
	Module                  Size  Used by
	nls_iso8859_1          12617  0 
	nls_cp437              12751  0 
	usb_storage            39646  0 
	btrfs                 638208  0 
	zlib_deflate           26622  1 btrfs
	libcrc32c              12543  1 btrfs
	ufs                    78131  0 
	qnx4                   13309  0 
	hfsplus                83507  0 
	hfs                    49479  0 
	minix                  31418  0 
	ntfs                  100171  0 
	vfat                   17308  0 
	msdos                  17132  0 
	fat                    55605  2 msdos,vfat
	jfs                   175085  0 
	xfs                   747494  0 
	reiserfs              230896  0 
	ext2                   67987  0 
	usblp                  17885  0 
	vmwgfx                102138  2 
	ttm                    65344  1 vmwgfx
	drm                   197692  3 ttm,vmwgfx
	acpiphp                23535  0 
	vmw_balloon            12700  0 
	psmouse                72919  0 
	serio_raw              13027  0 
	btusb                  17912  2 
	joydev                 17393  0 
	rfcomm                 38139  0 
	bnep                   17830  2 
	bluetooth             158438  13 bnep,rfcomm,btusb
	ppdev                  12849  0 
	nfsd                  229850  13 
	nfs                   307376  0 
	lockd                  78804  2 nfs,nfsd
	fscache                50642  1 nfs
	i2c_piix4              13093  0 
	auth_rpcgss            39597  2 nfs,nfsd
	nfs_acl                12771  2 nfs,nfsd
	sunrpc                205647  19 nfs_acl,auth_rpcgss,lockd,nfs,nfsd
	parport_pc             32114  1 
	shpchp                 32325  0 
	mac_hid                13077  0 
	snd_ens1371            24819  4 
	gameport               15060  1 snd_ens1371
	snd_rawmidi            25424  1 snd_ens1371
	snd_seq_device         14172  1 snd_rawmidi
	snd_ac97_codec        106082  1 snd_ens1371
	ac97_bus               12642  1 snd_ac97_codec
	snd_pcm                80845  3 snd_ac97_codec,snd_ens1371
	snd_timer              28931  2 snd_pcm
	snd                    62064  12 snd_timer,snd_pcm,snd_ac97_codec,snd_seq_device,snd_rawmidi,snd_ens1371
	soundcore              14635  1 snd
	snd_page_alloc         14108  1 snd_pcm
	lp                     17455  0 
	parport                40930  3 lp,parport_pc,ppdev
	pcnet32                41110  0 
	usbhid                 41906  0 
	hid                    77367  1 usbhid
	mptspi                 22474  2 
	mptscsih               39530  1 mptspi
	mptbase                96852  2 mptscsih,mptspi
	floppy                 60310  0 
	vmw_pvscsi             18334  0 
	vmxnet3                44924  0 
	$ 

### lsmod 命令运行参数

该命令不支持带参数，因此后面如果跟某个模块名称，只会显示 usage ，不会显示模块的信息。

	$ ./kmod-11/tools/lsmod ufs
	Usage: ./kmod-11/tools/lsmod
	$


## THU-12-5 测试 modinfo 命令

### modinfo 命令运行参数
对于没有依赖关系的单个 .ko 内核模块，使用 modinfo 可以直接显示出模块的信息。

	$ ./kmod-11/tools/modinfo ./hello-module/hello.ko 
	filename:       ./hello-module/hello.ko
	license:        GPL
	description:    module example 
	author:         AKAEDU
	srcversion:     C928237C5C93794C5E0EF9C
	depends:        
	vermagic:       3.2.0-29-generic-pae SMP mod_unload modversions 686 
	$ 

### modinfo 命令检查依赖关系
对于有依赖关系的单个 .ko 内核模块，使用 modinfo 可以显示出模块的依赖关系信息depends，同时也可以显示出模块加载时的参数信息parm。这个参数信息是在编译内核模块的时候，源码中通过用 MODULE_PARM_DESC() 宏来指定的。

	$ modinfo /lib/modules/3.2.0-29-generic-pae/kernel/fs/nfs/nfs.ko 
	filename:       /lib/modules/3.2.0-29-generic-pae/kernel/fs/nfs/nfs.ko
	license:        GPL
	author:         Olaf Kirch <okir@monad.swb.de>
	srcversion:     BB0605CB0AF0BA47415CBEC
	depends:        fscache,sunrpc,lockd,auth_rpcgss,nfs_acl
	intree:         Y
	vermagic:       3.2.0-29-generic-pae SMP mod_unload modversions 686 
	parm:           callback_tcpport:portnr
	parm:           cache_getent:Path to the client cache upcall program (string)
	parm:           cache_getent_timeout:Timeout (in seconds) after which the cache upcall is assumed to have failed (ulong)
	parm:           enable_ino64:bool
	parm:           nfs4_disable_idmapping:Turn off NFSv4 idmapping when using 'sec=sys' (bool)
	$ 


### 查看别名信息 alias
对于可以使用别名的内核模块，也可以用它的别名 alias 来查看模块信息。别名是在 /lib/modules/3.2.0-29-generic-pae/modules.alias 描述的模块名称的简单形式。

	$ head /lib/modules/3.2.0-29-generic-pae/modules.alias
	# Aliases extracted from modules themselves.
	alias pci:v00008086d00003422sv*sd*bc*sc*i* mce_xeon75xx
	alias char-major-10-134 apm
	alias devname:cpu/microcode microcode
	alias char-major-10-184 microcode
	alias aes-asm aes_i586
	alias aes aes_i586
	alias twofish-asm twofish_i586
	alias twofish twofish_i586
	alias salsa20-asm salsa20_i586

但是在这个文件中，别名为 aes 的模块，还有很多个，通过 grep "alias aes" 可以看出一共有3个别名都是 aes 的模块，分别是 aes_i586, aesni_intel, padlock_aes。

	$ cat /lib/modules/3.2.0-29-generic-pae/modules.alias | grep "alias aes"
	alias aes-asm aes_i586
	alias aes aes_i586
	alias aes aesni_intel
	alias aes padlock_aes


### 使用 modinfo 查看各个依赖模块信息

通过 modinfo 来查看 aes 这个别名所对应的模块信息，可以看到这3个模块所对应的模块文件 crypto/aes-i586.ko，aesni-intel.ko，padlock-aes.ko 的详细信息。

	$ ./kmod-11/tools/modinfo aes 
	filename:       /lib/modules/3.2.0-29-generic-pae/kernel/arch/x86/crypto/aes-i586.ko
	alias:          aes-asm
	alias:          aes
	license:        GPL
	description:    Rijndael (AES) Cipher Algorithm, asm optimized
	srcversion:     24373C7FF739526E8AAF1B0
	depends:        
	intree:         Y
	vermagic:       3.2.0-29-generic-pae SMP mod_unload modversions 686 

	filename:       /lib/modules/3.2.0-29-generic-pae/kernel/arch/x86/crypto/aesni-intel.ko
	alias:          aes
	license:        GPL
	description:    Rijndael (AES) Cipher Algorithm, Intel AES-NI instructions optimized
	srcversion:     E0B859CB1FF480D0B70F6F2
	depends:        cryptd,aes-i586
	intree:         Y
	vermagic:       3.2.0-29-generic-pae SMP mod_unload modversions 686 

	filename:       /lib/modules/3.2.0-29-generic-pae/kernel/drivers/crypto/padlock-aes.ko
	alias:          aes
	author:         Michal Ludvig
	license:        GPL
	description:    VIA PadLock AES algorithm support
	srcversion:     6842B20FF8E68314ED45103
	depends:        
	intree:         Y
	vermagic:       3.2.0-29-generic-pae SMP mod_unload modversions 686 
	$ 

## THU-12-6 测试 depmod 命令

### depmod 命令运行时调试图

	$ ./kmod-11/tools/depmod | wc -l
	3529

	$ vi ./kmod-11/tools/depmod.c
	修改源码文件，在 output_deps 函数中间插入打印函数，打印输出到标准输出 stdout。

	1790 static int output_deps(struct depmod *depmod, FILE *out)
	1791 {
	1792         size_t i;
	1793 
	1794         fprintf(stdout, "total count %d", depmod->modules.count);
	1795 ...
	1798                 const char *p = mod_get_compressed_path(mod);
	1799                 size_t j, n_deps;
	1800 
	1801                 if (mod->dep_loop) {
	1802                         DBG("Ignored %s due dependency loops\n", p);
	1803                         continue;
	1804                 }
	1805 
	1806                 fprintf(out, "%s:", p);
	1807                 fprintf(stdout, "%s:", p);

	$ make -C kmod-11
	make[1]: Entering directory `/home/akaedu/Github/comment-subs/kmod-11'
	make --no-print-directory all-recursive
	Making all in .
	  CC       tools/depmod.o
	  CCLD     tools/kmod
	  CCLD     tools/kmod-nolib
	Making all in libkmod/docs
	make[3]: Nothing to be done for `all'.
	Making all in man
	make[3]: Nothing to be done for `all'.
	make[1]: Leaving directory `/home/akaedu/Github/comment-subs/kmod-11'
	
	$ sudo ./kmod-11/tools/depmod | head
	total count 3529
	kernel/arch/x86/kernel/cpu/mcheck/mce-xeon75xx.ko:
	kernel/arch/x86/kernel/cpu/mcheck/mce-inject.ko:
	kernel/arch/x86/kernel/msr.ko:
	kernel/arch/x86/kernel/cpuid.ko:
	kernel/arch/x86/kernel/apm.ko:
	kernel/arch/x86/kernel/microcode.ko:
	kernel/arch/x86/crypto/aes-i586.ko:
	kernel/arch/x86/crypto/twofish-i586.ko: kernel/crypto/twofish_common.ko
	kernel/arch/x86/crypto/salsa20-i586.ko:
	kernel/arch/x86/crypto/aesni-intel.ko: kernel/arch/x86/crypto/aes-i586.ko kernel/crypto/cryptd.ko
	$ 


## THU-12-7 测试 depmod 命令

### modprobe 命令运行时调试图

	$ sudo ./kmod-11/tools/modprobe -r nfs
	name = nfs

	line = kernel/fs/nfs/nfs.ko: kernel/fs/nfs_common/nfs_acl.ko kernel/net/sunrpc/auth_gss/auth_rpcgss.ko kernel/fs/fscache/fscache.ko kernel/fs/lockd/lockd.ko kernel/net/sunrpc/sunrpc.ko
	---------------------

	p = kernel/fs/nfs_common/nfs_acl.ko
	---------------------

	p = kernel/net/sunrpc/auth_gss/auth_rpcgss.ko
	---------------------

	p = kernel/fs/fscache/fscache.ko
	---------------------

	p = kernel/fs/lockd/lockd.ko
	---------------------

	p = kernel/net/sunrpc/sunrpc.ko
	---------------------
	$ 

	$ sudo ./kmod-11/tools/modprobe nfs
	name = nfs

	line = kernel/fs/nfs/nfs.ko: kernel/fs/nfs_common/nfs_acl.ko kernel/net/sunrpc/auth_gss/auth_rpcgss.ko kernel/fs/fscache/fscache.ko kernel/fs/lockd/lockd.ko kernel/net/sunrpc/sunrpc.ko
	---------------------

	p = kernel/fs/nfs_common/nfs_acl.ko
	---------------------

	p = kernel/net/sunrpc/auth_gss/auth_rpcgss.ko
	---------------------

	p = kernel/fs/fscache/fscache.ko
	---------------------

	p = kernel/fs/lockd/lockd.ko
	---------------------

	p = kernel/net/sunrpc/sunrpc.ko
	---------------------
	$ 



## THU-12-8 编译生成 testsuite 命令集

### 

	$ make check
	Making check in .
	  GEN      rootfs
	make --no-print-directory testsuite/uname.la testsuite/path.la testsuite/init_module.la testsuite/delete_module.la testsuite/libtestsuite.la testsuite/test-init testsuite/test-testsuite testsuite/test-loaded testsuite/test-modinfo testsuite/test-alias testsuite/test-new-module testsuite/test-modprobe testsuite/test-blacklist testsuite/test-dependencies testsuite/test-depmod
	  CC       testsuite/uname.lo
	  CCLD     testsuite/uname.la
	  CC       testsuite/path.lo
	  CCLD     testsuite/path.la
	  CC       testsuite/init_module.lo
	  CC       testsuite/mkdir.lo
	  CCLD     testsuite/init_module.la
	  CC       testsuite/delete_module.lo
	  CCLD     testsuite/delete_module.la
	  CC       testsuite/testsuite_libtestsuite_la-testsuite.lo
	  CCLD     testsuite/libtestsuite.la
	  CC       testsuite/testsuite_test_init-test-init.o
	  CCLD     testsuite/test-init
	  CC       testsuite/testsuite_test_testsuite-test-testsuite.o
	  CCLD     testsuite/test-testsuite
	  CC       testsuite/testsuite_test_loaded-test-loaded.o
	  CCLD     testsuite/test-loaded
	  CC       testsuite/testsuite_test_modinfo-test-modinfo.o
	  CCLD     testsuite/test-modinfo
	  CC       testsuite/testsuite_test_alias-test-alias.o
	  CCLD     testsuite/test-alias
	  CC       testsuite/testsuite_test_new_module-test-new-module.o
	  CCLD     testsuite/test-new-module
	  CC       testsuite/testsuite_test_modprobe-test-modprobe.o
	  CCLD     testsuite/test-modprobe
	  CC       testsuite/testsuite_test_blacklist-test-blacklist.o
	  CCLD     testsuite/test-blacklist
	  CC       testsuite/testsuite_test_dependencies-test-dependencies.o
	  CCLD     testsuite/test-dependencies
	  CC       testsuite/testsuite_test_depmod-test-depmod.o
	  CCLD     testsuite/test-depmod
	make --no-print-directory check-TESTS
	TESTSUITE: running test_initlib, in forked context
	TESTSUITE: 'test_initlib' [20196] exited with return code 0
	TESTSUITE: PASSED: test_initlib
	TESTSUITE: running test_insert, in forked context
	TESTSUITE: 'test_insert' [20197] exited with return code 0
	TESTSUITE: PASSED: test_insert
	TESTSUITE: running test_remove, in forked context
	TESTSUITE: 'test_remove' [20198] exited with return code 0
	TESTSUITE: PASSED: test_remove
	PASS: testsuite/test-init
	TESTSUITE: running testsuite_uname, in forked context
	TRAP uname(): missing export TESTSUITE_UNAME_R?
	TESTSUITE: 'testsuite_uname' [20202] exited with return code 0
	TESTSUITE: PASSED: testsuite_uname
	TESTSUITE: running testsuite_rootfs_fopen, in forked context
	TESTSUITE: 'testsuite_rootfs_fopen' [20203] exited with return code 0
	TESTSUITE: PASSED: testsuite_rootfs_fopen
	TESTSUITE: running testsuite_rootfs_open, in forked context
	TESTSUITE: 'testsuite_rootfs_open' [20204] exited with return code 0
	TESTSUITE: PASSED: testsuite_rootfs_open
	TESTSUITE: running testsuite_rootfs_stat_access, in forked context
	TESTSUITE: 'testsuite_rootfs_stat_access' [20205] exited with return code 0
	TESTSUITE: PASSED: testsuite_rootfs_stat_access
	TESTSUITE: running testsuite_rootfs_opendir, in forked context
	TESTSUITE: 'testsuite_rootfs_opendir' [20206] exited with return code 0
	TESTSUITE: PASSED: testsuite_rootfs_opendir
	PASS: testsuite/test-testsuite
	TESTSUITE: running loaded_1, in forked context
	TESTSUITE: 'loaded_1' [20210] exited with return code 0
	TESTSUITE: PASSED: loaded_1
	PASS: testsuite/test-loaded
	TESTSUITE: running modinfo_jonsmodules, in forked context
	TESTSUITE: 'modinfo_jonsmodules' [20214] exited with return code 0
	TESTSUITE: PASSED: modinfo_jonsmodules
	PASS: testsuite/test-modinfo
	TESTSUITE: running alias_1, in forked context
	TESTSUITE: 'alias_1' [20218] exited with return code 0
	TESTSUITE: PASSED: alias_1
	PASS: testsuite/test-alias
	TESTSUITE: running from_name, in forked context
	TESTSUITE: 'from_name' [20222] exited with return code 0
	TESTSUITE: PASSED: from_name
	TESTSUITE: running from_alias, in forked context
	TESTSUITE: 'from_alias' [20223] exited with return code 0
	TESTSUITE: PASSED: from_alias
	PASS: testsuite/test-new-module
	TESTSUITE: running modprobe_show_depends, in forked context
	TESTSUITE: 'modprobe_show_depends' [20227] exited with return code 0
	TESTSUITE: PASSED: modprobe_show_depends
	TESTSUITE: running modprobe_show_depends2, in forked context
	TESTSUITE: 'modprobe_show_depends2' [20228] exited with return code 0
	TESTSUITE: PASSED: modprobe_show_depends2
	TESTSUITE: running modprobe_builtin, in forked context
	TESTSUITE: 'modprobe_builtin' [20229] exited with return code 0
	TESTSUITE: PASSED: modprobe_builtin
	TESTSUITE: running modprobe_softdep_loop, in forked context
	TESTSUITE: 'modprobe_softdep_loop' [20230] exited with return code 0
	TESTSUITE: PASSED: modprobe_softdep_loop
	TESTSUITE: running modprobe_install_cmd_loop, in forked context
	TESTSUITE: ERR: Test 'modprobe_install_cmd_loop' timed out, killing 20231
	TESTSUITE: ERR: 'modprobe_install_cmd_loop' [20231] terminated by signal 9 (Killed)
	FAIL: testsuite/test-modprobe
	TESTSUITE: running blacklist_1, in forked context
	TESTSUITE: 'blacklist_1' [20283] exited with return code 0
	TESTSUITE: PASSED: blacklist_1
	PASS: testsuite/test-blacklist
	TESTSUITE: running test_dependencies, in forked context
	TRAP uname(): missing export TESTSUITE_UNAME_R?
	TESTSUITE: 'test_dependencies' [20287] exited with return code 0
	TESTSUITE: PASSED: test_dependencies
	PASS: testsuite/test-dependencies
	TESTSUITE: running depmod_modules_order_for_compressed, in forked context
	TESTSUITE: 'depmod_modules_order_for_compressed' [20291] exited with return code 0
	TESTSUITE: ERR: sizes do not match /home/akaedu/Github/test-kmod-11/kmod-11/testsuite/rootfs/test-depmod/modules-order-compressed/lib/modules/3.5.4-1-ARCH/correct-modules.alias /home/akaedu/Github/test-kmod-11/kmod-11/testsuite/rootfs/test-depmod/modules-order-compressed/lib/modules/3.5.4-1-ARCH/modules.alias
	TESTSUITE: ERR: FAILED: exit ok but outputs do not match: depmod_modules_order_for_compressed
	FAIL: testsuite/test-depmod
	==============================================
	2 of 10 tests failed
	Please report to linux-modules@vger.kernel.org
	==============================================
	make[2]: *** [check-TESTS] Error 1
	make[1]: *** [check-am] Error 2
	make: *** [check-recursive] Error 1
	$ Killed

	$ 



### 























