#!/usr/bin/python

env = Environment()

env.Append(CCFLAGS='-g -Wall -Wextra'.split(' '))

env.Program(target='main',
        source=['main.c'],
        LIBS=['elf'])

env.Program(target='elfsh',
        source=['elfsh.c'],
        LIBS=['elfsh64'],
        CPPDEFINES=['ERESI64'])

# SConscript('a/SConscript', exports='env', duplicate=0)
