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

py = import('python').find_installation(pure: false)
py.extension_module(
    'pysorteddict',
    source_files,
    install: true,
)
