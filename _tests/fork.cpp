#include <unistd.h>
#include <iostream>

int main() {
    int pid = fork();
    std::cout << pid << std::endl;
}
