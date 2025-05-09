project(
    'doublespace',
    'c',
    default_options: ['python.install_env=auto'],
    license: 'GPL-2.0-or-later',
    meson_version: '>=1.3.0',
    version: '0.0.1',
)

py = import('python').find_installation('python3')

libdeds = subproject('libdeds')
libdeds_dep = libdeds.get_variable('libdeds_dep')

py.extension_module(
    'doublespace',
    'doublespace.c',
    dependencies: libdeds_dep,
    limited_api: '3.12',
    install: true,
)
