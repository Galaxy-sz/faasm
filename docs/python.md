# Python 

Faasm executes functions compiled to WebAssembly, which rules out _directly_
executing languages which can't be compiled to WebAssembly. However, we can
compile certain language runtimes to WebAssembly, then use these to execute
non-WebAssembly code.

This is true of CPython, which we can compile to WebAssembly (along with Python
C-extensions). This is covered in the [Faasm CPython
repo](https://github.com/faasm/faasm-cpython).

A separate Python library, [Pyfaasm](https://github.com/faasm/pyfaasm),
contains the custom C-extensions and decorators to support the [Faasm host
interface](host_interface.md). 

## Enabling Python support

Full Python support is **not enabled by default**. To enable the Python runtime
you must set up the relevant environment variables:

```bash
# On the "upload" container/ endpoint (see docker-compose.yml locally)
PYTHON_CODEGEN=on
```

The first time the system runs it will generate machine code for CPython and all
the Python C-extensions. This can take around a minute. 

## Running a Python function

An example Python function is found at `func/python/hello.py`. This can be
uploaded and invoked from the Faasm CLI with:

```bash
inv upload python hello --py
inv invoke python hello --py
```

This should give a message and the version of Python being run.

## Python API

A simple example of chaining Python functions in Faasm looks like:

```python
from pyfaasm.core import await_call, chain_this_with_input

def func_one(data):
    pass

def func_two(data):
    pass

def faasm_main():
    call_one = chain_this_with_input(func_one, b'')
    call_two = chain_this_with_input(func_two, b'')

    await_call(call_one)
    await_call(call_two)
```

A couple of the Python test functions also demonstrate chaining:

- [Simple example](../func/python/chain.py) 
- [Chaining with shared state](../func/python/dict_state.py) 

# Building CPython and new modules

NOTE: you only need to do this if you're adding new modules or changing
something about the Python cross-compilation.

If you're using the containerised environment, you need to mount the
[`faasm-cpython` repo](https://github.com/faasm/faasm-cpython) into your CLI 
container. 

You can then use the tasks in `faasm-cpython` to build CPython, rebuild the
Faasm python environment, and execute. 

Note that to copy over the cross-compiled Python libraries, you still need to
run:

```
inv python.runtime
```
