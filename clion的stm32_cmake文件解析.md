<div align='center' ><font size='5'>基于clion的stm32开发的cmake文件解析</font></div>

#### 1、缘由

​	最近去刷了了C语言的视频，教程用的基本都是微软的[**Visual\* *Studio\*:*](https://visualstudio.microsoft.com/)软件，讲到了多文件编程，而我自己用着clion，跟着老师敲代码！

​	clion开发c语言或者stm32(arm 嵌入式)等工程的时候，需要用到CMakeLists.txt这个文件来构建！然后我就跑去官网查看clion这个软件在嵌入式方面的使用方法，恰好发现了clion这个软件支持度stm32或者其余嵌入式开发的发展，我这里就借花献佛，好好说说！

​	备注：clion官网的产品文档页面，原版英文界面，这里方便你们查看，就谷歌翻译了界面,大家可以观看这里来学习clion开发嵌入式，也可以翻看我之前的专栏。

​	网址：https://www.jetbrains.com/help/clion/embedded-overview.html

![001官方文档](C:\Users\overfly\Desktop\b站写专栏\第三篇\photo\001官方文档.PNG)



#### 2、起航

​	起初，在比较旧的官方clion版本(2019之前)是不支持stm32嵌入式开发，想要支持，就要像在linux开发嵌入式那样，自己手写cmake或者更古老的Makefile编译。但国外有个折腾老哥，就想着怎么把clion应用到嵌入式开发，毕竟用了IDEA的，都说jetbrains产品好，全家桶才是永远的神！

​	没错，就是这个老哥Ilya Motornyy ，利用st公司的cubemx生成的sw4stm32工程，在这个基础上手撸了cmake文件，完成了在clion上，对stm32板子的伟大操作——点灯。然后，他就发现，这样做的话不得劲，每次都要自己写，繁杂又繁琐，而且当换其他芯片，cmake文件不通用，又要修改！

​	就想着，有没有一个工具，能够把cubemx生成的sw4stm32工程直接用软件解析成对应的cmake文件，以代替手动编码！没错，这位老哥，直接又手撸一个clion插件，直接集成生成，调试，修改一条龙服务。

![002插件](C:\Users\overfly\Desktop\b站写专栏\第三篇\photo\002插件.png)

​	接着最骚的来了，他于 2018 年底加入了clion的团队，并已将插件集成到 CLion 中，并推进对[CLion 的](https://www.jetbrains.com/clion/)嵌入式开发支持。因此，现在OpenOCD 调试器支持和与 STM32CubeMX 的集成开箱即用。

​	备注：[用于嵌入式开发的 CLion 第 I](https://blog.jetbrains.com/clion/2016/06/clion-for-embedded-development/?_ga=2.85382170.209288003.1642688957-1707541996.1610282742)部分、[第 II 部分](https://blog.jetbrains.com/clion/2017/12/clion-for-embedded-development-part-ii/?_ga=2.54861096.209288003.1642688957-1707541996.1610282742)、[第 III 部分](https://blog.jetbrains.com/clion/2019/02/clion-2019-1-eap-clion-for-embedded-development-part-iii/?_ga=2.131527820.209288003.1642688957-1707541996.1610282742)

​	网址：https://blog.jetbrains.com/clion/2016/06/clion-for-embedded-development/

![003教程](C:\Users\overfly\Desktop\b站写专栏\第三篇\photo\003教程.PNG)



#### 3、作者

​	这个老哥，**是 Ilia** **Motornyi**，又名**elmot**， 工作时间是 Java 开发 人员，有时是嵌入式开发人员。

​	博客：https://www.elmot.xyz/

​	github：https://github.com/elmot

​	看了下这老哥还在折腾clion对STM32L5 with TrustZone enabled，dual-core STM32 MCU的支持和适配。



#### 4、解析

​	说了这么，我们回到最初的目的，cmake文件的解析。让我们来看看这个老哥最新的CMakeLists.txt template for ARM GCC projects。模板文件在网址里！

​	网址：https://gist.github.com/elmot/3b4f0e9f8b23864fdf8bb509c329d051

​	如何学习cmake文件，我觉得最好去学习gcc命令行编译，cmake只一层一层封装上去的，实际最后都是命令行。

​	事实上，从源代码生成可执行文件可以分为四个步骤，分别是预处理（Preprocessing）、编译（Compilation）、汇编（Assembly）和链接（Linking）。

​	![006原理](C:\Users\overfly\Desktop\b站写专栏\第三篇\photo\006原理.png)

​	cmake无非是以上过程的一些封装，因为大型工程文件太多，不可能手敲命令行慢慢编译的。把cmake上的一些操作，和上述过程对应起来，就非常容易理解。而嵌入式开发无非用的只是gcc-arm-none-eabi工具链编译。



​	(1)交叉编译命令。交叉编译一个简单的例子，用x86window系统环境，编译arm架构的单片机程序。这 [`CMAKE_SYSTEM_NAME`](https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html#variable:CMAKE_SYSTEM_NAME) 是要构建的目标平台的 CMake 标识符。是linux就写linux，是arm就写arm，通用的话，就写Generic。我们也可以再加一行命令，#set(CMAKE_SYSTEM_PROCESSOR arm)，这 [`CMAKE_SYSTEM_PROCESSOR`](https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_PROCESSOR.html#variable:CMAKE_SYSTEM_PROCESSOR) 是要构建的目标架构的 CMake 标识符。clion生成的并没有加！因为后面指定了架构，这里也可以写也可以不写。

```cmake
#for non-Linux projects, it’s worth adding the following before the project clause:
#设置编译平台cmake环境
set(CMAKE_SYSTEM_NAME Generic)
#The version of the operating system for which CMake is to build.
#个人感觉这个命令没什么用
set(CMAKE_SYSTEM_VERSION 1)

#最低camke版本要求
cmake_minimum_required(VERSION 3.17)

#网站:
#解答:https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling-for-windows-10-universal-applications
#解答:https://discourse.cmake.org/t/how-does-cmake-vs-windows-target-platform-version-work/260
```



(2)指定交叉编译器和工具，arm-none-eabi-gcc是专门给arm开发的编译工具。

set(AA BB)有点类似于#define AA BB,把AA 定义为BB，不过有些AA已经被编译器定义过了。

```cmake
# specify cross compilers and tools
# 指定C 和 C++ 编译器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
#指定汇编器
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
#指定函数库管理器
set(CMAKE_AR arm-none-eabi-ar)
#指定文件格式转换器，比如转换bin文件
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
#指定反汇编用到的编译器
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
#用于查看文件占用flash和ram的大小
set(SIZE arm-none-eabi-size)
#Here’s a  slightly more accurate and less radical solution: relax the compiler check by adding the following before the project clause:
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#两个网站对这个有很好的解释，使用这个的原因,
#网站:https://www.e-learn.cn/topic/2470646
#网站:https://reviews.llvm.org/D91334
```



(3)工程设置。使用方法project(<PROJECT-NAME> [<language-name>...])。`project` 主要用于提供项目的名称、版本、使用编译语言等信息。这里是c，c++，汇编。并指定，c和c++的标准。

```cmake
# project settings
project(#[[TODO project name]] C CXX ASM)
#指定 C++ 17标准
set(CMAKE_CXX_STANDARD 17)
#指定 C 11 标准
set(CMAKE_C_STANDARD 11)
```



(4)就是设置下MCPU变量，在双核开发板上方便修改cmake文件。

```cmake
set(MCPU #[[TODO kernel type, ex cortex-m4]])
#实际应用
set(MCPU cortex-m4)
#项目参考
#CLion example project for STM32H745 MCU and NUCLEO-H745ZI-Q board
#网址:https://github.com/elmot/h7-trace-probe
```



(5)开启硬件浮点。(compile编译)

```cmake
#Uncomment for hardware floating point
#add_compile_definitions将预处理器定义添加到编译器命令行。(宏定义)
#网站:Define preprocessor macro through CMake?
#https://stackoverflow.com/questions/9017573/define-preprocessor-macro-through-cmake
#1_ARM_MATH_CM4这个就非常重要，必须要配置进去，否则在编译之后，会默认使用math.h的库函数，而不会用到硬件的FPU的。
#2_ARM_MATH_ROUNDING这个是库函数在运算是是否开启四舍五入的功能
#3_ARM_MATH_MATRIX_CHECK是库函数的参数检查开关
#基于HAL库的STM32的DSP库详解(附FFT应用)
#网站:https://www.cnblogs.com/lwwBKY/p/14063829.html
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)

#add_compile_options将选项添加到目录及其子目录中的所有目标
#-mfloat-abi指定是使用硬件指令还是软件库函数进行浮点运算，以及使用哪些寄存器来传递浮点参数和返回值。

#句法-mfloat-abi=value
#soft用于浮点运算和软件浮点链接的软件库函数。softfp硬件浮点指令和软件浮点联动。hard硬件浮点指令和硬件浮点联动。
#-mfpu指定目标 FPU 架构，即目标上可用的浮点硬件。
#vfpv4-d16启用Armv7 VFPv4-D16 浮点扩展。禁用高级 SIMD 扩展。
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#为当前目录及以下目录中的可执行文件、共享库或模块库目标的链接步骤添加选项
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
```

(6)开启软件浮点。

```cmake
#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)
```



(6)命令行选项。(compile编译)

```cmake
#-mcpu为特定Arm ® 处理器启用代码生成。-mthumb请求编译器以 T32 指令集为目标。
#大多数Arm ® v7‑A（及更早版本）处理器支持两个指令集。它们是 A32 指令集（以前的 ARM）和 T32 指令集（以前的 Thumb）。
#"-mthumb-interwork ”编译选项生成的目标文件就是thumb的，但是可以被其他的ARM的目标文件交叉调用。
#一般如果工程中需要一部分文件目标文件编译为ARM指令，一部分目标文件编译为Thumb指令时， 可以在这两部分的编译选项中都加入"-mthumb-interwork”选项，这样就可以在后面将这两部分链接为一个可执行文件
add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
#命令差不多，就是提前设置了set变量方便修改，add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)

，
#-ffunction-sections为源文件中的每个函数生成一个单独的 ELF 部分，链接时不使用的函数不会链接，可以缩小代码体积。
#-fdata-sections启用为源文件中的每个变量生成一个 ELF 节。
#-fno-common选项为暂定定义生成单独的零初始化定义，暂定定义是没有存储类和初始化程序的变量声明。
#-fmessage-length控制诊断消息格式，
#默认情况下，gnu工具链编译过程中，控制台输出的一行信息是不换行的这样，当输出信息过长时(如编译错误时的信息)，会导致你无法看到完整的输出信息，加入-fmessage-length=0后，输出信息会根据控制台的宽度自动换行，这样就能看全输出信息了。
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
```



(7)减轻 c++17 绝对地址警告

```cmake
# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")
```



(8)自己添加

```cmake
#使打印的中文字符不乱码,欺骗编译器
#-finput-charset 用来指定 C文件中的文字编码格式，-fexec-charset 用来指定编译之后的可执行文件的文字编码格式；默认情况下，gcc编译器认为编译前后的文字编码格式都是UTF-8,
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fexec-charset=GBK")
```



(9)编译优化选项。这里是根据clion的cmake设置，来运行。默认是debug。

```cmake
#-On	n 的取值范围：0~3。编译器的优化选项的 4 个级别，-O0 表示没有优化，-O1 为缺省值，-O3 优化级别最高
#-g		在编译的时候，生成调试信息，该程序可以被调试器调试
#其余参考网址:
#gcc -O1 -O2 -O3 -Os -Ofast -Og的作用
#https://www.cnblogs.com/zhchy89/p/8805691.html
#3. gcc 常用参数
#https://subingwen.cn/linux/gcc/#3-gcc常用参数

#STREQUAL 是 CMAKE 的关键字，用于字符串比较，相同返回 true
#${CMAKE_BUILD_TYPE}下面图片的界面定义了
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()
```



![005编译设定](C:\Users\overfly\Desktop\b站写专栏\第三篇\photo\005编译设定.PNG)



(10)导入头文件，设置可执行文件需要包含的库的路径。

```cmake
#这里使用的是老哥的模板，这里用的是stm32 HAL库的头文件路径，是H7的HAL路径
#include_directories将指定目录添加到编译器的头文件搜索路径之下，指定的目录被解释成当前源码路径的相对路径。
#CMAKE_SOURCE_DIR，根源代码目录，工程顶层目录。
include_directories(
        ${CMAKE_SOURCE_DIR}/Core/Inc
        ${CMAKE_SOURCE_DIR}/../Drivers/CMSIS/Include
        #[[ TODO rest of include folder locations ex.:
        ${CMAKE_SOURCE_DIR}/../Drivers/STM32H7xx_HAL_Driver/Inc
        ${CMAKE_SOURCE_DIR}/../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
        ${CMAKE_SOURCE_DIR}/../Drivers/CMSIS/Device/ST/STM32H7xx/Include
        ]]
        )
#这是HAL库 stm32f446re的例子，直接使用当前工程绝对路径
#include_directories(
		Core/Inc 
		Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F4xx/Include 
        Drivers/CMSIS/Include
        )
```



(10)添加全局宏定义。

```cmake
#在这个选项中添加宏，就相当于我们在文件中使用“#define”语句定义宏一样。在编译器中添加宏的好处就是，只要用了这个模版，就不用源文件中修改代码。
#STM32H745xx宏：为了告诉STM32HAL库，我们使用的芯片是STM32H745型号，使STM32HAL 库根据我们选定的芯片型号来配置。
#USE_HAL_DRIVER 宏：为了让 stm32H745xx.h 包含 stm32H7xx_hal_conf.h 这个头文件
#DEBUG动态定义宏开关，从而能够达到编译出不同逻辑流的代码。
#例子:
#首先，我在src代码里编写了若干debug的输出：
#IFDEF DEBUG 
#    some print command; 
#ENDIF
#CORE_CM4这个宏定义暂时不知道用来干嘛
add_definitions(#[[TODO insert definitions, ex. -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx]])
```



(11)把相关源文件和启动文件链接到编译器。使用GLOB命令使用通配符模式匹配来查找文件。file(GLOB SOURCES "src/*.*")使用这个通配符，表示所有".*"结尾的文件都会包含到这个SOURCES变量。 

```cmake
#这些指令在 cmake 运行时搜索目录树
#调用并将与模式匹配的所有文件放入变量中
#如果你不想，你可以使用 set(sources src/main.cpp) 等
#使用 globbing 自动查找文件。
file(GLOB_RECURSE SOURCES
        "Core/*.*" "Core/Startup/*.*"
        "Drivers/*.*" "../Drivers/*.*"
        "../Common/*.*"
        #[[TODO rest of cource locations]]
        )
```



(12)设置链接文件。把ld文件(st 官方连接器脚本)设置成LINKER_SCRIPT变量，方便后续链接操作。

编译STM32程序时，需要链接ld文件，主要作用在于告诉编译器单片机的Flash和RAM大小以及地址分布，这个文件在ST提供的STM32固件库中可以找到。

```cmake
#模板
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/#[[TODO linker script name, ex STM32H745ZITX_FLASH.ld]])
#例子
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32H745ZITX_FLASH.ld)
```



(13)链接命令选项。(link链接)

​	在链接生成最终可执行文件时，如果带有-Wl,--gc-sections参数，并且之前编译目标文件时带有-ffunction-sections、-fdata-sections参数，则链接器ld不会链接未使用的函数，从而减小可执行文件大小。

​	map文件就是通过编译器编译之后，生成的程序、数据及IO空间信息的一种映射文件，里面包含函数大小，入口地址等一些重要信息。

​	通过 map 文件，可以方便的查看工程 ROM/FLASH 和 RAM 的占用情况，包括单个源文件，甚至具体到每个函数的 rom 大小都给出了，通过这些信息可以很好的进行代码优化。

```cmake
#gcc解答，网址https://gcc.gnu.org/onlinedocs/gnat_ugn/Compilation-options.html
#链接选项 -Wl
#-gc-sections 消除未使用的函数。
#--print-memory-usage 编译器会输出内存使用情况
#-Map=xxx.map就是指定map文件输出的文件名称，这个名称xxx可以随意定义
#${PROJECT_BINARY_DIR}/${PROJECT_NAME}
#PROJECT_BINARY_DIR	暂认为就是CMAKE_BINARY_DIR，CMAKE_BINARY_DIR运行cmake的目录。外部构建时就是build目录
#PROJECT_NAME，项目名称
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)

#可以使用“  -T ”文件名“ 命令指定链接脚本文件
add_link_options(-T ${LINKER_SCRIPT})

```



(14) 生成可执行文件。链接之前的map文件，以及file命令生成的SOURCES和链接器ld脚本，得到.elf后缀的可执行文件：

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
```



(15)输出hex和bin文件。某些时候可能需要用到hex或者bin格式的文件，可以使用arm-none-eabi-objcopy工具来生成。
首先设置文件路径

```cmake
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
```



(16)生成hex和bin文件。

```cmake
#add_custom_command命令的解析，网址拉到最后面。
#网址：https://cmake.org/cmake/help/latest/command/add_custom_command.html
#命令解析$<TARGET_FILE:tgt>
#tgt二进制文件的完整路径。
#命令行生成bin和hex文件
#大伙可以参考https://mculover666.blog.csdn.net/article/details/84888539
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
```



#### 5、结尾

​		(1)这个文件是做个记录，以后来看这个cmake来编译stm32工程，也比较方便。、

​		(2)后续还要学习gcc以及gcc编译器分支的gcc-arm-none-eabi的学习！

#### 999、感谢

​	(1)arm keil 编译器参考指南

​		网址：https://www.keil.com/support/man/docs/armclang_ref/armclang_ref_fjl1564391650418.htm

​	(2)GNU Compiler Collection，[3.19.5 ARM Options](https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html#ARM-Options)

​		网址：https://gcc.gnu.org/onlinedocs/gcc/

​	(3)[Anastasia Kazakova](https://intellij-support.jetbrains.com/hc/en-us/profiles/1283077451-Anastasia-Kazakova),Embedded Development in CLion

​		网址：https://intellij-support.jetbrains.com/hc/en-us/articles/4405160320274-Embedded-Development-in-CLion

​	(4)How to cross-compile for embedded with CMake like a champ

​		网址：https://kubasejdak.com/how-to-cross-compile-for-embedded-with-cmake-like-a-champ

​	(5)Linux下实现飞控开发：使用CMake构建STM32工程

​		网址：https://blog.csdn.net/loveuav/article/details/101361408

​	(6)STM32开发环境 Linux下开发stm32

​		网址：https://mculover666.blog.csdn.net/article/details/84888539