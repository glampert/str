
//
// Simple C++ string type with an optional local buffer.
//
// Original source: https://github.com/ocornut/str
// This fork: https://github.com/glampert/str
//
// LICENSE
// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy,
// distribute, and modify this file as you see fit.
//

#include "Str.hpp"

// On some platform vsnprintf() takes va_list by reference and modifies it.
// va_copy is the 'correct' way to copy a va_list but Visual Studio prior to 2013 doesn't have it.
#ifndef va_copy
#define va_copy(dest, src) (dest = src)
#endif // va_copy

// Used by the default constructor, which creates a valid but empty string.
// ISO C++11 does not allow conversion from string literal to char*, so
// this function prevents a warning on Clang/GCC. As long as we play nice
// and don't attempt to mutate this string, everything will work fine.
static inline char * empty_dummy_str()
{
    static char dummy[]{ '\0', '\0' };
    return dummy;
}

// ========================================================
// Str implementation:
// ========================================================

Str::Str()
    : data{ empty_dummy_str() } // Shared read-only initial buffer for 0 capacity
    , len{ 0 }
    , numAvailable{ 0 }
    , localBufSize{ 0 }
    , owned{ false }
{
}

Str::~Str()
{
    if (owned && !is_using_local_buf())
    {
        STR_MEMFREE(data);
    }
}

Str::Str(const Str & rhs)
    : Str{} // Delegate to the default constructor (C++11)
{
    set(rhs);
}

Str::Str(const char * rhs)
    : Str{} // Delegate to the default constructor (C++11)
{
    set(rhs);
}

void Str::set(const char * src)
{
    STR_ASSERT(src != nullptr);

    if (*src == '\0')
    {
        clear();
        return;
    }

    const int buf_len = static_cast<int>(std::strlen(src) + 1);
    if (numAvailable < buf_len)
    {
        reserve_discard(buf_len);
    }

    std::memcpy(data, src, buf_len);
    len   = buf_len - 1;
    owned = true;
}

void Str::set(const Str & src)
{
    if (src.empty())
    {
        clear();
        return;
    }

    const int buf_len = static_cast<int>(std::strlen(src.data) + 1);
    if (numAvailable < buf_len)
    {
        reserve_discard(buf_len);
    }

    std::memcpy(data, src.data, buf_len);
    len   = buf_len - 1;
    owned = true;
}

void Str::set_ref(const char * src)
{
    STR_ASSERT(src != nullptr);

    if (*src == '\0')
    {
        clear();
        return;
    }

    if (owned && !is_using_local_buf())
    {
        STR_MEMFREE(data);
    }

    numAvailable = 0;
    data  = const_cast<char *>(src);
    len   = static_cast<int>(std::strlen(src));
    owned = false;
}

void Str::clear()
{
    if (owned && !is_using_local_buf())
    {
        STR_MEMFREE(data);
    }

    if (localBufSize > 0)
    {
        data         = local_buf();
        data[0]      = '\0';
        len          = 0;
        numAvailable = localBufSize;
        owned        = true;
    }
    else
    {
        data         = empty_dummy_str();
        len          = 0;
        numAvailable = 0;
        owned        = false;
    }
}

void Str::reserve(int new_capacity)
{
    // Reserve memory, preserving the current of the buffer.

    if (new_capacity <= numAvailable)
    {
        return;
    }

    char * new_data;
    if (new_capacity < localBufSize)
    {
        // Disowned -> local_buf
        new_data = local_buf();
        new_capacity = localBufSize;
    }
    else
    {
        // Disowned or local_buf -> Heap
        new_data = reinterpret_cast<char *>(STR_MEMALLOC(new_capacity * sizeof(char)));
        STR_ASSERT(new_data != nullptr);
    }

    std::strcpy(new_data, data);

    if (owned && !is_using_local_buf())
    {
        STR_MEMFREE(data);
    }

    data         = new_data;
    numAvailable = new_capacity;
    owned        = true; // To be sure...
}

void Str::reserve_discard(int new_capacity)
{
    // Reserve memory, discarding the current of the
    // buffer (if we expect to be fully overwritten).

    if (new_capacity <= numAvailable)
    {
        return;
    }

    if (owned && !is_using_local_buf())
    {
        STR_MEMFREE(data);
    }

    if (new_capacity < localBufSize)
    {
        // Disowned -> local_buf
        data = local_buf();
        numAvailable = localBufSize;
    }
    else
    {
        // Disowned or local_buf -> Heap
        data = reinterpret_cast<char *>(STR_MEMALLOC(new_capacity * sizeof(char)));
        STR_ASSERT(data != nullptr);
        numAvailable = new_capacity;
    }

    // Old contents are lost.
    data[0] = '\0';
    len     = 0;
    owned   = true;
}

void Str::shrink_to_fit()
{
    if (!owned || is_using_local_buf())
    {
        return;
    }

    const int new_capacity = len + 1;
    if (numAvailable <= new_capacity)
    {
        return;
    }

    char * new_data = reinterpret_cast<char *>(STR_MEMALLOC(new_capacity * sizeof(char)));
    STR_ASSERT(new_data != nullptr);

    std::memcpy(new_data, data, new_capacity);
    STR_MEMFREE(data);

    data = new_data;
    numAvailable = new_capacity;
}

int Str::setfv(const char * fmt, va_list args)
{
    STR_ASSERT(fmt != nullptr);

    // Needed for portability on platforms where va_list
    // are passed by reference and modified by functions.
    va_list args2;
    va_copy(args2, args);

// MSVC returns -1 on overflow when writing, which forces us to do two passes.
// FIXME-OPT: Find a way around that.
#ifdef _MSC_VER
    int result = std::vsnprintf(nullptr, 0, fmt, args);
    STR_ASSERT(result >= 0);
    if (numAvailable < result + 1)
    {
        reserve_discard(result + 1);
    }
    result = std::vsnprintf(data, result + 1, fmt, args2);
#else  // !_MSC_VER
    int result = std::vsnprintf(owned ? data : nullptr, owned ? numAvailable : 0, fmt, args);
    STR_ASSERT(result >= 0);
    if (numAvailable < result + 1)
    {
        reserve_discard(result + 1);
        result = std::vsnprintf(data, result + 1, fmt, args2);
    }
#endif // _MSC_VER

    len   = (result >= 0) ? result : 0;
    owned = true;
    return len;
}

int Str::setf(const char * fmt, ...)
{
    STR_ASSERT(fmt != nullptr);

    va_list args;
    int result;

    va_start(args, fmt);
    result = setfv(fmt, args);
    va_end(args);

    return result;
}

int Str::setfv_nogrow(const char * fmt, va_list args)
{
    STR_ASSERT(fmt != nullptr);

    const int result = std::vsnprintf(data, numAvailable, fmt, args);
    data[numAvailable - 1] = '\0';

    len   = (result == -1) ? numAvailable - 1 : result;
    owned = true;
    return len;
}

int Str::setf_nogrow(const char * fmt, ...)
{
    STR_ASSERT(fmt != nullptr);

    va_list args;
    int result;

    va_start(args, fmt);
    result = setfv_nogrow(fmt, args);
    va_end(args);

    return result;
}

bool Str::starts_with(const char * prefix) const
{
    STR_ASSERT(prefix != nullptr);
    const int prefixLen = static_cast<int>(std::strlen(prefix));

    if (len == 0 || prefixLen == 0)
    {
        return false;
    }
    if (len < prefixLen)
    {
        return false;
    }

    return std::strncmp(data, prefix, prefixLen) == 0;
}

bool Str::ends_with(const char * suffix) const
{
    STR_ASSERT(suffix != nullptr);
    const int suffixLen = static_cast<int>(std::strlen(suffix));

    if (len == 0 || suffixLen == 0)
    {
        return false;
    }
    if (len < suffixLen)
    {
        return false;
    }
    if (len == suffixLen)
    {
        return std::strcmp(data, suffix) == 0;
    }

    return std::strncmp(data + len - suffixLen, suffix, suffixLen) == 0;
}

int Str::find_first_occurrence(char c) const
{
    int i = 0;
    for (; data[i] != '\0'; ++i)
    {
        if (data[i] == c)
        {
            return i;
        }
    }

    // In case c == 0 (find the length of the string).
    if (c == '\0')
    {
        return i;
    }

    return -1; // Not found.
}

int Str::find_last_occurrence(char c) const
{
    int i = 0;
    int lastIndex = -1;
    for (; data[i] != '\0'; ++i)
    {
        if (data[i] == c)
        {
            lastIndex = i;
            // Continue till the last occurrence...
        }
    }

    // In case c == 0 (find the length of the string).
    if (c == '\0')
    {
        lastIndex = i;
    }

    return lastIndex;
}

// ========================================================
// std::string interoperability:
// ========================================================

#if STR_SUPPORT_STD_STRING

Str::Str(const std::string & rhs)
    : Str{} // Delegate to the default constructor (C++11)
{
    set(rhs);
}

void Str::set(const std::string & src)
{
    if (src.empty())
    {
        clear();
        return;
    }

    const int buf_len = static_cast<int>(src.length() + 1);
    if (numAvailable < buf_len)
    {
        reserve_discard(buf_len);
    }

    std::memcpy(data, src.c_str(), buf_len);
    len   = buf_len - 1;
    owned = true;
}

Str & Str::operator = (const std::string & rhs)
{
    set(rhs);
    return *this;
}

#endif // STR_SUPPORT_STD_STRING
