#ifndef STRUCTURES_ARRAY_QUEUE_H
#define STRUCTURES_ARRAY_QUEUE_H

#include <cstdint> // std::size_t
#include <stdexcept> // C++ Exceptions

namespace structures {

template<typename T>
class ArrayQueue {
public:
    ArrayQueue(): ArrayQueue(DEFAULT_SIZE) {}

    ArrayQueue(std::size_t max): contents(new T[max_size_]),size_(-1), max_size_(max) {}

    ~ArrayQueue(){
        delete[] contents;
    }

    void enqueue(const T& data){
      
      if (full())
        throw std::out_of_range("Full Queue!");
      
      size_ ++;
      contents[size_] = data;
      
    } 

    T dequeue(){
     
      if (empty())
        throw std::out_of_range("Empty Queue!");

      T data = contents[0];
      
      for (int i = 0; i < size_; i++){

          contents[i] = contents[i+1];

      }
      size_ --;
      return data;
      
    } 

    T& back(){

       if (empty())
        throw std::out_of_range("Empty Queue!");
      
      return contents[size_];
    }

    void clear(){
      size_ = -1;
    } 

    std::size_t size(){
        return (size_+ 1);
    } 
    std::size_t max_size(){
        return max_size_;
    }

    bool empty(){
      return size_ == -1;
    } 

    bool full(){
      return size_ == max_size_ -1;
    } 

private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;

    const static auto DEFAULT_SIZE = 10u;
};

}

#endif
