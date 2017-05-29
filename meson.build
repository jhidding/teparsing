# meson project file

project('Parsing library', 'cpp',
    version: '0.0.1',
    license: 'Apache v2')

subdir('./src')

local_include = include_directories('src')

add_project_arguments(
    '-std=c++17',
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


test_predicates = executable(
    'test-predicates',
    test_predicates_files,
    dependencies: [gtest_dep],
    include_directories: local_include)

test('predicates', test_predicates)

