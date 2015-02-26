#!/usr/bin/python

env = Environment()

env.Append(CCFLAGS='-g -Wall -Wextra'.split(' '))

env.Program(target='main',
        source=['main.c'],
        LIBS=['elf'])

# SConscript('a/SConscript', exports='env', duplicate=0)
