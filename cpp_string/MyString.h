//
// Created by user on 19.10.2019.
//

#ifndef MY_STRING_MYCPPSTRING_H
#define MY_STRING_MYCPPSTRING_H


#include <fstream>

class my_str_t {
private:
    char* data;
    size_t size_m;
    size_t capacity_m;

    static size_t my_strlen(const char *str);
    void insert(const char* from, size_t pos, size_t length);
    int cmp(const char* cstr, size_t size);

public:
    explicit my_str_t(size_t buf_size);
    my_str_t(const char* cstring, size_t buf_size);
    my_str_t(const my_str_t& other);
    ~my_str_t();
    my_str_t& operator=(const my_str_t& other);
    inline size_t size() { return size_m; }
    inline size_t capacity() { return capacity_m; }
    inline bool empty() { return size_m == 0; }

    inline char& operator[](size_t pos) { return data[pos]; }
    void push_back(char c);
    void pop_back();
    void clear();
    void insert(char c, size_t pos);
    void insert(const my_str_t& from, size_t pos);
    void insert(const char* from, size_t pos);
    void append(const my_str_t& from);
    void append(const char* from);
    my_str_t substr(size_t beg, size_t end);
    char* substr_c(size_t beg, size_t end);
    void reserve(size_t buf_size);
    void shrink();
    void resize(size_t new_size, char sym);
    size_t find(const my_str_t& tofind, size_t from);
    int cmp(const my_str_t& str);
    int cmp(const char* str);
    size_t find(char c, size_t from);
    size_t find_if(int predicat(int));
    void read_file(std::ifstream& f);
    void write_file(std::ofstream& f);
    void swap(my_str_t& other);
    char* cstring();
};

std::ostream& operator<<(std::ostream& os, my_str_t& str);


#endif //MY_STRING_MYCPPSTRING_H
