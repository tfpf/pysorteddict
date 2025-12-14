project('pysorteddict', 'cpp')

source_directory = 'src' / 'pysorteddict'
source_files = files(
    source_directory / 'sorted_dict_items_type.cc',
    source_directory / 'sorted_dict_keys_type.cc',
    source_directory / 'sorted_dict_module.cc',
    source_directory / 'sorted_dict_type.cc',
    source_directory / 'sorted_dict_values_type.cc',
    source_directory / 'sorted_dict_view_type.cc',
)

if host_machine.system() == 'windows'
    cpp_arg_cto_off = '/Od'
    cpp_arg_lto_on = '/GL'
    cpp_arg_std = '/std:c++20'
    link_arg_lto_on = '/LTCG'
else
    cpp_arg_cto_off = '-O0'
    cpp_arg_lto_on = '-flto'
    cpp_arg_std = '-std=c++20'
    link_arg_lto_on = '-flto'
endif
cpp_args = [cpp_arg_std]
link_args = []
if get_option('b_coverage')
    cpp_args += [cpp_arg_cto_off]
else
    cpp_args += [cpp_arg_lto_on]
    link_args += [link_arg_lto_on]
endif

py = import('python').find_installation(pure: false)
py.extension_module(
    'pysorteddict',
    source_files,
    cpp_args: cpp_args,
    link_args: link_args,
    install: true,
)
