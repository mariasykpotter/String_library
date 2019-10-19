#include <gtest/gtest.h>

extern "C"{
#include "my_str_t.h"
}

class StringFixture : public :: testing::Test{

protected:
    size_t buf_size = 20;
    int system_code = -1;
};

TEST_F(StringFixture, test_initilization)
{
    my_str_t str1;

    my_str_create(&str1, buf_size);
    EXPECT_EQ(my_str_capacity(&str1), buf_size);

    size_t exp_length = 0;
    size_t actual_size = my_str_size(&str1);
    EXPECT_EQ(actual_size, exp_length);
    EXPECT_TRUE(my_str_empty(&str1));

    my_str_free(&str1);
    size_t exp_capacity = 0;
    size_t actual_capacity = my_str_capacity(&str1);
    EXPECT_EQ(actual_capacity, exp_capacity);
}

TEST_F(StringFixture, test_cstring_strcmp)
{
    my_str_t str1, str2;
    const size_t string_size = 7;
    const char cstring[string_size + 1] = "example";
    my_str_from_cstr(&str1, cstring, buf_size);

    size_t actual_capacity = my_str_capacity(&str1);
    size_t actual_length = my_str_size(&str1);

    EXPECT_EQ(actual_capacity, buf_size);
    EXPECT_EQ(actual_length, string_size);


    EXPECT_EQ(my_str_create(0, buf_size), system_code);
    EXPECT_EQ(my_str_cmp_cstr(&str1, cstring), 0);

    const char* cstring2 = "examplee";
    const char* cstring3 = "axamplee";

    EXPECT_EQ(my_str_cmp_cstr(&str1, cstring2), -1);
    EXPECT_EQ(my_str_cmp_cstr(&str1, cstring3), 1);

    my_str_from_cstr(&str2, cstring2, buf_size);
    EXPECT_EQ(my_str_cmp(&str1, &str2), -1);
}

TEST_F(StringFixture, test_getc_putc)
{
    my_str_t str1;
    const size_t string_size = 7;
    const char cstring[string_size + 1] = "example";
    my_str_from_cstr(&str1, cstring, 10);

    char ch1 = 'j';
    size_t pos1 = 2;
    my_str_putc(&str1, pos1, ch1);
    EXPECT_EQ(my_str_getc(&str1, pos1), ch1);

    char ch2 = 'k';
    size_t pos2 = 7;
    my_str_putc(&str1, pos1, ch1);


    int actual_code = my_str_putc(&str1, pos2, ch2);
    EXPECT_EQ(my_str_putc(&str1, pos2, ch2), system_code);
    EXPECT_EQ(my_str_getc(&str1, pos2), system_code);
}

TEST_F(StringFixture, test_push_pop)
{
    my_str_t str1, str2;

    const size_t string_size = 3;
    char ch1 = '4';
    const char cstring[string_size + 1] = "123";
    my_str_from_cstr(&str1, cstring, buf_size);
    my_str_pushback(&str1, ch1);
    EXPECT_EQ(my_str_getc(&str1, string_size), ch1);
    EXPECT_EQ(my_str_size(&str1), string_size + 1);

    my_str_popback(&str1);

    EXPECT_EQ(my_str_getc(&str1, string_size), system_code);
    EXPECT_EQ(my_str_size(&str1), string_size);
}

TEST_F(StringFixture, test_insert)
{
    my_str_t str1, str2, str3;

    const char* cstring1 = "129";
    const char* cstring2 = "45";
    const char* cstring3 = "012";
    my_str_from_cstr(&str1, cstring1, buf_size);
    my_str_from_cstr(&str2, cstring2, buf_size);
    my_str_from_cstr(&str3, cstring3, buf_size);

    char ch1 = '3';
    size_t pos1 = 2;
    my_str_insert_c(&str1, ch1, pos1);
    EXPECT_EQ(my_str_getc(&str1, pos1), ch1);
    EXPECT_EQ(my_str_cmp_cstr(&str1, "1239"), 0);

    size_t pos2 = 3;
    my_str_insert(&str1, &str2, pos2);

    char ch2 = '4';
    EXPECT_EQ(my_str_getc(&str1, pos2), ch2);

    size_t exp_size = 6;
    EXPECT_EQ(my_str_size(&str1), exp_size);

    const char* cstring4 = "678";
    size_t pos3 = 5;
    char ch3 = '1';
    my_str_insert_cstr(&str1, cstring1, pos3);
    EXPECT_EQ(my_str_getc(&str1, pos3), ch3);

    size_t new_size = 12;
    size_t pos4 = 9;
    size_t ch4 = '0';
    my_str_append(&str1, &str3);
    EXPECT_EQ(my_str_size(&str1), new_size);
    EXPECT_EQ(my_str_getc(&str1, pos4), ch4);

}

TEST_F(StringFixture, test_substr)
{
    my_str_t str1, str2;

    const char* cstring1 = "12345";
    size_t beg = 1;
    size_t end = 3;
    my_str_from_cstr(&str1, cstring1, buf_size);
    my_str_substr(&str1, &str2, beg, end);
    size_t exp_size = 2;
    EXPECT_EQ(my_str_size(&str2), exp_size);

    char ch = '2';
    size_t pos= 0;
    EXPECT_EQ(my_str_getc(&str2, pos), ch);

    char new_string[10];
    my_str_substr_cstr(&str1, new_string, beg, end);

    EXPECT_EQ(new_string[pos], ch);
}

TEST_F(StringFixture, test_find)
{
    my_str_t str1, str2, str3;
    size_t actual_index, exp_index;
    const char* cstring1 = "012345";
    const char* cstring2 = "34";
    my_str_from_cstr(&str1, cstring1, buf_size);
    my_str_from_cstr(&str2, cstring2, buf_size);

    size_t from = 0;
    actual_index = my_str_find(&str1, &str2, from);
    exp_index = 3;
    EXPECT_EQ(actual_index, exp_index);

    char ch = '4';
    actual_index = my_str_find_c(&str1, ch, from);
    exp_index = 4;
    EXPECT_EQ(actual_index, exp_index);

    const char* cstring = "exaMple";
    my_str_from_cstr(&str3, cstring, buf_size);
    actual_index = my_str_find_if(&str3, isupper);
    exp_index = 3;
    EXPECT_EQ(actual_index, exp_index);

}

TEST_F(StringFixture, test_memory)
{
    my_str_t str1, str2;

    const char* cstring = "example";
    my_str_from_cstr(&str1, cstring, buf_size);
    my_str_create(&str2, buf_size);
    my_str_copy(&str1, &str2, 1);
    EXPECT_EQ(my_str_cmp(&str1, &str2), 0);
    EXPECT_EQ(my_str_capacity(&str1), my_str_capacity(&str2));

    my_str_copy(&str1, &str2, 0);
    EXPECT_EQ(my_str_size(&str1), my_str_capacity(&str2));

    my_str_reserve(&str2, buf_size);
    EXPECT_EQ(my_str_capacity(&str2), buf_size);

    char ch = '1';
    my_str_resize(&str2, buf_size/4, ch);
    EXPECT_EQ(my_str_size(&str2), buf_size/4);

    my_str_resize(&str2, buf_size, ch);
    EXPECT_EQ(my_str_capacity(&str2), buf_size);
    EXPECT_EQ(my_str_size(&str2), buf_size);
    EXPECT_EQ(my_str_getc(&str2, buf_size - 1), ch);
}

TEST_F(StringFixture, test_files_work)
{
    const char* file_name = "1.txt";
    FILE* f = fopen(file_name, "w");
    my_str_t str1, str2;
    const char* cstring = "Hello, World!";
    my_str_from_cstr(&str1, cstring, buf_size);
    my_str_create(&str2, buf_size);
    my_str_write_file(&str1, f);

    f = fopen(file_name, "r");
    my_str_read_file(&str2, f);
    EXPECT_EQ(my_str_cmp(&str1, &str2), 0);

    char delim = ',';
    f = fopen(file_name, "r");
    my_str_read_file_delim(&str2, f, delim);
    EXPECT_EQ(my_str_cmp_cstr(&str2, "Hello"), 0);
}






