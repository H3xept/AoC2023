#!/bin/bash
FILE=$1
FIRST_DIGIT="s/^[^0-9]*\([0-9]\).*$/\1/"

first=($(cat $FILE | sed $FIRST_DIGIT))
last=($(cat $FILE | rev | sed $FIRST_DIGIT))

total=0
for ((i=0; i<${#first[*]}; ++i)); do
     echo $(sed -n "$(($i+1))p" $FILE) ${first[i]}${last[i]} >&2
     echo $total >&2
     total=$((total + ${first[i]}${last[i]})) >&2
     echo $total >&2
     echo "---" >&2
done

echo $total

