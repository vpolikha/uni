#!/bin/bash

i=0

while [ "$i" -lt 12 ]; do
    MESSAGE="RUNNING. PID=$$. TIME=$(date)"
    echo "$MESSAGE" >> /home/ufull/info.log
    i=$((i+1))
    sleep 5
done
