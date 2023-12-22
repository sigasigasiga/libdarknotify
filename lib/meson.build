darknotify_includes = include_directories('.')
darknotify_headers = [
  './siga/dark_notify/dark_notify.hpp',
  './siga/dark_notify/util/basic_scoped_handle.hpp',
  './siga/dark_notify/util/export.hpp',
]
darknotify_sources = []
darknotify_dependencies = []

if host_machine.system().startswith('darwin')
  darknotify_headers += './siga/dark_notify/impl/macos/nsapp.h'

  darknotify_sources += [
    './siga/dark_notify/dark_notify.mm',
    './siga/dark_notify/impl/macos/nsapp.mm'
  ]

  darknotify_dependencies += dependency('appleframeworks', modules: ['Cocoa'])
elif host_machine.system().startswith('windows')
  darknotify_headers += [
    './siga/dark_notify/impl/windows/reg_monitor.hpp',
    './siga/dark_notify/impl/windows/util/error.hpp',
    './siga/dark_notify/impl/windows/util/winapi_scoped_handle.hpp',
  ]

  darknotify_sources += [
    './siga/dark_notify/dark_notify.cpp',
    './siga/dark_notify/impl/windows/reg_monitor.cpp',
    './siga/dark_notify/impl/windows/util/cancellable_event.cpp',
  ]
elif host_machine.system().startswith('linux')
  darknotify_dependencies += [
    dependency('glib-2.0'),
    dependency('gobject-2.0'),
    dependency('gio-2.0'),
  ]

  darknotify_headers += [
    './siga/dark_notify/impl/xdg/glib.hpp',
  ]

  darknotify_sources += [
    './siga/dark_notify/dark_notify.cpp',
    './siga/dark_notify/impl/xdg/glib.cpp',
  ]
endif

darknotify_lib = library(
  'darknotify',
  dependencies: darknotify_dependencies,
  include_directories: darknotify_includes,
  sources: darknotify_sources,
  install: true
)

darknotify_dep = declare_dependency(
  link_with: darknotify_lib,
  include_directories: darknotify_includes
)

install_headers(darknotify_headers, preserve_path: true)
