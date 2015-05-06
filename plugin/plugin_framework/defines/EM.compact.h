/*
 * Copyright 2015 KANG Yuxuan
 *
 * EM.compact.h by KANG Yuxuan <stonekyx@gmail.com>
 *
 * This file is part of Binary.
 *
 * Binary is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Binary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Binary.  If not, see <http://www.gnu.org/licenses/>.
 */

BINARY_PLUGIN_VALUE_TYPE, Elf64_Half
BINARY_PLUGIN_NAMESPACE, plugin_framework
BINARY_PLUGIN_OBJECTNAME, defines_EM

/* Legal values for e_machine (architecture). */

EM_NONE, "No machine"
EM_M32, "AT&T WE 32100"
EM_SPARC, "SUN SPARC"
EM_386, "Intel 80386"
EM_68K, "Motorola m68k family"
EM_88K, "Motorola m88k family"
EM_860, "Intel 80860"
EM_MIPS, "MIPS R3000 big-endian"
EM_S370, "IBM System/370"
EM_MIPS_RS3_LE, "MIPS R3000 little-endian"

EM_PARISC, "HPPA"
EM_VPP500, "Fujitsu VPP500"
EM_SPARC32PLUS, "Sun's \x22v8plus\x22"
EM_960, "Intel 80960"
EM_PPC, "PowerPC"
EM_PPC64, "PowerPC 64-bit"
EM_S390, "IBM S390"

EM_V800, "NEC V800 series"
EM_FR20, "Fujitsu FR20"
EM_RH32, "TRW RH-32"
EM_RCE, "Motorola RCE"
EM_ARM, "ARM"
EM_FAKE_ALPHA, "Digital Alpha"
EM_SH, "Hitachi SH"
EM_SPARCV9, "SPARC v9 64-bit"
EM_TRICORE, "Siemens Tricore"
EM_ARC, "Argonaut RISC Core"
EM_H8_300, "Hitachi H8/300"
EM_H8_300H, "Hitachi H8/300H"
EM_H8S, "Hitachi H8S"
EM_H8_500, "Hitachi H8/500"
EM_IA_64, "Intel Merced"
EM_MIPS_X, "Stanford MIPS-X"
EM_COLDFIRE, "Motorola Coldfire"
EM_68HC12, "Motorola M68HC12"
EM_MMA, "Fujitsu MMA Multimedia Accelerator"
EM_PCP, "Siemens PCP"
EM_NCPU, "Sony nCPU embeeded RISC"
EM_NDR1, "Denso NDR1 microprocessor"
EM_STARCORE, "Motorola Start*Core processor"
EM_ME16, "Toyota ME16 processor"
EM_ST100, "STMicroelectronic ST100 processor"
EM_TINYJ, "Advanced Logic Corp. Tinyj emb.fam"
EM_X86_64, "AMD x86-64 architecture"
EM_PDSP, "Sony DSP Processor"

EM_FX66, "Siemens FX66 microcontroller"
EM_ST9PLUS, "STMicroelectronics ST9+ 8/16 mc"
EM_ST7, "STmicroelectronics ST7 8 bit mc"
EM_68HC16, "Motorola MC68HC16 microcontroller"
EM_68HC11, "Motorola MC68HC11 microcontroller"
EM_68HC08, "Motorola MC68HC08 microcontroller"
EM_68HC05, "Motorola MC68HC05 microcontroller"
EM_SVX, "Silicon Graphics SVx"
EM_ST19, "STMicroelectronics ST19 8 bit mc"
EM_VAX, "Digital VAX"
EM_CRIS, "Axis Communications 32-bit embedded processor"
EM_JAVELIN, "Infineon Technologies 32-bit embedded processor"
EM_FIREPATH, "Element 14 64-bit DSP Processor"
EM_ZSP, "LSI Logic 16-bit DSP Processor"
EM_MMIX, "Donald Knuth's educational 64-bit processor"
EM_HUANY, "Harvard University machine-independent object files"
EM_PRISM, "SiTera Prism"
EM_AVR, "Atmel AVR 8-bit microcontroller"
EM_FR30, "Fujitsu FR30"
EM_D10V, "Mitsubishi D10V"
EM_D30V, "Mitsubishi D30V"
EM_V850, "NEC v850"
EM_M32R, "Mitsubishi M32R"
EM_MN10300, "Matsushita MN10300"
EM_MN10200, "Matsushita MN10200"
EM_PJ, "picoJava"
EM_OPENRISC, "OpenRISC 32-bit embedded processor"
EM_ARC_A5, "ARC Cores Tangent-A5"
EM_XTENSA, "Tensilica Xtensa Architecture"
EM_ALTERA_NIOS2, "Altera Nios II"
EM_AARCH64, "ARM AARCH64"
EM_TILEPRO, "Tilera TILEPro"
EM_MICROBLAZE, "Xilinx MicroBlaze"
EM_TILEGX, "Tilera TILE-Gx"
/* EM_NUM		192 */

/* If it is necessary to assign new unofficial EM_* values, please
   pick large random numbers (0x8523, 0xa7f2, etc.) to minimize the
   chances of collision with official or non-GNU unofficial values. */

EM_ALPHA
