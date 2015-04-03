#ifndef PLUGIN_PLUGIN_FRAMEWORK_DEFINES_H
#define PLUGIN_PLUGIN_FRAMEWORK_DEFINES_H

#include <elf.h>
#include <vector>
#include <cstdlib>

#include "common.h"

#include "defines/all-extern.h"

BEGIN_PLUG_NAMESPACE(plugin_framework)

class Defines {
public:
    static const char *macroText_DT(Elf64_Sxword);
    static const char *commentText_DT(Elf64_Sxword);
    static std::vector<const char *> macroText_SHF(Elf64_Xword);
    static std::vector<const char *> commentText_SHF(Elf64_Xword);
    static const char *macroText_SHT(Elf64_Word);
    static const char *commentText_SHT(Elf64_Word);
    static const char *macroText_STB(unsigned char);
    static const char *commentText_STB(unsigned char);
    static const char *macroText_STT(unsigned char);
    static const char *commentText_STT(unsigned char);
    static const char *macroText_STV(unsigned char);
    static const char *commentText_STV(unsigned char);
    static const char *macroText_PT(Elf64_Word);
    static const char *commentText_PT(Elf64_Word);
    static size_t commentText_PF(Elf64_Word, char*);
    static const char *macroText_EM(Elf64_Half);
    static const char *commentText_EM(Elf64_Half);
    static const char *macroText_ET(Elf64_Half);
    static const char *commentText_ET(Elf64_Half);
    static const char *macroText_EV(Elf64_Word);
    static const char *commentText_EV(Elf64_Word);
    static const char *macroText_ELFCLASS(int);
    static const char *commentText_ELFCLASS(int);
};

END_PLUG_NAMESPACE

#endif
