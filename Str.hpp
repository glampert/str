
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
// - This isn't a fully featured string class.
// - Strings are mutable.
// - Length is now stored, so length() is constant time.
// - Call reserve() to grow the buffer.
// - Maximum string size currently limited to 2 MB (we allocate 21 bits to hold capacity)
// - Local buffer size is currently limited to 1023 bytes (we allocate 10 bits to hold local buffer size)
// - This code hasn't been tested very much. It is probably incomplete or broken.
//
// The idea is that you can provide an arbitrary sized local buffer if you expect
// the string to fit most of the time, and then you avoid using costly heap allocation.
//    Str16 s = "filename.h";
//
// But it can also use heap if necessary.
//    Str16  s = "long_filename_not_very_long_but_longer_than_expected.h"; // use heap
//    Str256 s = "long_filename_not_very_long_but_longer_than_expected.h"; // use local buffer
//
// Always use heap:
//    Str s = "hey";
//
// You can also copy references/literal pointer without allocation:
//    Str s;
//    s.set_ref("hey!"); // setter for literals/references
//
// Or via the helper constructor:
//    StrRef("hey!"); // constructor for literals/reference
//
// StrSized<N> can be templated with any size from 1 to 1023.
// A couple aliases to common power-of-two sizes are also already provided.
//
// TODO:
// - More methods/helpers.
// - C++11 Move/assignment.
// - noexcept specification.
//

#ifndef STR_HPP_
#define STR_HPP_

#include <cstdarg> // va_list & friends
#include <cstdlib> // std::malloc/free
#include <cstring> // std::memcpy/strcpy

#ifndef STR_SUPPORT_STD_STRING
#define STR_SUPPORT_STD_STRING 1
#endif // STR_SUPPORT_STD_STRING

#ifdef STR_SUPPORT_STD_STRING
#include <string>
#endif // STR_SUPPORT_STD_STRING

#ifndef STR_ASSERT
#include <cassert>
#define STR_ASSERT assert
#endif // STR_ASSERT

#ifndef STR_MEMALLOC
#define STR_MEMALLOC std::malloc
#endif // STR_MEMALLOC

#ifndef STR_MEMFREE
#define STR_MEMFREE std::free
#endif // STR_MEMFREE

#ifndef STR_CMP_NOCASE
#ifdef _MSC_VER
#define STR_CMP_NOCASE _stricmp
#else // !_MSC_VER
#define STR_CMP_NOCASE strcasecmp
#endif // _MSC_VER
#endif // STR_CMP_NOCASE

#if defined(__GNUC__) || defined(__clang__)
#define STR_FORMAT_FUNC(fmtIndex, varIndex) __attribute__((format(printf, fmtIndex, varIndex)))
#else // !GNU && !Clang
#define STR_FORMAT_FUNC(fmtIndex, varIndex) /* unimplemented for now... */
#endif // GNU/Clang

// ========================================================
// class Str:
// This is the base class that you can pass around.
// Footprint is about 16-bytes in a 64-bits architecture.
// Destructor attempts to free the string if we own it.
//
// NOTE: We are taking a shortcut here by not making
// the class destructor virtual, as a means of avoiding
// a potential vtable pointer that would further increase
// the size of this class and all child classes. The object
// of Str is to remain as compact as possible, however, this
// also makes the class non-compliant with the Standard
// in respect of deleting an instance of a child class from
// a base class pointer. Since we'd still like to be able
// to declare instances of Str, we can't make its destructor
// protected and non-virtual either. So If you dynamically
// allocate one of the StrSized<N> types, you must free it
// from the appropriate pointer. Trying to 'delete' an Str
// that in reality is an StrSized<N> is Undefined Behavior.
// ========================================================

class Str
{
public:

    Str();
    Str(const char * rhs);
    void set(const char * src);
    Str & operator = (const char * rhs)
    {
        set(rhs);
        return *this;
    }

    Str(const Str & rhs);
    void set(const Str & src);
    Str & operator = (const Str & rhs)
    {
        set(rhs);
        return *this;
    }

    int  length()        const { return len; }
    bool empty()         const { return data[0] == '\0'; }
    int  capacity()      const { return numAvailable; }
    bool owns_buffer()   const { return owned; }
    const char * c_str() const { return data;  }

    char & operator[](int i)
    {
        STR_ASSERT(i >= 0 && i < len);
        return data[i];
    }
    char operator[](int i) const
    {
        STR_ASSERT(i >= 0 && i < len);
        return data[i];
    }

    // Keeps a non-owning reference to the external string.
    void set_ref(const char * src);

    // Copy from format string:
    int setf(const char * fmt, ...) STR_FORMAT_FUNC(2, 3);
    int setfv(const char * fmt, va_list args) STR_FORMAT_FUNC(2, 0);
    int setf_nogrow(const char * fmt, ...) STR_FORMAT_FUNC(2, 3);
    int setfv_nogrow(const char * fmt, va_list args) STR_FORMAT_FUNC(2, 0);

    // Memory management:
    void clear();
    void reserve(int cap);
    void reserve_discard(int cap);
    void shrink_to_fit();

    // strcmp-style comparison:
    int compare(const char * rhs) const { return std::strcmp(data, rhs); }
    int compare(const Str & rhs)  const { return std::strcmp(data, rhs.data); }

    // Compare ignoring character case (operators are always case-sensitive!):
    int compare_no_case(const char * rhs) const { return STR_CMP_NOCASE(data, rhs); }
    int compare_no_case(const Str & rhs)  const { return STR_CMP_NOCASE(data, rhs.data); }

    // Check for prefix substring:
    bool starts_with(const char * prefix) const;
    bool starts_with(const Str & prefix)  const { return starts_with(prefix.data); }

    // Check for suffix substring:
    bool ends_with(const char * suffix) const;
    bool ends_with(const Str & suffix)  const { return ends_with(suffix.data); }

    // Find characters in string. Returns index or -1 if not found.
    int find_first_occurrence(char c) const;
    int find_last_occurrence(char c)  const;

    // std::string interoperability:
#if STR_SUPPORT_STD_STRING
    Str(const std::string & rhs);
    void set(const std::string & src);
    Str & operator = (const std::string & rhs);
    int compare(const std::string & rhs) const { return std::strcmp(data, rhs.c_str()); }
    int compare_no_case(const std::string & rhs) const { return STR_CMP_NOCASE(data, rhs.c_str()); }
#endif // STR_SUPPORT_STD_STRING

    // Frees the underlaying buffer if we own it.
    ~Str();

protected:

    // NOTE: The local buffer is assumed to be whatever follows the class instance.
    // This is somewhat hackish and potentially fragile, but avoids the need for
    // a virtual function or a fully templated Str class...
    char * local_buf() { return reinterpret_cast<char *>(this) + sizeof(Str); }
    const char * local_buf() const { return reinterpret_cast<const char *>(this) + sizeof(Str); }
    bool is_using_local_buf() const { return data == local_buf() && localBufSize != 0; }

    // Constructor for StrSized<N> variants with a local buffer.
    explicit Str(int local_buf_size)
    {
        STR_ASSERT(local_buf_size < 1024); // Already static_asserted in StrSized, but doesn't harm either...
        data         = local_buf();
        data[0]      = '\0';
        len          = 0;
        numAvailable = local_buf_size;
        localBufSize = local_buf_size;
        owned        = true;
    }

private:

    char * data;           // Pointer to local_buf() or heap allocated memory.
    int len;               // Current length in chars, not counting the null terminator.
    int numAvailable : 21; // Allocated capacity in chars. Max 2 MB.
    int localBufSize : 10; // Max 1023 bytes.
    bool owned       : 1;  // True if the data buffer is owned by Str and must be freed.
};

// ========================================================
// Inline comparison operators for Str:
// ========================================================

#define STR_DECLARE_OPERATOR_SET(typeA, typeB) \
inline bool operator <  (typeA lhs, typeB rhs) { return lhs.compare(rhs)  < 0; } \
inline bool operator >  (typeA lhs, typeB rhs) { return lhs.compare(rhs)  > 0; } \
inline bool operator <= (typeA lhs, typeB rhs) { return lhs.compare(rhs) <= 0; } \
inline bool operator >= (typeA lhs, typeB rhs) { return lhs.compare(rhs) >= 0; } \
inline bool operator == (typeA lhs, typeB rhs) { return lhs.compare(rhs) == 0; } \
inline bool operator != (typeA lhs, typeB rhs) { return lhs.compare(rhs) != 0; }

STR_DECLARE_OPERATOR_SET(const Str &, const Str &);
STR_DECLARE_OPERATOR_SET(const Str &, const char *);
// Very easy to extend for comparison with other custom string types!

#if STR_SUPPORT_STD_STRING
STR_DECLARE_OPERATOR_SET(const Str &, const std::string &);
#endif // STR_SUPPORT_STD_STRING

// ========================================================
// class StrRef:
// Reference to literal C-string (now owned by Str).
// ========================================================

class StrRef final
    : public Str
{
public:

    StrRef(const char * rhs)
        : Str{} { set_ref(rhs); }
};

// ========================================================
// template class StrSized<N>:
// String type embedding a local buffer.
// ========================================================

template<int N>
class StrSized final
    : public Str
{
public:

    // Because Str only reserved 10 bits for it, max size is 1023.
    static_assert(N < 1024, "Local buffer size is currently limited to 1023 characters!");
    static_assert(N > 0, "Negative/zero size doesn't make sense!");

    StrSized()
        : Str{ N } { }
    StrSized(const char * rhs)
        : Str{ N } { set(rhs); }
    StrSized(const Str & rhs)
        : Str{ N } { set(rhs); }
    StrSized(const StrSized & rhs)
        : Str{ N } { set(rhs); }

    StrSized & operator = (const char * rhs)
    {
        set(rhs);
        return *this;
    }
    StrSized & operator = (const Str & rhs)
    {
        set(rhs);
        return *this;
    }
    StrSized & operator = (const StrSized & rhs)
    {
        set(rhs);
        return *this;
    }

#if STR_SUPPORT_STD_STRING
    StrSized(const std::string & rhs)
        : Str{ N }
    {
        set(rhs);
    }
    StrSized & operator = (const std::string & rhs)
    {
        set(rhs);
        return *this;
    }
#endif // STR_SUPPORT_STD_STRING

private:

    char local_buf[N];
};

// Aliases for frequently used sizes:
using Str16  = StrSized<16>;
using Str32  = StrSized<32>;
using Str64  = StrSized<64>;
using Str128 = StrSized<128>;
using Str256 = StrSized<256>;

#endif // STR_HPP_
