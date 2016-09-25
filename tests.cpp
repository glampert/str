
// ================================================================================================
// -*- C++ -*-
// File: tests.cpp
// Author: Guilherme R. Lampert
// Created on: 25/09/16
// Brief: Unit tests for the string classes.
// ================================================================================================

#define STR_IMPLEMENTATION
#include "str.hpp"

// For printf
#include <cstdio>

namespace unittest
{

void test_str_basics()
{
    // Test the basic assumptions about size, capacity, etc.
    str s;
    STR_ASSERT( s.empty()       == true    );
    STR_ASSERT( s.length()      == 0       );
    STR_ASSERT( s.capacity()    == 0       );
    STR_ASSERT( s.c_str()       != nullptr );
    STR_ASSERT( s.c_str()[0]    == '\0'    );
    STR_ASSERT( s.owns_buffer() == false   );

    s = "hello";
    STR_ASSERT( s.empty()       == false   );
    STR_ASSERT( s.length()      == 5       );
    STR_ASSERT( s.capacity()    >= 5       );
    STR_ASSERT( s.c_str()       != nullptr );
    STR_ASSERT( s.c_str()[0]    == 'h'     );
    STR_ASSERT( s.owns_buffer() == true    );

    STR_ASSERT( s == "hello" );
    STR_ASSERT( s.compare("hello") == 0 );
    STR_ASSERT( s.compare_no_case("hello") == 0 );
    STR_ASSERT( s.compare_no_case("HELLO") == 0 );
    STR_ASSERT( s.compare_no_case("hEllO") == 0 );

    s.setf("%s%s%s", "hello", " ", "world");

    STR_ASSERT( s.empty()    == false );
    STR_ASSERT( s.length()   == 11    );
    STR_ASSERT( s.capacity() >= 11    );

    STR_ASSERT( s == "hello world" );
    STR_ASSERT( s.compare("hello world") == 0 );
    STR_ASSERT( s.compare_no_case("hello world") == 0 );
    STR_ASSERT( s.compare_no_case("HELLO WORLD") == 0 );
    STR_ASSERT( s.compare_no_case("hEllO WorlD") == 0 );

    STR_ASSERT( s.starts_with("hello") == true  );
    STR_ASSERT( s.ends_with("world")   == true  );
    STR_ASSERT( s.starts_with("foo")   == false );
    STR_ASSERT( s.ends_with("bar")     == false );

    STR_ASSERT( s.first_index_of('o') ==  4 ); // First 'o' is in "hello"
    STR_ASSERT( s.first_index_of('X') == -1 ); // No such character
    STR_ASSERT( s.last_index_of('o')  ==  7 ); // Last 'o' is in "world"
    STR_ASSERT( s.last_index_of('X')  == -1 ); // No such character

    // Now clear and re-validate:
    s.clear();
    STR_ASSERT( s.empty()    == true    );
    STR_ASSERT( s.length()   == 0       );
    STR_ASSERT( s.c_str()    != nullptr );
    STR_ASSERT( s.c_str()[0] == '\0'    );

    // Test shrink_to_fit():
    s = "1234567890-+qwertyuiop[]asdfghjkl;'zxcvbnm,./<>?|`~";

    STR_ASSERT( s.length()      == 51   );
    STR_ASSERT( s.capacity()    >= 15   );
    STR_ASSERT( s.owns_buffer() == true );

    s = "tiny";
    s.shrink_to_fit();

    STR_ASSERT( s               == "tiny" );
    STR_ASSERT( s.length()      == 4      );
    STR_ASSERT( s.capacity()    == 5      ); // + the null terminator
    STR_ASSERT( s.owns_buffer() == true   );

    // Test reserve:
    s.reserve(128);
    STR_ASSERT( s               == "tiny" );
    STR_ASSERT( s.empty()       == false  );
    STR_ASSERT( s.length()      == 4      ); // length/contents should be unchanged.
    STR_ASSERT( s.capacity()    >= 128    );
    STR_ASSERT( s.owns_buffer() == true   );

    // Now reserve_discard, which clears the string:
    s.reserve_discard(512);
    STR_ASSERT( s.c_str()[0]    == '\0' );
    STR_ASSERT( s.empty()       == true );
    STR_ASSERT( s.length()      == 0    ); // length should be zero now.
    STR_ASSERT( s.capacity()    >= 512  );
    STR_ASSERT( s.owns_buffer() == true );

    // Clearing an empty string releases memory:
    s.clear();
    STR_ASSERT( s.empty()    == true );
    STR_ASSERT( s.length()   == 0    );
    STR_ASSERT( s.capacity() == 0    );

    s = "test";
    s.push_back('-');
    s.push_back('1');
    s.push_back('2');
    s.push_back('3');
    STR_ASSERT( s == "test-123" );
    STR_ASSERT( s.length() == 8 );

    s += "-abcd";
    STR_ASSERT( s == "test-123-abcd" );
    STR_ASSERT( s.length() == 13 );

    // swap:
    str other{ "foobar" };
    swap(other, s);

    STR_ASSERT( other != s               );
    STR_ASSERT( other == "test-123-abcd" );
    STR_ASSERT( s     == "foobar"        );

    // Trimming:
    s = " \t \t \n \nHello\n \n \t \t ";

    s.trim_left();
    STR_ASSERT( s == "Hello\n \n \t \t " );

    s.trim_right();
    STR_ASSERT( s == "Hello" );

    // Iteration:
    s.setf("%s", "AESTHETICS");

    other.clear_no_free();
    for (auto c : s)
    {
        other += c;
        other += " ";
    }
    STR_ASSERT( other.trim() == "A E S T H E T I C S" );

    other.clear_no_free();
    for (auto iter = s.rbegin(); iter != s.rend(); ++iter)
    {
        other += *iter;
        other += " ";
    }
    STR_ASSERT( other.trim() == "S C I T E H T S E A" );

    // Lowercase/uppercase:
    STR_ASSERT( s.to_lower() == "aesthetics" );
    STR_ASSERT( s.to_upper() == "AESTHETICS" );

    // Truncation:
    STR_ASSERT( s.truncate(10) == "AESTHETICS" ); // Already 10 chars, does nothing.
    STR_ASSERT( s.length() == 10 );
    STR_ASSERT( s.truncate(5) == "AESTH" );
    STR_ASSERT( s.length() == 5 );
}

void test_str_ref()
{
    str_ref s{ "Hello World!" };
    STR_ASSERT( s.empty()       == false   );
    STR_ASSERT( s.length()      == 12      );
    STR_ASSERT( s.capacity()    == 0       );
    STR_ASSERT( s.c_str()       != nullptr );
    STR_ASSERT( s.c_str()[0]    == 'H'     );
    STR_ASSERT( s.owns_buffer() == false   );

    // Compare against a heap-allocated str:
    const str s2{ "Hello World!" };
    STR_ASSERT( s == s2 );
    STR_ASSERT( s.compare(s2) == 0 );
    STR_ASSERT( s.compare_no_case(s2) == 0 );
}

void test_str_sized()
{
    str16  s16;
    str32  s32;
    str64  s64;
    str128 s128;
    str256 s256;

    STR_ASSERT( s16.length()      == 0    );
    STR_ASSERT( s16.capacity()    == 16   );
    STR_ASSERT( s16.owns_buffer() == true );

    STR_ASSERT( s32.length()      == 0    );
    STR_ASSERT( s32.capacity()    == 32   );
    STR_ASSERT( s32.owns_buffer() == true );

    STR_ASSERT( s64.length()      == 0    );
    STR_ASSERT( s64.capacity()    == 64   );
    STR_ASSERT( s64.owns_buffer() == true );

    STR_ASSERT( s128.length()      == 0    );
    STR_ASSERT( s128.capacity()    == 128  );
    STR_ASSERT( s128.owns_buffer() == true );

    STR_ASSERT( s256.length()      == 0    );
    STR_ASSERT( s256.capacity()    == 256  );
    STR_ASSERT( s256.owns_buffer() == true );

    //
    // Stress str16 to see if the dynamic alloc works properly:
    //
    s16.setf("long_filename_not_very_long_but_longer_than_expected-%i", 1234);
    STR_ASSERT( s16.empty()       == false );
    STR_ASSERT( s16.length()      == 57    );
    STR_ASSERT( s16.capacity()    >= 57    );
    STR_ASSERT( s16.owns_buffer() == true  );

    //
    // Compare different types of strings:
    //
    s256 = "Hey there bud!";
    s32  = "Hey there friend!";
    STR_ASSERT( s32 != s256 );
    STR_ASSERT( !(s32 == s256) );
    STR_ASSERT( s256.compare("Hey there bud!") == 0 );
    STR_ASSERT( s256.compare_no_case("Hey THERE bud!") == 0 );
    STR_ASSERT( s32.starts_with("Hey there") == true );
    STR_ASSERT( s256.ends_with("bud!") == true );
}

} // namespace unittest {}

// ========================================================
// Test suite entry point:
// ========================================================

#define STR_TEST(func)                       \
    do {                                     \
        std::printf("- test_%s()\n", #func); \
        unittest::test_##func();             \
        std::printf("- Ok\n");               \
    } while (0,0)

int main()
{
    std::printf("---- Running unit tests for the str classes ----\n");

    STR_TEST(str_basics);
    STR_TEST(str_ref);
    STR_TEST(str_sized);
    // Should add more tests here!

    std::printf("All passed.\n");
}

#undef STR_TEST
