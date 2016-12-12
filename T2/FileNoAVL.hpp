#ifndef FILE_NO_AVL_HPP
#define FILE_NO_AVL_HPP
#include <vector>
#include <algorithm>
#include <stdio.h>
//-------------------------------------------------------------------------------------------------------
//!  Struct do elemento nó.
/*!  Implementação do nó AVL, que será inserido
 *   na árvore de índices em arquivo.
 */
typedef struct Node {
    char command[100]; /*!< Array de chars que guarda o nome do comando */
    int height,         /*!< Altura do nodo na árvore */
        right,          /*!< Ponteiro para o filho da direita na árvore */
        left,           /*!< Ponteiro para o filho da esquerda na árvore */
        data;           /*!< Ponteiro para o arquivo que possui as informações dessa ManPage */

//!  Funções de operadores
/*!  Responsável por fazer a comparação entre os comandos
 *   usado para saber por onde caminhar na árvore.
 *   strcmp é usado para fazer as comparações alfabeticas
 *  \param rhs nome do comando a ser comparado.
 */
    bool operator==(const Node& rhs) const {
        if (strcmp(command, rhs.command) == 0) {
            return true;
        }
        return false;
    }
    bool operator<(const Node& rhs) const {
        if (strcmp(command, rhs.command) < 0) {
            return true;
        }
        return false;
    }

    bool operator>(const Node& rhs) const {
        if (strcmp(command, rhs.command) > 0) {
            return true;
        }
        return false;
    }

} Node;
//-------------------------------------------------------------------------------------------------------
//!  Classe do elemento da estrutura de dado: Árvore AVL.
/*!  Implementação do nó AVL com especialização para operações em disco.
 */
class FileNoAVL  {
 private:
    FILE *indexDat;     /*!< Ponteiro para o arquivo que possui a árvore */
    Node node;          /*!< Struct Node que contém as informações do nó */
 public:
//! Contrutor com argumentos.
/*! Sempre que usado, este construtor criará um 
 *  nó AVL, que possuirá filhos nulos, e seu dado conforme
 *  valor passado por parâmetro, além de altura 1;
 *  \param data posição do comando no manpage.dat.
 */
    FileNoAVL(int data, char command[100]) {
        node.height = 1;                        /*! Altura iniciada sempre com 1 */
        node.right = 0;                         /*! Filho da direita iniciado como 0 = null */
        node.left = 0;                          /*! Filho da esquerda iniciado como 0 = null */
        node.data = data;                       /*! Ponteiro para o local do Nodo em disco */
        
        /*! Preenchimento do array com o nome do comando com ' ' pra evitar sujeiras*/
        for(int j = 0; j < 100; j++) {          
            node.command[j] = ' ';
        }
        /*! Preenchimento no array com o comando passado por parâmetro */
        strcpy(node.command, command);
        /*! Abertura do arquivo onde guardamos a estrutura da arvore em modo de update e leitura*/
        indexDat = fopen("index.dat", "r+");   
    }

//! Destrutor.
/*! Tem a função de destruir o nó AVL e fechar o arquivo da árvore.
 */
    ~FileNoAVL(){
        fclose(indexDat);
    }

//!  Função getHeight, informa a altura do nó.
/*   \return um inteiro que informa a altura do nó.
 */
    int getHeight() {
        return node.height;
    }

//!  Função getLeft, informa o ponteiro para o filho da esquerda.
/*   \return um ponteiro que informa a posição do filho esquerdo no arquivo.
 */
    int getLeft() {
        return node.left;
    }

//!  Função getRight, informa o ponteiro para o filho da direita.
/*   \return um ponteiro que informa a posição do filho direito no arquivo.
 */
    int getRight() {
        return node.right;
    }

//!  Função getNode, informa o nodo.
/*   \return uma struct Node que foi criada.
 */
    Node& getNode() {
        return node;
    }

//!  Função insert, insere um dado em uma árvore AVL.
/*   \param node o nó a ser inserido na árvore.
 *   \param root ponteiro para raiz da árvore.
 *   \return um ponteiro para o nó inserido.
 */
    int insert(Node &node, int root) {
        //! Se a raiz for a 0 vamos inserir o nodo e retorna a raiz do nodo no arquivo
        //! fseek vai posicionar o cabeçote no arquivo de index, onde guardo a construcao da arvore
        //! fwrite vai escrever as informações do node, com tamanho do node no arquivo indexDat
        if (root == 0) {
            // Posicionar cabeçote no final do arquivo
            fseek(indexDat, 0, SEEK_END);
            // Atualiza a raiz
            root = ftell(indexDat);
            // incluir o nodo no arquivo que guarda a estrutura da arvore
            // retorna a raiz
            fwrite(&node, sizeof(Node), 1, indexDat);
            return root;
        }
        // Posiciona o cabeçote na posicao root
        fseek(indexDat, root, SEEK_SET);
        // Lê e guarda as informacoes da raiz no nodeTmp pra usar depois no caminho e no balanceamento
        Node nodeTmp;
        fread(&nodeTmp, sizeof(Node), 1, indexDat);

    //////////////
    // CAMINHOS //
    //////////////
    //! Com esse if se faz a chamada recursiva para caminhar pela arvore usando os operadores de comparacao
    //! que foram definidos na Struct Node

        if (node < nodeTmp) {
            nodeTmp.left = insert(node, nodeTmp.left);
        } else {
            nodeTmp.right = insert(node, nodeTmp.right);
        }

    // Quando inserimos um novo filho é necessário, no regitro do Pai atualizar o ponteiro do filho que foi incluido.
        // Posicionar o cabeçote no arquivo indexDat conforme a raiz atual da recursão
        fseek(indexDat, root, SEEK_SET);
        // Atualiza no pai o filho que era nulo        
        fwrite(&nodeTmp, sizeof(Node), 1, indexDat);

    ///////////////////
    // BALANCEAMENTO //
    ///////////////////
        // Seta a altura do nodo pai
        nodeTmp.height = std::max( heightAVL(nodeTmp.left), 
                                   heightAVL(nodeTmp.right) ) +1;

        // Posiciona o cabeçote no arquivo Index onde o Nodo pai estava guardado
        fseek(indexDat, root, SEEK_SET);
        // Atualizando a altura do pai
        fwrite(&nodeTmp, sizeof(Node), 1, indexDat);

        // Verifica se o pai esta balanceado
        int balance = getBalance(nodeTmp);

        Node nodeTmpRight, nodeTmpLeft;

        // Se a altura do pai a esquerda for diferente de 0 então lemos as informações do pai e guardamos
        if(nodeTmp.left != 0){
            fseek(indexDat, nodeTmp.left, SEEK_SET);
            fread(&nodeTmpLeft, sizeof(Node), 1, indexDat);
        }
        // Se a altura do pai a direita for diferente de 0 então lemos as informações do pai e guardamos
        if(nodeTmp.right != 0){
            fseek(indexDat, nodeTmp.right, SEEK_SET);
            fread(&nodeTmpRight, sizeof(Node), 1, indexDat);
        }

        // Caso Esquerda-Esquerda
        if (balance > 1 && node < nodeTmpLeft) {
            return rightRotation(nodeTmp, root);
        }

        // Caso Esquerda-Direita
        if (balance > 1 && node > nodeTmpLeft) {
            nodeTmp.left = leftRotation(nodeTmpLeft, nodeTmp.left);
            fseek(indexDat, root, SEEK_SET);
            fwrite(&nodeTmp, sizeof(Node), 1, indexDat);
            return rightRotation(nodeTmp, root);
        }

        // Caso Direita-Direita
        if (balance < -1 && node > nodeTmpRight) {
            return leftRotation(nodeTmp, root);
        }

        // Caso Direita-Esquerda
        if (balance < -1 && node < nodeTmpRight) {
            nodeTmp.right = rightRotation(nodeTmpRight, nodeTmp.right);
            fseek(indexDat, root, SEEK_SET);
            fwrite(&nodeTmp, sizeof(Node), 1, indexDat);
            return leftRotation(nodeTmp, root);
        }
        return root;
    }

//!  Função getData, informa o ponteiro para o dado do nó.
/*!  
 *   \return um ponteiro que informa a posição do dado no arquivo.
 *   \sa FileNoAVL(int data, char command[100])
 */
    int getData() {
        return node.data;
    }

//!  Função search, busca por um dado em uma árvore AVL.
/*!  
 *   \param node nó a ser buscado na árvore.
 *   \return um ponteiro para o dado.
 */
    int search(Node node) {
        Node temp;
        int root;
        // Posicionamento do cabeçote no inicio do arquivo IndexDat
        fseek(indexDat, 0, SEEK_SET);
        // Atribui a raiz o nodo de raiz absoluta da árvore
        fread(&root, sizeof(int), 1, indexDat);
        // Posicionamento do cabeçote no nodo raiz da árvore
        fseek(indexDat, root, SEEK_SET);
        // Lê o Nodo corespondente a raiz guardada e gurda o nodo no temp
        fread(&temp, sizeof(Node), 1, indexDat);

        // Enquanto a raiz for diferente de 0 e o nodo pesquisado for diferente do temporário
        while (root != 0 && !(node == temp)) {
            // Se o nodo procurado for menor que o temp
            if (node < temp) {
                // Posiciona o cabeçote no filho da esqueda
                fseek(indexDat, temp.left, SEEK_SET);
                // Atualiza a raiz
                root = temp.left;
                // Lê o Nodo corespondente a raiz guardada e gurda o nodo no temp
                fread(&temp, sizeof(Node), 1, indexDat);
            } else {
                // Posiciona o cabeçote no filho da direita
                fseek(indexDat, temp.right, SEEK_SET);
                // Atualiza a raiz
                root = temp.right;
                // Lê o Nodo corespondente a raiz guardada e gurda o nodo no temp
                fread(&temp, sizeof(Node), 1, indexDat);
            }
        }
        // Retorno caso o Nodo procurado não exista na Árvore
        if (root == 0) {
            return -1;
        } else {
        // Retorno do ponteiro para onde as informações do Nodo estão salvas (Arquivo ManPages.dat)
            return temp.data;
        }
    }

//!  Função heightAVL, informa altura de um nó.
/*!  
 *   \param node posição do nó no arquivo
 *   \return o valo da altura do nó.
 *   \sa FileNoAVL(int data, char command[100])
 */
    int heightAVL(int node) {
        if (node == 0) {
            return 0;
        } else {
            fseek(indexDat, node, SEEK_SET);
            Node nodeTmp;
            fread(&nodeTmp, sizeof(Node), 1, indexDat);
            return nodeTmp.height;
        }
    }
/*
                y        Right Rotation           x
               / \    (Rotação a direita)        / \
              x   T3    – – – – – – – >        T1   y
             / \        < - - - - - - -            / \
            T1  T2         Left Rotation        T2  T3
                        (Rotação a esquerda)
*/
//!  Função rightRotation, realiza uma rotação simples para direita.
/*!  
 *   \param y o nó raiz da rotação.
 *   \param root a posição do nó no arquivo.
 *   \return um ponteiro para o nó raiz ao final da rotação.
 *   \sa FileNoAVL(int data, char command[100])
 */
    int rightRotation(Node y, int root) {
        Node x; 

        fseek(indexDat, y.left, SEEK_SET);
        fread(&x, sizeof(Node), 1, indexDat);

        int rootNew = y.left;
        int aux = x.right;

        x.right = root;
        fseek(indexDat, y.left, SEEK_SET);
        fwrite(&x, sizeof(Node), 1, indexDat);

        y.left = aux;
        fseek(indexDat, root, SEEK_SET);
        fwrite(&y, sizeof(Node), 1, indexDat);
        
        //  Atualiza altura
        y.height = std::max(heightAVL(y.left), heightAVL(y.right))+1;
        fseek(indexDat, root, SEEK_SET);
        fwrite(&y, sizeof(Node), 1, indexDat);

        x.height = std::max(heightAVL(x.left), heightAVL(x.right))+1;
        fseek(indexDat, rootNew, SEEK_SET);
        fwrite(&x, sizeof(Node), 1, indexDat);

        // Retorna nova raiz
        return rootNew;
    }

//!  Função leftRotation, realiza uma rotação simples para esquerda.
/*!  
 *   \param x o nó raiz da rotação.
 *   \param root a posição do nó no arquivo.
 *   \return um ponteiro para o nó raiz ao final da rotação.
 *   \sa FileNoAVL(int data, char command[100])
 */
    int leftRotation(Node x, int root) {
        Node y; 

        fseek(indexDat, x.right, SEEK_SET);
        fread(&y, sizeof(Node), 1, indexDat);

        int rootNew = x.right;
        int aux = y.left;

        y.left = root;
        fseek(indexDat, x.right, SEEK_SET);
        fwrite(&y, sizeof(Node), 1, indexDat);
        
        x.right = aux;
        fseek(indexDat, root, SEEK_SET);
        fwrite(&x, sizeof(Node), 1, indexDat);


        //  Atualiza altura
        x.height = std::max(heightAVL(x.left), heightAVL(x.right))+1;
        fseek(indexDat, root, SEEK_SET);
        fwrite(&x, sizeof(Node), 1, indexDat);

        y.height = std::max(heightAVL(y.left), heightAVL(y.right))+1;
        fseek(indexDat, rootNew, SEEK_SET);
        fwrite(&y, sizeof(Node), 1, indexDat);
        // Retorna nova raíz
        return rootNew;
    }

//!  Função getBalance, calcula a diferença das alturas dos nós filhos.
/*!  
 *   \param nodo o nó pai.
 *   \return um inteiro que informa a diferença das alturas dos nós filhos.
 *   \sa FileNoAVL(int data, char command[100])
 */
    int getBalance(Node &node) {
        return heightAVL(node.left) - heightAVL(node.right);
    }

};

#endif /* FILE_NO_AVL_HPP */
