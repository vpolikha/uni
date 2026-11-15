#!/bin/bash

i=0

while true; do
    MESSAGE="RUNNING. PID=$$. TIME=$(date)"
    echo "$MESSAGE" >> /home/ufull/info.log
    i=$((i+1))
    sleep 5
done
