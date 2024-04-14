#include <iostream>
#include <fstream>
#include <sstream>

int main () {

    std::ifstream f1("test_picture.jpeg", std::fstream::binary);

    std::ofstream f2("test_picture_copy.html", std::fstream::trunc | std::fstream::binary);

    std::string bf;

    std::ostringstream ss;
	ss << f1.rdbuf();
    bf = ss.str();

    std::cout << bf << std::endl;

    f2 << bf;


    f1.close();
    f2.close();
    return 0;
}
