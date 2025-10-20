project(
    'tosu-overlay', 
    'cpp', 
    version : '1.0.0',  
    license : 'MIT'
)

app_version = vcs_tag(
  command: ['git', 'describe'],
  input:  'include/version.h.in',
  output: 'version.h')

app_name = meson.project_name()
add_project_arguments('-DAPPLICATION_NAME="' + app_name + '"', language : 'cpp')

# Find dependencies
qt = import('qt6')
qt_dep = dependency('Qt6', required: true, modules: ['Core', 'DBus', 'Widgets', 'WebEngineWidgets'])
layershellqt_dep = dependency('LayerShellQt', required: true, modules: ['LayerShellQt::Interface'])

src_files = files(
  'src/commandline.cpp',
  'src/ipc.cpp',
  'src/main.cpp',
  'src/overlay.cpp',
  'src/systemtray.cpp',
  'src/webview.cpp'
)

moc_header_files = files(
  'include/ipc.h',
  'include/overlay.h',
  'include/systemtray.h',
  'include/webview.h'
)

header_files = files(
  'include/commandline.h'
) + moc_header_files

qrc_files = files('resources/resources.qrc')
moc_files = qt.compile_moc(headers: moc_header_files, dependencies: qt_dep)

# Compile the resources
qt_res = qt.compile_resources(name: 'resources', sources: qrc_files)

# Add include directory for header files
inc = include_directories('include')

# Add executable target
executable = executable(
    app_name, 
    header_files + src_files + moc_files + qt_res, 
    dependencies: [qt_dep, layershellqt_dep], 
    include_directories: inc,
    install: true
)
