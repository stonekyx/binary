#!/usr/bin/python

import os
import subprocess

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

# Test helpers
testlib = SConscript(dirs='test', exports='baseEnv', variant_dir='_build/test', duplicate=1)

def build_or_run_test(target, source, env):
    program = env.Program('unittest', source, LIBS=[testlib])
    env.AlwaysBuild(env.Alias('test', [program], program[0].abspath))
testBld = Builder(action = build_or_run_test)
baseEnv.Append(BUILDERS={'Test' : testBld})

def build_or_run_test_dir(self, path, libs):
    program = self.Program(os.path.join(path, 'unittest'),
            Glob(os.path.join(path, '*.cpp')),
            LIBS=['cppunit', testlib]+libs)
    self.AlwaysBuild(self.Alias('test', [program], program[0].abspath))
baseEnv.AddMethod(build_or_run_test_dir, 'TestDir')

# Other helpers
def get_library_dir(baseEnv, lib):
    fullPath = subprocess.check_output([baseEnv['CC'], '-print-file-name='+lib])
    if fullPath.rfind('/')==-1:
        return None
    return fullPath[:fullPath.rfind('/')]
baseEnv.AddMethod(get_library_dir, 'LibraryDir')

# Find header's full path used by compiler, in case of non-standard locations.
def get_header_dir(env, header):
    p = subprocess.Popen([env['CC'], '-E', '-'],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=open(os.devnull, 'w'))
    (out,err) = p.communicate('#include <' + header + '>')
    if p.wait() == None:
        p.kill()
    outputLines = [x for x in out.split('\n') if x.find(os.path.basename(header))!=-1]
    if len(outputLines)>0 and len(outputLines[0].split('"'))>1:
        path = outputLines[0].split('"')[1]
        return os.path.dirname(path)
    else:
        return None
baseEnv.AddMethod(get_header_dir, 'HeaderDir')

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
