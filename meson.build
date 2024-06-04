#Only to perform standalone tests during dev.
project(
    'test-demo',
    ['c', 'cpp'],
    default_options: ['c_std=gnu11', 'cpp_std=gnu++20'],
)

test_dev = executable(
    'dev',
    ['./src/dev/main.cpp', './src/native/sow.cpp'],
    include_directories: ['./src/native'],
)

test('basic test', test_dev)