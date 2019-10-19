//
// Created by user on 19.10.2019.
//

#include "MyString.h"
#include <cstring>
size_t my_str_t::my_strlen(const char *str) {
    size_t index = 0;
    while(str[index++] != '\0');
    return index - 1;
}

void my_str_t::insert(const char *from, size_t pos, size_t length) {
    if(size_m + length > capacity_m)
    {
        reserve((size_m + length)*2);
    }

    memmove(data + pos + length, data + pos, sizeof(char)*length);
    memcpy(data + pos, from, sizeof(char)*length);
    size_m += length;
}

int my_str_t::cmp(const char *cstr, size_t size) {
    for(int i = 0; i < size_m && i < size; ++i){
        if (*(data + i) < *(cstr + i))
            return -1;
        if (*(data + i) > *(cstr + i))
            return 1;
    }
    if (size_m < size)
        return -1;
    if (size_m > size)
        return 1;
    return 0;
}

my_str_t::my_str_t(size_t buf_size) {
    data = new char[buf_size + 1];
    capacity_m = buf_size;
    size_m = 0;
}

my_str_t::my_str_t(const my_str_t& other)
{
    capacity_m = other.capacity_m;
    data = new char[capacity_m + 1];
    size_m = other.size_m;
    memcpy(data, other.data, sizeof(char)*size_m);
}

my_str_t::my_str_t(const char *cstring, size_t buf_size) {
    size_t str_len = my_strlen(cstring);
    capacity_m = buf_size;
    size_m = str_len;
    data = new char[buf_size + 1];
    memcpy(data, cstring, sizeof(char)*size_m);
}

my_str_t::~my_str_t() {
    delete[] data;
    data = nullptr;
    capacity_m = 0;
    size_m = 0;
}

my_str_t& my_str_t::operator=(const my_str_t &other) {
    my_str_t str(other);
    swap(str);
    return *this;
}


void my_str_t::push_back(char c) {
    if(++size_m > capacity_m)
        reserve(size_m * 2);
    *(data + size_m - 1) = c;
}

void my_str_t::pop_back() {
    if(size_m != 0)
        --size_m;
}

void my_str_t::clear() {
    size_m = 0;
}

void my_str_t::insert(char c, size_t pos) {
    if(++size_m > capacity_m)
    {
        reserve(size_m*2);
    }

    memmove(data + pos + 1, data + pos, sizeof(char)*(size_m - pos - 1));
    data[pos] = c;
}

void my_str_t::insert(const my_str_t &from, size_t pos) {
    insert(from.data, pos, from.size_m);
}

void my_str_t::insert(const char *from, size_t pos) {
    insert(from, pos, my_strlen(from));
}

void my_str_t::append(const my_str_t &from) {
    insert(from, size_m);
}

void my_str_t::append(const char *from) {
    insert(from, size_m);
}

my_str_t my_str_t::substr(size_t beg, size_t end) {
    if(end > size_m)
    {
        end = size_m;
    }
    return my_str_t(substr_c(beg, end), beg - end + 1);
}

char* my_str_t::substr_c(size_t beg, size_t end) {
    char* to = new char[end - beg];
    if(end > size_m)
    {
        end = size_m;
    }

    memcpy(to, data + beg, sizeof(char)*(end - beg));
    to[end-beg] = '\0';
    return to;
}

void my_str_t::reserve(size_t buf_size) {
    if (buf_size <= capacity_m)
        return;
    char *new_data = new char[buf_size + 1];
    memcpy(new_data, data, sizeof(char)*size_m);
    data = new_data;
    capacity_m = buf_size;
}

void my_str_t::shrink() {
    char* new_data = new char[size_m + 1];
    capacity_m = size_m;
    memcpy(new_data, data, sizeof(char)*size_m);
}

void my_str_t::resize(size_t new_size, char sym) {
    if (new_size <= size_m){
        size_m = new_size;
        return;
    }
    reserve(new_size);
    for (size_t i = size_m; i < new_size; i++)
        *(data + i) = sym;
    size_m = new_size;
}

size_t my_str_t::find(const my_str_t &tofind, size_t from) {
    if (from > size_m)
        return (size_t)(-1);
    for (size_t i = from; i < size_m; i++){
        for (size_t j = 0; j < tofind.size_m; j++)
            if (*(data + i + j) != *(tofind.data + j)){
                //If one of chars in substring is not equal
                //to char of tofind go to next iteration
                goto label;
            }
        return i;

        label:;
    }
    return (size_t)(-1);
}

int my_str_t::cmp(const my_str_t &str) {
    return cmp(str.data, str.size_m);
}

int my_str_t::cmp(const char *str) {
    size_t size = my_strlen(str);
    return cmp(str, size);
}

size_t my_str_t::find(char c, size_t from) {
    if (from >= size_m)
        return (size_t)(-1);
    for (size_t i = from; i < size_m; ++i)
        if (*(data + i) == c)
            return i;
    return (size_t)(-1);
}

size_t my_str_t::find_if(int predicat(int)) {
    for (size_t i = 0; i < size_m; ++i)
        if (predicat(*(data + i)))
            return i;
    return (size_t)(-1);
}

void my_str_t::read_file(std::ifstream &f) {
    clear();
    char c;
    f.get(c);
    push_back(c);
    while(f.eof() == 0)
    {
        f.get(c);
        push_back(c);
    }
}


void my_str_t::write_file(std::ofstream &f) {
    data[size_m] = '\0';

    f.write(data, sizeof(data));
}

void my_str_t::swap(my_str_t& other) {
    std::swap(data, other.data);
    std::swap(size_m, other.size_m);
    std::swap(capacity_m, other.capacity_m);
}

char* my_str_t::cstring()
{
    char* new_data = new char[size_m + 1];
    memcpy(new_data, data, sizeof(char)*size_m);
    new_data[size_m] = '\0';
    return new_data;
}

std::ostream& operator<<(std::ostream& os, my_str_t& str) {
    os << str.cstring();
    return os;
}

