#include <iostream>

void printString( int n ) {
    const char* var = "Hello, World!";
    for( int i = 0; i < n; i++ ) {
        std::cout << var << std::endl;
    }
    std::cout << std::endl;
    int i = 0;
    while ( i < n ) {
        std::cout << var << std::endl;
        i++;
    }
    std::cout << std::endl;
    i = 0;
    do {
        std::cout << var << std::endl;
        i++;
    }
    while (i < n);

}

int main() {
    const char* var = "Hello, World!";
    std::cout << var << std::endl;
    std::cout << std::endl;
    printString(5);
}