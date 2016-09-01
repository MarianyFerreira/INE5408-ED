#ifndef STRUCTURES_ARRAY_LIST_H
#define STRUCTURES_ARRAY_LIST_H

#include <cstdint>

namespace structures {
/**
 * @brief      Implementa uma estrutura de dados do tipo Lista,
 *             que é um conjunto de dados dispostos e/ou acessáveis
 *             em uma sequência determinada.
 * @tparam     T     Parâmetro genérico de Template
 */
template<typename T>
class ArrayList {
public:
//------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief      Construtor da classe Lista sem parâmetros.
 */
    ArrayList(): ArrayList(DEFAULT_SIZE){}
/**
 * @brief      Construtor da classe Lista com parâmetros.
 * @tparam     max_size     Parâmetro que recebe o tamanho da lista a ser criada
 */
    ArrayList(std::size_t max_size): ArrayList(new T [max_size_], size_ (0), max_size_ (max_size)){}
/**
 * @brief      Destrutor da classe Lista sem parâmetros.
 *             Usado para destruir em memoria tudo o que foi construido pela Lista
 */
    ~ArrayList(){
        delete [] contents;
    }
//------------------------------------------------------------------------------------------------------------------------------
/**
 * @brief      Metodo que limpa a lista.
 */
    void clear(){
        size_ = 0;
    }
/**
 * @brief      Metodo que inclui um dado no final da Lista.
 * @param[in]  data     Dado a ser inserido na Lista
 */
    void push_back(const T& data){
        insert(data, size_);
    }
/**
 * @brief      Metodo que inclui um dado no inicio da Lista.
 * @param[in]  data     Dado a ser inserido na Lista
 */
    void push_front(const T& data){
        insert(data, 0);
    }
/**
 * @brief      Metodo que inclui um dado em uma posição da Lista.
 * @param[in]  data     Dado a ser consultado na Lista
  * @param[in]  index   Local onde o dado deve ser inserido na Lista
 */
    void insert(const T& data, std::size_t index){
        if (full())
            throw std::out_of_range("Full ArrayList!");
        
        if (index > size_)
            throw std::out_of_range("Index not found!");
        
        int current = size_++;

        while (current > index){
            contents[current] = contents[current - 1];
            --current;
        }
        
        contents[index] = data;
    }
/**
 * @brief      Metodo que inclui um dado em ordem conforme os criterios
 *             definidos no metodo de comparação > que deve ser implementado
 *             na classe que usa o Array List caso os dados comparados não
 *             sejam primitivos.
 * @param[in]  data     Dado a ser consultado na Lista
 */
    void insert_sorted(const T& data){
        if (full())
            throw std::out_of_range("Full ArrayList!");
        
        while (index <= size + 1 && data > contents[index++]);

        insert(data, index);
    }
/**
 * @brief      Metodo que retira um dado na posição desejada da Lista.
 * @param[in]  index    Posição do dado a ser retirado da lista
 */
    T pop(std::size_t index){
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        if (index >= size_)
            throw std::out_of_range("Index not found!");
    
        int current = index;
        T data = contents[index];

        while(current < size_){

            contents[current] = contents[current+1];
            ++current;
        }
        --size_;
        return data;
    }
/**
 * @brief      Metodo que retira o ultimo elemento da Lista.
 */
    T pop_back(){
        return pop(size_);
    }
/**
 * @brief      Metodo que retira o primeiro elemento da Lista.
 */
    T pop_front(){
        return pop(0);
    }
/**
 * @brief      Metodo que retira o dado da Lista.
 * @param[in]  data   Dado a ser retirado da Lista.
 */
    void remove(const T& data){
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        std::size_t index = find(data);
        pop(index);
    }
/**
 * @brief      Metodo que verifica e retorna se a Lista está cheia.
 */
    bool full(){
        return size_ == max_size_ -1;
    }
/**
 * @brief      Metodo que verifica e retorna se a Lista está vazia.
 */
    bool empty(){
        return size_ == -1;
    }
/**
 * @brief      Metodo que verifica e retorna se a Lista contem determinado dado.
 * @param[in]  data     Dado a ser consultado na Lista
 */
    bool contains(const T& data){  
        bool contain = false;
        
        if (empty())
            return contain;
        
        for (int i = 0; i <= size_; i ++){
            if (contents[i] == data)
                contain = true;
        }
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
         throw std::out_of_range("Data not find");
    }
/**
 * @brief      Metodo que retorna o tamanho da lista.
 */
    std::size_t size(){
        return size_;
    }
/**
 * @brief      Metodo que retorna o tamanho máximo da lista.
 */
    std::size_t max_size(){
        return max_size_;
    }

/**
 * @brief      Retorna o dado no endereço
 * @param[in]  index indice do dado na lista
 */
    T& at(std::size_t index){
        
        return at(index);
    }
/**
 * @brief      Retorna o dado no endereço const
 * @param[in]  index indice do dado na lista const
 */
    const T& at(std::size_t index) const {
        if (empty())
            throw std::out_of_range("Empty ArrayList!");

        if (index > size_)
            throw std::out_of_range("Index not found!");
        
        return contents[index];
    }
/**
 * @brief      Retorna o dado no endereço
 * @param[in]  index indice do dado na lista
 */

    T& operator[](std::size_t index){
        return contents[index];
    }
/**
 * @brief      Retorna o dado no endereço const
 * @param[in]  index indice do dado na lista const
 */
    const T& operator[](std::size_t index) const {
        return contents[index];
    }

private:

    T* contents;            // Guarda um ponteiro para o inicio da lista de elementos do tipo T
    std::size_t size_;      // Guarda o final da lista
    std::size_t max_size_;  // Guarda o tamanho maximo que a lista pode suportar

    const static auto DEFAULT_SIZE = 10u; // Constante definida para um tamanho padrão de lista caso este não seja especificado
};

}

#endif