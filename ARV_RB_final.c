/*                                                                                                                                                                                                        Cada unidade em um livro-texto em ingl�s cont�m um vocabul�rio Ingl�s-Portugu�s de  
Cada unidade em um livro-texto em ingl�s cont�m um vocabul�rio Ingl�s-Portugu�s de palavras que s�o usadas pela primeira vez em uma unidade em particular. 
Desejamos converter um conjunto de tais vocabul�rios armazenados em um arquivo Ingl�s em um conjunto de vocabul�rios Portugu�s-Ingl�s.  
Para isso considere que: 
(a) Os nomes das unidades s�o precedidos por um s�mbolo de porcentagem. 
(b) H� somente uma entrada por linha. 
(c) Uma palavra em Ingl�s � separada por dois pontos de sua(s) equivalente(s) portuguesa(s); se h� mais de uma equivalente, elas s�o separadas por v�rgula. 
Exemplo: 
 % Unidade 1 
 Bus: �nibus, barramento; 
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
 
Para produzir as palavras em ordem alfab�tica, crie uma �rvore vermelha-preta contendo todas as palavras em portugu�s de todas as unidades e 
uma �rvore bin�ria de busca com as equivalentes inglesas e em cada n� da lista encadeada contenha tamb�m a unidade que pertence aquela palavra em ingl�s. 
Assegure-se de que exista apenas um n� para cada palavra portuguesa na �rvore.  
Al�m disso, permita ao usu�rio:  
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


// N� para representar uma unidade em uma lista encadeada
typedef struct NoUnidade {
    char unidade[TAM];                 // Nome da unidade
    struct NoUnidade* prox;            // Ponteiro para a pr�xima unidade
} NoUnidade;

// N� para palavras em ingl�s (ABB), com lista de unidades associadas
typedef struct NoPalavrasUnidadesInglesABB {
    char palavra[TAM];                 // Palavra em ingl�s
    NoUnidade* unidades;               // Lista encadeada de unidades
    struct NoPalavrasUnidadesInglesABB *esq, *dir; // Filhos da ABB
} NoPalavrasUnidadesInglesABB;

// N� para palavras em portugu�s (ARB), com ABB associada
typedef struct NoPalavrasPortuguesARB {
    char palavra[TAM];                 // Palavra em portugu�s
    int cor;                           // Cor do n� (RED/BLACK)
    struct NoPalavrasPortuguesARB *esq, *dir; // Filhos da ARB
    NoPalavrasUnidadesInglesABB* arvIngles;   // ABB de palavras em ingl�s associadas
} NoPalavrasPortuguesARB;


                                                                   //BLOCO LISTA ENCADEADA PARA UNIDADES

//Fun��o para criar uma nova unidade na lista encadeada																    
NoUnidade* criaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* nova = (NoUnidade*)malloc(sizeof(NoUnidade));
    if (!nova) {
        printf("Erro ao alocar memoria para unidade!\n");
        exit(1);
    }
    strcpy(nova->unidade, unidade);
    nova->prox = lista;
    return nova; // Retorna o in�cio atualizado da lista
}

//Fun��o para inserir uma nova unidade na lista encadeada
NoUnidade* insereUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = lista;
    int existe = 0; // Indicador para verificar se a unidade j� existe

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            existe = 1; // Unidade j� existe
        }
        atual = atual->prox; // Avan�a na lista
    }

    if (!existe) {
        lista = criaUnidade(lista, unidade); // Chama a fun��o para criar uma nova unidade
    }

    return lista; // Retorna a lista (modificada ou n�o)
}

//Fun��o para buscar uma unidade na lista encadeada
NoUnidade* buscaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* resultado = NULL; // Inicializa o ponteiro para o resultado como NULL
    NoUnidade* atual = lista;    // Ponteiro para percorrer a lista

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            resultado = atual;   // Encontra a unidade e atualiza o resultado
        }
        atual = atual->prox;     // Avan�a para o pr�ximo n�
    }
    return resultado;            // Retorna o resultado (NULL se n�o encontrado)
}

//Fun��o para remover uma unidade na lista encadeada
NoUnidade* removeUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = buscaUnidade(lista, unidade); // Localiza o n� a ser removido
    NoUnidade* anterior = NULL;
    NoUnidade* novaLista = lista; // Ponteiro para a nova lista (possivelmente modificada)

    if (atual != NULL) {
        if (lista == atual) {
            // O n� a ser removido � o primeiro da lista
            novaLista = atual->prox;
        } else {
            // Localiza o n� anterior ao n� atual
            anterior = lista;
            while (anterior != NULL && anterior->prox != atual) {
                anterior = anterior->prox;
            }
            if (anterior != NULL) {
                anterior->prox = atual->prox;
            }
        }
        free(atual); // Libera o n� encontrado
    }

    return novaLista; // Retorna a lista (modificada ou n�o)
}

//Fun��o para imprimir as unidades na lista encadeada
void imprimeUnidades(NoUnidade* lista) {
    NoUnidade* atual = lista;
    int vazio = 1; // Indicador para verificar se a lista est� vazia

    while (atual != NULL) {
        printf("%s ", atual->unidade);
        atual = atual->prox; // Avan�a na lista
        vazio = 0; // A lista n�o est� vazia
    }

    if (vazio) {
        printf("Lista de unidades vazia.");
    }

    printf("\n"); // Quebra de linha ap�s imprimir a lista
}

//Fun��o para Liberar a Lista de Unidades
void liberarListaUnidades(NoUnidade* lista) {
    NoUnidade* atual = lista;
    while (atual != NULL) {
        NoUnidade* temp = atual;
        atual = atual->prox;
        free(temp);
    }
}



                                                       ////BLOCO �RVORE RUBRO NEGRA - ARB(palavras portugues)

//FUN��ES AUXILIARES                                                                    
//acessando a cor de um n�
int cor(NoPalavrasPortuguesARB* raiz){
	if (raiz == NULL)
		return BLACK;
	else
		return raiz->cor;
} 
                                                                    

//inverter a cor do pai e de seus filhos � uma opera��o administrativa: n�o altera a estrutura ou conte�do da �rvore
//MUDANDO A COR DE UM N� E DE SEUS FILHOS
void trocarCor(NoPalavrasPortuguesARB* raiz){
	raiz->cor = !raiz->cor;
	if (raiz->esq != NULL)
		raiz->esq->cor = !raiz->esq->cor;
	if (raiz->dir != NULL)
		raiz->dir->cor = !raiz->dir->cor;
}
/*
Rota��o da �rvore rubro negra -> caida para a esquerda
opera��es b�sicas para balanceamento (rota��o � esquerda e rota��o � direita)
exemplo: recebe um n� A, tendo B com filho a direita. 
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
Rota��o � direita:
-recebe um n� A com B como filho esquerdo
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
-se o filho direito � vermelho: rota��o � esquerda
-se o filho direito e o neto da esquerda s�o vermelhos: rota��o � direita
-se ambos os filhos s�o vermelhos: troca a cor do pai e dos filhos
*/

NoPalavrasPortuguesARB* balancear(NoPalavrasPortuguesARB* raiz){
	//n� vermelho � sempre filho � esquerda
	if (cor(raiz->dir) == RED)
		raiz = rotacionaEsquerda(raiz);
	//filho da direita e neto da esquerda s�o vermelho
	if (raiz->esq != NULL && cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED)
		raiz = rotacionaDireita(raiz);
	//2 filhos vermelhos: troca cor
	if (cor(raiz->esq) == RED && cor(raiz->dir) == RED)
		trocarCor(raiz);
	
	return raiz;
}
                                                                   
//Fun��o p/ criar um n� com palavra em portugu�s                                                                    
NoPalavrasPortuguesARB* criaNoPortugues(char* palavra) {
    NoPalavrasPortuguesARB* novo = (NoPalavrasPortuguesARB*)malloc(sizeof(NoPalavrasPortuguesARB));
    if (novo == NULL) {
        printf("Erro de aloca��o de memoria!\n");
        exit(1); // Se o malloc falhar, o programa ser� encerrado
    }
    strcpy(novo->palavra, palavra); // Copia a palavra para o campo 'palavra'
    novo->cor = RED; // Define a cor inicial como vermelho
    novo->esq = novo->dir = NULL; // Inicializa os filhos como NULL
    novo->arvIngles = NULL; // Inicializa a �rvore de palavras em ingl�s como NULL
    return novo;
}

//
NoPalavrasPortuguesARB* insereRubroNegra(NoPalavrasPortuguesARB* raiz, char* palavra, int* resp) {
    if (raiz == NULL) {
        *resp = 1; // Palavra inserida com sucesso
        return criaNoPortugues(palavra); // Cria um novo n� e o retorna
    }

    if (strcmp(palavra, raiz->palavra) == 0) {
        *resp = 0; // Palavra duplicada
        return raiz;
    }

    if (strcmp(palavra, raiz->palavra) < 0) { // Se a palavra � menor, insere � esquerda
        raiz->esq = insereRubroNegra(raiz->esq, palavra, resp);
    } else { // Se a palavra � maior, insere � direita
        raiz->dir = insereRubroNegra(raiz->dir, palavra, resp);
    }

    // **Regras de balanceamento da �rvore rubro-negra**
    if (cor(raiz->dir) == RED && cor(raiz->esq) == BLACK) // Caso 1: filho direito vermelho e esquerdo preto
        raiz = rotacionaEsquerda(raiz);
    if (cor(raiz->esq) == RED && cor(raiz->esq->esq) == RED) // Caso 2: dois filhos vermelhos consecutivos � esquerda
        raiz = rotacionaDireita(raiz);
    if (cor(raiz->esq) == RED && cor(raiz->dir) == RED) // Caso 3: dois filhos vermelhos
        trocarCor(raiz);

    return raiz; // Retorna o n� balanceado
}


int insere_ArvoreRubroNegra(NoPalavrasPortuguesARB** raiz, char* palavra) {
    int resp;
    *raiz = insereRubroNegra(*raiz, palavra, &resp); // Chama a fun��o recursiva para inserir
    if (*raiz != NULL)
        (*raiz)->cor = BLACK; // Ap�s a inser��o, a raiz da �rvore rubro-negra deve ser preta
    return resp;
}


//FUN��ES AUXILIARES P/ REMO��O

/*
Arvore rubro negra - caida para a esquerda
-movendo os n�s vermelhos:
-rota��o e trocas de cores podem causar uma viola��o das propriedades da �rvore
exemplo: a fun��o trocaCor pode introduzir sucessivos n�s vermelhos � direita, o que viola uma das propriedades da �rvore.

-al�m das rota��es, precisa de mais 3 fun��es para restabelecer o balanceamento e garantir que as propriedades da �rvore seja respeitadas
-mover um n� vermelho para a esquerda
- mover um n� vermelho para a direita
-arrumar o balanceamento
essas fun��es movimentam um n� vermelho para a sub-�rvore esquerda ou direita.

exemplo: 
-recebe um n� H e troca as cores dele e de seus filhos
-se o filho � esquerda do filho direito � vermelho, aplicar uma rota��o � direita no filho direito e uma rota��o esquerda no pai
-troca as cores do n� pai e de seus filhos
*/

int consulta_ArvoreRubroNegra(NoPalavrasPortuguesARB* raiz, char* palavra) {
    NoPalavrasPortuguesARB* atual = raiz;  // Ponteiro para percorrer a �rvore
    while (atual != NULL) {
        if (strcmp(palavra, atual->palavra) == 0) // Verifica se encontrou a palavra
            return 1;  // Encontrado
        if (strcmp(palavra, atual->palavra) < 0) // Se palavra � menor, vai para a esquerda
            atual = atual->esq;
        else // Se palavra � maior, vai para a direita
            atual = atual->dir;
    }
    return 0;  // N�o encontrado
}



//Movendo um n� vermelho para a esquerda
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
Mover um n� vermelho para a direita:
-recebe um n� H e troca as cores dele e de seus filhos
-se o filho a esquerda do filho esquerdo � vermelho, aplica-se uma rota��o � direita no pai
-troca as cores do n� pai e de seus filhos 
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

//Removendo um elemento da �rvore rubro negra
NoPalavrasPortuguesARB* removeNO_ARB(NoPalavrasPortuguesARB* raiz, char* palavra) {
	if (raiz == NULL) return NULL; // Verifica se a raiz � nula

	if (strcmp(palavra, raiz->palavra) < 0) { // Se a palavra a ser removida � menor, vai para a esquerda
		if (cor(raiz->esq) == BLACK && cor(raiz->esq->esq) == BLACK)
			raiz = move2EsqVermelho(raiz);
		raiz->esq = removeNO_ARB(raiz->esq, palavra);
	} else {
		if (cor(raiz->esq) == RED)
			raiz = rotacionaDireita(raiz);
		if (strcmp(palavra, raiz->palavra) == 0 && raiz->dir == NULL) { // N� encontrado e n�o tem filho direito
			free(raiz);
			return NULL;
		}
		if (cor(raiz->dir) == BLACK && cor(raiz->dir->esq) == BLACK)
			raiz = move2DirVermelho(raiz);
		if (strcmp(palavra, raiz->palavra) == 0) {
			NoPalavrasPortuguesARB* X = procuraMenor(raiz->dir);
			strcpy(raiz->palavra, X->palavra); // Copia o conte�do do menor n�
			raiz->dir = removerMenor(raiz->dir);
		} else {
			raiz->dir = removeNO_ARB(raiz->dir, palavra);
		}
	}
	return balancear(raiz);
}


int remove_ARB(NoPalavrasPortuguesARB **raiz, char* palavra) {
	if (consulta_ArvoreRubroNegra(*raiz, palavra)) { // Se a palavra est� na �rvore
		*raiz = removeNO_ARB(*raiz, palavra);
		if (*raiz != NULL)
			(*raiz)->cor = BLACK; // Ap�s a remo��o, a raiz deve ser preta
		return 1; // Remo��o bem-sucedida
	} else {
		return 0; // Palavra n�o encontrada
	}
}


void imprimeARB_EmOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_EmOrdem(raiz->esq);  // Visita o n� esquerdo
        printf("%s (%s) ", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");  // Imprime o valor e a cor
        imprimeARB_EmOrdem(raiz->dir);  // Visita o n� direito
    }
}

// Fun��o para imprimir palavras em ordem na ABB de palavras em ingl�s
void imprimeABB_EmOrdem(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        imprimeABB_EmOrdem(raiz->esq); // Percorre sub�rvore esquerda

        // Imprime a palavra em ingl�s
        printf("    - %s\n", raiz->palavra);

        // Imprime as unidades associadas
        NoUnidade* atual = raiz->unidades;
        printf("      Unidades: ");
        while (atual != NULL) {
            printf("%s ", atual->unidade);
            atual = atual->prox;
        }
        printf("\n");

        imprimeABB_EmOrdem(raiz->dir); // Percorre sub�rvore direita
    }
}

// Fun��o para imprimir palavras em ordem na �rvore Rubro-Negra
void imprimeARB_Ordem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_Ordem(raiz->esq); // Percorre sub�rvore esquerda

        // Imprime a palavra em portugu�s
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as tradu��es associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Tradu��es em Ingl�s:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        imprimeARB_Ordem(raiz->dir); // Percorre sub�rvore direita
    }
}

// Fun��o para imprimir palavras em pr�-ordem na �rvore Rubro-Negra
void imprimeARB_PreOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        // Imprime a palavra em portugu�s
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as tradu��es associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Tradu��es em Ingl�s:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        imprimeARB_PreOrdem(raiz->esq); // Percorre sub�rvore esquerda
        imprimeARB_PreOrdem(raiz->dir); // Percorre sub�rvore direita
    }
}

// Fun��o para imprimir palavras em p�s-ordem na �rvore Rubro-Negra
void imprimeARB_PosOrdem(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        imprimeARB_PosOrdem(raiz->esq); // Percorre sub�rvore esquerda
        imprimeARB_PosOrdem(raiz->dir); // Percorre sub�rvore direita

        // Imprime a palavra em portugu�s
        printf("%s (%s)\n", raiz->palavra, raiz->cor == RED ? "Vermelho" : "Preto");

        // Imprime as tradu��es associadas, se houver
        if (raiz->arvIngles != NULL) {
            printf("  Tradu��es em Ingl�s:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }
    }
}

void liberarABB(NoPalavrasUnidadesInglesABB* raiz);
//Fun��o para Liberar a �rvore Rubro-Negra (ARB) de Palavras em Portugu�s
void liberarARB(NoPalavrasPortuguesARB* raiz) {
    if (raiz != NULL) {
        liberarARB(raiz->esq); // Libera a sub�rvore esquerda
        liberarARB(raiz->dir); // Libera a sub�rvore direita
        liberarABB(raiz->arvIngles); // Libera a ABB associada
        free(raiz); // Libera o n� atual
    }
}




                                                         //////BLOCO �RVORE BINARIA DE BUSCA - ABB(palavras Ingl�s)

																 
// Fun��o para criar um novo n� de palavras na �rvore																 
NoPalavrasUnidadesInglesABB* criaNoIngles(char* palavra, char* unidade) {
    NoPalavrasUnidadesInglesABB* novo = (NoPalavrasUnidadesInglesABB*)malloc(sizeof(NoPalavrasUnidadesInglesABB));
    if (!novo) {
        printf("Erro ao alocar mem�ria para n� de palavras em ingl�s!\n");
        exit(1);
    }
    strcpy(novo->palavra, palavra);
    novo->unidades = criaUnidade(NULL, unidade); // Cria a lista de unidades com a unidade inicial
    novo->esq = novo->dir = NULL;
    return novo;
}


//FUN��ES AUXILIARES ABB
// Fun��o que verifica se o n� � uma folha
int ehfolha(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL && raiz->esq == NULL && raiz->dir == NULL) {
        resultado = 1;
    }
    return resultado;
}
// Fun��o que verifica se o n� tem um �nico filho
int unicoFilho(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL) {
        if ((raiz->esq != NULL && raiz->dir == NULL) || (raiz->esq == NULL && raiz->dir != NULL)) {
            resultado = 1;
        }
    }
    return resultado;
}
// Fun��o que verifica se o n� tem dois filhos
int doisFilhos(NoPalavrasUnidadesInglesABB *raiz) {
    int resultado = 0;
    if (raiz != NULL && raiz->esq != NULL && raiz->dir != NULL) {
        resultado = 1;
    }
    return resultado;
}

// Fun��o que retorna a maior palavra em ingl�s

char* maiorPalavra(NoPalavrasUnidadesInglesABB *raiz) {
    if (raiz == NULL) return NULL;

    NoPalavrasUnidadesInglesABB *atual = raiz;
    while (atual->dir != NULL) {
        atual = atual->dir;
    }
    return atual->palavra;
}

// Fun��o para inserir uma palavra de uma unidade na �rvore bin�ria de ingl�s
NoPalavrasUnidadesInglesABB* insereIngles(NoPalavrasUnidadesInglesABB* raiz, char* palavra, char* unidade) {
    if (raiz == NULL) {
        raiz = criaNoIngles(palavra, unidade);
    } else if (strcmp(palavra, raiz->palavra) < 0) {
        raiz->esq = insereIngles(raiz->esq, palavra, unidade);
    } else if (strcmp(palavra, raiz->palavra) > 0) {
        raiz->dir = insereIngles(raiz->dir, palavra, unidade);
    } else {
        raiz->unidades = insereUnidade(raiz->unidades, unidade); // Adiciona a unidade � lista
    }
    return raiz;
}

void normalizarString(char* str);
// Fun��o para buscar uma palavra em ingles na ABB
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

    return 0;  // Palavra ou unidade n�o encontrada
}






//Fun��o para Liberar a �rvore Bin�ria de Busca (ABB) de Palavras em Ingl�s
void liberarABB(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        liberarABB(raiz->esq); // Libera a sub�rvore esquerda
        liberarABB(raiz->dir); // Libera a sub�rvore direita
        liberarListaUnidades(raiz->unidades); // Libera a lista de unidades
        free(raiz); // Libera o n� atual
    }
}

void limparEspacos(char* str) {
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) *fim-- = '\0'; // Remove espa�os � direita
    while (*str && isspace(*str)) str++;             // Remove espa�os � esquerda
}


                                                    ////////FUN��ES ESPEC�FICAS PARA O MENU DE OP��ES DA MAIN

// Fun��o para remover espa�os e converter para min�sculas
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


//Fun��o de cadastro													
void cadastrarPalavra(NoPalavrasPortuguesARB** raiz, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    char linha[TAM];
    char unidadeAtual[TAM] = "";
    char palavraIngles[TAM];
    char palavrasPortugues[TAM];

    if (arquivo != NULL) {
        // L� o arquivo linha por linha
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

                        // Insere a palavra na �rvore rubro-negra
                        int inserida = insere_ArvoreRubroNegra(raiz, palavraPortugues);
                        NoPalavrasPortuguesARB* noAtual = *raiz;
                        int encontrado = 0;

                        // Busca a palavra inserida na �rvore
                        while (noAtual != NULL && encontrado == 0) {
                            if (strcmp(noAtual->palavra, palavraPortugues) == 0) {
                                // Associa a palavra em ingl�s � palavra em portugu�s na �rvore
                                noAtual->arvIngles = insereIngles(noAtual->arvIngles, palavraIngles, unidadeAtual);
                                encontrado = 1;
                            } else if (strcmp(palavraPortugues, noAtual->palavra) < 0) {
                                noAtual = noAtual->esq;
                            } else {
                                noAtual = noAtual->dir;
                            }
                        }

                        if (inserida == 0 && encontrado == 0) {
                            printf("Aviso: Palavra '%s' j� existente na �rvore rubro-negra.\n", palavraPortugues);
                        } else if (inserida == 1) {
                            printf("Palavra '%s' inserida com sucesso na �rvore rubro-negra.\n", palavraPortugues);
                        }

                        palavraPortugues = strtok(NULL, ",");
                    }
                }
            }
        }

        fclose(arquivo);
        printf("Carregamento do arquivo conclu�do com sucesso.\n");
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
}


//(i) informar uma unidade e ent�o imprima todas as palavras da unidade em portugu�s seguida das equivalentes em ingl�s; 
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



//(ii)informar uma palavra em portugu�s e ent�o imprima todas as palavras em ingl�s equivalente a palavra em portugu�s dada, independente da unidade.

//Fun��o para buscar palavra em portugu�s
void buscaPorPalavra(NoPalavrasPortuguesARB* raiz, char* palavra) {
    NoPalavrasPortuguesARB* atual = raiz;
    int encontrada = 0;  // Vari�vel de controle para indicar se a palavra foi encontrada

    normalizarString(palavra);  // Normaliza a palavra antes da busca

    while (atual != NULL && encontrada == 0) {
        char palavraNormalizada[TAM];
        strcpy(palavraNormalizada, atual->palavra);
        normalizarString(palavraNormalizada);  // Normaliza a palavra armazenada no n�

        if (strcmp(palavra, palavraNormalizada) == 0) {
            // Palavra encontrada
            printf("\nPalavra em Portugu�s: %s\n", atual->palavra);
            printf("Tradu��es em Ingl�s: \n");
            imprimeABB_EmOrdem(atual->arvIngles);
            printf("\n");
            encontrada = 1;
        } else if (strcmp(palavra, palavraNormalizada) < 0) {
            atual = atual->esq;  // Busca na sub�rvore esquerda
        } else {
            atual = atual->dir;  // Busca na sub�rvore direita
        }
    }

    if (encontrada == 0) {
        // Se a palavra n�o foi encontrada
        printf("Palavra em Portugues nao encontrada: %s\n", palavra);
    }
}



/*
(iii)informar uma palavra em ingl�s e e a unidade a qual a mesma pertence remove-la das �rvores bin�rias  das quais ela pertence. 
 Caso ela seja a �nica palavra em uma das �rvores bin�rias, remover tamb�m da  �rvore vermelha-preta;
 */


// Fun��o para remover um n� com palavra em ingl�s da ABB
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
(iv)informar uma palavra em portugu�s e a unidade a qual a mesma pertence e ent�o remove-la, para isto  deve remover a palavra em ingl�s da �rvore bin�ria 
correspondente a palavra em portugu�s da mesma unidade. Caso ela seja a �nica palavra na �rvore bin�ria, a palavra em portugu�s deve ser removida 
da �rvore vermelha-preta.
*/

//Fun��o para remover palavras em portugu�s 
NoPalavrasPortuguesARB* removePalavraPortugues(NoPalavrasPortuguesARB* raiz, char* palavra, char* unidade) {
    NoPalavrasPortuguesARB* resultado = raiz; // Inicializa o resultado como a raiz atual

    if (raiz != NULL) {
        if (strcmp(palavra, raiz->palavra) < 0) {
            raiz->esq = removePalavraPortugues(raiz->esq, palavra, unidade); // Remove na sub�rvore esquerda
        } else if (strcmp(palavra, raiz->palavra) > 0) {
            raiz->dir = removePalavraPortugues(raiz->dir, palavra, unidade); // Remove na sub�rvore direita
        } else {
            // A palavra em portugu�s corresponde � raiz atual
            raiz->arvIngles = removerPalavraInglesABB(raiz->arvIngles, palavra, unidade);

            // Verifica se a �rvore de palavras em ingl�s est� vazia
            if (raiz->arvIngles == NULL) {
                // Remove o n� correspondente na �rvore Rubro-Negra
                NoPalavrasPortuguesARB* temp = removeNO_ARB(raiz, palavra);
                resultado = temp;
            }
        }
    }

    return resultado; // Retorna a raiz ajustada
}

                                                          ////////FUN��ES PARA O EXPERIMENTO

														  
// Fun��o para buscar uma palavra na �rvore Rubro-Negra e imprimir o caminho percorrido
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

    printf("Palavra n�o encontrada.\n");
}

// Fun��o principal para executar o experimento de busca
void experimentoBusca(NoPalavrasPortuguesARB* raiz) {
    printf("\n=== Experimento de Busca na �rvore Rubro-Negra ===\n");

    clock_t inicio, fim;
    double tempoGasto;

    // Busca em ordem
    printf("\n--- Busca em Ordem ---\n");
    inicio = clock();
    imprimeARB_Ordem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pr�-ordem
    printf("\n--- Busca em Pr�-Ordem ---\n");
    inicio = clock();
    imprimeARB_PreOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em p�s-ordem
    printf("\n--- Busca em P�s-Ordem ---\n");
    inicio = clock();
    imprimeARB_PosOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca por palavras espec�ficas
    char* palavras[] = {"bus", "bug", "bike", "system", "network"};
    int tamanho = sizeof(palavras) / sizeof(palavras[0]);
    int encontrada, i;

    for (i = 0; i < tamanho; i++) {
        //printf("\nBuscando a palavra: '%s'\n", palavras[i]);
        buscarPalavraComCaminho(raiz, palavras[i], &encontrada);

        if (encontrada) {
            printf("Palavra '%s' encontrada com sucesso!\n", palavras[i]);
        } else {
            printf("Palavra '%s' n�o encontrada.\n", palavras[i]);
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
    NoPalavrasPortuguesARB* arvorePortugues = NULL; // Raiz da �rvore Rubro-Negra de palavras em portugu�s
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
        getchar(); // Limpa o buffer ap�s scanf

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
                printf("\nImprimindo a �rvore Rubro-Negra (Portugues):\n");
                imprimeARB_EmOrdem(arvorePortugues);
                break;
            
            case 7:
			    cadastrarPalavra(&arvorePortugues, nomeArquivo);
    			experimentoBusca(arvorePortugues);
			    break;

            case 0:
                printf("\nEncerrando o programa...\n");
                liberarARB(arvorePortugues); // Libera a mem�ria alocada
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

