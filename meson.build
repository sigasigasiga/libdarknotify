project('libdarknotify', 'cpp', default_options: ['cpp_std=c++20'])

if host_machine.system().startswith('darwin')
  add_languages('objcpp')
endif

subdir('lib')

# FIXME: build this subdir conditionally
subdir('example')
