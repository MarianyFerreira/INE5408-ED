#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "FileNoAVL.hpp"

static const int commandSize = 100;
static const int contentSize = 139900;

//!  Struct do elemento comando com manpage.
/*!  Struct que possui o nome do comando e
 *   conteúdo da manpage do comando.
 */
typedef struct ManPage {
	char command[commandSize];
	char content[contentSize];
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
	printf("\nInforme o nome do comando a ser procurado:\n");
	char command[100] = "ManPages/";
	for(int i=9; i<commandSize; i++) {
		 command[i] = ' ';
	}
	scanf("%s", &command[9]);

	FileNoAVL *avlNode = new FileNoAVL(0, command);
	int commandAddress = avlNode->search(avlNode->getNode());
	if (commandAddress == -1) {
		printf("Comando inexistente\n");
		return;
	}
	ManPage manPage;
	FILE* manPageDat = fopen("manpage.dat", "r");
	fseek(manPageDat, commandAddress, SEEK_SET);
	fread(&manPage, sizeof(ManPage), 1, manPageDat);
	printf("COMANDO: %s\n", manPage.command);
	printf("CONTEÚDO: %s\n", manPage.content);
	delete avlNode;
	fclose(manPageDat);
}

int main (int argc, char* argv[]) {
	// INICIA O MENU DO PROGRAMA
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