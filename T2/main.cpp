#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "FileNoAVL.hpp"

static const int commandSize = 100;		/*! Tamanho máximo do nome de um comando*/
static const int contentSize = 139900;	/*! Tamanho máximo da explicação de um comando*/

//!  Struct do elemento comando com manpage.
/*!  Struct que possui o nome do comando e
 *   conteúdo da manpage do comando.
 */
typedef struct ManPage {
	char command[commandSize];		/*! Inicialização dos arrays de char com o tamanho maximo possivel para o nome de um comando */
	char content[contentSize];		/*! Inicialização dos arrays de char com o tamanho maximo possivel para a descrição de um comando */
} ManPage;

//! Função createDatFiles.
/*! Responsável por gerar os arquivos (manpage/index/inverted).dat
 *  \param argc número de arquivos.txt que serão lidos
 *  \param argv array com os nomes dos arquivos.txt a serem lidos
 */
void createDatFiles(int argc, char *argv[]) {
	// Cria o arquivo manpage.dat (guarda as manpages)
	FILE* manPageDat = fopen("manpage.dat", "w");

	// Cria o arquivo index.dat (guarda a arvore de chave primaria)
	FILE* indexDat = fopen("index.dat", "w+");
	int indexRoot = 0;
	fwrite(&indexRoot, sizeof(int), 1, indexDat);
	printf("\nGerando MANPAGE.DAT E INDEX.DAT\n");

	for(int i = 1; i < argc; i++) {

	//MANPAGE.DAT//
		printf("%d - %s\n", i, &argv[i][9]);
		// Abre manpage.txt
		FILE* manPageTxt = fopen(argv[i], "r");
		// Verifica se ocorreu erro ao abrir o arquivo
		if(manPageTxt == NULL) {
			perror ("Erro ao abrir manpage.txt");
		}

		ManPage manPage;

		// Preenche o comando com espaço em branco
		for(int j = 0; j < commandSize; j++) {
			manPage.command[j] = ' ';
		}

		// Altera a ultima ocorrencia do '.' para o char de término '\0'
		*strrchr(argv[i], '.') = '\0';

		// Copia o nome do comando
		strcpy(manPage.command, argv[i]);

		// Preenche o conteudo com espaços em branco
		// Por motivos de bugs...
		for(int j = 0; j < contentSize; j++) {
			manPage.content[j] = ' ';
		}

		// Copia o conteudo do txt e as palavras aceitas
		int k;
		for(k = 0; !feof(manPageTxt); k++) {
			char c = fgetc(manPageTxt);
			manPage.content[k] = c;
		}

		// Seta o char de término do conteúdo
		manPage.content[k-1] = '\0';
		// Fecha o arquivo txt
		fclose(manPageTxt);
		// Escreve em manpages.dat
		// ftell retorna a posicao final do registro pra uso posterior
		int manpagePosition = ftell(manPageDat);
		fwrite(&manPage, sizeof(ManPage), 1, manPageDat);

	//INDEX.DAT//
		// Cria um nó com o comando atual
		// Quando a arvore é criada o dado guardado é p ftell do arquivo guardado
		FileNoAVL *avlNode = new FileNoAVL(manpagePosition, manPage.command);
		rewind(indexDat);
		// Lê a raiz
		fread(&indexRoot, sizeof(int), 1, indexDat);
		// Insere o nó na árvore
		indexRoot = avlNode->insert(avlNode->getNode(), indexRoot);
		rewind(indexDat);
		// Atualiza a raiz
		fwrite(&indexRoot, sizeof(int), 1, indexDat);
		delete avlNode;
	}
	fclose(manPageDat);
	fclose(indexDat);
	printf("Arquivos gerados com sucesso\n");
}

//! Função searchByCommand.
/*! Realiza a busca da manpage de um comando, pela sua chave primária.
 */
void searchByCommand(){
	// Pergunta para o usuário o comando a ser pesquisado
	printf("\nInforme o nome do comando a ser procurado:\n");
	// Guarda em command o caminho da pasta
	char command[100] = "ManPages/";
	// Preenche o restante do array com ' ' pra evitar sujeiras
	for(int i=9; i<commandSize; i++) {
		 command[i] = ' ';
	}
	// Esqueve após ManPages/ o nome do arquivo a ser procurado
	scanf("%s", &command[9]);

	// Cria um Nodo para utilizar na pesquisa
	FileNoAVL *avlNode = new FileNoAVL(0, command);
	// Chama a função search da arvore AVL e guarda o retorno na variavel commandAdress
	int commandAddress = avlNode->search(avlNode->getNode());
	// Se o retorno da função search for -1 o comando não existe na Árvore
	if (commandAddress == -1) {
		printf("Comando inexistente\n");
		return;
	}
	// Se o comando existir é necessário abrir o ManPages.dat e posicionar o cabeçote no
	// endereço retornado pela função search e depois printá-lo na Tela.
	ManPage manPage;
	FILE* manPageDat = fopen("manpage.dat", "r");
	
	// Posicionamento do cabeçote na posição de retorno do search, onde está as informações da Manpage
	fseek(manPageDat, commandAddress, SEEK_SET);
	// Leitura e armazenamento das informações da ManPage encontrada
	fread(&manPage, sizeof(ManPage), 1, manPageDat);
	// Print das informações
	printf("COMANDO: %s\n", manPage.command);
	printf("CONTEÚDO: %s\n", manPage.content);
	// Deleção do nodo de pesquisa
	delete avlNode;
	// Fechamento do arquivo ManPages.dat
	fclose(manPageDat);
}

int main (int argc, char* argv[]) {
	// MENU DO PROGRAMA
	int input;
	system("clear");
	do{
		printf("\n");
		printf("------------------------>> Escolha a Opção: << ------------------------\n");
		printf("[1] - Gerar ManPage.dat\n");
		printf("[2] - Buscar conteudo por nome do comando\n");
		printf("\n");
		printf("[0] - Para sair\n");
		printf("\n");
		printf("\n");
		scanf("%d",&input);
		switch (input) {
			case 0:
				exit(0);
			case 1:
				createDatFiles(argc, argv);
				break;
			case 2:
				searchByCommand();
				break;
			default: printf("Opção inválida\n");
		}
	} while(input != 0);
	return 0;
}