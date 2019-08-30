
#pragma once

template<typename T>
class array_container {
public:
    using index = unsigned int;

    explicit array_container() = default;

    virtual T& operator[](index i) = 0;
    virtual T* data() = 0;
    virtual const T& peek() = 0;
    virtual void push_back(const T & v) = 0;
    virtual void pop_back() = 0;
    virtual void move(index idx) = 0;
    virtual void clear() = 0;
    virtual index size() = 0;
    virtual index capacity() = 0;

};
