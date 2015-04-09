#!/bin/bash

build_dir="$1"
if [ ! -d "$build_dir" ]; then
    exit 1
fi

for i in $build_dir/*.compact.h; do
    expanded="${i/%.compact.h/.h}"
    expanded_groups=`grep '^#ifdef\|^#if ' "$expanded" | wc -l`

    prefix="`basename "$expanded"`"
    prefix="${prefix/%.h/}"
    if [ $prefix = "EM_R_mapping" ]; then
        prefix="EM_"
    fi
    compact_groups=`grep '^'"$prefix" "$i" | wc -l`

    if [ "$expanded_groups" -ne "$compact_groups" ]; then
        exit 2
    fi
    echo "$i	OK"
done

echo "All OK"
