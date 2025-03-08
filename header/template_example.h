#ifndef TEMPLATE_EXAMPLE_H_
#define TEMPLATE_EXAMPLE_H_

// Template cho một lớp đơn giản
template <class T>
class Container {
private:
    T data;
public:
    Container(T value) : data(value) {}
    
    T getData() const { return data; }
    void setData(T value) { data = value; }
    
    // Template method để so sánh với một giá trị khác
    bool isEqual(const T& other) const {
        return data == other;
    }
};

// Template cho một hàm đơn giản
template <typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Template với nhiều tham số kiểu
template <class T1, class T2>
class Pair {
private:
    T1 first;
    T2 second;
public:
    Pair(T1 f, T2 s) : first(f), second(s) {}
    
    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }
};

#endif 