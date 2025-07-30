project('crafting-interpreters', 'c', default_options: ['c_std=c23'])

incdir = include_directories('include')

executable(
  'loxc',
  sources: [
    'src/main.c',
    'src/chunk.c',
    'src/memory.c',
    'src/debug.c',
    'src/value.c',
    'src/vm.c',
    'src/compiler.c',
    'src/scanner.c',
  ],
  include_directories: incdir,
  c_args: [
    '-Wall',
    '-Wformat',
    '-Wconversion',
    '-Wimplicit-fallthrough',
    '-Werror=format-security',
  ],
)