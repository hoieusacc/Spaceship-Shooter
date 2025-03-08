#include "../header/template_example.h"
#include <iostream>

int main() {
    // Sử dụng Container template với kiểu int
    Container<int> intContainer(42);
    std::cout << "Container chứa số nguyên: " << intContainer.getData() << std::endl;
    
    // Sử dụng Container template với kiểu double
    Container<double> doubleContainer(3.14);
    std::cout << "Container chứa số thực: " << doubleContainer.getData() << std::endl;
    
    // Sử dụng hàm template max
    std::cout << "Max(5, 10): " << max(5, 10) << std::endl;
    std::cout << "Max(3.14, 2.72): " << max(3.14, 2.72) << std::endl;
    
    // Sử dụng Pair template với hai kiểu khác nhau
    Pair<std::string, int> pair("Hello", 42);
    std::cout << "Pair: " << pair.getFirst() << ", " << pair.getSecond() << std::endl;
    
    return 0;
} 