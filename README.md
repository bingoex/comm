# Comm Lib #

一个简单C语言库<br/>
A common lib in C language for all C\* project 


## cm\_cfg
	提供简单k-v形式配置文件读取API

## cm\_log
    提供带轮询功能的日志API

## cm\_shm
    封装SystemV系列共享内存操作API
    - 各种获取API
    - 获取共享内存大小
    - 删除共享内存API

## cm\_debug
	提供以16进制打印Buf的API

## cm\_net
    提供网络相关API
    - 字符串转in_addr
    - 字符串转端口
    - 创建socket并监听
    - 创建发布socket（tcp则默认connect）
    - 判断ip是否内网
    - 根据网卡名字获取ip
    - 设置fd为非阻塞
    - 根据ip、port生成sockaddr_in

## cm\_process
    提供进程相关API
    - 初始化Daemon进程
    - pid文件写入

## cm\_math
	提供简单数学API
    - 素数判断
    - 素数数组生成

## cm\_binary
	提供简单易用的二进制封包解包API
	
## cm\_time
	提供tm转字符串API

## cm\_type
	提供统一的类型宏定义
    - INT32

## cm\_procenum\_ctol
    通过信号量，提供保证进程只启动一次的API

## cm\_lock
	提供简单信号量封装API
