#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyModuleDef sorted_dict_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pysorteddict",
    .m_doc = "enriches Python with a sorted dictionary\n\n"
             "See https://tfpf.github.io/pysorteddict/.",
    .m_size = 0,
};

PyMODINIT_FUNC PyInit_pysorteddict(void)
{
    return PyModuleDef_Init(&sorted_dict_module);
}
