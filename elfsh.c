#include <stdio.h>
#include <stdlib.h>

#include <libelfsh.h>

const char *dynentry_type_string(elfsh_Sword type)
{
    if(type == DT_NULL) {
        return "DT_NULL: Marks end of dynamic section";
    }
    if(type == DT_NEEDED) {
        return "DT_NEEDED: Name of needed library";
    }
    if(type == DT_PLTRELSZ) {
        return "DT_PLTRELSZ: Size in bytes of PLT relocs";
    }
    if(type == DT_PLTGOT) {
        return "DT_PLTGOT: Processor defined value";
    }
    if(type == DT_HASH) {
        return "DT_HASH: Address of symbol hash table";
    }
    if(type == DT_STRTAB) {
        return "DT_STRTAB: Address of string table";
    }
    if(type == DT_SYMTAB) {
        return "DT_SYMTAB: Address of symbol table";
    }
    if(type == DT_RELA) {
        return "DT_RELA: Address of Rela relocs";
    }
    if(type == DT_RELASZ) {
        return "DT_RELASZ: Total size of Rela relocs";
    }
    if(type == DT_RELAENT) {
        return "DT_RELAENT: Size of one Rela reloc";
    }
    if(type == DT_STRSZ) {
        return "DT_STRSZ: Size of string table";
    }
    if(type == DT_SYMENT) {
        return "DT_SYMENT: Size of one symbol table entry";
    }
    if(type == DT_INIT) {
        return "DT_INIT: Address of init function";
    }
    if(type == DT_FINI) {
        return "DT_FINI: Address of termination function";
    }
    if(type == DT_SONAME) {
        return "DT_SONAME: Name of shared object";
    }
    if(type == DT_RPATH) {
        return "DT_RPATH: Library search path (deprecated)";
    }
    if(type == DT_SYMBOLIC) {
        return "DT_SYMBOLIC: Start symbol search here";
    }
    if(type == DT_REL) {
        return "DT_REL: Address of Rel relocs";
    }
    if(type == DT_RELSZ) {
        return "DT_RELSZ: Total size of Rel relocs";
    }
    if(type == DT_RELENT) {
        return "DT_RELENT: Size of one Rel reloc";
    }
    if(type == DT_PLTREL) {
        return "DT_PLTREL: Type of reloc in PLT";
    }
    if(type == DT_DEBUG) {
        return "DT_DEBUG: For debugging; unspecified";
    }
    if(type == DT_TEXTREL) {
        return "DT_TEXTREL: Reloc might modify .text";
    }
    if(type == DT_JMPREL) {
        return "DT_JMPREL: Address of PLT relocs";
    }
    if(type == DT_BIND_NOW) {
        return "DT_BIND_NOW: Process relocations of object";
    }
    if(type == DT_INIT_ARRAY) {
        return "DT_INIT_ARRAY: Array with addresses of init fct";
    }
    if(type == DT_FINI_ARRAY) {
        return "DT_FINI_ARRAY: Array with addresses of fini fct";
    }
    if(type == DT_INIT_ARRAYSZ) {
        return "DT_INIT_ARRAYSZ: Size in bytes of DT_INIT_ARRAY";
    }
    if(type == DT_FINI_ARRAYSZ) {
        return "DT_FINI_ARRAYSZ: Size in bytes of DT_FINI_ARRAY";
    }
    if(type == DT_RUNPATH) {
        return "DT_RUNPATH: Library search path";
    }
    if(type == DT_FLAGS) {
        return "DT_FLAGS: Flags for the object being loaded";
    }
    if(type == DT_ENCODING) {
        return "DT_ENCODING: Start of encoded range";
    }
    if(type == DT_PREINIT_ARRAY) {
        return "DT_PREINIT_ARRAY: Array with addresses of preinit fct";
    }
    if(type == DT_PREINIT_ARRAYSZ) {
        return "DT_PREINIT_ARRAYSZ: size in bytes of DT_PREINIT_ARRAY";
    }
    if(type == DT_NUM) {
        return "DT_NUM: Number used";
    }
    if(type >= DT_LOOS && type <= DT_HIOS) {
        return "OS-specific";
    }
    if(type >= DT_LOPROC && type <= DT_HIPROC) {
        return "Processor-specific";
    }
    if(type == DT_PROCNUM) {
        return "DT_PROCNUM: Most used by any processor";
    }

    /* DT_* entries which fall between DT_VALRNGHI & DT_VALRNGLO use the
       Dyn.d_un.d_val field of the Elf*_Dyn structure.  This follows Sun's
       approach.  */
    if(type == DT_VALRNGLO) {
        return "DT_VALRNGLO";
    }
    if(type == DT_GNU_PRELINKED) {
        return "DT_GNU_PRELINKED: Prelinking timestamp";
    }
    if(type == DT_GNU_CONFLICTSZ) {
        return "DT_GNU_CONFLICTSZ: Size of conflict section";
    }
    if(type == DT_GNU_LIBLISTSZ) {
        return "DT_GNU_LIBLISTSZ: Size of library list";
    }
    if(type == DT_CHECKSUM) {
        return "DT_CHECKSUM";
    }
    if(type == DT_PLTPADSZ) {
        return "DT_PLTPADSZ";
    }
    if(type == DT_MOVEENT) {
        return "DT_MOVEENT";
    }
    if(type == DT_MOVESZ) {
        return "DT_MOVESZ";
    }
    if(type == DT_FEATURE_1) {
        return "DT_FEATURE_1: Feature selection (DTF_*).";
    }
    if(type == DT_POSFLAG_1) {
        return "DT_POSFLAG_1: Flags for DT_* entries, effecting the following DT_* entry.";
    }
    if(type == DT_SYMINSZ) {
        return "DT_SYMINSZ: Size of syminfo table (in bytes)";
    }
    if(type == DT_SYMINENT) {
        return "DT_SYMINENT: Entry size of syminfo";
    }
    if(type == DT_VALRNGHI) {
        return "DT_VALRNGHI";
    }
    if(type == DT_VALNUM) {
        return "DT_VALNUM";
    }

    /* DT_* entries which fall between DT_ADDRRNGHI & DT_ADDRRNGLO use the
       Dyn.d_un.d_ptr field of the Elf*_Dyn structure.

       If any adjustment is made to the ELF object after it has been
       built these entries will need to be adjusted.  */
    if(type == DT_ADDRRNGLO) {
        return "DT_ADDRRNGLO";
    }
    if(type == DT_GNU_HASH) {
        return "DT_GNU_HASH: GNU-style hash table.";
    }
    if(type == DT_TLSDESC_PLT) {
        return "DT_TLSDESC_PLT";
    }
    if(type == DT_TLSDESC_GOT) {
        return "DT_TLSDESC_GOT";
    }
    if(type == DT_GNU_CONFLICT) {
        return "DT_GNU_CONFLICT: Start of conflict section";
    }
    if(type == DT_GNU_LIBLIST) {
        return "DT_GNU_LIBLIST: Library list";
    }
    if(type == DT_CONFIG) {
        return "DT_CONFIG: Configuration information.";
    }
    if(type == DT_DEPAUDIT) {
        return "DT_DEPAUDIT: Dependency auditing.";
    }
    if(type == DT_AUDIT) {
        return "DT_AUDIT: Object auditing.";
    }
    if(type == DT_PLTPAD) {
        return "DT_PLTPAD: PLT padding.";
    }
    if(type == DT_MOVETAB) {
        return "DT_MOVETAB: Move table.";
    }
    if(type == DT_SYMINFO) {
        return "DT_SYMINFO: Syminfo table.";
    }
    if(type == DT_ADDRRNGHI) {
        return "DT_ADDRRNGHI";
    }
    if(type == DT_ADDRNUM) {
        return "DT_ADDRNUM";
    }

    /* The versioning entry types.  The next are defined as part of the
       GNU extension.  */
    if(type == DT_VERSYM) {
        return "DT_VERSYM";
    }

    if(type == DT_RELACOUNT) {
        return "DT_RELACOUNT";
    }
    if(type == DT_RELCOUNT) {
        return "DT_RELCOUNT";
    }

    /* These were chosen by Sun.  */
    if(type == DT_FLAGS_1) {
        return "DT_FLAGS_1: State flags, see DF_1_* below.";
    }
    if(type == DT_VERDEF) {
        return "DT_VERDEF: Address of version definition table";
    }
    if(type == DT_VERDEFNUM) {
        return "DT_VERDEFNUM: Number of version definitions";
    }
    if(type == DT_VERNEED) {
        return "DT_VERNEED: Address of table with needed versions";
    }
    if(type == DT_VERNEEDNUM) {
        return "DT_VERNEEDNUM: Number of needed versions";
    }
    if(type == DT_VERSIONTAGNUM) {
        return "DT_VERSIONTAGNUM";
    }

    /* Sun added these machine-independent extensions in the "processor-specific"
       range.  Be compatible.  */
    if(type == DT_AUXILIARY) {
        return "DT_AUXILIARY: Shared object to load before self";
    }
    if(type == DT_FILTER) {
        return "DT_FILTER: Shared object to get values from";
    }
    if(type == DT_EXTRANUM) {
        return "DT_EXTRANUM";
    }
    return "Unknown";
}

void print_dynamic(elfshobj_t *obj)
{
    unsigned int i;
    unsigned int num;
    elfsh_Dyn *dyn = elfsh_get_dynamic(obj, &num);

    printf("%d entries found.\n", num);
    for(i=0; i<num; i++) {
        elfsh_Sword type = elfsh_get_dynentry_type(dyn+i);
        printf("-----Entry %d-----\n", i+1);
        printf("Type: %s\n", dynentry_type_string(type));
        if(type == DT_NEEDED) {
            printf("Needed library: %s\n", elfsh_get_dynentry_string(obj, dyn+i));
        }
    }
}

int main(int argc, char *argv[])
{
    elfshobj_t *obj;
    if(argc<2) {
        exit(2);
    }
    obj = elfsh_load_obj(argv[1]);
    print_dynamic(obj);
    elfsh_unload_obj(obj);
    return 0;
}

