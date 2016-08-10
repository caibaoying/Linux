#!/bin/bash

i=0
str=''
arr=('|' '/' '-' '\\' ) 
index=0
while [ $i -le 100 ]
do
    printf "[%-101s][%d%%][%c]\r" "$str" "$i" "${arr[index]}"
    str='#'$str
    let i++
    let index=i%4
    sleep 0.5
done
printf "\n"
