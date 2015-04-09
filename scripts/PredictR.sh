#!/bin/bash

if [ $# -ne 1 ]
then
    exit 1
fi
elf_header="$1"
if [ ! -f "$elf_header" ]; then
    exit 5
fi

special=('S390:390' 'IA_64:IA64' 'ALTERA_NIOS2:NIOS2')

for i in `grep '^#define[ 	]*EM_' "$elf_header" | awk '{print $2;}'`; do
    suffix=${i#EM_}
    for j in ${special[@]}; do
        if [ "$suffix" = "${j/:*/}" ]; then
            suffix="${j/*:/}"
            break
        fi
    done
    grep -q '^#define[ 	]*R_'"${suffix}"'_' "$elf_header" &&
        echo "R_${suffix}.compact.h"
done

echo "EM_R_mapping.compact.h"
