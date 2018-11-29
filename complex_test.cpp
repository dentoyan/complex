#include "complex.h"
#include <iostream>
#include <cmath>


int main()
{
    Complex a(3.0, 8.0);
    Complex b(2.0, 1.0);

    Complex p = a * b;
    std::cout << "a*b=";
    p.write(std::cout);
    std::cout << " abs=" << p.abs() << std::endl;

    Complex d = a / b;
    std::cout << "a/b=";
    d.write(std::cout);
    std::cout << " abs=" << d.abs() << std::endl;

    return 0;
}
