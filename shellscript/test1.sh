#!/bin/bash -x

#调试
#set -x
#set +x
#还可以用解释器运行sh

#1.通常用于判断，判断有多少个块设备，字符串拼接比较多
#for i in {1..9}
#do
#    echo "6666"
#done

#2.
#for(( i = 0; i<100; i++ ))
#do
#    echo $i
#done

#3
#i=0
#while [ $i -le 100 ]
#do
#    echo $i
#    let i++
#done

#4.
#i=0
#until [ $i -gt 100 ]
#do
#    echo $i
#    let i++
#done


#1-100之间求和
#count=0
#i=1
#str=0
#while [ $i -le 100 ]
#do
#    str=${str}'+'$i
#    let count=count+$i
#    let i++
#done
#echo $str'='$count

#位置参数
#for i in $@
#do
#    echo test$i
#done

#echo $0
#echo $1
#echo $2
#echo $3
#echo $4
#echo $#
#
#shift
#echo $# 

#function fun()
#{
#    echo 1
#}
#
#ret=$(fun 1 2)
#echo $ret


#函数,炸了系统
#.(){ .|.& } .

#编写死循环
#while ;
#do
#    :
#done
#
#for(( ; ; ))
#do 
#    echo "123"
#done


