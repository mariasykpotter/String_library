#include <stdio.h>
typedef struct my_str_t {
    size_t capacity_m; // Розмір блока
    size_t size_m;     // Фактичний розмір стрічки
    char *data;       // Вказівник на блок пам'яті
} my_string_t;

int main(int argc, char *argv[]) {
    my_string_t str;
}
int my_str_create(my_str_t *str, size_t buf_size) {
    if (!str) {
        return -1;
    }
    str->data = malloc((buf_size + 1));
    if (!str->data) {
        return -1;
    }
    str->size_m = 0;
    str->capacity_m = buf_size;
    return 0;
}
void my_str_free(my_str_t *str) {
    if (!str)
        return -1;
    free(str->data);
    str->data = 0;
    str->size_m = 0;
    str->capacity_m = 0;
}
int my_str_from_cstr(my_str_t* str, const char* cstr,size_t buf_size){
    size_t str_length = cstr_len(cstr);
    if (!str) {
        return FAILURE;
    }
    if (buf_size < str_length){
        return -1;
    }
    str->size_m = str_length;
    if (buf_size <= 0)
        str->capacity_m = str->size_m + 1;
    else
        str->capacity_m = buf_size;
    str->data = malloc(buf_size);

    if (!str->data) {
        errno = ENOMEM;
        return FAILURE;
    }
    for (size_t i = 0; i < str->size_m; i++)
        *(str->data + i) = *(cstr + i);
    return EXIT_SUCCESS;
}