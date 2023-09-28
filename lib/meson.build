libdarknotify_headers = ['./siga/dark_notify/dark_notify.hpp', './siga/dark_notify/appearance.hpp']
libdarknotify_includes = include_directories('.')
libdarknotify_sources = ['./siga/dark_notify/dark_notify.cpp']
libdarknotify_dependencies = []

if host_machine.system().startswith('darwin')
  libdarknotify_sources += './siga/dark_notify/impl/macos/impl.mm'
  libdarknotify_dependencies += dependency('appleframeworks', modules: ['Cocoa'])
  libdarknotify_headers += './siga/dark_notify/impl/macos/impl.hpp'
endif

libdarknotify = library(
  'darknotify',
  dependencies: libdarknotify_dependencies,
  include_directories: libdarknotify_includes,
  sources: libdarknotify_sources,
  install: true
)

install_headers(libdarknotify_headers, preserve_path: true)
