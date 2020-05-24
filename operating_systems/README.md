# Operating Systems 操作系统 2020/T1 (COMP9201)
## Overview and Links 
System/161 or OS 161 is a synthetic (read: made up) hardware platform designed specifically for teaching operating systems and for experimental operating system hacking. It includes a simulated CPU, system bus, and bus devices that are realistic but simple and easy to work with. <br/>
[Course Website 课程网站](http://cgi.cse.unsw.edu.au/~cs3231/notices.php)<br/>
[Similar Course 类似课程网站](https://www.ops-class.org)<br/>
[Relative Blogs 相关博客](http://jhshi.me/home/)<br/>
[System/161 2.x Software and Hardware Manual 系统介绍网站](http://os161.eecs.harvard.edu/documentation/sys161-2.0.8/)<br/>
[OS/161 2.0 Reference Manual 系统手册](http://cgi.cse.unsw.edu.au/~cs3231/16s1/os161/man/index.html)<br/>

## Project
熟悉并编写教学操作系统os161（简单版Linux）<br/>
* asst0-src(Getting warmed up): 简单了解os161是如何运行的，同时如何用GDB debug。
* asst1-src(Synchronisation): 用os161提供的 lock, semaphore 和 condition variable 去解决操作系统的同步（Synchronisation）问题，和如何发现并解决可能出现的死锁（deadlock）。
* asst2-src(System calls and processes): 实现file syste和procces的一些system call操作 比如： open(), read(), write(), lseek(), close(), dup2(), fork(), getpid() ... <br/>
(代码主要在 kern/syscall/file.c, kern/arch/mips/syscall/syscall.c)
* asst3-src(Virtual Memory): 使系统具有虚拟内存，TLB工作正常，设计是2-level hierarchical page table. <br/>
[简单介绍什么是TLB (Youtube)](https://www.youtube.com/watch?v=95QpHJX55bM)<br/>
(代码主要在 kern/vm/addrspace.c, kern/vm/vm.c)

### 如何运行os161？
我用的是mac上的VMware Fusion Linux 模拟器。<br/>

#### Resources:
Working on Linux On Debian-based distributions (including Ubuntu, Mint)<br/>
We have built the cross-compilation tools on Ubuntu amd64. <br/>
$ wget http://www.cse.unsw.edu.au/~cs3231/os161-files/os161-utils_2.0.8-3.deb <br/>

#### Install:
$ sudo dpkg -i os161-utils_2.0.8-3.deb <br/>

The tools are installed in /usr/local/bin, so you if that directory is not in your PATH already, you'll need to add it via your shell's startup file. <br/>
add this line into  ~/.bash_profile <br/>
export PATH=${PATH}:/usr/local/bin <br/>

Additional Linux tools <br/>
$ sudo apt-get install build-essential <br/>
$ sudo apt-get install python <br/>
$ sudo apt-get install libmpfr6 <br/>

#### Testing:
$ os161-gcc <br/>
Expected output: <br/>
os161-gcc: fatal error: no input files <br/>
compilation terminated. <br/>

If you get the following error when using the tools <br/>

libmpc.so.3: cannot open shared object file: No such file or directory <br/>

then try installing the libmpc3 package <br/>

$ sudo apt-get install libmpc3 <br/>

## Topics
1. Operating Systems Overview
2. Processes And Threads
3. Concurrency and Synchronisation
4. Deadlock
5. Process and Thread Implementationa
6. System Calls and R3000 Overview
7. Computer Hardware, Memory Hierarchy, and Caching
8. File Management (Ext2, Ext3)
9. Memory Management
10. Virtual Memory
11. Multiprocessors
12. Scheduling
13. I/O input/output devices
