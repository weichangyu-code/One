# One
## 介绍
简单语法、关注业务、高性能。

## 特性
. 简单的语法规则。  
. 内存安全：不会出现内存被其他功能破坏的情况。  
. 对象析构：不被引用的时候，自动析构，并且调用析构函数。后再内存回收。  
. 单线程下的协程模型：每个线程下是一个独立的协程引擎，相互间不会干扰，可以通过特殊渠道交换数据。应用层的代码，基本不会用到真正的内核对象和锁对象，也不会用到原子操作。
. 同步开发模型：简化多线程、异步带来的程序复杂度问题。  
. 代码性能略低于C++，整体服务器的性能超过C++。  
. 跨平台。  

## 原理
. 将One语言解释成C++语言用CMAKE+GCC编译。  
. 集成协程库、网络库、异步IO库。  

## 编译
. 开发工具：vscode  
. 编译工具：linux gcc x86_64，windows vs15 x86_amd64, windows mingw32(不支持异常) 

## 进展
. 完成大流程，HELLO ONE已经出来。  
. 完成协程库，可以独立使用。  
. 完成协程下的网络通讯库，LINUX使用EPOLL，WINDOWS使用IOCP，可以独立使用。
. 完成异步IO库  

## 接下来  
. 完善各种基础库。  
. 开发一个HTTP服务，用于验证。  
. 继承Linux io_uring

## 注意
. 规则文件在compiler/src/analyzer/rule/one.rule，修改规则后需要执行compiler/test/analyzer/RuleGenerator生成规则执行文件compiler/src/analyzer/one/OneRuleMap.inl

## 最后
. 未完待......  
