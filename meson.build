project('pysorteddict', 'cpp')

source_directory = 'src/pysorteddict/'
source_files = files(
    source_directory + 'sorted_dict_items_type.cc',
    source_directory + 'sorted_dict_keys_type.cc',
    source_directory + 'sorted_dict_module.cc',
    source_directory + 'sorted_dict_type.cc',
    source_directory + 'sorted_dict_values_type.cc',
    source_directory + 'sorted_dict_view_type.cc',
)

if host_machine.system() == 'windows'
    cpp_args = ['/GL', '/std:c++20']
    link_args = ['/LTCG']
else
    cpp_args = ['-flto', '-std=c++20']
    link_args = ['-flto']
endif

py = import('python').find_installation(pure: false)
py.extension_module(
    'pysorteddict',
    source_files,
    cpp_args: cpp_args,
    link_args: link_args,
    install: true,
)
