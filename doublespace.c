#define PY_SSIZE_T_CLEAN 1		/* Use Py_ssize_t */
#include <string.h>
#include <Python.h>
#include <deds.h>

static PyObject *doublespace_decompress(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
	PyObject *errno_tuple;
	size_t result_length;
	Py_buffer in, out;
	int ret;

	if (PyVectorcall_NARGS(nargs) != 2) {
		return PyErr_Format(PyExc_TypeError,
				    "decompress() takes exactly two arguments (%zd given)",
				    nargs);
	}

	if (!PyArg_Parse(args[0], "y*:doublespace", &in)) {
		return NULL;
	}

	if (!PyArg_Parse(args[1], "w*:doublespace", &out)) {
		PyBuffer_Release(&in);
		return NULL;
	}

	ret = ds_decompress(in.buf, in.len, out.buf, out.len, &result_length);
	PyBuffer_Release(&in);
	PyBuffer_Release(&out);
	if (ret < 0) {
		errno_tuple = Py_BuildValue("(is)", -ret, strerror(-ret));
		if (!errno_tuple) {
			return NULL;
		}

		PyErr_SetObject(PyExc_OSError, errno_tuple);
		Py_DecRef(errno_tuple);
		return NULL;
	}

	if (result_length != out.len) {
		PyErr_SetString(PyExc_RuntimeError, "Decompression did not consume all data");
		return NULL;
	}

	return PyLong_FromLong(ret);
}

static PyMethodDef doublespace_methods[] = {
	{
		.ml_name = "decompress",
		.ml_meth = (PyCFunction)doublespace_decompress,
		.ml_flags = METH_FASTCALL,
		.ml_doc = PyDoc_STR(
			"decompress($module, in, out, /)\n"
			"--\n"
			"\n"
			"Performs doublespace decompression of the binary data inside\n"
			"the bytes-like object \"in\" and puts the resulting data into\n"
			"the bytes-like object \"out\"."),
	},
	{NULL, NULL, 0, NULL},
};

static int doublespace_exec(PyObject *module)
{
	return PyModule_AddFunctions(module, doublespace_methods);
}

static PyModuleDef_Slot doublespace_slots[] = {
	{Py_mod_multiple_interpreters, Py_MOD_PER_INTERPRETER_GIL_SUPPORTED},
	{Py_mod_exec, doublespace_exec},
	{0, NULL},
};

static PyModuleDef doublespace_def = {
	.m_base = PyModuleDef_HEAD_INIT,
	.m_name = "doublespace",
	.m_doc = PyDoc_STR("Python extension for doublespace decompression"),
	.m_slots = doublespace_slots,
};

PyMODINIT_FUNC PyInit_doublespace(void)
{
	return PyModuleDef_Init(&doublespace_def);
}
