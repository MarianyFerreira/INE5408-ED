#ifndef STRUCTURES_ARRAY_STACK_H
#define STRUCTURES_ARRAY_STACK_H

#include <cstdint> // std::size_t
#include <stdexcept> // C++ exceptions

namespace structures {

template<typename T>
class ArrayStack {
public:
    ArrayStack(): ArrayStack(DEFAULT_SIZE) {}

    ArrayStack(std::size_t max): contents(new T[max_size_]), top_(-1), max_size_(max) {}

    ~ArrayStack(){
        delete[] contents;
    }

    void push(const T& data){
      
      if (full())
        throw std::out_of_range("Full stack!");
      
      top_ ++;
      contents[top_] = data;
      
    }

    T pop(){
     
      if (empty())
        throw std::out_of_range("Empty stack!");
     
      top_ --;
      return contents[top_+1];
      
    }

    T& top(){
      return contents[top_];
    }

    void clear(){
      top_ = -1;
    }

    std::size_t size(){
        return (top_+ 1);
    }

    std::size_t max_size(){
        return max_size_;
    }

    bool empty(){
      return top_ == -1;
    }

    bool full(){
      return top_ == max_size_ -1;
    }

private:
    T* contents;
    int top_;
    std::size_t max_size_;

    const static auto DEFAULT_SIZE = 10u;
};

} 
#endif