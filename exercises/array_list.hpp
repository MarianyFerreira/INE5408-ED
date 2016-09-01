#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <cstdint>
#include <stdexcept>

namespace structures {

template<typename T>
class ArrayList {
public:

    ArrayList(){
        max_size_ = DEFAULT_MAX;
        contents = new T [max_size_];
        size_ = 0;
    }

    ArrayList(std::size_t max_size){
        max_size_ = max_size;
        contents = new T [max_size_];
        size_ = 0;
    }

    ~ArrayList(){
        delete [] contents;
    }

    void clear(){
        size_ = 0;
    }

    void push_back(const T& data){

        insert(data, size_);
    }

    void push_front(const T& data){

        insert(data, 0);
    }
    
    void insert(const T& data, std::size_t index){
        
        if (full())
            throw std::out_of_range("Full");
        
        if (index > size_)
            throw std::out_of_range("Index");
        
        size_t current = size_++;

        while (current > index){
            contents[current] = contents[current - 1];
            --current;
        }
        
        contents[index] = data;        
    }

    void insert_sorted(const T& data){
        
        if (full())
            throw std::out_of_range("Full ArrayList!");
            
        std::size_t index = 0;
        
        while (index < size_ && data > contents[index]){
            ++index;
        }

        insert(data, index);
    }

    T pop(std::size_t index){
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        if (index > size_ -1)
            throw std::out_of_range("Index not found!");
    
        std::size_t current = index;
        T data = contents[index];
        --size_;
        while(current < size_){

            contents[current] = contents[current+1];
            ++current;
        }
        return data;
    }

    T pop_back(){
        return pop(size_ -1);
    }

    T pop_front(){
        return pop(0);
    }

    void remove(const T& data){

        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        std::size_t index = find(data);
        pop(index);
    }

    bool full() const{
        return size_ == max_size_;
    }

    bool empty() const{
        return size_ == 0;
    }
    bool contains(const T& data) const{
        bool contain = false;
        
        if (empty())
            return contain;
        
        for (int i = 0; i <= size_; i ++){
            if (contents[i] == data)
                contain = true;
        }
        return contain;
//         return std::find_if(contents, contents + max_size_, data) != contents + max_size_;

    }

    std::size_t find(const T& data) const{

        if(empty())
            throw std::out_of_range("Empty ArrayList!");

         for(int index = 0; index <= size_; index++){
            if (contents[index] == data)
                return index;
         }
         return size_;
    }

    std::size_t size() const{
        return size_;
    }

    std::size_t max_size() const{
        return max_size_;
    }

    T& at(std::size_t index){
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        if (index > size_)
            throw std::out_of_range("Index not found!");
        
        return contents[index];
    }

    T& operator[](std::size_t index){
        return contents[index];
    }

    const T& at(std::size_t index) const{
        
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        if (index > size_)
            throw std::out_of_range("Index not found!");
        
        return contents[index];
    }

    const T& operator[](std::size_t index) const{
        return contents[index];
    }

private:
    T* contents;
    std::size_t size_;
    std::size_t max_size_;

    const static auto DEFAULT_MAX = 10u;
};

}

#endif
