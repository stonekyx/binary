# Binary
Binary analysis and exhibition of ELF.

# Dependencies

依赖的库如下：

+ glibc-2.17 (如果平台上有的话，还需要同版本的glibc-headers)
+ elfutils-libelf-devel-0.158 (或者没有的话elfutils)
+ qt-4.8.5 (如果平台上有的话，还需要同版本的qt-devel)
+ python-2.7.5
+ binutils-devel-2.23.52 (提供libiberty。有的平台上是gcc或gcc-multilib提供)
+ cppunit-1.12.1 (如果平台上有的话，还需要同版本的cppunit-devel)

# Compile
编译命令如下：

`$ python2 vendor/scons.py`

编译后自动安装到`./_install`目录下，主程序路径是`./_install/bin/mainwindow`，插件目录是`./_install/share/binary`。可以通过主程序的命令行参数快速加载所有插件和要分析的文件，命令如下：

`$ _install/bin/mainwindow --plugins _install/share/binary/ --file _install/bin/mainwindow`

以上命令将展示主程序本身的内容。
