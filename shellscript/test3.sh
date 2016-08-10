#!/bin/bash

#序列式数组,数组存放要连续
#arr=('hello' '123' 'bit' '456')
#echo ${arr[2]}
#echo ${arr[@]}
#echo ${#arr[@]}
#
#for (( i=0; i<${#arr[*]}; i++))
#do 
#    echo ${arr[$i]}
#done


#进度条
#i=0
#while [ $i -le 100 ]
#do
#
#done

if [ $# -le 2 ];then
    echo  "至少三个参数"
else 
    ave=0
    max=$1
    min=$1
    i=1
    while [ $i -le $# ]
    do
        if [ max -lt i ];then
            let max=i
        fi
        if [ min -gt i ];then
            let min=i
        fi
        let i++
    done
    echo max:$max
    echo min:$min
fi
