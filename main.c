#include <stdio.h>
#include <stdlib.h>
#include "my_str_t.h"


int main(int argc, char *argv[]) {
    my_str_t str;
    size_t n = 3;
    my_str_create(&str, n);
}