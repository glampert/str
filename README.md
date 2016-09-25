
Simple C++ string classes that are partially compatible with `std::string`.
This code is based on another C++ string class found at: https://github.com/ocornut/str

I've added several new functionalities to the original code and fixed a few small issues
and limitations of the original implementation, as well as adding full support for C++11.

This single file library is comprised of the classes `str`, `str_sized<>` and `str_ref`.

`str` is similar to `std::string` and allocates dynamic memory. `str_sized`
is a template class with an embedded local buffer for small strings that
avoids a default memory allocation but can also resort to the heap if the
string grows too big for the local buffer. `str_ref` is a convenient way
of wrapping a C-style `char*` string into a C++ string class without taking
ownership of the pointer and without copying the whole string.

This is a single header file library, so you have to include the file at least *once*
in a `.cpp` and defined `STR_IMPLEMENTATION` to enable the implementation on that file.
After that, `str.hpp` can be used as a normal header file elsewhere.

Example:

```cpp
// some_file.cpp
#define STR_IMPLEMENTATION
#include "str.hpp"
```

**LICENSE**:

This software is in the public domain. Where that dedication is not
recognized, you are granted a perpetual, irrevocable license to copy,
distribute, and modify this file as you see fit.

**NOTES:**

- This isn't a fully featured string class but should be usable enough and a nice utility for when you need finer control over memory. Still, not a drop-in replacement for `std::string`.
- Maximum string size currently limited to 2 MB (we allocate 21 bits to hold the capacity).
- Local buffer size is currently limited to 1023 bytes (we allocate only 10 bits to hold the local buffer size).
- In "non-owned" mode for literals/reference we don't do any tracking/counting of references.
- This code hasn't been tested very much. There is a small set of unit tests included, but their coverage is likely small.

The idea is that you can provide an arbitrary sized local buffer if you expect string to fit
most of the time, and then you avoid using costly dynamic allocations (Small String Optimization - SSO).

No local buffer, always use heap:

```cpp
str s = "hey";
```

Local buffer of 16 bytes, `sizeof(str)+16` bytes:

```cpp
str16 s1 = "filename.h"; // copy into local buffer
str16 s2 = "long_filename_not_very_long_but_longer_than_expected.h"; // use heap
```

Local buffer of 256 bytes, `sizeof(str)+256` bytes:

```cpp
str256 s = "long_filename_not_very_long_but_longer_than_expected.h"; // copy into local bufer
```

Format string helpers:

```cpp
str256 filename;
filename.setf("%s/%s.tmp", folder, filename);
```

You can also pass your `strXXX*` as a `str*` to functions and it will still work.

You can also copy references/literal pointer without allocating:

```cpp
str s;
s.set_ref("hey!"); // setter for literals/references
```

Or via the helper `str_ref` class directly:

```cpp
str_ref s{ "hey!" }; // constructor for literals/reference
```

