#!/usr/bin/python

import os

qtdir = '/usr'

# Create base environment
baseEnv = Environment()
#...further customization of base env

conf = Configure(baseEnv)
if not conf.CheckHeader('elf.h'):
    print 'elf.h not found!'
    Exit(1)
baseEnv = conf.Finish()

baseEnv.Append(CCFLAGS="-g -Wall -Wextra -Werror".split(' '))
baseEnv.Append(CPPPATH='#inc')
baseEnv['LOCAL_INSTALLDIR'] = os.path.join(Dir('#').abspath, '_install')
baseEnv['LOCAL_BINDIR'] = os.path.join(baseEnv['LOCAL_INSTALLDIR'], 'bin')
baseEnv['LOCAL_LIBDIR'] = os.path.join(baseEnv['LOCAL_INSTALLDIR'], 'lib')
baseEnv['LOCAL_DATADIR'] = os.path.join(baseEnv['LOCAL_INSTALLDIR'], 'share', 'binary')

# Clone Qt environment
qtEnv = baseEnv.Clone()
# Set QT4DIR and PKG_CONFIG_PATH
qtEnv['ENV']['PKG_CONFIG_PATH'] = os.path.join(qtdir, 'lib/pkgconfig')
qtEnv['QT4DIR'] = qtdir
# Add qt4 tool
qtEnv.Tool('qt4', toolpath=['vendor/tools'])
#...further customization of qt env
qtEnv['QT4_AUTOSCAN'] = 0

# Export environments
Export('baseEnv qtEnv')

SConscript('SConscript', variant_dir='_build', duplicate=1)
