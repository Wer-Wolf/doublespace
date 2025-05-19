# doublespace

[![Hatch project](https://img.shields.io/badge/%F0%9F%A5%9A-Hatch-4051b5.svg)](https://github.com/pypa/hatch)
![Tests](https://github.com/Wer-Wolf/doublespace/actions/workflows/Test.yaml/badge.svg)

Python C extension for doublespace decompression, based on the [libdeds](https://github.com/Wer-Wolf/libdeds) library.

## Requirements

Python >= 3.12 is required to use this extension. Additionally not all python implementations
support C extensions.

## Installation

```sh
python3 -m pip install doublespace
```

## Example

This example uses the ``decompress`` function to perform doublespace decompression.
Please keep in mind that a output buffer with a mismatched size will result in an
exception. Usually the container format used for carrying doublespace-compressed
data tells you the size of the decompressed data.

```
from doublespace import decompress

out = bytearray(255)    # buffer for output data
in = b'\xBE\xEF'        # input data (bogus data)

version: int = decompress(in, out)

print(f"Format version: {version}")
print(f"Result: {out}")
```
