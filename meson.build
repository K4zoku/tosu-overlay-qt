project('tosu-overlay', 'cpp', version: '1.0.0', license: 'MIT')

# Define application name
app_name = meson.project_name()
add_project_arguments('-DAPPLICATION_NAME="' + app_name + '"', language: 'cpp')

# Define application version
app_version = vcs_tag(
  command: ['git', 'describe'],
  input: 'include/version.h.in',
  output: 'version.h',
)

# Find dependencies
if get_option('use_qt6')
qt = import('qt6')
qt_dep = dependency(
  'Qt6',
  required: true,
  modules: ['Core', 'DBus', 'Widgets', 'WebEngineWidgets'],
)
else
qt = import('qt5')
qt_dep = dependency(
  'Qt5',
  required: true,
  modules: ['Core', 'DBus', 'Widgets', 'WebEngineWidgets'],
)
endif
layershellqt_dep = dependency('LayerShellQt', required: true, modules: ['LayerShellQt::Interface'])

# Source files
src_files = files(
  'src/beveledbutton.cpp',
  'src/commandline.cpp',
  'src/ipc.cpp',
  'src/main.cpp',
  'src/overlay.cpp',
  'src/systemtray.cpp',
  'src/webview.cpp',
)

# Header files that need moc
moc_header_files = files(
  'include/beveledbutton.h',
  'include/ipc.h',
  'include/overlay.h',
  'include/systemtray.h',
  'include/webview.h',
)

# Compile moc
moc_files = qt.compile_moc(headers: moc_header_files, dependencies: qt_dep)

# Header files
header_files = files('include/commandline.h') + moc_header_files

# Resources
qrc_files = files('res/resources.qrc')

# Compile the resources
qt_res = qt.compile_resources(name: app_name + '-resources', sources: qrc_files)

# Add include directory for header files
inc = include_directories('include')

# Add executable target
executable(
  app_name,
  src_files + moc_files + qt_res + app_version,
  dependencies: [qt_dep, layershellqt_dep],
  include_directories: inc,
  install: true,
)