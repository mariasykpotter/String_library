#include <stdio.h>
#include <stdlib.h>
#include "my_str_t.h"
#include <ctype.h>
#include <assert.h>

#define BUFF_SIZE 20
//P1. Підрахувати середню довжину слова у текстовому файлі. Словом вважати послідовність літер, розділену
// пробільними символами (whitespace). Перед розрахунком слід викинути із слів
// всі розділові знаки. Результат вивести в stdout.
int word_average(FILE* f)
{
    my_str_t str1;
    size_t init_capacity = 50;
    int status_code;
    status_code = my_str_create(&str1, 50);
    if(status_code < 0)
        return status_code;

    status_code = my_str_read_file(&str1, f);
    if(status_code < 0)
        return status_code;

    my_str_t str2;
    status_code = my_str_create(&str2, 50);
    if(status_code < 0)
        return status_code;

    for(size_t i = 0; i < my_str_size(&str1); ++i)
    {
        char c = (char)my_str_getc(&str1, i);
        if(!ispunct(c))
        {
            status_code = my_str_pushback(&str2, c);
            if(status_code < 0)
                return status_code;
        }

    }
    size_t words_len = 0;
    size_t words_num = 0;
    for(size_t i = 0; i < my_str_size(&str2); ++i)
    {
        if(!isspace(my_str_getc(&str2, i)))
        {
            ++words_len;
            if(i == my_str_size(&str2) - 1)
            {
                ++words_num;
            }
            continue;
        }

        if(i != 0 && !isspace(my_str_getc(&str2, i - 1)))
            words_num += 1;

    }


    char cstr[32];
    snprintf(cstr, sizeof(cstr), "%.2f", (double)words_len/words_num);
    my_str_t average_len;
    status_code = my_str_from_cstr(&average_len, cstr, 10);
    if(status_code < 0)
        return status_code;

    return my_str_write(&average_len);
}

void string_init()
{
    my_str_t str1, str2;
    my_str_create(&str1, BUFF_SIZE);
    assert(my_str_capacity(&str1) == BUFF_SIZE);
    my_str_from_cstr(&str2, "example", BUFF_SIZE);
    printf("Actual string: %s\n\n", my_str_get_cstr(&str2));
}

void string_cmp()
{
    my_str_t str1, str2;
    my_str_from_cstr(&str1, "example", BUFF_SIZE);
    my_str_from_cstr(&str2, "example", BUFF_SIZE);
    //Comparing two strings
    assert(my_str_cmp(&str1, &str2) == 0);

    //Comparing with cstring
    assert(my_str_cmp_cstr(&str1, "dxample") == 1);
}

void string_getc_putc()
{
    my_str_t str1;
    my_str_from_cstr(&str1, "example", BUFF_SIZE);
    my_str_putc(&str1, 2, 'x');
    assert(my_str_getc(&str1, 2) == 'x');
    printf("Put x to position 2: %s\n\n", my_str_get_cstr(&str1));

}

void string_push_pop()
{
    my_str_t str1;
    my_str_from_cstr(&str1, "example", BUFF_SIZE);
    my_str_pushback(&str1, '1');
    printf("Push 1 to example: %s\n\n", my_str_get_cstr(&str1));
    my_str_t str2;
    my_str_from_cstr(&str1, "example", BUFF_SIZE);
    my_str_popback(&str1);
    printf("Pop example: %s\n\n",  my_str_get_cstr(&str1));
}

void string_insert()
{
    my_str_t str1, str2;
    my_str_from_cstr(&str1, "example", BUFF_SIZE);
    my_str_from_cstr(&str2, "HelloWorld!", BUFF_SIZE);
    my_str_insert(&str1, &str2, 2);

    my_str_insert_cstr(&str1, ", ", 7);
    printf("Insert Hello, World to pos 2: %s\n\n", my_str_get_cstr(&str1));
}

void string_substr()
{
    my_str_t str1, str2;
    my_str_from_cstr(&str1,  "Hello, World", BUFF_SIZE);
    my_str_create(&str2, BUFF_SIZE);
    my_str_substr(&str1, &str2, 0, 5);
    printf("Substring of Hello, World: %s\n\n", my_str_get_cstr(&str2));
    char new_char[100];
    my_str_substr_cstr(&str1, new_char, 0, 5);
    printf("C-Substring of Hello, World: %s\n\n", new_char);

}

void string_find()
{
    my_str_t str1;
    my_str_from_cstr(&str1,  "Hello, World", BUFF_SIZE);
    assert(my_str_find_c(&str1, 'e', 0) == 1);
}

int main(int argc, char *argv[]) {
    string_init();
    string_cmp();
    string_getc_putc();
    string_push_pop();
    string_insert();
    string_substr();
    string_find();
    printf("Count word len average!\n\n");
    FILE* f = fopen("../input.txt", "r");
    if(word_average(f) < 0)
    {
        printf("something went wrong!");
        return -1;
    }
    return 0;
}