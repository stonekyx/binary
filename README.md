# Binary
Binary analysis and exhibition of ELF.

# Dependencies

一般平台上依赖的库如下：

+ glibc-2.17 (如果平台上有的话，还需要同版本的glibc-headers)
+ elfutils-0.158 (如果平台上有的话，还需要同版本的elfutils-libs，elfutils-libelf，elfutils-devel和elfutils-libelf-devel)
+ qt-4.8.5 (如果平台上有的话，还需要同版本的qt-devel)
+ python-2.7.5
+ binutils-devel-2.23.52 (提供libiberty。有的平台上是gcc或gcc-multilib提供)
+ cppunit-1.12.1 (如果平台上有的话，还需要同版本的cppunit-devel)

对于Ubuntu，依赖情况更加复杂。请参考如下列表：

+ libc6-dev
+ elfutils
+ libelf1
+ libelf-dev
+ libasm-dev
+ libqt4-dev
+ libqt4-core
+ libqt4-gui
+ binutils
+ binutils-dev
+ libcppunit-dev

# Compile
编译命令如下：

`$ python2 vendor/scons.py`

编译后自动安装到`./_install`目录下，主程序路径是`./_install/bin/mainwindow`，插件目录是`./_install/share/binary`。可以通过主程序的命令行参数快速加载所有插件和要分析的文件，命令如下：

`$ _install/bin/mainwindow --plugins _install/share/binary/ --file _install/bin/mainwindow`

以上命令将展示主程序本身的内容。
