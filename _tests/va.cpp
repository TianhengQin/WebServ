#include <stdarg.h>
#include <iostream>

void print(int i, const char *s, ...) {
    va_list arg;
    va_start(arg, s);
    std::cout << *s;
    s++;
    while (*s) {
        std::cout << *s;
        s++;
    }
    std::cout << std::endl;
}


int main() {

    print(1, "yes", "yes");

    return 0;
}
