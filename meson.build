# meson project file

project('Parsing library', 'cpp',
    version: '0.0.1',
    license: 'Apache v2')

subdir('./src')

local_include = include_directories('src')

add_project_arguments(
    ['-Wall', '-Werror', '-std=c++17'],
    include_directories: local_include,
    language: 'cpp')

# ===========================================================================

subdir('./test')

gtest_dep = dependency('gtest')

test_trivial = executable(
    'test-trivial',
    test_trivial_files,
    dependencies: [gtest_dep],
    include_directories: local_include)

test('trivial', test_trivial)


test_parsing = executable(
    'test-parsing',
    test_parsing_files,
    dependencies: [gtest_dep],
    include_directories: local_include)

test('parsing', test_parsing)

