/*                                                                                                                                                                                                        Cada unidade em um livro-texto em inglês contém um vocabulário Inglês-Português de  
Cada unidade em um livro-texto em inglês contém um vocabulário Inglês-Português de palavras que são usadas pela primeira vez em uma unidade em particular. 
Desejamos converter um conjunto de tais vocabulários armazenados em um arquivo Inglês em um conjunto de vocabulários Português-Inglês.  
Para isso considere que: 
(a) Os nomes das unidades são precedidos por um símbolo de porcentagem. 
(b) Há somente uma entrada por linha. 
(c) Uma palavra em Inglês é separada por dois pontos de sua(s) equivalente(s) portuguesa(s); se há mais de uma equivalente, elas são separadas por vírgula. 
Exemplo: 
 % Unidade 1 
 Bus: ônibus, barramento; 
 Bug: inseto, problema; 
 Bike: bicicleta; 
 Coller: ventilador; 
 Network: rede de computadores; 
 System: sistema; 
 % Unidade 2 
 Bug: inseto; 
 Bicycle: bicicleta; 
 Fan: ventilador; 
 Network: rede de relacionamento; 
 
Para produzir as palavras em ordem alfabética, crie uma árvore vermelha-preta contendo todas as palavras em português de todas as unidades e 
uma árvore binária de busca com as equivalentes inglesas e em cada nó da lista encadeada contenha também a unidade que pertence aquela palavra em inglês. 
Assegure-se de que exista apenas um nó para cada palavra portuguesa na árvore.  
Além disso, permita ao usuário:  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

#define TAM 100
#define RED 1
#define BLACK 0


// Nó para representar uma unidade em uma lista encadeada
typedef struct NoUnidade {
    char unidade[TAM];                 // Nome da unidade
    struct NoUnidade* prox;            // Ponteiro para a próxima unidade
} NoUnidade;

// Nó para palavras em inglês (ABB), com lista de unidades associadas
typedef struct NoPalavrasUnidadesInglesABB {
    char palavra[TAM];                 // Palavra em inglês
    NoUnidade* unidades;               // Lista encadeada de unidades
    struct NoPalavrasUnidadesInglesABB *esq, *dir; // Filhos da ABB
} NoPalavrasUnidadesInglesABB;

// Nó para palavras em português (ARB), com ABB associada
typedef struct NoPalavrasPortuguesARB {
    char palavra[TAM];                 // Palavra em português
    int cor;                           // Cor do nó (RED/BLACK)
    struct NoPalavrasPortuguesARB *esq, *dir; // Filhos da ARB
    NoPalavrasUnidadesInglesABB* arvIngles;   // ABB de palavras em inglês associadas
} NoPalavrasPortuguesARB;


                                                                   //BLOCO LISTA ENCADEADA PARA UNIDADES

//Função para criar uma nova unidade na lista encadeada																    
NoUnidade* criaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* nova = (NoUnidade*)malloc(sizeof(NoUnidade));
    if (!nova) {
        printf("Erro ao alocar memoria para unidade!\n");
        exit(1);
    }
    strcpy(nova->unidade, unidade);
    nova->prox = lista;
    return nova; // Retorna o início atualizado da lista
}

//Função para inserir uma nova unidade na lista encadeada
NoUnidade* insereUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = lista;
    int existe = 0; // Indicador para verificar se a unidade já existe

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            existe = 1; // Unidade já existe
        }
        atual = atual->prox; // Avança na lista
    }

    if (!existe) {
        lista = criaUnidade(lista, unidade); // Chama a função para criar uma nova unidade
    }

    return lista; // Retorna a lista (modificada ou não)
}

//Função para buscar uma unidade na lista encadeada
NoUnidade* buscaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* resultado = NULL; // Inicializa o ponteiro para o resultado como NULL
    NoUnidade* atual = lista;    // Ponteiro para percorrer a lista

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            resultado = atual;   // Encontra a unidade e atualiza o resultado
        }
        atual = atual->prox;     // Avança para o próximo nó
    }
    return resultado;            // Retorna o resultado (NULL se não encontrado)
}

//Função para remover uma unidade na lista encadeada
NoUnidade* removeUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = buscaUnidade(lista, unidade); // Localiza o nó a ser removido
    NoUnidade* anterior = NULL;
    NoUnidade* novaLista = lista; // Ponteiro para a nova lista (possivelmente modificada)

    if (atual != NULL) {
        if (lista == atual) {
            // O nó a ser removido é o primeiro da lista
            novaLista = atual->prox;
        } else {
            // Localiza o nó anterior ao nó atual
            anterior = lista;
            while (anterior != NULL && anterior->prox != atual) {
                anterior = anterior->prox;
            }
            if (anterior != NULL) {
                anterior->prox = atual->prox;
            }
        }
        free(atual); // Libera o nó encontrado
    }

    return novaLista; // Retorna a lista (modificada ou não)
}

//Função para imprimir as unidades na lista encadeada
void imprimeUnidades(NoUnidade* lista) {
    NoUnidade* atual = lista;
    int vazio = 1; // Indicador para verificar se a lista está vazia

    while (atual != NULL) {
        printf("%s ", atual->unidade);
        atual = atual->prox; // Avança na lista
        vazio = 0; // A lista não está vazia
    }

    if (vazio) {
        printf("Lista de unidades vazia.");
    }

    printf("\n"); // Quebra de linha após imprimir a lista
}

//Função para Liberar a Lista de Unidades
void liberarListaUnidades(NoUnidade* lista) {
    NoUnidade* atual = lista;
    while (atual != NULL) {
        NoUnidade* temp = atual;
        atual = atual->prox;
        free(temp);
    }
}



                                                       ////BLOCO ÁRVORE RUBRO NEGRA - ARB(palavras portugues)

//FUNÇÕES AUXILIARES                                                                    
//acessando a cor de um nó
int cor(NoPalavrasPortuguesARB* raiz){
	if (raiz == NULL)
		return BLACK;
	else
		return raiz->cor;
} 
                                                                    

//inverter a cor do pai e de seus filhos é uma operação administrativa: não altera a estrutura ou conteúdo da árvore
//MUDANDO A COR DE UM NÓ E DE SEUS FILHOS
void trocarCor(NoPalavrasPortuguesARB* raiz){
	raiz->cor = !raiz->cor;
	if (raiz->esq != NULL)
		raiz->esq->cor = !raiz->esq->cor;
	if (raiz->dir != NULL)
		raiz->dir->cor = !raiz->dir->cor;
}
/*
Rotação da árvore rubro negra -> caida para a esquerda
operações básicas para balanceamento (rotação á esquerda e rotação à direita)
exemplo: recebe um nó A, tendo B com filho a direita. 
Move B para o lugar de A, A se se torna o filho esquerdo de B. 
B recebe a cor de A, A fica vermelho.
*/
NoPalavrasPortuguesARB* rotacionaEsquerda(NoPalavrasPortuguesARB* A){
	NoPalavrasPortuguesARB* B = A->dir;
	A->dir = B->esq;
	B->esq = A;
	B->cor = A->cor;
	A->cor = RED;
	return B;
}

/*
Rotação à direita:
-recebe um nó A com B como filho esquerdo
-move B para o lugar de A, A se torna o filho direito de B
-B recebe a cor de A, A fica vermelho.
*/
NoPalavrasPortuguesARB* rotacionaDireita(NoPalavrasPortuguesARB* A){
	NoPalavrasPortuguesARB* B = A->esq;
	A->esq = B->dir;
	B->dir = A;
	B->cor = A->cor;
	A->cor = RED;
	return B;
}

/*
ARRUMANDO O BALANCEAMENTO DA RUBRO-NEGRA:
-se o filho direito é vermelho: rotação à esquerda
-se o filho direito e o neto da esquerda são vermelhos: rotação à direita
-se ambos os filhos são vermelhos: troca a cor do pai e dos filhos
*/

NoPalavrasPortuguesARB* balancear(NoPalavrasPortuguesARB* raiz){
	//nó vermelho é sempre filho à esquerda
	if (cor(raiz->dir) == RED)
		raiz = rotacionaEsquerda(raiz);
	//filho da direita e neto da esquerda são vermelho
	if (raiz->esq != NULL && cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED)
		raiz = rotacionaDireita(raiz);
	//2 filhos vermelhos: troca cor
	if (cor(raiz->esq) == RED && cor(raiz->dir) == RED)
		trocarCor(raiz);
	
	return raiz;
}
                                                                   
//Função p/ criar um nó com palavra em português                                                                    
NoPalavrasPortuguesARB* criaNoPortugues(char* palavra) {
    NoPalavrasPortuguesARB* novo = (NoPalavrasPortuguesARB*)malloc(sizeof(NoPalavrasPortuguesARB));
    if (novo == NULL) {
        printf("Erro de alocação de memoria!\n");
        exit(1); // Se o malloc falhar, o programa será encerrado
    }
    strcpy(novo->palavra, palavra); // Copia a palavra para o campo 'palavra'
    novo->cor = RED; // Define a cor inicial como vermelho
    novo->esq = novo->dir = NULL; // Inicializa os filhos como NULL
    novo->arvIngles = NULL; // Inicializa a árvore de palavras em inglês como NULL
    return novo;
}

//
NoPalavrasPortuguesARB* insereRubroNegra(NoPalavrasPortuguesARB* raiz, char* palavra, int* resp) {
    if (raiz == NULL) {
        *resp = 1; // Palavra inserida com sucesso
        return criaNoPortugues(palavra); // Cria um novo nó e o retorna
    }

    if (strcmp(palavra, raiz->palavra) == 0) {
        *resp = 0; // Palavra duplicada
        return raiz;
    }

    if (strcmp(palavra, raiz->palavra) < 0) { // Se a palavra é menor, insere à esquerda
        raiz->esq = insereRubroNegra(raiz->esq, palavra, resp);
    } else { // Se a palavra é maior, insere à direita
        raiz->dir = insereRubroNegra(raiz->dir, palavra, resp);
    }

    // **Regras de balanceamento da árvore rubro-negra**
    if (cor(raiz->dir) == RED && cor(raiz->esq) == BLACK) // Caso 1: filho direito vermelho e esquerdo preto
        raiz = rotacionaEsquerda(raiz);
    if (cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED) // Caso 2: dois filhos vermelhos consecutivos à esquerda
        raiz = rotacionaDireita(raiz);
    if (cor(raiz->esq) == RED && cor(raiz->dir) == RED) // Caso 3: dois filhos vermelhos
        trocarCor(raiz);

    return raiz; // Retorna o nó balanceado
}


int insere_ArvoreRubroNegra(NoPalavrasPortuguesARB** raiz, char* palavra) {
    int resp;
    *raiz = insereRubroNegra(*raiz, palavra, &resp); // Chama a função recursiva para inserir
    if (*raiz != NULL)
        (*raiz)->cor = BLACK; // Após a inserção, a raiz da árvore rubro-negra deve ser preta
    return resp;
}


//FUNÇÕES AUXILIARES P/ REMOÇÃO

/*
Arvore rubro negra - caida para a esquerda
-movendo os nós vermelhos:
-rotação e trocas de cores podem causar uma violação das propriedades da árvore
exemplo: a função trocaCor pode introduzir sucessivos nós vermelhos à direita, o que viola uma das propriedades da árvore.

-além das rotações, precisa de mais 3 funções para restabelecer o balanceamento e garantir que as propriedades da árvore seja respeitadas
-mover um nó vermelho para a esquerda
- mover um nó vermelho para a direita
-arrumar o balanceamento
essas funções movimentam um nó vermelho para a sub-árvore esquerda ou direita.

exemplo: 
-recebe um nó H e troca as cores dele e de seus filhos
-se o filho à esquerda do filho direito é vermelho, aplicar uma rotação à direita no filho direito e uma rotação esquerda no pai
-troca as cores do nó pai e de seus filhos
*/

int consulta_ArvoreRubroNegra(NoPalavrasPortuguesARB* raiz, char* palavra) {
    NoPalavrasPortuguesARB* atual = raiz;  // Ponteiro para percorrer a árvore
    while (atual != NULL) {
        if (strcmp(palavra, atual->palavra) == 0) // Verifica se encontrou a palavra
            return 1;  // Encontrado
        if (strcmp(palavra, atual->palavra) < 0) // Se palavra é menor, vai para a esquerda
            atual = atual->esq;
        else // Se palavra é maior, vai para a direita
            atual = atual->dir;
    }
    return 0;  // Não encontrado
}



//Movendo um nó vermelho para a esquerda
NoPalavrasPortuguesARB* move2EsqVermelho(NoPalavrasPortuguesARB* raiz){
	trocarCor(raiz);
	if (raiz->dir != NULL && cor(raiz->dir->esq) == RED){
		raiz->dir = rotacionaDireita(raiz->dir);
		raiz = rotacionaEsquerda(raiz);
		trocarCor(raiz);
	}
	return raiz;
}

/*
Mover um nó vermelho para a direita:
-recebe um nó H e troca as cores dele e de seus filhos
-se o filho a esquerda do filho esquerdo é vermelho, aplica-se uma rotação à direita no pai
-troca as cores do nó pai e de seus filhos 
*/
NoPalavrasPortuguesARB* move2DirVermelho(NoPalavrasPortuguesARB* raiz){
	trocarCor(raiz);
	if (raiz->esq != NULL && cor(raiz->esq->esq) == RED){
		raiz = rotacionaDireita(raiz);
		trocarCor(raiz);
	}
	return raiz;
}


//PROCURANDO E REMOVENDO O MENOR ELEMENTO DA RUBRO-NEGRA
NoPalavrasPortuguesARB* procuraMenor(NoPalavrasPortuguesARB* atual){
	while (atual->esq != NULL){
		atual = atual->esq;
	}
	return atual; 
}


//Removendo o menor elemento da rubro negra
NoPalavrasPortuguesARB* removerMenor(NoPalavrasPortuguesARB* raiz){
	if (raiz->esq == NULL){
		free(raiz);
		return NULL;
	}
	if(cor(raiz->esq) == BLACK && cor(raiz->esq->esq) == BLACK)
		raiz = move2EsqVermelho(raiz);
	raiz->esq = removerMenor(raiz->esq);
	
	return balancear(raiz);
}

//Removendo um elemento da árvore rubro negra
NoPalavrasPortuguesARB* removeNO_ARB(NoPalavrasPortuguesARB* raiz, char* palavra) {
	if (raiz == NULL) return NULL; // Verifica se a raiz é nula

	if (strcmp(palavra, raiz->palavra) < 0) { // Se a palavra a ser removida é menor, vai para a esquerda
		if (cor(raiz->esq) == BLACK && cor(raiz->esq->esq) == BLACK)
			raiz = move2EsqVermelho(raiz);
		raiz->esq = removeNO_ARB(raiz->esq, palavra);
	} else {
		if (cor(raiz->esq) == RED)
			raiz = rotacionaDireita(raiz);
		if (strcmp(palavra, raiz->palavra) == 0 && raiz->dir == NULL) { // Nó encontrado e não tem filho direito
			free(raiz);
			return NULL;
		}
		if (cor(raiz->dir) == BLACK && cor(raiz->dir->esq) == BLACK)
			raiz = move2DirVermelho(raiz);
		if (strcmp(palavra, raiz->palavra) == 0) {
			NoPalavrasPortuguesARB* X = procuraMenor(raiz->dir);
			strcpy(raiz->palavra, X->palavra); // Copia o conteúdo do menor nó
			raiz->dir = removerMenor(raiz->dir);
		} else {
			raiz->dir = removeNO_ARB(raiz->dir, palavra);
		}
	}
	return balancear(raiz);
}


int remove_ARB(NoPalavrasPortuguesARB **raiz, char* palavra) {
	if (consulta_ArvoreRubroNegra(*raiz, palavra)) { // Se a palavra está na árvore
		*raiz = removeNO_ARB(*raiz, palavra);
		if (*raiz != NULL)
			(*raiz)->cor = BLACK; // Após a remoção, a raiz deve ser preta
		return 1; // Remoção bem-sucedida
	} else {
		return 0; // Palavra não encontrada
	}
}


void imprimeARB_EmOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_EmOrdem(raiz->esq);  // Visita o nó esquerdo
        printf("%s (%s) ", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");  // Imprime o valor e a cor
        imprimeARB_EmOrdem(raiz->dir);  // Visita o nó direito
    }
}

// Função para imprimir palavras em ordem na ABB de palavras em inglês
void imprimeABB_EmOrdem(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        imprimeABB_EmOrdem(raiz->esq); // Percorre subárvore esquerda

        // Imprime a palavra em inglês
        printf("    - %s\n", raiz->palavra);

        // Imprime as unidades associadas
        NoUnidade* atual = raiz->unidades;
        printf("      Unidades: ");
        while (atual != NULL) {
            printf("%s ", atual->unidade);
            atual = atual->prox;
        }
        printf("\n");

        imprimeABB_EmOrdem(raiz->dir); // Percorre subárvore direita
    }
}

// Função para imprimir palavras em ordem na Árvore Rubro-Negra
void imprimeARB_Ordem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_Ordem(raiz->esq); // Percorre subárvore esquerda

        // Imprime a palavra em português
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as traduções associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Traduções em Inglês:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        imprimeARB_Ordem(raiz->dir); // Percorre subárvore direita
    }
}

// Função para imprimir palavras em pré-ordem na Árvore Rubro-Negra
void imprimeARB_PreOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        // Imprime a palavra em português
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as traduções associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Traduções em Inglês:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        imprimeARB_PreOrdem(raiz->esq); // Percorre subárvore esquerda
        imprimeARB_PreOrdem(raiz->dir); // Percorre subárvore direita
    }
}

// Função para imprimir palavras em pós-ordem na Árvore Rubro-Negra
void imprimeARB_PosOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_PosOrdem(raiz->esq); // Percorre subárvore esquerda
        imprimeARB_PosOrdem(raiz->dir); // Percorre subárvore direita

        // Imprime a palavra em português
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as traduções associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Traduções em Inglês:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }
    }
}

void liberarABB(NoPalavrasUnidadesInglesABB* raiz);
//Função para Liberar a Árvore Rubro-Negra (ARB) de Palavras em Português
void liberarARB(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        liberarARB(raiz->esq); // Libera a subárvore esquerda
        liberarARB(raiz->dir); // Libera a subárvore direita
        liberarABB(raiz->arvIngles); // Libera a ABB associada
        free(raiz); // Libera o nó atual
    }
}




                                                         //////BLOCO ÁRVORE BINARIA DE BUSCA - ABB(palavras Inglês)

																 
// Função para criar um novo nó de palavras na árvore																 
NoPalavrasUnidadesInglesABB* criaNoIngles(char* palavra, char* unidade) {
    NoPalavrasUnidadesInglesABB* novo = (NoPalavrasUnidadesInglesABB*)malloc(sizeof(NoPalavrasUnidadesInglesABB));
    if (!novo) {
        printf("Erro ao alocar memória para nó de palavras em inglês!\n");
        exit(1);
    }
    strcpy(novo->palavra, palavra);
    novo->unidades = criaUnidade(NULL, unidade); // Cria a lista de unidades com a unidade inicial
    novo->esq = novo->dir = NULL;
    return novo;
}


//FUNÇÕES AUXILIARES ABB
// Função que verifica se o nó é uma folha
int ehfolha(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL && raiz->esq == NULL && raiz->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}
// Função que verifica se o nó tem um único filho
int unicoFilho(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL) {
        if ((raiz->esq != NULL && raiz->dir == NULL) || (raiz->esq == NULL && raiz->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}
// Função que verifica se o nó tem dois filhos
int doisFilhos(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL && raiz->esq != NULL && raiz->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Função que retorna a maior palavra em inglês

char* maiorPalavra(NoPalavrasUnidadesInglesABB *raiz) {
    if (raiz == NULL) return NULL;

    NoPalavrasUnidadesInglesABB *atual = raiz;
    while (atual->dir != NULL) {
        atual = atual->dir;
    }
    return atual->palavra;
}

// Função para inserir uma palavra de uma unidade na árvore binária de inglês
NoPalavrasUnidadesInglesABB* insereIngles(NoPalavrasUnidadesInglesABB* raiz, char* palavra, char* unidade) {
    if (raiz == NULL) {
        raiz = criaNoIngles(palavra, unidade);
    } else if (strcmp(palavra, raiz->palavra) < 0) {
        raiz->esq = insereIngles(raiz->esq, palavra, unidade);
    } else if (strcmp(palavra, raiz->palavra) > 0) {
        raiz->dir = insereIngles(raiz->dir, palavra, unidade);
    } else {
        raiz->unidades = insereUnidade(raiz->unidades, unidade); // Adiciona a unidade à lista
    }
    return raiz;
}

void normalizarString(char* str);
// Função para buscar uma palavra em ingles na ABB
NoPalavrasUnidadesInglesABB* buscarPalavraInglesABB(NoPalavrasUnidadesInglesABB* raiz, char* palavra, char* unidade) {
    NoPalavrasUnidadesInglesABB* atual = raiz;
    NoPalavrasUnidadesInglesABB* resultado = NULL;
    normalizarString(palavra);

    while (atual != NULL) {
        char palavraNormalizada[TAM];
        strcpy(palavraNormalizada, atual->palavra);
        normalizarString(palavraNormalizada);

        if (strcmp(palavra, palavraNormalizada) == 0) {
            // Verifica se a unidade existe na lista de unidades
            NoUnidade* unidadeAtual = atual->unidades;
            while (unidadeAtual != NULL) {
                if (strcmp(unidadeAtual->unidade, unidade) == 0) {
                    resultado = atual;
                    break;
                }
                unidadeAtual = unidadeAtual->prox;
            }
            break;
        }

        if (strcmp(palavra, palavraNormalizada) < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    return resultado;
}

int buscarPalavraInglesComUnidade(NoPalavrasUnidadesInglesABB* raiz, char* palavra, char* unidade) {
    normalizarString(palavra);
    NoPalavrasUnidadesInglesABB* atual = raiz;

    while (atual != NULL) {
        char palavraNormalizada[TAM];
        strcpy(palavraNormalizada, atual->palavra);
        normalizarString(palavraNormalizada);

        if (strcmp(palavra, palavraNormalizada) == 0) {
            NoUnidade* unidadeAtual = atual->unidades;
            while (unidadeAtual != NULL) {
                if (strcmp(unidadeAtual->unidade, unidade) == 0) {
                    return 1;  // Palavra encontrada com a unidade correspondente
                }
                unidadeAtual = unidadeAtual->prox;
            }
        }

        if (strcmp(palavra, palavraNormalizada) < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    return 0;  // Palavra ou unidade não encontrada
}






//Função para Liberar a Árvore Binária de Busca (ABB) de Palavras em Inglês
void liberarABB(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        liberarABB(raiz->esq); // Libera a subárvore esquerda
        liberarABB(raiz->dir); // Libera a subárvore direita
        liberarListaUnidades(raiz->unidades); // Libera a lista de unidades
        free(raiz); // Libera o nó atual
    }
}

void limparEspacos(char* str) {
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) *fim-- = '\0'; // Remove espaços à direita
    while (*str && isspace(*str)) str++;             // Remove espaços à esquerda
}


                                                    ////////FUNÇÕES ESPECÍFICAS PARA O MENU DE OPÇÕES DA MAIN

// Função para remover espaços e converter para minúsculas
void normalizarString(char* str) {
    int i = 0, j = 0;
    int len = strlen(str);

    while (isspace(str[i])) {
        i++;
    }

    while (i < len) {
        if (!isspace(str[i]) || (j > 0 && !isspace(str[j - 1]))) {
            str[j++] = tolower(str[i]);
        }
        i++;
    }

    if (j > 0 && isspace(str[j - 1])) {
        j--;
    }

    str[j] = '\0';
}


//Função de cadastro													
void cadastrarPalavra(NoPalavrasPortuguesARB** raiz, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    char linha[TAM];
    char unidadeAtual[TAM] = "";
    char palavraIngles[TAM];
    char palavrasPortugues[TAM];

    if (arquivo != NULL) {
        // Lê o arquivo linha por linha
        while (fgets(linha, TAM, arquivo) != NULL) {
            linha[strcspn(linha, "\n")] = '\0';  // Remove o '\n' no final da linha

            if (linha[0] == '%') {  // Identifica a unidade
                strcpy(unidadeAtual, linha + 2);
                limparEspacos(unidadeAtual);
            } else {
                char* separador = strchr(linha, ':');
                if (separador != NULL) {
                    strncpy(palavraIngles, linha, separador - linha);
                    palavraIngles[separador - linha] = '\0';
                    limparEspacos(palavraIngles);

                    strcpy(palavrasPortugues, separador + 1);
                    char* pontoEVirgula = strchr(palavrasPortugues, ';');
                    if (pontoEVirgula != NULL) {
                        *pontoEVirgula = '\0';
                    }

                    char* palavraPortugues = strtok(palavrasPortugues, ",");
                    while (palavraPortugues != NULL) {
                        limparEspacos(palavraPortugues);

                        // Insere a palavra na árvore rubro-negra
                        int inserida = insere_ArvoreRubroNegra(raiz, palavraPortugues);
                        NoPalavrasPortuguesARB* noAtual = *raiz;
                        int encontrado = 0;

                        // Busca a palavra inserida na árvore
                        while (noAtual != NULL && encontrado == 0) {
                            if (strcmp(noAtual->palavra, palavraPortugues) == 0) {
                                // Associa a palavra em inglês à palavra em português na árvore
                                noAtual->arvIngles = insereIngles(noAtual->arvIngles, palavraIngles, unidadeAtual);
                                encontrado = 1;
                            } else if (strcmp(palavraPortugues, noAtual->palavra) < 0) {
                                noAtual = noAtual->esq;
                            } else {
                                noAtual = noAtual->dir;
                            }
                        }

                        if (inserida == 0 && encontrado == 0) {
                            printf("Aviso: Palavra '%s' já existente na árvore rubro-negra.\n", palavraPortugues);
                        } else if (inserida == 1) {
                            printf("Palavra '%s' inserida com sucesso na árvore rubro-negra.\n", palavraPortugues);
                        }

                        palavraPortugues = strtok(NULL, ",");
                    }
                }
            }
        }

        fclose(arquivo);
        printf("Carregamento do arquivo concluído com sucesso.\n");
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
}


//(i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês; 
//Imprimir palavras de uma unidade
void imprimePorUnidade(NoPalavrasPortuguesARB* raiz, char* unidade) {
    if (raiz != NULL) {
        imprimePorUnidade(raiz->esq, unidade);

        NoPalavrasUnidadesInglesABB* atual = raiz->arvIngles;
        int encontrou = 0;

        while (atual != NULL) {
            NoUnidade* listaUnidades = atual->unidades;
            while (listaUnidades != NULL) {
                if (strcmp(listaUnidades->unidade, unidade) == 0) {
                    if (!encontrou) {
                        printf("\nPalavra em Portugues: %s\n", raiz->palavra);
                        encontrou = 1;
                    }
                    printf("  - Ingles: %s\n", atual->palavra);
                }
                listaUnidades = listaUnidades->prox;
            }
            atual = atual->esq ? atual->esq : atual->dir; // Continua na ABB
        }

        imprimePorUnidade(raiz->dir, unidade);
    }
}



//(ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade.

//Função para buscar palavra em português
void buscaPorPalavra(NoPalavrasPortuguesARB* raiz, char* palavra) {
    NoPalavrasPortuguesARB* atual = raiz;
    int encontrada = 0;  // Variável de controle para indicar se a palavra foi encontrada

    normalizarString(palavra);  // Normaliza a palavra antes da busca

    while (atual != NULL && encontrada == 0) {
        char palavraNormalizada[TAM];
        strcpy(palavraNormalizada, atual->palavra);
        normalizarString(palavraNormalizada);  // Normaliza a palavra armazenada no nó

        if (strcmp(palavra, palavraNormalizada) == 0) {
            // Palavra encontrada
            printf("\nPalavra em Português: %s\n", atual->palavra);
            printf("Traduções em Inglês: \n");
            imprimeABB_EmOrdem(atual->arvIngles);
            printf("\n");
            encontrada = 1;
        } else if (strcmp(palavra, palavraNormalizada) < 0) {
            atual = atual->esq;  // Busca na subárvore esquerda
        } else {
            atual = atual->dir;  // Busca na subárvore direita
        }
    }

    if (encontrada == 0) {
        // Se a palavra não foi encontrada
        printf("Palavra em Portugues nao encontrada: %s\n", palavra);
    }
}



/*
(iii)informar uma palavra em inglês e e a unidade a qual a mesma pertence remove-la das árvores binárias  das quais ela pertence. 
 Caso ela seja a única palavra em uma das árvores binárias, remover também da  árvore vermelha-preta;
 */


// Função para remover um nó com palavra em inglês da ABB
NoPalavrasUnidadesInglesABB* removerPalavraInglesABB(NoPalavrasUnidadesInglesABB* raiz, char* palavra, char* unidade) {
    if (raiz == NULL) {
        return NULL;
    }

    if (strcmp(palavra, raiz->palavra) < 0) {
        raiz->esq = removerPalavraInglesABB(raiz->esq, palavra, unidade);
    } else if (strcmp(palavra, raiz->palavra) > 0) {
        raiz->dir = removerPalavraInglesABB(raiz->dir, palavra, unidade);
    } else {
        raiz->unidades = removeUnidade(raiz->unidades, unidade);
        if (raiz->unidades == NULL) {
            if (raiz->esq == NULL && raiz->dir == NULL) {
                free(raiz);
                return NULL;
            } else if (raiz->esq == NULL) {
                NoPalavrasUnidadesInglesABB* temp = raiz->dir;
                free(raiz);
                return temp;
            } else if (raiz->dir == NULL) {
                NoPalavrasUnidadesInglesABB* temp = raiz->esq;
                free(raiz);
                return temp;
            } else {
                NoPalavrasUnidadesInglesABB* sucessor = raiz->dir;
                while (sucessor->esq != NULL) {
                    sucessor = sucessor->esq;
                }
                strcpy(raiz->palavra, sucessor->palavra);
                raiz->unidades = sucessor->unidades;
                raiz->dir = removerPalavraInglesABB(raiz->dir, sucessor->palavra, unidade);
            }
        }
    }

    return raiz;
}

void removerPalavraEmInglesDaArvore(NoPalavrasPortuguesARB** raiz, char* palavra, char* unidade) {
    NoPalavrasPortuguesARB* atual = *raiz;
    int encontrada = 0;

    while (atual != NULL && encontrada == 0) {
        if (buscarPalavraInglesComUnidade(atual->arvIngles, palavra, unidade)) {
            atual->arvIngles = removerPalavraInglesABB(atual->arvIngles, palavra, unidade);
            printf("\nPalavra removida com sucesso!\n");
            encontrada = 1;
        } else if (strcmp(palavra, atual->palavra) < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    if (!encontrada) {
        printf("\nPalavra nao encontrada!\n");
    }
}




/*
(iv)informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto  deve remover a palavra em inglês da árvore binária 
correspondente a palavra em português da mesma unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida 
da árvore vermelha-preta.
*/

//Função para remover palavras em português 
NoPalavrasPortuguesARB* removePalavraPortugues(NoPalavrasPortuguesARB* raiz, char* palavra, char* unidade) {
    NoPalavrasPortuguesARB* resultado = raiz; // Inicializa o resultado como a raiz atual

    if (raiz != NULL) {
        if (strcmp(palavra, raiz->palavra) < 0) {
            raiz->esq = removePalavraPortugues(raiz->esq, palavra, unidade); // Remove na subárvore esquerda
        } else if (strcmp(palavra, raiz->palavra) > 0) {
            raiz->dir = removePalavraPortugues(raiz->dir, palavra, unidade); // Remove na subárvore direita
        } else {
            // A palavra em português corresponde à raiz atual
            raiz->arvIngles = removerPalavraInglesABB(raiz->arvIngles, palavra, unidade);

            // Verifica se a árvore de palavras em inglês está vazia
            if (raiz->arvIngles == NULL) {
                // Remove o nó correspondente na Árvore Rubro-Negra
                NoPalavrasPortuguesARB* temp = removeNO_ARB(raiz, palavra);
                resultado = temp;
            }
        }
    }

    return resultado; // Retorna a raiz ajustada
}

                                                          ////////FUNÇÕES PARA O EXPERIMENTO

														  
// Função para buscar uma palavra na Árvore Rubro-Negra e imprimir o caminho percorrido
void buscarPalavraComCaminho(NoPalavrasPortuguesARB* raiz, char* palavra, int* encontrada) {
    NoPalavrasPortuguesARB* atual = raiz;
    normalizarString(palavra);

    printf("Caminho: ");
    *encontrada = 0;

    while (atual != NULL) {
        char palavraNormalizada[TAM];
        strcpy(palavraNormalizada, atual->palavra);
        normalizarString(palavraNormalizada);

        printf("[%s] -> ", atual->palavra);

        if (*encontrada == 0 && strcmp(palavra, palavraNormalizada) == 0) {
            *encontrada = 1;
            printf("Palavra encontrada!\n");
            return;
        } else if (strcmp(palavra, palavraNormalizada) < 0) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    printf("Palavra não encontrada.\n");
}

// Função principal para executar o experimento de busca
void experimentoBusca(NoPalavrasPortuguesARB* raiz) {
    printf("\n=== Experimento de Busca na Árvore Rubro-Negra ===\n");

    clock_t inicio, fim;
    double tempoGasto;

    // Busca em ordem
    printf("\n--- Busca em Ordem ---\n");
    inicio = clock();
    imprimeARB_Ordem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pré-ordem
    printf("\n--- Busca em Pré-Ordem ---\n");
    inicio = clock();
    imprimeARB_PreOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pós-ordem
    printf("\n--- Busca em Pós-Ordem ---\n");
    inicio = clock();
    imprimeARB_PosOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca por palavras específicas
    char* palavras[] = {"bus", "bug", "bike", "system", "network"};
    int tamanho = sizeof(palavras) / sizeof(palavras[0]);
    int encontrada, i;

    for (i = 0; i < tamanho; i++) {
        //printf("\nBuscando a palavra: '%s'\n", palavras[i]);
        buscarPalavraComCaminho(raiz, palavras[i], &encontrada);

        if (encontrada) {
            printf("Palavra '%s' encontrada com sucesso!\n", palavras[i]);
        } else {
            printf("Palavra '%s' não encontrada.\n", palavras[i]);
        }
    }
}
				  
														  
void exibirMenu() {
    printf("\n==== MENU PRINCIPAL ====\n");
    printf("1 - Cadastrar palavra e unidade\n");
	printf("2 - Informar uma unidade e imprimir palavras (Portugues -> Ingles)\n");
    printf("3 - Buscar uma palavra em Portugues e listar equivalentes em Ingles\n");
    printf("4 - Remover uma palavra em Ingles e a unidade a qual pertence\n");
    printf("5 - Remover uma palavra em Portugues e suas palavras associadas\n");
    printf("6 - Imprimir toda a arvore Rubro-Negra (Portugues)\n");
    printf("7 - Experimento: busca de 30 palavras em portugues com caminho percorrido\n");
    printf("0 - Sair\n");
    printf("========================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    NoPalavrasPortuguesARB* arvorePortugues = NULL; // Raiz da Árvore Rubro-Negra de palavras em português
    char palavra[TAM], unidade[TAM];
    char nomeArquivo[] = "C:/Users/MEU CAMINHO DO ARQUIVO/vocabulario.txt";
    setlocale(LC_ALL, ""); // Habilita suporte a UTF-8
    int opcao;

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            printf("\nEntrada invalida! Por favor, insira um numero.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }
        getchar(); // Limpa o buffer após scanf

        switch (opcao) {
            case 1:
                printf("\nCarregando palavras do arquivo...\n");
                cadastrarPalavra(&arvorePortugues, nomeArquivo);
                break;

            case 2:
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0'; // Remove o '\n'
                printf("\nPalavras da Unidade: %s\n", unidade);
                imprimePorUnidade(arvorePortugues, unidade);
                break;

            case 3:
                printf("Informe a palavra em Portugues: ");
			    fgets(palavra, TAM, stdin);
			    palavra[strcspn(palavra, "\n")] = '\0';  // Remove o '\n'
			
			    buscaPorPalavra(arvorePortugues, palavra);
			    break;

            case 4:
			    printf("Informe a palavra em Ingles: ");
			    fgets(palavra, TAM, stdin);
			    palavra[strcspn(palavra, "\n")] = '\0';  // Remove o '\n'
			
			    printf("Informe a unidade: ");
			    fgets(unidade, TAM, stdin);
			    unidade[strcspn(unidade, "\n")] = '\0';  // Remove o '\n'
			
			    removerPalavraEmInglesDaArvore(&arvorePortugues, palavra, unidade);
			    break;



            case 5:
                printf("Informe a palavra em Portugues: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0';
                arvorePortugues = removePalavraPortugues(arvorePortugues, palavra, unidade);
                printf("\nPalavra em Portugues e palavras associadas removidas!\n");
                break;

            case 6:
                printf("\nImprimindo a árvore Rubro-Negra (Portugues):\n");
                imprimeARB_EmOrdem(arvorePortugues);
                break;
            
            case 7:
			    cadastrarPalavra(&arvorePortugues, nomeArquivo);
    			experimentoBusca(arvorePortugues);
			    break;

            case 0:
                printf("\nEncerrando o programa...\n");
                liberarARB(arvorePortugues); // Libera a memória alocada
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

