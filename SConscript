#!/usr/bin/python

Import('baseEnv')

env = baseEnv.Clone()

env.Append(CCFLAGS='-g -Wall -Wextra'.split(' '))

env.Program(target='main',
        source=['main.c'],
        LIBS=['elf'])

env.Program(target='elfsh',
        source=['elfsh.c'],
        LIBS=['elfsh64'],
        CPPDEFINES=['ERESI64'])

SConscript('frontend/SConscript', duplicate=1)
