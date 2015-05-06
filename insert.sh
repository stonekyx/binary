#!/bin/bash

if [ "$#" -ne "1" ]; then
    exit 1
fi

file=`basename $1`
if [ -f "$file" ]; then
    exit 2
fi

sed 's/--FILENAME--/'"$file"'/g' license | cat - $1 > /tmp/tmp.ch && mv -vf /tmp/tmp.ch $1
