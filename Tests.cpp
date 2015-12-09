
//
// Minimal set of unit tests for the Str classes.
//
// LICENSE
// This software is in the public domain. Where that dedication is not
// recognized, you are granted a perpetual, irrevocable license to copy,
// distribute, and modify this file as you see fit.
//

#include "Str.hpp"
#include <iostream>
#include <cassert>

#define TEST_ASSERT(expr) assert("Test assumption failed: " && (expr))

void Test_StrBasics()
{
    // Test the basic assumptions about size, capacity, etc.
    Str s;
    TEST_ASSERT( s.empty()       == true    );
    TEST_ASSERT( s.length()      == 0       );
    TEST_ASSERT( s.capacity()    == 0       );
    TEST_ASSERT( s.c_str()       != nullptr );
    TEST_ASSERT( s.c_str()[0]    == '\0'    );
    TEST_ASSERT( s.owns_buffer() == false   );

    s = "hello";
    TEST_ASSERT( s.empty()       == false   );
    TEST_ASSERT( s.length()      == 5       );
    TEST_ASSERT( s.capacity()    >= 5       );
    TEST_ASSERT( s.c_str()       != nullptr );
    TEST_ASSERT( s.c_str()[0]    == 'h'     );
    TEST_ASSERT( s.owns_buffer() == true    );

    TEST_ASSERT( s == "hello" );
    TEST_ASSERT( s.compare("hello") == 0 );
    TEST_ASSERT( s.compare_no_case("hello") == 0 );
    TEST_ASSERT( s.compare_no_case("HELLO") == 0 );
    TEST_ASSERT( s.compare_no_case("hEllO") == 0 );

    s.setf("%s %s", "hello", "world");

    TEST_ASSERT( s.empty()    == false );
    TEST_ASSERT( s.length()   == 11    );
    TEST_ASSERT( s.capacity() >= 11    );

    TEST_ASSERT( s == "hello world" );
    TEST_ASSERT( s.compare("hello world") == 0 );
    TEST_ASSERT( s.compare_no_case("hello world") == 0 );
    TEST_ASSERT( s.compare_no_case("HELLO WORLD") == 0 );
    TEST_ASSERT( s.compare_no_case("hEllO WorlD") == 0 );

    TEST_ASSERT( s.starts_with("hello") == true  );
    TEST_ASSERT( s.ends_with("world")   == true  );
    TEST_ASSERT( s.starts_with("foo")   == false );
    TEST_ASSERT( s.ends_with("bar")     == false );

    TEST_ASSERT( s.find_first_occurrence('o') ==  4 ); // First 'o' is in "hello"
    TEST_ASSERT( s.find_first_occurrence('X') == -1 ); // No such character
    TEST_ASSERT( s.find_last_occurrence('o')  ==  7 ); // Last 'o' is in "world"
    TEST_ASSERT( s.find_last_occurrence('X')  == -1 ); // No such character

    // Now clear and re-validate:
    s.clear();
    TEST_ASSERT( s.empty()    == true    );
    TEST_ASSERT( s.length()   == 0       );
    TEST_ASSERT( s.c_str()    != nullptr );
    TEST_ASSERT( s.c_str()[0] == '\0'    );

    // Test shrink_to_fit:
    s = "1234567890-+qwertyuiop[]asdfghjkl;'zxcvbnm,./<>?|`~";

    TEST_ASSERT( s.length()      == 51   );
    TEST_ASSERT( s.capacity()    >= 15   );
    TEST_ASSERT( s.owns_buffer() == true );

    s = "tiny";
    s.shrink_to_fit();

    TEST_ASSERT( s.length()      == 4    );
    TEST_ASSERT( s.capacity()    == 5    ); // + the null terminator
    TEST_ASSERT( s.owns_buffer() == true );

    // Test reserve:
    s.reserve(128);
    TEST_ASSERT( s.empty()       == false );
    TEST_ASSERT( s.length()      == 4     ); // length should be unchanged.
    TEST_ASSERT( s.capacity()    >= 128   );
    TEST_ASSERT( s.owns_buffer() == true  );

    // Now reserve_discard, which clears the string:
    s.reserve_discard(512);
    TEST_ASSERT( s.empty()       == true );
    TEST_ASSERT( s.length()      == 0    ); // length should be zero now.
    TEST_ASSERT( s.capacity()    >= 512  );
    TEST_ASSERT( s.owns_buffer() == true );
}

void Test_StrRef()
{
    StrRef s{ "Hello World!" };
    TEST_ASSERT( s.empty()       == false   );
    TEST_ASSERT( s.length()      == 12      );
    TEST_ASSERT( s.capacity()    == 0       );
    TEST_ASSERT( s.c_str()       != nullptr );
    TEST_ASSERT( s.c_str()[0]    == 'H'     );
    TEST_ASSERT( s.owns_buffer() == false   );

    // Compare again a heap-allocated Str:
    const Str s2{ "Hello World!" };
    TEST_ASSERT( s == s2 );
    TEST_ASSERT( s.compare(s2) == 0 );
    TEST_ASSERT( s.compare_no_case(s2) == 0 );
}

void Test_SizedStrings()
{
    Str16  s16;
    Str32  s32;
    Str64  s64;
    Str128 s128;
    Str256 s256;

    TEST_ASSERT( s16.length()      == 0    );
    TEST_ASSERT( s16.capacity()    == 16   );
    TEST_ASSERT( s16.owns_buffer() == true );

    TEST_ASSERT( s32.length()      == 0    );
    TEST_ASSERT( s32.capacity()    == 32   );
    TEST_ASSERT( s32.owns_buffer() == true );

    TEST_ASSERT( s64.length()      == 0    );
    TEST_ASSERT( s64.capacity()    == 64   );
    TEST_ASSERT( s64.owns_buffer() == true );

    TEST_ASSERT( s128.length()      == 0    );
    TEST_ASSERT( s128.capacity()    == 128  );
    TEST_ASSERT( s128.owns_buffer() == true );

    TEST_ASSERT( s256.length()      == 0    );
    TEST_ASSERT( s256.capacity()    == 256  );
    TEST_ASSERT( s256.owns_buffer() == true );

    //
    // Stress Str16 to see if the dynamic alloc works properly:
    //
    s16.setf("long_filename_not_very_long_but_longer_than_expected-%d", 123);
    TEST_ASSERT( s16.empty()       == false );
    TEST_ASSERT( s16.length()      == 56    );
    TEST_ASSERT( s16.capacity()    >= 56    );
    TEST_ASSERT( s16.owns_buffer() == true  );

    //
    // Compare different types of strings:
    //
    s256 = "Hey there bud!";
    s32  = "Hey there friend!";
    TEST_ASSERT( s32 != s256 );
    TEST_ASSERT( !(s32 == s256) );
    TEST_ASSERT( s256.compare("Hey there bud!") == 0 );
    TEST_ASSERT( s256.compare_no_case("Hey THERE bud!") == 0 );
    TEST_ASSERT( s32.starts_with("Hey there") == true );
    TEST_ASSERT( s256.ends_with("bud!") == true );
}

// ========================================================
// Test suite entry point:
// ========================================================

#define TEST(func)                               \
    do                                           \
    {                                            \
        std::cout << "- Test " << #func << "\n"; \
        Test_##func();                           \
        std::cout << "- Ok\n";                   \
    } while (0)

int main()
{
    std::cout << "---- Running Unit Tests for the Str classes ----\n";

    TEST(StrBasics);
    TEST(StrRef);
    TEST(SizedStrings);
    // TODO: There are certainly more tests to be added here.

    std::cout << "All passed.\n";
}

#undef TEST

