
// ================================================================================================
// -*- C++ -*-
// File: str.hpp
// Author: Guilherme R. Lampert
// Created on: 25/09/16
//
// About:
//  Simple C++ string classes that can be used as replacement for std::string or char* strings.
//
// License:
//  This software is in the public domain. Where that dedication is not recognized,
//  you are granted a perpetual, irrevocable license to copy, distribute, and modify
//  this file as you see fit. Source code is provided "as is", without warranty of any
//  kind, express or implied. No attribution is required, but a mention about the author
//  is appreciated.
// ================================================================================================

#ifndef STR_HPP
#define STR_HPP

// Defining this before including the file prevents pulling the Standard headers.
// Useful to be able to place this file inside a user-defined namespace or to simply
// avoid redundant inclusions. User is responsible for providing all the necessary
// Standard headers before #including this one.
#ifndef STR_NO_STD_INCLUDES
    #include <cstddef>
    #include <cstdint>
    #include <cstdarg>
    #include <cstring>
    #include <cctype>
    #include <string>
    #include <iterator>
    #include <type_traits>
#endif // STR_NO_STD_INCLUDES

// Hook to allow providing a custom assert() before including this file.
#ifndef STR_ASSERT
    #ifndef STR_NO_STD_INCLUDES
        #include <cassert>
    #endif // STR_NO_STD_INCLUDES
    #define STR_ASSERT assert
#endif // STR_ASSERT

// Hook to allow providing custom memory allocation funcs before including this file.
#ifndef STR_MEM_ALLOC
    #ifndef STR_NO_STD_INCLUDES
        #include <cstdlib>
    #endif // STR_NO_STD_INCLUDES
    #define STR_MEM_ALLOC(count) static_cast<char *>(std::malloc(count))
    #define STR_MEM_FREE(ptr)    std::free(ptr)
#endif // STR_MEM_ALLOC

#if defined(__GNUC__) || defined(__clang__)
    #define STR_FORMAT_FUNC(fmtIndex, varIndex) __attribute__((format(printf, fmtIndex, varIndex)))
#else // !GNU && !Clang
    #define STR_FORMAT_FUNC(fmtIndex, varIndex) /* unimplemented for now */
#endif // GNU || Clang

// ========================================================
// class str:
// ========================================================

class str
{
public:

    str() noexcept;
    ~str();

    str(str && rhs) noexcept;
    str & operator = (str && rhs) noexcept;

    // Constructors:

    str(const str & rhs);
    str(const str & rhs, int count);
    str(const str & rhs, int first, int count);

    str(const char * rhs);
    str(const char * rhs, int count);
    str(const char * rhs, int first, int count);

    str(const std::string & rhs);
    str(const std::string & rhs, int count);
    str(const std::string & rhs, int first, int count);

    // Set/assign:

    void set(const str & src);
    void set(const str & src, int count);
    void set(const str & src, int first, int count);

    void set(const char * src);
    void set(const char * src, int count);
    void set(const char * src, int first, int count);

    void set(const std::string & src);
    void set(const std::string & src, int count);
    void set(const std::string & src, int first, int count);

    // Keeps a non-owning reference to the external string:

    void set_ref(const str & src);
    void set_ref(const str & src, int first);

    void set_ref(const char * src);
    void set_ref(const char * src, int first);

    void set_ref(const std::string & src);
    void set_ref(const std::string & src, int first);

    // Append/push-pop:

    void append(const str & src);
    void append(const str & src, int count);
    void append(const str & src, int first, int count);

    void append(const char * src);
    void append(const char * src, int count);
    void append(const char * src, int first, int count);

    void append(const std::string & src);
    void append(const std::string & src, int count);
    void append(const std::string & src, int first, int count);

    void push_back(char c);
    void pop_back();

    // Compare/search:

    int compare(const str & rhs) const;
    int compare(const char * rhs) const;
    int compare(const std::string & rhs) const;

    int compare_no_case(const str & rhs) const;
    int compare_no_case(const char * rhs) const;
    int compare_no_case(const std::string & rhs) const;

    bool starts_with(const str & prefix) const;
    bool starts_with(const char * prefix) const;
    bool starts_with(const char * prefix, int prefix_len) const;
    bool starts_with(const std::string & prefix) const;

    bool ends_with(const str & suffix) const;
    bool ends_with(const char * suffix) const;
    bool ends_with(const char * suffix, int suffix_len) const;
    bool ends_with(const std::string & suffix) const;

    int first_index_of(char c) const;
    int last_index_of(char c) const;

    int first_index_of(const char * substring) const;
    int last_index_of(const char * substring) const;

    int find_any_matching_index(const char * charset) const;

    // Trim (remove whitespace) in place:

    str & trim_right();
    str & trim_left();
    str & trim();

    // Whole string to uppercase or lowercase, in place:

    str & to_upper();
    str & to_lower();

    // Mutating operators:

    str & operator = (const str & rhs);
    str & operator = (const char * rhs);
    str & operator = (const std::string & rhs);

    str & operator += (char c);
    str & operator += (const str & rhs);
    str & operator += (const char * rhs);
    str & operator += (const std::string & rhs);

    char & operator[](int index);
    char   operator[](int index) const;

    // Relational operators:

    bool operator == (const str & rhs) const;
    bool operator != (const str & rhs) const;
    bool operator <= (const str & rhs) const;
    bool operator >= (const str & rhs) const;
    bool operator >  (const str & rhs) const;
    bool operator <  (const str & rhs) const;

    bool operator == (const char * rhs) const;
    bool operator != (const char * rhs) const;
    bool operator <= (const char * rhs) const;
    bool operator >= (const char * rhs) const;
    bool operator >  (const char * rhs) const;
    bool operator <  (const char * rhs) const;

    bool operator == (const std::string & rhs) const;
    bool operator != (const std::string & rhs) const;
    bool operator <= (const std::string & rhs) const;
    bool operator >= (const std::string & rhs) const;
    bool operator >  (const std::string & rhs) const;
    bool operator <  (const std::string & rhs) const;

    // C-style formatted strings:

    bool setf(const char * fmt, ...) STR_FORMAT_FUNC(2, 3);
    bool setf_no_grow(const char * fmt, ...) STR_FORMAT_FUNC(2, 3);

    bool setfv(const char * fmt, va_list args) STR_FORMAT_FUNC(2, 0);
    bool setfv_no_grow(const char * fmt, va_list args) STR_FORMAT_FUNC(2, 0);

    bool appendf(const char * fmt, ...) STR_FORMAT_FUNC(2, 3);
    bool appendfv(const char * fmt, va_list args) STR_FORMAT_FUNC(2, 0);

    // Queries/accessors:

    int length()   const noexcept;
    int capacity() const noexcept;

    bool empty() const noexcept;
    bool valid() const noexcept;

    bool owns_buffer() const noexcept;
    bool using_local_buffer() const noexcept;

    const char * c_str()  const noexcept;
    const char * data()   const noexcept;
    std::string std_str() const;

    // Memory and size management:

    str & truncate(int max_len);

    void clear();
    void clear_no_free() noexcept;
    void shrink_to_fit();

    void reserve(int new_capacity, int dynamic_alloc_extra = 16);
    void reserve_discard(int new_capacity, int dynamic_alloc_extra = 16);

    void resize(int new_len, char fill_char, int dynamic_alloc_extra = 16);
    void resize_discard(int new_len, char fill_char, int dynamic_alloc_extra = 16);

    // Non-throwing swap() function:

    friend void swap(str & lhs, str & rhs) noexcept;

    // C-string helpers:

    static int compare(const char * a, const char * b);
    static int compare(const char * a, const char * b, int chars_to_compare);
    static int compare_no_case(const char * a, const char * b);

    static int copy(char * dest, int dest_size_in_chars, const char * source);
    static int copy(char * dest, int dest_size_in_chars, const char * source, int chars_to_copy);

    static int append(char * dest, int dest_size_in_chars, const char * source);
    static int append(char * dest, int dest_size_in_chars, const char * source, int chars_to_append);

    static char * to_upper(char * str);
    static char * to_lower(char * str);

    static char * split(char * str, const char * delimiters, char ** remaining);
    static int length(const char * str);

    static char * skip_leading_whitespace(char * str);
    static const char * skip_leading_whitespace(const char * str);

public:

    //
    // Bidirectional iterator for strings.
    // Allows bidirectional traversal (with ++ and --).
    // Technically, strings can use a RandomAccessIterator,
    // but to keep the implementation simpler, we use the
    // bidirectional variant. This covers the majority of
    // use cases. Iterators are bounds checked with STR_ASSERTs.
    //
    template<typename T>
    class basic_bidirectional_iterator final
        : public std::iterator<std::bidirectional_iterator_tag, T>
    {
    public:
        // Constructors:
        basic_bidirectional_iterator() noexcept
            : m_ptr  { nullptr }
            , m_start{ nullptr }
            , m_end  { nullptr }
        { }

        basic_bidirectional_iterator(T * ptr, T * start, T * end)
            : m_ptr  { ptr   }
            , m_start{ start }
            , m_end  { end   }
        {
            STR_ASSERT(valid());
        }

        basic_bidirectional_iterator(T * start, T * end)
            : basic_bidirectional_iterator{ start, start, end }
        { }

        bool valid() const noexcept
        {
            return m_ptr != nullptr && m_ptr >= m_start && m_ptr <= m_end;
        }

        // Pointer dereference:
        T operator*() const
        {
            STR_ASSERT(valid());
            return *m_ptr;
        }
        T & operator*()
        {
            STR_ASSERT(valid());
            return *m_ptr;
        }

        // Pre-increment/decrement (++/--i):
        basic_bidirectional_iterator & operator++()
        {
            STR_ASSERT(m_ptr != nullptr);
            STR_ASSERT(m_ptr < m_end); // Don't allow incrementing past the '\0'
            ++m_ptr;
            return *this;
        }
        basic_bidirectional_iterator & operator--()
        {
            STR_ASSERT(m_ptr != nullptr);
            STR_ASSERT(m_ptr > m_start);
            --m_ptr;
            return *this;
        }

        // Post-increment/decrement (i++/--):
        basic_bidirectional_iterator operator++(int)
        {
            STR_ASSERT(m_ptr != nullptr);
            STR_ASSERT(m_ptr < m_end); // Don't allow incrementing past the '\0'
            auto old{ *this };
            ++m_ptr;
            return old;
        }
        basic_bidirectional_iterator operator--(int)
        {
            STR_ASSERT(m_ptr != nullptr);
            STR_ASSERT(m_ptr > m_start);
            auto old{ *this };
            --m_ptr;
            return old;
        }

        // Comparison:
        bool operator == (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr == other.m_ptr;
        }
        bool operator != (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr != other.m_ptr;
        }
        bool operator <= (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr <= other.m_ptr;
        }
        bool operator >= (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr >= other.m_ptr;
        }
        bool operator < (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr < other.m_ptr;
        }
        bool operator > (const basic_bidirectional_iterator & other) const
        {
            STR_ASSERT(valid());
            STR_ASSERT(other.valid());
            return m_ptr > other.m_ptr;
        }

        // One way conversion: iterator -> const_iterator
        operator basic_bidirectional_iterator<const T>() const
        {
            return { m_ptr, m_start, m_end };
        }

    private:
        T * m_ptr;   // Current pointer in the string.
        T * m_start; // Pointer to the beginning of the string.
        T * m_end;   // Pointer to one-past-the-end (the '\0' terminator).
    };

    using iterator               = basic_bidirectional_iterator<char>;
    using const_iterator         = basic_bidirectional_iterator<const char>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Forward iterators:
    iterator       begin()        { return iterator(m_data, &m_data[m_length]); }
    const_iterator begin()  const { return const_iterator(m_data, &m_data[m_length]); }
    const_iterator cbegin() const { return const_iterator(m_data, &m_data[m_length]); }

    iterator       end()          { return iterator(&m_data[m_length], m_data, &m_data[m_length]); }
    const_iterator end()    const { return const_iterator(&m_data[m_length], m_data, &m_data[m_length]); }
    const_iterator cend()   const { return const_iterator(&m_data[m_length], m_data, &m_data[m_length]); }

    // Reverse iterators:
    reverse_iterator       rbegin()        { return reverse_iterator{ end() }; }
    const_reverse_iterator rbegin()  const { return const_reverse_iterator{ end() }; }
    const_reverse_iterator crbegin() const { return const_reverse_iterator{ cend() }; }

    reverse_iterator       rend()          { return reverse_iterator{ begin() }; }
    const_reverse_iterator rend()    const { return const_reverse_iterator{ begin() }; }
    const_reverse_iterator crend()   const { return const_reverse_iterator{ cbegin() }; }

protected:

    // Cast U to T, checking that the value is still the same in case of narrowing.
    template<typename T, typename U>
    static T narrow(U u)
    {
        const T t = static_cast<T>(u);
        STR_ASSERT(static_cast<U>(t) == u);
        return t;
    }

    char * get_local_buffer() const noexcept;
    char * get_empty_dummy_string() const noexcept;

    // Constructor for str_sized<N> variants with a local buffer.
    explicit str(int local_buf_size);

    char *        m_data;                   // Pointer to an inline/local buffer or heap allocated memory.
    std::int32_t  m_length;                 // Current length in chars, not counting the null terminator.
    std::int32_t  m_capacity          : 21; // Allocated capacity in chars. Max 2 MB.
    std::int32_t  m_local_buffer_size : 10; // Size of inline/local storage (SSO), if using any. Max 1023 bytes.
    std::uint32_t m_owns_buffer       : 1;  // True if the data buffer is owned by the str and must be freed.
};

// ========================================================
// templated class str_sized:
// ========================================================

//
// CAVEAT:
// This class inherits publicly from str, which doesn't provide a virtual
// destructor to avoid the overhead of a vtable, thus it is not safe to
// dynamically allocate str_sized instances with `new` that might later be
// deleted as str instances. So avoid having new'ed instances of str_sized
// as pointers to the str base class.
//
template<int Size>
class str_sized
    : public str
{
public:

    str_sized() : str{ Size } { }

    str_sized(const str & rhs) : str{ Size } { set(rhs); }
    str_sized(const str & rhs, int count) : str{ Size } { set(rhs, count); }
    str_sized(const str & rhs, int first, int count) : str{ Size } { set(rhs, first, count); }

    str_sized(const str_sized & rhs) : str{ Size } { set(rhs); }
    str_sized(const str_sized & rhs, int count) : str{ Size } { set(rhs, count); }
    str_sized(const str_sized & rhs, int first, int count) : str{ Size } { set(rhs, first, count); }

    str_sized(const char * rhs) : str{ Size } { set(rhs); }
    str_sized(const char * rhs, int count) : str{ Size } { set(rhs, count); }
    str_sized(const char * rhs, int first, int count) : str{ Size } { set(rhs, first, count); }

    str_sized(const std::string & rhs) : str{ Size } { set(rhs); }
    str_sized(const std::string & rhs, int count) : str{ Size } { set(rhs, count); }
    str_sized(const std::string & rhs, int first, int count) : str{ Size } { set(rhs, count, first); }

    str_sized & operator = (const str & rhs)         { set(rhs); return *this; }
    str_sized & operator = (const str_sized & rhs)   { set(rhs); return *this; }
    str_sized & operator = (const std::string & rhs) { set(rhs); return *this; }
    str_sized & operator = (const char * rhs)        { set(rhs); return *this; }

private:

    // Because str only reserves 10 bits for m_local_buffer_size, max length is 1023.
    static_assert(Size < 1024, "local buffer size is currently limited to 1023 characters!");
    static_assert(Size > 0,    "negative/zero size doesn't make sense!");

    char m_local_buf[Size];
};

template<int Size>
inline void swap(str_sized<Size> & lhs, str_sized<Size> & rhs) noexcept
{
    // Have to create the temporary since it is not safe to swap the
    // m_data pointers referencing the inline buffer inside the objects.
    str_sized<Size> tmp{ rhs };
    rhs = lhs;
    lhs = tmp;
}

//
// Aliases for frequently used power-of-two sizes:
// (optional)
//
#ifndef STR_SIZED_NO_GLOBAL_ALIASES
    using str16  = str_sized<16>;
    using str32  = str_sized<32>;
    using str64  = str_sized<64>;
    using str128 = str_sized<128>;
    using str256 = str_sized<256>;
    using str512 = str_sized<512>;
#endif // STR_SIZED_NO_GLOBAL_ALIASES

// ========================================================
// class str_ref:
// ========================================================

class str_ref final
    : public str
{
public:

    str_ref(const str & rhs) { set_ref(rhs); }
    str_ref(const str & rhs, int first) { set_ref(rhs, first); }

    str_ref(const char * rhs) { set_ref(rhs); }
    str_ref(const char * rhs, int first) { set_ref(rhs, first); }

    str_ref(const std::string & rhs) { set_ref(rhs); }
    str_ref(const std::string & rhs, int first) { set_ref(rhs, first); }
};

// ========================================================
// Inline methods of str:
// ========================================================

inline str::str(str && rhs) noexcept
    : str{}
{
    if (rhs.using_local_buffer())
    {
        // If the storage is inline with the object then we can't move.
        set(rhs);
        rhs.clear_no_free();
    }
    else
    {
        // This string is default initialized, so swapping
        // will get the contents from rhs and then clear it.
        swap(*this, rhs);
    }
}

inline str & str::operator = (str && rhs) noexcept
{
    if (rhs.using_local_buffer())
    {
        // If the storage is inline with the object then we can't move.
        set(rhs);
        rhs.clear_no_free();
    }
    else
    {
        str temp{};
        swap(*this, rhs);
        swap(temp,  rhs);
    }
    return *this;
}

inline str::str(const str & rhs) : str{}
{
    set(rhs);
}

inline str::str(const str & rhs, int count) : str{}
{
    set(rhs, count);
}

inline str::str(const str & rhs, int first, int count) : str{}
{
    set(rhs, first, count);
}

inline str::str(const char * rhs) : str{}
{
    set(rhs);
}

inline str::str(const char * rhs, int count) : str{}
{
    set(rhs, count);
}

inline str::str(const char * rhs, int first, int count) : str{}
{
    set(rhs, first, count);
}

inline str::str(const std::string & rhs) : str{}
{
    set(rhs);
}

inline str::str(const std::string & rhs, int count) : str{}
{
    set(rhs, count);
}

inline str::str(const std::string & rhs, int first, int count) : str{}
{
    set(rhs, first, count);
}

inline str & str::operator = (const str & rhs)
{
    set(rhs);
    return *this;
}

inline str & str::operator = (const char * rhs)
{
    set(rhs);
    return *this;
}

inline str & str::operator = (const std::string & rhs)
{
    set(rhs);
    return *this;
}

inline str & str::operator += (const char c)
{
    push_back(c);
    return *this;
}

inline str & str::operator += (const str & rhs)
{
    append(rhs);
    return *this;
}

inline str & str::operator += (const char * rhs)
{
    append(rhs);
    return *this;
}

inline str & str::operator += (const std::string & rhs)
{
    append(rhs);
    return *this;
}

inline char & str::operator[](const int index)
{
    STR_ASSERT(valid());
    STR_ASSERT(index >= 0);
    STR_ASSERT(index < length());
    return m_data[index];
}

inline char str::operator[](const int index) const
{
    STR_ASSERT(valid());
    STR_ASSERT(index >= 0);
    STR_ASSERT(index < length());
    return m_data[index];
}

inline bool str::operator == (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) == 0;
}

inline bool str::operator != (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) != 0;
}

inline bool str::operator <= (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) <= 0;
}

inline bool str::operator >= (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) >= 0;
}

inline bool str::operator > (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) > 0;
}

inline bool str::operator < (const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) < 0;
}

inline bool str::operator == (const char * rhs) const
{
    return str::compare(c_str(), rhs) == 0;
}

inline bool str::operator != (const char * rhs) const
{
    return str::compare(c_str(), rhs) != 0;
}

inline bool str::operator <= (const char * rhs) const
{
    return str::compare(c_str(), rhs) <= 0;
}

inline bool str::operator >= (const char * rhs) const
{
    return str::compare(c_str(), rhs) >= 0;
}

inline bool str::operator > (const char * rhs) const
{
    return str::compare(c_str(), rhs) > 0;
}

inline bool str::operator < (const char * rhs) const
{
    return str::compare(c_str(), rhs) < 0;
}

inline bool str::operator == (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) == 0;
}

inline bool str::operator != (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) != 0;
}

inline bool str::operator <= (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) <= 0;
}

inline bool str::operator >= (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) >= 0;
}

inline bool str::operator > (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) > 0;
}

inline bool str::operator < (const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str()) < 0;
}

inline void str::set(const str & src)
{
    set(src.c_str(), 0, src.length());
}

inline void str::set(const str & src, int count)
{
    set(src.c_str(), 0, count);
}

inline void str::set(const str & src, int first, int count)
{
    set(src.c_str(), first, count);
}

inline void str::set(const char * src)
{
    set(src, 0, str::length(src));
}

inline void str::set(const char * src, int count)
{
    set(src, 0, count);
}

inline void str::set(const std::string & src)
{
    set(src.c_str(), 0, narrow<int>(src.length()));
}

inline void str::set(const std::string & src, int count)
{
    set(src.c_str(), 0, count);
}

inline void str::set(const std::string & src, int first, int count)
{
    set(src.c_str(), first, count);
}

inline void str::set_ref(const str & src)
{
    set_ref(src.c_str(), 0);
}

inline void str::set_ref(const str & src, const int first)
{
    STR_ASSERT(first < src.length());
    set_ref(src.c_str(), first);
}

inline void str::set_ref(const char * src)
{
    set_ref(src, 0);
}

inline void str::set_ref(const std::string & src)
{
    set_ref(src.c_str(), 0);
}

inline void str::set_ref(const std::string & src, const int first)
{
    STR_ASSERT(first < narrow<int>(src.length()));
    set_ref(src.c_str(), first);
}

inline void str::append(const str & src)
{
    append(src.c_str(), 0, src.length());
}

inline void str::append(const str & src, int count)
{
    append(src.c_str(), 0, count);
}

inline void str::append(const str & src, int first, int count)
{
    append(src.c_str(), first, count);
}

inline void str::append(const char * src)
{
    append(src, 0, str::length(src));
}

inline void str::append(const char * src, int count)
{
    append(src, 0, count);
}

inline void str::append(const std::string & src)
{
    append(src.c_str(), 0, narrow<int>(src.length()));
}

inline void str::append(const std::string & src, int count)
{
    append(src.c_str(), 0, count);
}

inline void str::append(const std::string & src, int first, int count)
{
    append(src.c_str(), first, count);
}

inline int str::compare(const str & rhs) const
{
    return str::compare(c_str(), rhs.c_str());
}

inline int str::compare(const char * rhs) const
{
    return str::compare(c_str(), rhs);
}

inline int str::compare(const std::string & rhs) const
{
    return str::compare(c_str(), rhs.c_str());
}

inline int str::compare_no_case(const str & rhs) const
{
    return str::compare_no_case(c_str(), rhs.c_str());
}

inline int str::compare_no_case(const char * rhs) const
{
    return str::compare_no_case(c_str(), rhs);
}

inline int str::compare_no_case(const std::string & rhs) const
{
    return str::compare_no_case(c_str(), rhs.c_str());
}

inline bool str::starts_with(const str & prefix) const
{
    return starts_with(prefix.c_str(), prefix.length());
}

inline bool str::starts_with(const char * prefix) const
{
    return starts_with(prefix, str::length(prefix));
}

inline bool str::starts_with(const std::string & prefix) const
{
    return starts_with(prefix.c_str(), narrow<int>(prefix.length()));
}

inline bool str::ends_with(const str & suffix) const
{
    return ends_with(suffix.c_str(), suffix.length());
}

inline bool str::ends_with(const char * suffix) const
{
    return ends_with(suffix, str::length(suffix));
}

inline bool str::ends_with(const std::string & suffix) const
{
    return ends_with(suffix.c_str(), narrow<int>(suffix.length()));
}

inline int str::length(const char * str)
{
    STR_ASSERT(str != nullptr);
    return narrow<int>(std::strlen(str)); // Don't care about 32/64-bits truncation.
}

inline int str::compare(const char * a, const char * b)
{
    STR_ASSERT(a != nullptr);
    STR_ASSERT(b != nullptr);

    return std::strcmp(a, b);
}

inline int str::compare(const char * a, const char * b, const int chars_to_compare)
{
    STR_ASSERT(a != nullptr);
    STR_ASSERT(b != nullptr);

    return std::strncmp(a, b, chars_to_compare);
}

inline int str::append(char * dest, const int dest_size_in_chars, const char * source)
{
    return append(dest, dest_size_in_chars, source, str::length(source));
}

inline int str::length() const noexcept
{
    return m_length;
}

inline int str::capacity() const noexcept
{
    return m_capacity;
}

inline bool str::empty() const noexcept
{
    return m_length == 0;
}

inline bool str::valid() const noexcept
{
    return m_data != get_empty_dummy_string() && m_length != 0;
}

inline bool str::owns_buffer() const noexcept
{
    return !!m_owns_buffer; // Double NOT to silence conversion from integer=>bool warning.
}

inline bool str::using_local_buffer() const noexcept
{
    return m_data == get_local_buffer() && m_local_buffer_size != 0;
}

inline const char * str::c_str() const noexcept
{
    return m_data;
}

inline const char * str::data() const noexcept
{
    return m_data;
}

inline std::string str::std_str() const
{
    return m_data;
}

inline char * str::get_local_buffer() const noexcept
{
    // NOTE: The local buffer is assumed to be whatever follows the class instance.
    // This is somewhat hackish and potentially fragile, but avoids the need for a
    // virtual function or a fully templated string class. As long as the layout of
    // str remains simple (no virtual inheritance), this should work fine.
    auto base = const_cast<str *>(this);
    return reinterpret_cast<char *>(base) + sizeof(str);
}

inline void str::clear_no_free() noexcept
{
    if (m_data != get_empty_dummy_string())
    {
        m_data[0] = '\0';
        m_length  = 0;
    }
}

inline void str::push_back(const char c)
{
    reserve(m_length + 2);
    m_data[m_length++] = c;
    m_data[m_length  ] = '\0';
}

inline void str::pop_back()
{
    if (m_length > 0)
    {
        m_data[--m_length] = '\0';
    }
}

// ================== End of header file ==================
#endif // STR_HPP
// ================== End of header file ==================

// ================================================================================================
//
//                                     Str Implementation
//
// ================================================================================================

#ifdef STR_IMPLEMENTATION

// 12 bytes on a 32bits machine, 16 on 64bits.
static_assert(sizeof(str) == (sizeof(char *) == 4 ? 12 : 16), "size of str class should not be greater than 16 bytes!");

// Need to maintain plain data layout for str::get_local_buffer() to work.
static_assert(!std::is_polymorphic<str>::value, "str class will not work with virtual inheritance!");

// Assert we don't have any unexpected padding:
#ifndef STR_SIZED_NO_GLOBAL_ALIASES
    static_assert(sizeof(str16)  == sizeof(str) + 16,  "unexpected size!");
    static_assert(sizeof(str32)  == sizeof(str) + 32,  "unexpected size!");
    static_assert(sizeof(str64)  == sizeof(str) + 64,  "unexpected size!");
    static_assert(sizeof(str128) == sizeof(str) + 128, "unexpected size!");
    static_assert(sizeof(str256) == sizeof(str) + 256, "unexpected size!");
    static_assert(sizeof(str512) == sizeof(str) + 512, "unexpected size!");
#endif // STR_SIZED_NO_GLOBAL_ALIASES

// On some platform vsnprintf() takes va_list by reference and modifies it.
// va_copy is the 'correct' way to copy a va_list but Visual Studio prior to 2013 doesn't have it.
#if (defined(_MSC_VER) && (_MSC_VER <= 1700))
    #ifndef va_copy
        #define va_copy(dest, src) (dest = src)
    #endif // va_copy
#endif // _MSC_VER

// ========================================================
// swap(str&, str&):
// ========================================================

void swap(str & lhs, str & rhs) noexcept
{
#define STR_SWAP(a, b) \
    do {               \
        auto tmp{ b }; \
        b = a;         \
        a = tmp;       \
    } while (0,0)

    if (lhs.using_local_buffer() || rhs.using_local_buffer())
    {
        // Not safe to swap m_data pointer if either is pointing
        // to the inline data buffer of a str_sized. In this case
        // we have to do a normal copied swap.
        STR_SWAP(lhs, rhs);
    }
    else // Lightweight swap:
    {
        // std::swap doesn't work here because a reference cannot
        // bind to a bitfield and some members of str are bitfields.
        STR_SWAP(lhs.m_data,              rhs.m_data);
        STR_SWAP(lhs.m_length,            rhs.m_length);
        STR_SWAP(lhs.m_capacity,          rhs.m_capacity);
        STR_SWAP(lhs.m_local_buffer_size, rhs.m_local_buffer_size);
        STR_SWAP(lhs.m_owns_buffer,       rhs.m_owns_buffer);
    }

#undef STR_SWAP
}

// ========================================================
// class str implementation:
// ========================================================

str::str() noexcept
    : m_data{ get_empty_dummy_string() }
    , m_length{ 0 }
    , m_capacity{ 0 }
    , m_local_buffer_size{ 0 }
    , m_owns_buffer{ false }
{
}

str::str(const int local_buf_size)
    : m_data{ get_local_buffer() }
    , m_length{ 0 }
    , m_capacity{ local_buf_size }
    , m_local_buffer_size{ local_buf_size }
    , m_owns_buffer{ true }
{
    // Already static_asserted in str_sized, but doesn't harm to check again...
    STR_ASSERT(local_buf_size < 1024);
    m_data[0] = '\0';
}

str::~str()
{
    if (m_owns_buffer && !using_local_buffer())
    {
        STR_ASSERT(m_data != get_empty_dummy_string());
        STR_MEM_FREE(m_data);
    }
}

char * str::get_empty_dummy_string() const noexcept
{
    // Used by the default constructor, which creates a valid but empty string.
    // ISO C++11 does not allow conversion from string literal to char*, so
    // this function prevents a warning on Clang/GCC. As long as we play nice
    // and don't attempt to mutate this string, everything will work fine.
    static char s_dummy[] = {'\0','\0','\0','\0'};
    return s_dummy;
}

void str::set(const char * src, const int first, const int count)
{
    STR_ASSERT(src != nullptr);
    STR_ASSERT(first >= 0);
    STR_ASSERT(count >= 0);

    src += first;

    // Calling str.set("") makes the string empty.
    if (*src == '\0' || count == 0)
    {
        clear_no_free();
        return;
    }

    const int chars_needed = count + 1;
    if (m_capacity < chars_needed)
    {
        reserve_discard(chars_needed);
    }

    std::memcpy(m_data, src, count);

    m_data[count] = '\0';
    m_length      = count;
    m_owns_buffer = true;
}

void str::set_ref(const char * src, const int first)
{
    STR_ASSERT(src != nullptr);

    src += first;

    if (*src == '\0')
    {
        clear_no_free();
        return;
    }

    if (m_owns_buffer && !using_local_buffer())
    {
        STR_ASSERT(m_data != get_empty_dummy_string());
        STR_MEM_FREE(m_data);
    }

    m_data        = const_cast<char *>(src); // Taken in good faith that the user won't try to modify it!
    m_length      = str::length(src);
    m_capacity    = 0;
    m_owns_buffer = false;
}

void str::append(const char * src, const int first, const int count)
{
    STR_ASSERT(src != nullptr);
    STR_ASSERT(first >= 0);
    STR_ASSERT(count >= 0);

    src += first;

    if (*src == '\0' || count == 0)
    {
        return;
    }

    const int chars_needed = m_length + count + 1;
    if (m_capacity < chars_needed)
    {
        reserve(chars_needed);
    }

    std::memcpy(m_data + m_length, src, count);

    m_length += count;
    m_data[m_length] = '\0';
}

bool str::starts_with(const char * prefix, const int prefix_len) const
{
    STR_ASSERT(prefix != nullptr);
    STR_ASSERT(prefix_len >= 0);

    if (m_length == 0 || prefix_len == 0 || m_length < prefix_len)
    {
        return false;
    }

    return str::compare(m_data, prefix, prefix_len) == 0;
}

bool str::ends_with(const char * suffix, const int suffix_len) const
{
    STR_ASSERT(suffix != nullptr);
    STR_ASSERT(suffix_len >= 0);

    if (m_length == 0 || suffix_len == 0 || m_length < suffix_len)
    {
        return false;
    }

    if (m_length == suffix_len)
    {
        return str::compare(m_data, suffix) == 0;
    }

    return str::compare(m_data + m_length - suffix_len - 1, suffix, suffix_len) == 0;
}

int str::first_index_of(const char c) const
{
    int i = 0;
    const char * ptr = m_data;

    for (; ptr[i] != '\0'; ++i)
    {
        if (ptr[i] == c)
        {
            return i;
        }
    }

    // In case c == '\0' (find the length of the string).
    if (c == '\0')
    {
        return i;
    }

    return -1; // Not found.
}

int str::last_index_of(const char c) const
{
    int i            =  0;
    int last_index   = -1;
    const char * ptr = m_data;

    for (; ptr[i] != '\0'; ++i)
    {
        if (ptr[i] == c)
        {
            last_index = i;
            // Continue till the last occurrence.
        }
    }

    // In case c == '\0' (find the length of the string).
    if (c == '\0')
    {
        last_index = i;
    }

    return last_index;
}

int str::first_index_of(const char * substring) const
{
    STR_ASSERT(substring != nullptr);
    if (m_length == 0 || *substring == '\0')
    {
        return -1;
    }

    const int substr_len = str::length(substring);
    const char * ptr     = m_data;

    for (int i = 0; *ptr != '\0'; ++i, ++ptr)
    {
        if (*ptr == *substring && str::compare(ptr, substring, substr_len) == 0)
        {
            return i;
        }
    }

    return -1; // Not found.
}

int str::last_index_of(const char * substring) const
{
    STR_ASSERT(substring != nullptr);
    if (m_length == 0 || *substring == '\0')
    {
        return -1;
    }

    int last_index       = -1;
    const int substr_len = str::length(substring);
    const char * ptr     = m_data;

    for (int i = 0; *ptr != '\0'; ++i, ++ptr)
    {
        if (*ptr == *substring && str::compare(ptr, substring, substr_len) == 0)
        {
            last_index = i;
            // Continue till the last occurrence.
        }
    }

    return last_index;
}

int str::find_any_matching_index(const char * charset) const
{
    STR_ASSERT(charset != nullptr);
    if (m_length == 0 || *charset == '\0')
    {
        return -1;
    }

    const char * start_ptr = m_data;
    const char * ptr       = m_data;

    for (; *ptr != '\0'; ++ptr)
    {
        const char * set_ptr = charset;
        for (; *set_ptr != '\0'; ++set_ptr)
        {
            if (*ptr == *set_ptr)
            {
                break;
            }
        }
        if (*set_ptr != '\0')
        {
            break;
        }
    }

    const int match_index = narrow<int>(ptr - start_ptr);
    return (match_index >= m_length ? -1 : match_index);
}

str & str::trim_right()
{
    if (m_length == 0)
    {
        return *this;
    }

    int new_len = m_length;
    char * ptr  = m_data + (new_len - 1);

    while (new_len != 0 && std::isspace(*ptr))
    {
        *ptr-- = '\0';
        --new_len;
    }

    m_length = new_len;
    return *this;
}

str & str::trim_left()
{
    if (m_length == 0)
    {
        return *this;
    }

    int new_len = m_length;
    char * ptr  = str::skip_leading_whitespace(m_data);

    const int displacement = narrow<int>(ptr - m_data);
    if (displacement > 0) // Any white space to the left?
    {
        new_len -= displacement;
        if (new_len != 0)
        {
            std::memmove(m_data, ptr, new_len);
        }

        m_data[new_len] = '\0';
        m_length = new_len;
    }

    return *this;
}

str & str::trim()
{
    trim_right();
    trim_left();
    return *this;
}

str & str::to_upper()
{
    for (char * ptr = m_data; *ptr != '\0'; ++ptr)
    {
        *ptr = static_cast<char>(std::toupper(*ptr));
    }
    return *this;
}

str & str::to_lower()
{
    for (char * ptr = m_data; *ptr != '\0'; ++ptr)
    {
        *ptr = static_cast<char>(std::tolower(*ptr));
    }
    return *this;
}

bool str::setf(const char * fmt, ...)
{
    STR_ASSERT(fmt != nullptr);

    va_list args;

    va_start(args, fmt);
    const bool result = setfv(fmt, args);
    va_end(args);

    return result;
}

bool str::setf_no_grow(const char * fmt, ...)
{
    STR_ASSERT(fmt != nullptr);

    va_list args;

    va_start(args, fmt);
    const bool result = setfv_no_grow(fmt, args);
    va_end(args);

    return result;
}

bool str::setfv(const char * fmt, va_list args)
{
    STR_ASSERT(fmt != nullptr);

    // Needed for portability on platforms where va_list
    // are passed by reference and modified by functions.
    va_list args2;
    va_copy(args2, args);

// MSVC returns -1 on overflow when writing, which forces us to do two passes.
#ifdef _MSC_VER
    int result = std::vsnprintf(nullptr, 0, fmt, args);
    STR_ASSERT(result >= 0);
    if (m_capacity < result + 1)
    {
        reserve_discard(result + 1);
    }
    result = std::vsnprintf(m_data, result + 1, fmt, args2);
#else // !_MSC_VER
    int result = std::vsnprintf(m_owns_buffer ? m_data : nullptr, m_owns_buffer ? m_capacity : 0, fmt, args);
    STR_ASSERT(result >= 0);
    if (m_capacity < result + 1)
    {
        reserve_discard(result + 1);
        result = std::vsnprintf(m_data, result + 1, fmt, args2);
    }
#endif // _MSC_VER

    if (result >= 0)
    {
        m_length      = result;
        m_owns_buffer = true;
        return true;
    }
    else
    {
        clear_no_free();
        return false;
    }
}

bool str::setfv_no_grow(const char * fmt, va_list args)
{
    STR_ASSERT(fmt != nullptr);

    int result = std::vsnprintf(m_data, m_capacity, fmt, args);
    if (result < 0)
    {
        clear_no_free();
        return false;
    }

    if (result >= m_capacity)
    {
        result = m_capacity - 1;
        // Overflowed the string, but still succeeds.
    }

    m_data[result] = '\0'; // Ensure null terminated.
    m_length       = result;
    m_owns_buffer  = true;
    return true;
}

bool str::appendf(const char * fmt, ...)
{
    STR_ASSERT(fmt != nullptr);

    va_list args;

    va_start(args, fmt);
    const bool result = appendfv(fmt, args);
    va_end(args);

    return result;
}

bool str::appendfv(const char * fmt, va_list args)
{
    // Needed for portability on platforms where va_list
    // are passed by reference and modified by functions.
    va_list args2;
    va_copy(args2, args);

    const int cur_len = m_length;

// MSVC returns -1 on overflow when writing, which forces us to do two passes
#ifdef _MSC_VER
    int add_len = std::vsnprintf(nullptr, 0, fmt, args);
    STR_ASSERT(add_len >= 0);

    if (m_capacity < cur_len + add_len + 1)
    {
        reserve(cur_len + add_len + 1);
    }

    add_len = std::vsnprintf(m_data + cur_len, add_len + 1, fmt, args2);
#else // !_MSC_VER
    int add_len = std::vsnprintf(m_owns_buffer ? m_data + cur_len : nullptr, m_owns_buffer ? m_capacity - cur_len : 0, fmt, args);
    STR_ASSERT(add_len >= 0);
    if (m_capacity < cur_len + add_len + 1)
    {
        reserve(cur_len + add_len + 1);
        add_len = std::vsnprintf(m_data + cur_len, add_len + 1, fmt, args2);
    }
#endif // _MSC_VER

    m_owns_buffer = true;
    return add_len >= 0;
}

str & str::truncate(const int max_len)
{
    if (max_len < m_length)
    {
        m_length = max_len;
        m_data[max_len] = '\0';
    }
    return *this;
}

void str::clear()
{
    if (m_owns_buffer && !using_local_buffer())
    {
        STR_ASSERT(m_data != get_empty_dummy_string());
        STR_MEM_FREE(m_data);
    }

    if (m_local_buffer_size > 0) // Have a local buffer?
    {
        m_data        = get_local_buffer();
        m_data[0]     = '\0';
        m_length      = 0;
        m_capacity    = m_local_buffer_size;
        m_owns_buffer = true;
    }
    else // Reset to initials:
    {
        m_data        = get_empty_dummy_string();
        m_length      = 0;
        m_capacity    = 0;
        m_owns_buffer = false;
    }
}

void str::shrink_to_fit()
{
    if (!m_owns_buffer || using_local_buffer())
    {
        return;
    }

    const int new_capacity = m_length + 1;
    if (m_capacity <= new_capacity)
    {
        return;
    }

    char * new_data = STR_MEM_ALLOC(new_capacity);
    STR_ASSERT(new_data != nullptr);

    std::memcpy(new_data, m_data, new_capacity);

    STR_ASSERT(m_data != get_empty_dummy_string());
    STR_MEM_FREE(m_data);

    m_data     = new_data;
    m_capacity = new_capacity;
}

void str::reserve(int new_capacity, const int dynamic_alloc_extra)
{
    STR_ASSERT(dynamic_alloc_extra >= 0);

    if (new_capacity <= m_capacity)
    {
        return;
    }

    // Reserve memory, preserving the current contents of the string buffer.
    char * new_data;
    if (new_capacity < m_local_buffer_size)
    {
        // Disowned -> local buffer
        new_data     = get_local_buffer();
        new_capacity = m_local_buffer_size;
    }
    else
    {
        // Disowned or local buffer -> Heap
        new_capacity += dynamic_alloc_extra;
        new_data = STR_MEM_ALLOC(new_capacity);
        STR_ASSERT(new_data != nullptr);
    }

#ifdef _MSC_VER
    strcpy_s(new_data, new_capacity, m_data);
#else // !_MSC_VER
    std::strcpy(new_data, m_data);
#endif // MSC_VER

    if (m_owns_buffer && !using_local_buffer())
    {
        STR_ASSERT(m_data != get_empty_dummy_string());
        STR_MEM_FREE(m_data);
    }

    m_data        = new_data;
    m_capacity    = new_capacity;
    m_owns_buffer = true;
}

void str::reserve_discard(int new_capacity, const int dynamic_alloc_extra)
{
    STR_ASSERT(dynamic_alloc_extra >= 0);

    // Reserve memory, discarding the current contents of the
    // string buffer (useful if we expect it to be fully overwritten).

    if (new_capacity <= m_capacity)
    {
        return;
    }

    if (m_owns_buffer && !using_local_buffer())
    {
        STR_ASSERT(m_data != get_empty_dummy_string());
        STR_MEM_FREE(m_data);
    }

    if (new_capacity < m_local_buffer_size)
    {
        // Disowned -> local buffer
        m_data     = get_local_buffer();
        m_capacity = m_local_buffer_size;
    }
    else
    {
        // Disowned or local buffer -> Heap
        new_capacity += dynamic_alloc_extra;
        m_data = STR_MEM_ALLOC(new_capacity);
        STR_ASSERT(m_data != nullptr);
        m_capacity = new_capacity;
    }

    // Previous contents are lost.
    m_data[0]     = '\0';
    m_length      = 0;
    m_owns_buffer = true;
}

void str::resize(const int new_len, const char fill_char, const int dynamic_alloc_extra)
{
    STR_ASSERT(new_len >= 0);
    STR_ASSERT(dynamic_alloc_extra >= 0);

    if (new_len == 0)
    {
        clear_no_free();
        return;
    }

    reserve(m_length + new_len + 1, dynamic_alloc_extra);

    std::memset(m_data + m_length, fill_char, new_len);
    m_length += new_len;
    m_data[m_length] = '\0';
}

void str::resize_discard(const int new_len, const char fill_char, const int dynamic_alloc_extra)
{
    STR_ASSERT(new_len >= 0);
    STR_ASSERT(dynamic_alloc_extra >= 0);

    if (new_len == 0)
    {
        clear_no_free();
        return;
    }

    reserve_discard(new_len + 1, dynamic_alloc_extra);

    std::memset(m_data, fill_char, new_len);
    m_length = new_len;
    m_data[m_length] = '\0';
}

int str::compare_no_case(const char * a, const char * b)
{
    STR_ASSERT(a != nullptr);
    STR_ASSERT(b != nullptr);

    #ifdef _MSC_VER
    return _stricmp(a, b);
    #else // !_MSC_VER
    return strcasecmp(a, b);
    #endif // _MSC_VER
}

int str::copy(char * dest, int dest_size_in_chars, const char * source)
{
    STR_ASSERT(dest   != nullptr);
    STR_ASSERT(source != nullptr);
    STR_ASSERT(dest_size_in_chars > 0);

    // Copy until the end of src or until we run out of space in dest:
    char * ptr = dest;
    while ((*ptr++ = *source++) && --dest_size_in_chars > 0) { }

    // Truncate on overflow:
    if (dest_size_in_chars == 0)
    {
        *(--ptr) = '\0';
        return narrow<int>(ptr - dest);
    }

    // Return the number of chars written to dest (not counting the null terminator).
    return narrow<int>(ptr - dest - 1);
}

int str::copy(char * dest, const int dest_size_in_chars, const char * source, const int chars_to_copy)
{
    STR_ASSERT(dest   != nullptr);
    STR_ASSERT(source != nullptr);
    STR_ASSERT(dest_size_in_chars > 0);

    if (chars_to_copy <= 0)
    {
        return 0;
    }

    // Copy until the end of source or until chars_to_copy have been
    // copied, whichever comes first. Also halt and truncate the string
    // if dest_size_in_chars is exceeded.

    char * ptr          = dest;
    int chars_copied    = 0;
    int chars_available = dest_size_in_chars;

    for (;;)
    {
        if (!(*ptr++ = *source++) || --chars_available == 0)
        {
            break;
        }
        if (++chars_copied == chars_to_copy)
        {
            *ptr++ = '\0';
            break;
        }
    }

    // Truncate on overflow:
    if (chars_available == 0)
    {
        *(--ptr) = '\0';
        return narrow<int>(ptr - dest);
    }

    // Return the number of chars written to dest (not counting the null terminator).
    return narrow<int>(ptr - dest - 1);
}

int str::append(char * dest, const int dest_size_in_chars, const char * source, int chars_to_append)
{
    STR_ASSERT(dest   != nullptr);
    STR_ASSERT(source != nullptr);
    STR_ASSERT(dest_size_in_chars > 0); // Total size of buffer, not available count.

    if (chars_to_append <= 0)
    {
        return 0;
    }

    const int dest_len = str::length(dest);
    STR_ASSERT(dest_len < dest_size_in_chars);

    const int chars_available = dest_size_in_chars - dest_len;
    if (chars_to_append >= chars_available)
    {
        chars_to_append = chars_available - 1;
    }

    std::memcpy(dest + dest_len, source, chars_to_append);
    dest[dest_len + chars_to_append] = '\0';

    return dest_len + chars_to_append;
}

char * str::to_upper(char * str)
{
    STR_ASSERT(str != nullptr);
    for (char * ptr = str; *ptr != '\0'; ++ptr)
    {
        *ptr = static_cast<char>(std::toupper(*ptr));
    }
    return str;
}

char * str::to_lower(char * str)
{
    STR_ASSERT(str != nullptr);
    for (char * ptr = str; *ptr != '\0'; ++ptr)
    {
        *ptr = static_cast<char>(std::tolower(*ptr));
    }
    return str;
}

char * str::split(char * str, const char * delimiters, char ** remaining)
{
    // Similar to std::strtok, but keeps no local state.
    STR_ASSERT(delimiters != nullptr);
    STR_ASSERT(remaining  != nullptr);
    STR_ASSERT(str != nullptr || *remaining != nullptr);

    // If 'str' is null, continue with previous string.
    if (str == nullptr)
    {
        str = *remaining;
    }

    for (; *str != '\0'; ++str)
    {
        const char * ctl = delimiters;
        for (; *ctl != '\0' && *ctl != *str; ++ctl)
        {
        }
        if (*ctl == '\0')
        {
            break;
        }
    }

    char * token = str;

    // Find the end of the token.
    // If it is not the end of the string, put a null char there.
    for (; *str != '\0'; ++str)
    {
        const char * ctl = delimiters;
        for (; *ctl != '\0' && *ctl != *str; ++ctl)
        {
        }
        if (*ctl != '\0')
        {
            *str++ = '\0';
            break;
        }
    }

    // Update remaining string and check if a token has been found:
    *remaining = str;
    return (token == str ? nullptr : token);
}

char * str::skip_leading_whitespace(char * str)
{
    STR_ASSERT(str != nullptr);

    for (; *str != '\0'; ++str)
    {
        if (!std::isspace(*str))
        {
            break;
        }
    }

    return str;
}

const char * str::skip_leading_whitespace(const char * str)
{
    STR_ASSERT(str != nullptr);

    for (; *str != '\0'; ++str)
    {
        if (!std::isspace(*str))
        {
            break;
        }
    }

    return str;
}

// ================ End of implementation =================
#endif // STR_IMPLEMENTATION
// ================ End of implementation =================
