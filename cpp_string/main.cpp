#include <iostream>
#include "MyString.h"

using namespace std;
int main()
{
    my_str_t str("example", 10);
    cout << str << endl;
    cout << "The char in position 2: " << str[2] << endl;
    str.pop_back();
    cout << "Remove last elemet: " << str << endl;
    str.insert('x', 2);
    cout << "insert x in pos 2: " << str << endl;
    my_str_t extra_str(" Hello, World! ", 20);
    str.insert(extra_str, 2);
    cout << "insert Hello, World! in pos 2: " << str << endl;
    char *c = str.substr_c(2, 15);
    cout << "Substring of str: " << c << endl;
    cout << "Find Hello, World! in str: " << str.find(extra_str, 0) << endl;
    cout << "Find char in upper case from pos 1: " << str.find_if(isupper) << endl;

    return 0;
}

