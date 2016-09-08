#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>


namespace structures {

template<typename T>
class LinkedList {
public:
    LinkedList() {

    data_ = null;
    size_ = 0;
    
    } // construtor padrão

    ~LinkedList() {
        clear();
    } // destrutor

    void clear() {
        Node *current, *previous;

        if (empty())
            throw std::out_of_range("Empty Linked List!");

        return size_ == 0;
    } // limpar lista

    void push_back(const T& data) {

    } // inserir no fim

    void push_front(const T& data) {

		*aux = new Node(data);

        if (aux = null)
            throw std::out_of_range("Full Linked List!");

        //novo ->_proximo <- _dados;
		//novo ->_info <- dado;
		//_dados <- novo;
		++size_;        
    } // inserir no início

    void insert(const T& data, std::size_t index) {

   		if (index > size())
   			throw std::out_of_range("Index not found!");

        if (index = 1)
        	return push_front(data);

        *aux = new Node(data);

        if (aux = null)
            throw std::out_of_range("Full Linked List!");
        
        *previous = new Node(data_);

        for (int i = 0; i < index - 2; i++){

        	//anterior <- anterior ->_proximo;
        }

        //novo ->_proximo <- anterior ->_proximo;
		//novo ->_info <- info;
		//anterior ->_proximo <- novo;
		++size_;                              // tamanho = tamanho + 1;
    }  // inserir na posição

    void insert_sorted(const T& data) {

        if (empty())
            return pop_front(data);

        *aux = new Node[data_];
        int index = 1;

        // ENQUANTO (atual ->_proximo ~= NULO E
            // dado > atual ->_info)) FACA
            // atual <- atual ->_proximo;
            ++index;                        // posicao <- posicao +1;

        if (data > aux[])
            return insert(data, index +1);
        
        return insert(data, index);

    } // inserir em ordem

    T& at(std::size_t index) {

    } // acessar um elemento na posição index

    T pop(std::size_t index) {

        if (index > size())
            throw std::out_of_range("Index not found!");

        if (index = 1)
            return push_front(data);

        previous = new Node[data];
        del = new Node[nullptr];

        for (int i = 0; i < index - 2){
            // anterior <- anterior ->_proximo;
        }
        // eliminar <- anterior ->_proximo;
        // volta <- eliminar ->_info;
        // anterior ->_proximo <- eliminar ->_proximo;
        --size; // _tamanho <- _tamanho - 1;
        // LIBERE(eliminar);
        // RETORNE(volta);


    } // retirar da posição

    T pop_back() {

    } // retirar do fim

    T pop_front() {

    	if(empty())
    		throw std::out_of_range("Empty Linked List!");

    	*out = new Node[data_];       // saiu <- _dados;
    	*back = new T out[data_];		   // volta <- saiu ->info;
    	data_ = out[next_];		           // _dados <- saiu ->próximo;

    	++size_;

    	// libere(saiu)

    	return back;
    } // retirar do início

    void remove(const T& data) {

    } // remover específico

    bool empty() const {

        return size_ == 0;

    } // lista vazia

    bool contains(const T& data) const {

    } // contém

    std::size_t find(const T& data) const {

    } // posição do dado

    std::size_t size() const {

    } // tamanho da lista

// Linked List tem:
//   Node* head{nullptr};       Elemento *_dados;
//  std::size_t size_{0u};      inteiro _tamanho;

// Node tem: 
//  T data_;                    T *_info;
//  Node* next_{nullptr};       Elemento *_proximo;

private:
    class Node { // Elemento
    public:
        Node(const T& data):
            data_{data}
        {}

        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() { // getter: dado
            return data_;
        }

        const T& data() const { // getter const: dado
            return data_;
        }

        Node* next() { // getter: próximo
            return next_;
        }

        const Node* next() const { // getter const: próximo
            return next_;
        }

        void next(Node* node) { // setter: próximo
            next_ = node;
        }

    private:
        T data_;
        Node* next_{nullptr};
    };

    Node* end() { // último nodo da lista
        auto it = head;
        for (auto i = 1u; i < size(); ++i) {
            it = it->next();
        }
        return it;
    }

    Node* head{nullptr};
    std::size_t size_{0u};
};

}

#endif

