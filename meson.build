project('libdarknotify', 'cpp', default_options: ['cpp_std=c++17'])

if host_machine.system().startswith('darwin')
  add_languages('objcpp')
endif

subdir('lib')
subdir('example')
