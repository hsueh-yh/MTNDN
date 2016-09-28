#!/bin/bash  
  
#变量定义  
ip_array=("10.103.243.176")  
user="home632"
remote_cmd="cd ~/workspace/capture/ && make && ~/workspace/capture/capture"
  
#本地通过ssh执行远程服务器的脚本  
for ip in ${ip_array[*]}  
do  
    if [ $ip = "192.168.1.1" ]; then  
        port="7777"  
    else  
        port="22"
    fi  
    ssh -t -p $port $user@$ip $remote_cmd  
done  
