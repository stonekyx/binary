#!/bin/bash

if [ $# -ne 2 ]
then
    exit 1
fi
output_dir=`dirname $1`
md5_file=`basename $1`
elf_header="$2"
if [ ! -e "$output_dir" ]; then
    if ! mkdir -p "$output_dir"; then exit 2; fi
elif [ ! -d "$output_dir" ]; then
    exit 3
fi

if [ -d "$output_dir/$md5_file" ]; then
    exit 4
fi

if [ ! -f "$elf_header" ]; then
    exit 5
fi

em_r="EM_R_mapping.compact.h"

> "$output_dir/$em_r"
> "$output_dir/$md5_file"

echo 'BINARY_PLUGIN_VALUE_TYPE, Elf64_Half, ExpandDefine<Elf64_Xword>' >> "$output_dir/$em_r"
echo 'BINARY_PLUGIN_NAMESPACE, plugin_framework' >> "$output_dir/$em_r"
echo 'BINARY_PLUGIN_OBJECTNAME, defines_EM_R_mapping' >> "$output_dir/$em_r"

special=('S390:390' 'IA_64:IA64' 'ALTERA_NIOS2:NIOS2')

for i in `grep '^#define[ 	]*EM_' "$elf_header" | awk '{print $2;}'`; do
    suffix=${i#EM_}
    for j in ${special[@]}; do
        if [ "$suffix" = "${j/:*/}" ]; then
            suffix="${j/*:/}"
            break
        fi
    done
    sed ':s; /^#define[ \t]*R_'"${suffix}"'_/ { /\/\*/ { /\*\// { s/\n//g; s/[ \t]\+/ /g; p;d;b; }; N; b s; }; s/[ \t]\+/ /g; p;d;b; }; d;' "$elf_header" > "$output_dir/R_${suffix}.compact.h"
    if [ -z "`head -1 "$output_dir/R_${suffix}.compact.h"`" ]; then
        rm -f "$output_dir/R_${suffix}.compact.h"
        continue
    fi
    echo "$i, &defines_R_${suffix}" >> "$output_dir/$em_r"
    sed -i -e 's/"/\\"/g' -e '1i BINARY_PLUGIN_VALUE_TYPE, Elf64_Xword\
BINARY_PLUGIN_NAMESPACE, plugin_framework\
BINARY_PLUGIN_OBJECTNAME, defines_'"R_${suffix}"'\
' -e 's/^#define[ \t]\+\([A-Z0-9a-z_]\+\)[ \t]\+.*\/\*[ \t]*\(.*\)\*\//\1, "\2/' -e 't a; s/^#define[ \t]\+\([A-Z0-9a-z_]\+\).*/\1/; b; :a; s/[ \t]*$//; s/$/"/;' "$output_dir/R_${suffix}.compact.h"
    md5sum "$output_dir/R_${suffix}.compact.h" >> "$output_dir/$md5_file"
done

md5sum "$output_dir/$em_r" >> "$output_dir/$md5_file"
