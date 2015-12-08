
### This is a C++11 "port" of the `Str` classes found [here](https://github.com/ocornut/str).

Change List:

- Fixed a couple warnings on Clang (now compiles cleanly with `-std=c++11 -O3 -Wall -Wextra -Weffc++ -pedantic`).
- Added C++11 extensions (`final`, `using` alias, etc).
- Replaced macros with a single template class.
- Changed 'Owned' flag from `unsigned int` to `bool` so it is more idiomatic.
- Added new set of comparison methods and string search helpers, including case-insensitive comparison support.
- Added the full set of comparison operators, now non-members at the global scope.
- Added `__attribute__((format))` annotation to the formatting functions (right now GCC and Clang only).
- String now keeps it's own length to avoid O(N) `strlen` calls. A better trade-of than saving a single `int` member variable.
- Array subscript operator `[]` now asserts that the index is withing the string bounds.
- Imported dependencies (`#includes`) are now explicit in the header file.
- Removed redundant `inline` keywords within the class body.
- Added `static_assert` to ensure max size of local buffer in `StrSized<N>` is enforced at compile-time.
- Added minimal unit tests (see `Tests.cpp`).
- Header file is now `Str.hpp` just to keep the filename extension three letters long `;)`.
- Ran Clang-Format in the source using a variation of the Mozilla style, so now indenting and spacing are automated and uniform.

----

### Original release notes:

Simple C++ string type with an optional local buffer.

**LICENSE**:

This software is in the public domain. Where that dedication is not
recognized, you are granted a perpetual, irrevocable license to copy,
distribute, and modify this file as you see fit.

- This isn't a fully featured string class.
- Maximum string size currently limited to 2 MB (we allocate 21 bits to hold capacity).
- Local buffer size is currently limited to 1023 bytes (we allocate 10 bits to hold local buffer size).
- In "non-owned" mode for literals/reference we don't do any tracking/counting of references.
- This code hasn't been tested very much. It is probably incomplete or broken. Made it for my own use.

The idea is that you can provide an arbitrary sized local buffer if you expect string to fit
most of the time, and then you avoid using costly heap.

No local buffer, always use heap, `sizeof() == 8`:

    Str s = "hey";

Local buffer of 16 bytes, `sizeof() == 8+16` bytes:

    Str16 s = "filename.h"; // copy into local buffer
    Str16 s = "long_filename_not_very_long_but_longer_than_expected.h";   // use heap

Local buffer of 256 bytes, `sizeof() == 8+256` bytes:

    Str256 s = "long_filename_not_very_long_but_longer_than_expected.h";  // copy into local bufer

Format string helpers:

    Str256 filename;
    filename.setf("%s/%s.tmp", folder, filename);

You can pass your `StrXXX*` as a `Str*` to functions and it will still be functional.

You can also copy references/literal pointer without allocation:

    Str s;
    s.set_ref("hey!");   // setter for literals/references

Or via the helper constructor:

    StrRef("hey!");      // constructor for literals/reference

