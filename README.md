[![Build Status](https://travis-ci.org/stonekyx/binary.svg?branch=master)](https://travis-ci.org/stonekyx/binary)

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
+ graphviz-2.26.0 (如果平台上有的话，还需要同版本的graphviz-dev)

对于Ubuntu，依赖情况更加复杂。请参考如下列表：

+ gcc g++ make autoconf automake m4 python
+ libc6-dev
+ elfutils
    - 由于Ubuntu软件源不包含libebl，所以需要自行编译安装完整的elfutils。步骤参考[Linux from scratch](http://www.linuxfromscratch.org/blfs/view/svn/general/elfutils.html)。如果不想执行这些操作，可以下载本项目Travis CI使用的二进制包，位于[私有服务器](http://kamijou.info/elfutils_0.161-1_amd64.deb)。注意：这个包不适合发布，也不应该在个人系统上长期安装。
+ libqt4-dev
+ libqt4-core
+ libqt4-gui
+ binutils
+ binutils-dev
+ libcppunit-dev
+ graphviz-dev

# Compile
编译命令如下：

`$ python2 vendor/scons.py`

编译后自动安装到`./_install`目录下，主程序路径是`./_install/bin/mainwindow`，插件目录是`./_install/share/binary`。可以通过主程序的命令行参数快速加载所有插件和要分析的文件，命令如下：

`$ _install/bin/mainwindow --plugins _install/share/binary/ --file _install/bin/mainwindow`

以上命令将展示主程序本身的内容。
