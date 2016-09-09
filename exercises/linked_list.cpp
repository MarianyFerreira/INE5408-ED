#ifndef STRUCTURES_LINKED_LIST_H
#define STRUCTURES_LINKED_LIST_H

#include <cstdint>
#include <stdexcept> 


namespace structures {

template<typename T>

class LinkedList {
public:

    LinkedList() = default;

    ~LinkedList() {
        clear();
    } // destrutor

    void clear() {
        
        Node *current, *previous;
        
        // if (empty())
        //    throw std::out_of_range("Empty Linked List!");
        
        current = head;
        
        while (current != nullptr){
            previous = current;
            current = current->next();
            delete previous;
        }
        
        size_ = 0;
        head = nullptr;
        
    } // limpar lista

    void push_back(const T& data) {
        
        if (empty()) {
            push_front(data);
            return;
        }
        
        insert(data, size_);
        
    } // inserir no fim

    void push_front(const T& data) {

		Node *aux = new Node(data); // PROXIMO <- _DADOS

        if (aux == nullptr)
            throw std::out_of_range("Full Linked List!");
        
        aux->next(head); // novo ->_proximo
        head = aux;     // proximo <- novo
		++size_;
		
    } // inserir no início

    void insert(const T& data, std::size_t index) {

   		if (index > size())
   			throw std::out_of_range("Index not found!");

        if (index == 0)
        	return push_front(data);

        Node *aux = new Node(data); //novo ->_info <- info;

        if (aux == nullptr)
            throw std::out_of_range("Full Linked List!");
        
        Node *previous;
        
        previous = head;

        for (int i = 0; i < index -1; i++){
            
        	previous = previous->next(); //anterior <- anterior ->_proximo;
            
        }

        aux->next(previous->next());    //novo ->_proximo <- anterior->_proximo;
		previous->next(aux);            //anterior ->_proximo <- novo;
		++size_;                        // tamanho = tamanho + 1;
    }  // inserir na posição

    void insert_sorted(const T& data) {

        if (empty()) {
            push_front(data);
            return;
        }
        
        auto current = head; //atual <- _dados;
        int index = 0;

        while (current->next() != nullptr and data > current->data()){
        // ENQUANTO (atual ->_proximo ~= NULO E dado > atual ->_info)) FACA
        
            current = current->next();           // atual <- atual ->_proximo;
            ++index;                        // posicao <- posicao +1;
        }
        
        if (data > current->data()){
            insert(data, index +1);
            return;
        }
        
        insert(data, index);

    } // inserir em ordem

    T& at(std::size_t index) {
        
        if (empty())
            throw std::out_of_range("Empty Linked List!");
        if (index > size_)
            throw std::out_of_range("Index not found!");
        
        auto previous = head;
         for(int i = 0; i < index; ++i){
             previous = previous->next();
         }
         
         return previous->data();
    } // acessar um elemento na posição index

    T pop(std::size_t index) {

        if (index >= size())
            throw std::out_of_range("Index not found!");

        if (index == 0)
            return pop_front();

        auto previous = head;        

        for (int i = 0; i < index -1; i++){
             previous = previous->next(); // anterior <- anterior ->_proximo;
        }
        
        auto del = previous->next();    // eliminar <- anterior ->_proximo;
        auto back = del->data();        // volta <- eliminar ->_info;
        previous->next(del->next());   // anterior ->_proximo <- eliminar ->_proximo;
        --size_;                        // _tamanho <- _tamanho - 1;
        
        delete del;                     // LIBERE(eliminar);
        return back;                    // RETORNE(volta);

    } // retirar da posição

    T pop_back() {
        return pop(size_ -1);
    } // retirar do fim

    T pop_front() {

    	if(empty())
    		throw std::out_of_range("Empty Linked List!");

    	Node *out = head;                // saiu <- _dados;
    	auto back = out->data();		 // volta <- saiu ->info;
    	head = out->next();		         // _dados <- saiu ->próximo;

    	--size_;
        delete out;
        
    	return back;
    } // retirar do início

    void remove(const T& data) {
        pop(find(data));
    } // remover específico

    bool empty() const {

        return size_ == 0;

    } // lista vazia

    bool contains(const T& data) const {
        auto previous = head;
        
        for(int i = 0; i < size_; ++i){
            if(previous->data() == data)
                return true;
            previous = previous->next();
        }
        return false;
    } // contém

    std::size_t find(const T& data) const {
        
        auto previous = head;
        
        for(int i = 0; i < size_; ++i){
            if(previous->data() == data)
                return i;
            previous = previous->next();
        }
        
            return size_;
    } // posição do dado

    std::size_t size() const {
        return size_;
    } // tamanho da lista

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
