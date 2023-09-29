libdarknotify_includes = include_directories('.')
libdarknotify_headers = ['./siga/dark_notify/dark_notify.hpp']
libdarknotify_sources = []
libdarknotify_dependencies = []

if host_machine.system().startswith('darwin')
  libdarknotify_headers += './siga/dark_notify/impl/macos/nsapp.h'
  libdarknotify_sources += ['./siga/dark_notify/dark_notify.mm', './siga/dark_notify/impl/macos/nsapp.mm']
  libdarknotify_dependencies += dependency('appleframeworks', modules: ['Cocoa'])
endif

libdarknotify = library(
  'darknotify',
  dependencies: libdarknotify_dependencies,
  include_directories: libdarknotify_includes,
  sources: libdarknotify_sources,
  install: true
)

install_headers(libdarknotify_headers, preserve_path: true)
