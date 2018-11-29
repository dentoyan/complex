# AVX Complex Class #


## About ##

Pure lean and fast complex arithmetic using AVX2 vector unit.
Almost double performance by parallel processing.

Copyright (c) 2014-2018 Joshua Dentoyan


## Overview ##

The class provides basic complex operations including
addition, subtraction, multiplication, division etc.


## Build ##

g++ -mavx2 -o complex_test complex_test.cpp

## Usage ##

The header only complex.h should be inluded in the main module.



```cpp
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
```

## License ##

<a href="LICENSE">LGPLv3+</a>.


---
EOF




