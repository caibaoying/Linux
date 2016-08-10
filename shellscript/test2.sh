#!/bin/bash
#shell 不允许空语句
read data

case $data in
    #star? )
    start|starT )
        echo "start"
    ;;
    "restart" )
        echo "restart"
    ;;
    * )
        echo "*"
    ;;
esac

#read data
#
#[ "$data" == "hello" ] && {
#    echo "yes"
#    exit 0
#}
#echo "no"

#read data
#
#[ "$data" == "hello" ] || {
#    echo "yes"
#    exit 0
#}
#echo "no"

#read data
#
#if [ "$data" == "hello" ];then
#    echo "yes"
#elif [ "$data" == "world" ];then
#    echo "no"
#else
#    echo "default"
#fi
