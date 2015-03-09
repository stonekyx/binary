#!/usr/bin/python

import os

qtdir = '/usr'

# Create base environment
baseEnv = Environment()
#...further customization of base env

baseEnv.Append(CCFLAGS="-g -Wall -Wextra".split(' '))
baseEnv.Append(CPPPATH='#inc')

# Clone Qt environment
qtEnv = baseEnv.Clone()
# Set QT4DIR and PKG_CONFIG_PATH
qtEnv['ENV']['PKG_CONFIG_PATH'] = os.path.join(qtdir, 'lib/pkgconfig')
qtEnv['QT4DIR'] = qtdir
# Add qt4 tool
qtEnv.Tool('qt4')
#...further customization of qt env
qtEnv['QT4_AUTOSCAN'] = 0

# Export environments
Export('baseEnv qtEnv')

SConscript('SConscript', variant_dir='_build', duplicate=1)
