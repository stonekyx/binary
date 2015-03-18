# Binary
Binary analysis and exhibition of ELF.

# Compile
编译命令如下：

`$ python2 vendor/scons.py`

编译后自动安装到`./_install`目录下，主程序路径是`./_install/bin/mainwindow`，插件目录是`./_install/share/binary`。可以通过主程序的命令行参数快速加载所有插件和要分析的文件，命令如下：

`$ _install/bin/mainwindow --plugins _install/share/binary/ --file _install/bin/mainwindow`

以上命令将展示主程序本身的内容。
