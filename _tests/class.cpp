#include <cstdlib>
#include <iostream>


class A {

public:
    A(): test(0) {}
    ~A() {}

    void set(int i) {
        test = i;
    }

    int get() {
        return test;
    }

private:

    int test;

};


void f(A &a) {
    a.set(10);
}

int main() {
    A c;
    f(c);
    std::cout << c.get() << std::endl;
}
