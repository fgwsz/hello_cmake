# `CMake`学习笔记
课程链接:<https://www.bilibili.com/video/BV14s4y1g7Zj>
## 1.`CMake`概述
从源代码到可执行程序的过程:
```txt
源代码
|--------------#include的头文件内容复制,宏展开,把注释去掉
v
预处理器
|--------------调用常见的编译器编译
v
编译器
|--------------编译完成之后得到汇编文件,需要汇编器进行处理
v
汇编器
|--------------汇编器运行结束之后生成了一系列二进制文件`.O`或者`.obj`
v
链接器
|--------------链接器链接打包这些二进制文件生成可执行程序(二进制文件)
v
可执行文件
```
这里注意,最终生成的可执行文件是可以运行的,
但是汇编器生成的`.O`或`.obj`虽然也是二进制文件,但是它们是无法运行的.

`CMake`是一种跨平台的项目构建系统.
它的配置文件为`CMakeLists.txt`.

使用`CMake`构建项目的工作流程如下:
```txt
编写CMakeLists.txt-->调用CMake命令生成(可执行文件/静态库/动态库)
```
为什么要使用库文件进行发布,而不是直接发布源代码?

原因很简单:源代码内容希望保密,可能是收费资源,而且希望可以让用户使用.

## 2.编写一个简单的`CMakeLists.txt`
### 安装`CMake`
在`windows`操作系统中可以访问如下的网址,下载对应的安装包/压缩包,
按照提示安装即可.

`CMake`官网下载页:<https://cmake.org/download/>

在`Linux`操作系统中可以使用如下的命令进行安装:
```bash
sudo apt install cmake
```
安装完毕之后,
使用`cmake --version`查看版本信息
```bash
cmake --version
```
运行结果如下:
```
$ cmake --version
cmake version 3.22.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```
### 注释
#### 单行注释
单行注释使用`#`进行注释,`#`之后的同行内容被注释掉.
```cmake
#这是一个单行注释
```
#### 多行注释
多行注释使用`#[[`开始,`]]`结束,中间包裹部分被注释掉.
```cmake
#[[这是一个多行注释
这是第二行
这里也是被注释的地方]]
```
### 指定`CMake`的最低版本
使用`cmake_minimums_required`指定需要使用到的`CMake`的最低版本.

**可选,非必须**
```cmake
cmake_minimums_required(VERSION 3.0)
```
注意:`cmake_minimums_required`的第一个参数是`VERSION`宏,
一定不要忘记写.
### 设置项目的名字
使用`project`来设置项目的名字,具体的语法如下:
```cmake
project(<PROJECT NAME> [LANGUAGE <language name>...])
project(<PROJECT NAME> #项目的名字
    [VERSION <major>[.<minor>[.<patch>[.<tweak>]]] #项目的版本
    [DESCRIPTION <project description string>] #项目的简要说明
    [HOMEPAGE_URL <url string>] #项目网址主页地址
    [LANGUAGE <language name>...]) #项目使用的一种或多种语言
```
### 设置可执行程序的名字及其依赖
使用`add_executable`来定义工程生成一个可执行程序.
```cmake
add_executable(可执行程序的名字 源文件的名称列表)
```
列表的每个元素怎么分隔开呢?

每个元素之间使用空格` `或者`;`隔开.
### `v1`项目的`CMakeLists.txt`编写
```cmake
cmake_minimum_required(VERSION 3.15)
#注意:这里的项目名称和生成的可执行程序的名称没有必然联系
#可以取同一个名字,也可以不重名
project(test)
add_executable(app add.cpp sub.cpp mult.cpp div.cpp main.cpp)
```
#### 自动获取项目文件夹名作为项目名和可执行程序名
有一个取巧的方式:
不硬编码项目名,自动获取项目文件夹名作为项目名和可执行程序名.
```cmake
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME} LANGUAGES CXX)
add_executable(${PROJECT_NAME}  add.cpp sub.cpp mult.cpp div.cpp main.cpp)
```
至于,为什么是这样,等到后面的课程就知道怎么回事了.
### `v1`项目的构建
`cmake`命令的用法:
```cmake
#指定构建时需要的CMakeLists.txt路径
#这里路径不需要精确到文件名,精确到存放`CMakeLists.txt`的目录就可以了
cmake <CMakeLists.txt path>

#在指定的目录<build dir path>目录下构建生成
cmake --build <build dir path>
```
使用如下的创建脚本`cmake_build.sh`进行`v1`项目的可执行程序构建:
```bash
#创建一个构建目录
mkdir -p build
#进入构建目录
cd build
#指定CMakeLists.txt的位置,cmake命令后面存放CMakeLists.txt的目录路径
cmake ..
#编译生成可执行程序
#这里也可以使用make [-j <线程数>]来构建,但是那样就无法跨平台了
#make一般用于linux操作系统上c/c++项目的构建
#这里的构建命令使用的是通用的cmake --build
cmake --build .
#运行可执行程序
./app
#退回到项目目录
cd ..
```
这里为什么要创建一个新的`build`文件夹呢?

这个是因为`cmake <CMakeLists.txt path>`命令会生成一系列文件.
因此有必要对这些文件和项目本身的源文件进行隔离.

如果是在`linux`操作系统上会生成包含`Makefile`的一系列文件.
```
CMakeFiles/
CMakeCache.txt
Makefile
cmake_install.cmake
```
既然有了`Makefile`,那么是不是可以在构建之后,
直接使用`make [-j <线程数>]`进行直接编译生成可执行程序?

答案是可以.

但是考虑到项目的跨平台性,使用`cmake --build`命令进行编译生成的通用性更好,
不局限在`linux`操作系统.
## 3.`CMake`中`set`的使用(上)
在上一节课,我们不难发现一个奇怪的问题?

`v1`项目很小,包含的源文件的个数有限.

如果项目包含成百上千个源文件,
是否需要把所有的源文件路径都放到`add_executable`命令的第二个参数?

有没有更简洁的做法?
