/*
                                                                           ÁRVORE 2-3
Cada unidade em um livro-texto em inglês contém um vocabulário Inglês-Português de palavras que são usadas pela primeira vez em uma unidade em particular. 
Desejamos converter um conjunto de tais vocabulários armazenados em um arquivo Inglês em um conjunto de vocabulários Português-Inglês. 
Para isso considere que: 
(a) Os nomes das unidades são precedidos por um símbolo de porcentagem. 
(b) Há somente uma entrada por linha. 
(c) Uma palavra em Inglês é separada por dois pontos de sua(s) equivalente(s) portuguesa(s); se há mais de 
uma equivalente, elas são separadas por vírgula. 
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
 
Para produzir as palavras em ordem alfabética, crie uma árvore 2-3 contendo todas as palavras em português de todas as unidades e uma árvore 
binária de busca com as equivalentes em inglês e em cada nó da lista encadeada contenha também a unidade que pertence aquela palavra em inglês. 
Assegure-se de que exista apenas um nó para cada palavra em português na árvore. 
Além disso, permita ao usuário: 
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>


#define TAM 100

// Nó para representar uma unidade em uma lista encadeada
typedef struct NoUnidade {
    char unidade[TAM];                 // Nome da unidade
    struct NoUnidade* prox;            // Ponteiro para a próxima unidade
} NoUnidade;

// Nó para palavras em inglês (ABB), com lista de unidades associadas e traduções em português
typedef struct NoPalavrasUnidadesInglesABB {
    char palavra[TAM];                 // Palavra em inglês
    NoUnidade* unidades;               // Lista encadeada de unidades
    struct NoPalavrasUnidadesInglesABB* esq, *dir; // Filhos da ABB
} NoPalavrasUnidadesInglesABB;

// Nó para palavras em português (na árvore 2-3)
typedef struct NoPalavrasPortuguesArv23{
    char Info1[TAM];  // chave esquerda
    char Info2[TAM];  // chave direita
    int numeroChaves;       // Número de chaves no nó (1 ou 2)
    struct NoPalavrasPortuguesArv23 *esq, *cen, *dir; // Filhos a árvore 2-3
    NoPalavrasUnidadesInglesABB *arvIngles; // Árvore binária associada
} NoPalavrasPortuguesArv23;

typedef NoPalavrasPortuguesArv23* Arv23;
typedef NoPalavrasUnidadesInglesABB* ABB;

 
                                                             //BLOCO LISTA ENCADEADA PARA UNIDADES
															    

//Função para criar uma nova unidade na lista encadeada																    
NoUnidade* criaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* nova = (NoUnidade*)malloc(sizeof(NoUnidade));
    if (nova!= NULL) {
    	strcpy(nova->unidade, unidade);
    	nova->prox = lista;
	}
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
        atual = atual->prox;
    }

    if (!existe) {
        lista = criaUnidade(lista, unidade); // Chama a função para criar uma nova unidade
        printf("Inserindo nova unidade: %s\n", unidade);
    } else {
        printf("Unidade ja existente: %s\n", unidade);
    }

    return lista;
}

//Função para buscar uma unidade na lista encadeada
NoUnidade* buscaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = lista;
    NoUnidade* resultado = NULL;

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            resultado = atual;
            printf("Unidade encontrada: %s\n", unidade);
        }
        atual = atual->prox;
    }

    if (resultado == NULL) {
        printf("Unidade nao encontrada: %s\n", unidade);
    }

    return resultado;
}


//Função para remover uma unidade na lista encadeada
NoUnidade* removeUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = lista;
    NoUnidade* anterior = NULL;
    NoUnidade* novaLista = lista;

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            if (anterior == NULL) {
                // Se a unidade a ser removida está no início da lista
                novaLista = atual->prox;
            } else {
                // Ajusta o ponteiro do nó anterior para ignorar o nó removido
                anterior->prox = atual->prox;
            }
            printf("Removendo unidade: %s\n", unidade);
            free(atual);
            atual = NULL;
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }

    if (atual == NULL && anterior == NULL) {
        printf("Unidade não encontrada para remoção: %s\n", unidade);
    }

    return novaLista;
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


                                                    ////BLOCO ÁRVORE BINÁRIA DE BUSCA - ABB(palavras Inglês)


// Função para criar um novo nó de palavras na árvore binária de busca - ABB (palavras em inglês)
NoPalavrasUnidadesInglesABB* criaNoIngles(char* palavra, char* unidade) {
    NoPalavrasUnidadesInglesABB* novo = (NoPalavrasUnidadesInglesABB*)malloc(sizeof(NoPalavrasUnidadesInglesABB));
    if (novo != NULL) {
        strcpy(novo->palavra, palavra);
	    novo->unidades = criaUnidade(NULL, unidade);
	    novo->esq = novo->dir = NULL;
    }
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
NoPalavrasUnidadesInglesABB* insereIngles(NoPalavrasUnidadesInglesABB* raiz, char* palavraIngles, char* unidade) {
    if (raiz == NULL) {
        raiz = criaNoIngles(palavraIngles, unidade);
        printf("Inserindo nova palavra na ABB: %s (Unidade: %s)\n", palavraIngles, unidade);
    } else if (strcmp(palavraIngles, raiz->palavra) < 0) {
        raiz->esq = insereIngles(raiz->esq, palavraIngles, unidade);
    } else if (strcmp(palavraIngles, raiz->palavra) > 0) {
        raiz->dir = insereIngles(raiz->dir, palavraIngles, unidade);
    } else {
        raiz->unidades = insereUnidade(raiz->unidades, unidade);
        printf("Palavra '%s' já existe na ABB. Unidade '%s' adicionada.\n", palavraIngles, unidade);
    }
    return raiz;
}




// Função para buscar uma palavra em ingles na ABB
NoPalavrasUnidadesInglesABB* buscarPalavraInglesABB(NoPalavrasUnidadesInglesABB* raiz, char* palavra) {
    NoPalavrasUnidadesInglesABB* resultado = NULL;

    if (raiz != NULL) {
        if (strcmp(raiz->palavra, palavra) == 0) {
            resultado = raiz;
        } else if (strcmp(palavra, raiz->palavra) < 0) {
            resultado = buscarPalavraInglesABB(raiz->esq, palavra);
        } else {
            resultado = buscarPalavraInglesABB(raiz->dir, palavra);
        }
    }

    return resultado;
}


//Função para Remover uma palavra em inglês na Árvore Binária
NoPalavrasUnidadesInglesABB* removePalavraInglesABB(NoPalavrasUnidadesInglesABB* raiz, char* palavraIngles, char* unidade) {
    if (raiz != NULL) {
        if (strcmp(palavraIngles, raiz->palavra) < 0) {
            raiz->esq = removePalavraInglesABB(raiz->esq, palavraIngles, unidade);
        } else if (strcmp(palavraIngles, raiz->palavra) > 0) {
            raiz->dir = removePalavraInglesABB(raiz->dir, palavraIngles, unidade);
        } else {
            printf("Removendo unidade '%s' da palavra '%s'.\n", unidade, raiz->palavra);
            raiz->unidades = removeUnidade(raiz->unidades, unidade);

            // Se a lista de unidades ficar vazia, remover o nó da ABB
            if (raiz->unidades == NULL) {
                printf("Lista de unidades vazia. Removendo o nó '%s'.\n", raiz->palavra);

                // Caso nó folha
                if (ehfolha(raiz)) {
                    free(raiz);
                    raiz = NULL;
                }
                // Caso com um único filho
                else if (unicoFilho(raiz)) {
                    NoPalavrasUnidadesInglesABB* temp = (raiz->esq != NULL) ? raiz->esq : raiz->dir;
                    free(raiz);
                    raiz = temp;
                }
                // Caso com dois filhos
                else if (doisFilhos(raiz)) {
                    char* maiorPalavraEsq = maiorPalavra(raiz->esq);
                    printf("Substituindo '%s' por '%s'.\n", raiz->palavra, maiorPalavraEsq);
                    strcpy(raiz->palavra, maiorPalavraEsq);
                    raiz->unidades = removeUnidade(raiz->unidades, unidade);
                    raiz->esq = removePalavraInglesABB(raiz->esq, maiorPalavraEsq, unidade);
                }
            }
        }
    }
    return raiz;
}


// Função auxiliar para imprimir palavras em inglês na ABB associada

void imprimeABB_EmOrdem(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimeABB_EmOrdem(raiz->esq);

        // Imprime a palavra em inglês e as unidades associadas
        printf("Palavra em Ingles: %s\n", raiz->palavra);
        printf("Unidades associadas: ");
        NoUnidade* atual = raiz->unidades;
        while (atual != NULL) {
            printf("%s ", atual->unidade);
            atual = atual->prox;
        }
        printf("\n");

        // Percorre a subárvore direita
        imprimeABB_EmOrdem(raiz->dir);
    }
}

// Função para percorrer a árvore 2-3 e imprimir as ABBs associadas
void imprimeArv23ABB(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimeArv23ABB(raiz->esq);

        // Imprime a ABB associada à chave Info1
        printf("Palavra em Português: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Percorre a subárvore central
        imprimeArv23ABB(raiz->cen);

        // Imprime a ABB associada à chave Info2, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Português: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a subárvore direita
        imprimeArv23ABB(raiz->dir);
    }
}


//Função para imprimir todas as palavras em inglês na ABB
void imprimeABB_Ingles(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        static int first = 1; // Flag para evitar vírgula no início
        imprimeABB_Ingles(raiz->esq);
        if (!first) {
            printf(", ");
        }
        printf("%s", raiz->palavra);
        first = 0;
        imprimeABB_Ingles(raiz->dir);
    }
}



// Função para imprimir a árvore de palavras em inglês a partir da árvore 2-3
void imprimeArv23_Ingles(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        imprimeArv23_Ingles(raiz->esq);
        if (raiz->arvIngles != NULL) {
            imprimeABB_Ingles(raiz->arvIngles);
        }
        imprimeArv23_Ingles(raiz->cen);
        if (raiz->numeroChaves == 2 && raiz->arvIngles != NULL) {
            printf(", ");
            imprimeABB_Ingles(raiz->arvIngles);
        }
        imprimeArv23_Ingles(raiz->dir);
    }
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

                                                          //////BLOCO ÁRVORE 2-3(palavras português)


//Função para criar um novo nó da Árvore 2-3
NoPalavrasPortuguesArv23* criaNo(char* chave, NoPalavrasPortuguesArv23* esq, NoPalavrasPortuguesArv23* cen) {
    NoPalavrasPortuguesArv23* novoNo = (NoPalavrasPortuguesArv23*)malloc(sizeof(NoPalavrasPortuguesArv23));
    
    if (novoNo != NULL) {
        strcpy(novoNo->Info1, chave);
        novoNo->Info2[0] = '\0'; // Chave direita vazia
        novoNo->numeroChaves = 1; // Inicialmente, apenas uma chave
        novoNo->esq = esq;
        novoNo->cen = cen;
        novoNo->dir = NULL; // Sem filho direito ainda
        novoNo->arvIngles = NULL; // Sem ABB associada inicialmente
    } else {
        fprintf(stderr, "Erro ao alocar memória para o novo nó.\n");
    }

    printf("Criado novo nó com Info1=%s\n", chave);
    return novoNo;
}


// Função para adicionar uma nova chave ao nó
NoPalavrasPortuguesArv23* adicionaChave(NoPalavrasPortuguesArv23* no, char* palavra, NoPalavrasPortuguesArv23* subarvore) {
    if (no->numeroChaves == 1) {
        if (strcmp(palavra, no->Info1) < 0) {
            // Move a Info1 para Info2 e insere a nova chave como Info1
            strcpy(no->Info2, no->Info1);
            strcpy(no->Info1, palavra);
            no->dir = no->cen;
            no->cen = subarvore;
        } else {
            // Insere a nova chave como Info2
            strcpy(no->Info2, palavra);
            no->dir = subarvore;
        }
        no->numeroChaves = 2;
    }

    printf("Chave adicionada ao nó: Info1=%s, Info2=%s\n", no->Info1, no->numeroChaves == 2 ? no->Info2 : "(vazio)");
    return no;
}


//Função quebraNo
//Função que quebra um nó da árvore 2-3 quando ele está cheio e precisa ser dividido
NoPalavrasPortuguesArv23* quebraNo(NoPalavrasPortuguesArv23** Raiz, char* palavra, char** sobe, NoPalavrasPortuguesArv23* subarvore) {
    NoPalavrasPortuguesArv23* novoNo = NULL;
    char* centro = NULL;

    if (*Raiz != NULL) {
        if (strcmp(palavra, (*Raiz)->Info2) > 0) {
            centro = (*Raiz)->Info2;
            novoNo = criaNo(palavra, (*Raiz)->dir, subarvore);
            (*Raiz)->Info2[0] = '\0';
            (*Raiz)->numeroChaves = 1;
            (*Raiz)->dir = NULL;
        } else if (strcmp(palavra, (*Raiz)->Info1) > 0) {
            centro = palavra;
            novoNo = criaNo((*Raiz)->Info2, subarvore, (*Raiz)->dir);
            (*Raiz)->Info2[0] = '\0';
            (*Raiz)->numeroChaves = 1;
            (*Raiz)->dir = NULL;
        } else {
            centro = (*Raiz)->Info1;
            novoNo = criaNo((*Raiz)->Info2, (*Raiz)->cen, (*Raiz)->dir);
            strcpy((*Raiz)->Info1, palavra);
            (*Raiz)->Info2[0] = '\0';
            (*Raiz)->numeroChaves = 1;
            (*Raiz)->cen = subarvore;
            (*Raiz)->dir = NULL;
        }

        if (sobe != NULL) {
            *sobe = centro;
        }
    } else {
        fprintf(stderr, "Erro: Ponteiro para a raiz é NULL na quebra de nó.\n");
    }

    return novoNo;
}



// Função que verifica se o nó é folha na árvore 2-3
int isLeaf(NoPalavrasPortuguesArv23 *no) {
    return (no->esq == NULL && no->cen == NULL && no->dir == NULL);
}

//Função para inserir uma nova palavra na árvore 2-3
NoPalavrasPortuguesArv23* insereArv23(NoPalavrasPortuguesArv23** no, char* palavra, char** sobe, NoPalavrasPortuguesArv23** pai) {
    NoPalavrasPortuguesArv23* MaiorNo = NULL;
    char* chavePromovida = NULL;

    // Caso base: o nó atual é nulo (inserção em um nó vazio)
    if (*no == NULL) {
        *no = criaNo(palavra, NULL, NULL);
    } else {
        // Verifica duplicação de palavras nas chaves Info1 e Info2
        if (strcmp(palavra, (*no)->Info1) == 0 || (strcmp(palavra, (*no)->Info2) == 0 && (*no)->numeroChaves == 2)) {
            printf("Palavra já existente: %s\n", palavra);
        } else {
            // Inserção em um nó folha
            if ((*no)->numeroChaves == 1) {
                // Navega para a subárvore esquerda ou central
                if (strcmp(palavra, (*no)->Info1) < 0) {
                    MaiorNo = insereArv23(&((*no)->esq), palavra, &chavePromovida, no);
                } else {
                    MaiorNo = insereArv23(&((*no)->cen), palavra, &chavePromovida, no);
                }
            } else {
                // Inserção em um nó com duas chaves
                if (strcmp(palavra, (*no)->Info1) < 0) {
                    MaiorNo = insereArv23(&((*no)->esq), palavra, &chavePromovida, no);
                } else if (strcmp(palavra, (*no)->Info2) < 0) {
                    MaiorNo = insereArv23(&((*no)->cen), palavra, &chavePromovida, no);
                } else {
                    MaiorNo = insereArv23(&((*no)->dir), palavra, &chavePromovida, no);
                }
            }

            // Ajusta a árvore após a inserção
            if (MaiorNo != NULL) {
                if ((*no)->numeroChaves == 1) {
                    *no = adicionaChave(*no, chavePromovida, MaiorNo);
                } else {
                    MaiorNo = quebraNo(no, chavePromovida, sobe, MaiorNo);
                    if (*pai == NULL) {
                        *no = criaNo(*sobe, *no, MaiorNo);
                        MaiorNo = NULL;
                    }
                }
            }
        }
    }

    return MaiorNo;
}




//Funções relacionadas a remoção da árvore 2-3

// Função para encontrar o nó com a menor chave/palavra
NoPalavrasPortuguesArv23* encontrarMinimo(NoPalavrasPortuguesArv23 *No) {
    // Continua descendo à esquerda até encontrar o nó mais à esquerda
    while (No != NULL && No->esq != NULL) {
        No = No->esq;
    }
    return No;
}

/*
O sucessor de uma chave em uma Árvore 2-3 é o menor valor na subárvore central ou direita do nó.
Se estamos procurando o sucessor de Info1, o sucessor está no menor valor da subárvore central.
Se estivermos removendo a Info2, o sucessor está na subárvore direita.
*/
// Função para encontrar o sucessor
NoPalavrasPortuguesArv23* encontrarSucessor(NoPalavrasPortuguesArv23 *No) {
    if (No == NULL) return NULL;
    
    NoPalavrasPortuguesArv23 *atual = No->cen; // O sucessor de Info1 está na subárvore central
    if (No->numeroChaves == 2) {
        atual = No->dir; // Se o nó tem duas chaves, o sucessor de Info2 está na subárvore direita
    }

    // Encontrar o menor valor na subárvore
    while (atual != NULL && atual->esq != NULL) {
        atual = atual->esq;
    }

    return atual;
}

/*A fusão/junção de nós ocorre quando um nó perde uma chave e fica com 0 chaves, o que é proibido na Árvore 2-3. 
Quando isso acontece, é necessário fundir o nó com um irmão.
*/
//Função para juntar nós após remoção
void fundirNos(NoPalavrasPortuguesArv23* raiz, NoPalavrasPortuguesArv23* Info1, NoPalavrasPortuguesArv23* Info2) {
    // Validação inicial dos ponteiros
    if (raiz == NULL || Info1 == NULL || Info2 == NULL) {
        fprintf(stderr, "Atencao: Um ou mais nos sao nulos.\n");
        
    }

    printf("Iniciando fusao de nos...\n");
    printf("Raiz: %s, Info1: %s, Info2: %s\n", raiz->Info1, Info1->Info1, Info2->Info1);

    // Caso 1: Filho esquerdo e central
    if (raiz->esq == Info1 && raiz->cen == Info2) {
        strcpy(Info1->Info2, raiz->Info1);
        Info1->numeroChaves = 2;
        Info1->cen = Info2->esq;
        Info1->dir = Info2->cen;
        strcpy(raiz->Info1, raiz->Info2);
        raiz->numeroChaves--;
        raiz->cen = raiz->dir;
        raiz->dir = NULL;
    }
    // Caso 2: Filho central e direito
    else if (raiz->cen == Info1 && raiz->dir == Info2) {
        strcpy(Info1->Info2, raiz->Info2);
        Info1->numeroChaves = 2;
        Info1->cen = Info2->esq;
        Info1->dir = Info2->cen;
        raiz->numeroChaves--;
        raiz->dir = NULL;
    }
    // Caso 3: Filho esquerdo, central e direito (nó completo)
    else if (raiz->esq == Info1 && raiz->dir == Info2) {
        strcpy(Info1->Info2, raiz->Info1);
        Info1->numeroChaves = 2;
        Info1->cen = Info2->esq;
        Info1->dir = Info2->cen;
        raiz->numeroChaves--;
        raiz->cen = raiz->dir;
        raiz->dir = NULL;
    }

    printf("Fusao concluída. No Info1 atualizado: %s, %s\n", Info1->Info1, Info1->Info2);

    // Libera o espaço de memória de Info2
    free(Info2);
}

/*
A função ajustarNo é responsável por corrigir a estrutura de um nó na árvore 2-3 após a remoção de uma chave, redistribuindo as chaves entre o nó, 
o pai e o irmão.
*/
void ajustarNo(NoPalavrasPortuguesArv23* pai, NoPalavrasPortuguesArv23* no, NoPalavrasPortuguesArv23* irmao) {
    if (pai->numeroChaves == 1) {
        // Caso em que o pai tem apenas uma chave e dois filhos (esquerdo e central)
        if (pai->esq == no) {
            // No é o filho esquerdo, irmao é o central
            strcpy(no->Info2, pai->Info1); // Move a chave do pai para o nó
            strcpy(pai->Info1, irmao->Info1); // Move a chave do irmão para o pai
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        } else if (pai->cen == no) {
            // No é o filho central, irmao é o esquerdo
            strcpy(no->Info2, pai->Info1);
            strcpy(pai->Info1, irmao->Info2);
            irmao->Info2[0] = '\0';
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        }
    } else if (pai->numeroChaves == 2) {
        // Caso em que o pai tem duas chaves e três filhos (esquerdo, central e direito)
        if (pai->esq == no) {
            strcpy(no->Info2, pai->Info1);
            strcpy(pai->Info1, irmao->Info1);
            strcpy(irmao->Info1, irmao->Info2);
            irmao->Info2[0] = '\0';
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        } else if (pai->cen == no) {
            if (pai->dir != NULL && pai->dir->numeroChaves == 2) {
                strcpy(no->Info2, pai->Info2);
                strcpy(pai->Info2, pai->dir->Info1);
                strcpy(pai->dir->Info1, pai->dir->Info2);
                pai->dir->Info2[0] = '\0';
                no->numeroChaves = 2;
                pai->dir->numeroChaves = 1;
            } else {
                strcpy(no->Info2, pai->Info1);
                strcpy(pai->Info1, irmao->Info2);
                irmao->Info2[0] = '\0';
                no->numeroChaves = 2;
                irmao->numeroChaves = 1;
            }
        } else if (pai->dir == no) {
            strcpy(no->Info2, pai->Info2);
            strcpy(pai->Info2, irmao->Info2);
            irmao->Info2[0] = '\0';
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        }
    }
}


// Função para remover a chave de um nó folha
void removeNoFolha(NoPalavrasPortuguesArv23* no, char* palavra) {
    printf("Removendo a palavra %s de um no folha.\n", palavra);

    if (strcmp(palavra, no->Info1) == 0) {
        strcpy(no->Info1, no->Info2);
    }

    no->Info2[0] = '\0';
    no->numeroChaves--;

    printf("Nó atualizado após remoção: Info1=%s, Info2=%s, Chaves=%d\n", no->Info1, no->Info2, no->numeroChaves);
}

// Função principal de remoção
void removerArv23(NoPalavrasPortuguesArv23** raiz, char* palavra, NoPalavrasPortuguesArv23* pai) {
    NoPalavrasPortuguesArv23* no = *raiz;
    NoPalavrasPortuguesArv23* sucessor = NULL;

    if (no != NULL) {
        // **Caso 1: A palavra está no nó atual**
        if (strcmp(palavra, no->Info1) == 0 || strcmp(palavra, no->Info2) == 0) {
            // **Remoção de uma chave em uma folha**
            if (isLeaf(no)) {
                removeNoFolha(no, palavra);

                if (no->numeroChaves == 0) {
                    free(no);
                    *raiz = NULL;
                }
            } else {
                // **Remoção de uma chave em um nó interno**
                sucessor = (strcmp(palavra, no->Info1) == 0) ? encontrarMinimo(no->cen) : encontrarMinimo(no->dir);
                strcpy((strcmp(palavra, no->Info1) == 0) ? no->Info1 : no->Info2, sucessor->Info1);
                removerArv23((strcmp(palavra, no->Info1) == 0) ? &(no->cen) : &(no->dir), sucessor->Info1, no);
            }
        }
        // **Caso 2: Navegar pela árvore para encontrar a palavra**
        else if (strcmp(palavra, no->Info1) < 0) {
            removerArv23(&(no->esq), palavra, no);
        } else if (no->numeroChaves == 1 || strcmp(palavra, no->Info2) < 0) {
            removerArv23(&(no->cen), palavra, no);
        } else {
            removerArv23(&(no->dir), palavra, no);
        }

        // **Ajustar a árvore após a remoção**
        if (*raiz != NULL && (*raiz)->numeroChaves == 0) {
            NoPalavrasPortuguesArv23* irmao = (pai != NULL) ? ((pai->esq == *raiz) ? pai->cen : pai->esq) : NULL;
            if (irmao != NULL && irmao->numeroChaves == 2) {
                ajustarNo(pai, *raiz, irmao);
            } else {
                fundirNos(pai, *raiz, irmao);
            }
        }
    }
}

//Função para buscar uma palavra específica na árvore 2-3 de palavras em português
NoPalavrasPortuguesArv23* buscarNoArv23(NoPalavrasPortuguesArv23* raiz, char* palavra) {
    NoPalavrasPortuguesArv23* resultado = NULL;
    NoPalavrasPortuguesArv23* atual = raiz;

    while (atual != NULL && resultado == NULL) {
        if (strcmp(palavra, atual->Info1) == 0) {
            resultado = atual;
        } else if (atual->numeroChaves == 2 && strcmp(palavra, atual->Info2) == 0) {
            resultado = atual;
        } else if (strcmp(palavra, atual->Info1) < 0) {
            atual = atual->esq;
        } else if (atual->numeroChaves == 1 || strcmp(palavra, atual->Info2) < 0) {
            atual = atual->cen;
        } else {
            atual = atual->dir;
        }
    }

    return resultado;
}


// Função para imprimir palavras em português em ordem na árvore 2-3
void imprimeArv23_EmOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimeArv23_EmOrdem(raiz->esq);

        // Imprime a chave esquerda
        printf("Palavra em Português: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Traduções associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Percorre a subárvore central
        imprimeArv23_EmOrdem(raiz->cen);

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Português: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Traduções associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a subárvore direita
        imprimeArv23_EmOrdem(raiz->dir);
    }
}

// Função para imprimir palavras em português em pré-ordem na árvore 2-3
void imprimeArv23_PreOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Imprime a chave esquerda
        printf("Palavra em Português: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Traduções associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Português: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Traduções associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a subárvore esquerda
        imprimeArv23_PreOrdem(raiz->esq);

        // Percorre a subárvore central
        imprimeArv23_PreOrdem(raiz->cen);

        // Percorre a subárvore direita
        imprimeArv23_PreOrdem(raiz->dir);
    }
}

// Função para imprimir palavras em português em pós-ordem na árvore 2-3
void imprimeArv23_PosOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a subárvore esquerda
        imprimeArv23_PosOrdem(raiz->esq);

        // Percorre a subárvore central
        imprimeArv23_PosOrdem(raiz->cen);

        // Percorre a subárvore direita
        imprimeArv23_PosOrdem(raiz->dir);

        // Imprime a chave esquerda
        printf("Palavra em Português: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Traduções associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Português: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Traduções associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }
    }
}

// Função para imprimir todas as palavras em português na árvore 2-3
void imprimeArv23_Portugues(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        imprimeArv23_Portugues(raiz->esq);
        printf("%s", raiz->Info1);
        if (raiz->cen != NULL || (raiz->dir == NULL && raiz->numeroChaves == 1)) {
            printf(", ");
        }
        imprimeArv23_Portugues(raiz->cen);
        if (raiz->numeroChaves == 2) {
            printf(", %s", raiz->Info2);
        }
        imprimeArv23_Portugues(raiz->dir);
    }
}



// Função para liberar a memória da árvore 2-3
void liberarArv23(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Libera subárvores
        liberarArv23(raiz->esq);
        liberarArv23(raiz->cen);
        liberarArv23(raiz->dir);

        // Libera a ABB associada
        liberarABB(raiz->arvIngles);

        // Libera o nó atual
        printf("Liberando no com Info1: %s\n", raiz->Info1);
        if (raiz->numeroChaves == 2) {
            printf("Liberando no com Info2: %s\n", raiz->Info2);
        }
        free(raiz);
    }
}

// Função para limpar espaços corretamente
void limparEspacos(char* str) {
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) *fim-- = '\0'; // Remove espaços à direita
    while (*str && isspace(*str)) str++;             // Remove espaços à esquerda
}


                                                     ////////FUNÇÕES ESPECÍFICAS PARA O MENU NA MAIN
                                                       
// Função de cadastro a partir de um arquivo txt
// Função para cadastrar palavras e suas traduções corretamente na árvore 2-3 e ABB associada
void cadastrarPalavra(NoPalavrasPortuguesArv23** raiz, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    char linha[TAM];
    char unidadeAtual[TAM] = "";
    char palavraIngles[TAM];
    char palavrasPortugues[TAM];

    if (arquivo != NULL) {
        printf("Carregando vocabulário inicial do arquivo...\n");
        while (fgets(linha, TAM, arquivo) != NULL) {
            linha[strcspn(linha, "\n")] = '\0'; // Remove o '\n' no final da linha

            if (linha[0] == '%') {
                strcpy(unidadeAtual, linha + 2);
                limparEspacos(unidadeAtual);
                printf("Unidade atual: %s\n", unidadeAtual);
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

                        NoPalavrasPortuguesArv23* novoNo = NULL;
                        insereArv23(raiz, palavraPortugues, NULL, &novoNo);

                        NoPalavrasPortuguesArv23* noAtual = buscarNoArv23(*raiz, palavraPortugues);
                        if (noAtual != NULL) {
                            noAtual->arvIngles = insereIngles(noAtual->arvIngles, palavraIngles, unidadeAtual);
                        }

                        palavraPortugues = strtok(NULL, ",");
                    }
                }
            }
        }

        fclose(arquivo);
        printf("Vocabulário carregado com sucesso.\n");
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
}

//(i) informar uma unidade e então imprima todas as palavras da unidade em português seguida das equivalentes em inglês;
//função auxiliar de busca de unidade na ABB
/*void buscarNaArvoreBinaria(NoPalavrasUnidadesInglesABB* raiz, char* unidade, char* palavraPortugues) {
    if (raiz == NULL) return;

    buscarNaArvoreBinaria(raiz->esq, unidade, palavraPortugues);

    NoUnidade* unidadeAtual = raiz->unidades;
    while (unidadeAtual != NULL) {
        if (strcmp(unidadeAtual->unidade, unidade) == 0) {
            printf("  - Palavra em Inglês: %s (Unidade: %s)\n", raiz->palavra, unidadeAtual->unidade);
        }
        unidadeAtual = unidadeAtual->prox;
    }

    buscarNaArvoreBinaria(raiz->dir, unidade, palavraPortugues);
}
*/

// Função para imprimir palavras de uma unidade específica na árvore 2-3
void imprimePorUnidade(NoPalavrasPortuguesArv23* raiz, const char* unidade) {
    if (raiz == NULL) return;

    // Percorre a subárvore esquerda
    imprimePorUnidade(raiz->esq, unidade);

    // Imprime as traduções associadas à Info1
    if (raiz->Info1[0] != '\0') {
        NoPalavrasUnidadesInglesABB* atualIngles = raiz->arvIngles;
        int encontrou = 0;
        
        while (atualIngles != NULL) {
            NoUnidade* unidadeLista = atualIngles->unidades;
            while (unidadeLista != NULL) {
                if (strcmp(unidadeLista->unidade, unidade) == 0) {
                    if (!encontrou) {
                        printf("\nPalavra em Portugues: %s\n", raiz->Info1);
                        encontrou = 1;
                    }
                    printf("  - Ingles: %s\n", atualIngles->palavra);
                }
                unidadeLista = unidadeLista->prox;
            }
            atualIngles = atualIngles->esq ? atualIngles->esq : atualIngles->dir;
        }
    }

    // Percorre a subárvore central
    imprimePorUnidade(raiz->cen, unidade);

    // Imprime as traduções associadas à Info2, se existir
    if (raiz->numeroChaves == 2 && raiz->Info2[0] != '\0') {
        NoPalavrasUnidadesInglesABB* atualIngles = raiz->arvIngles;
        int encontrou = 0;

        while (atualIngles != NULL) {
            NoUnidade* unidadeLista = atualIngles->unidades;
            while (unidadeLista != NULL) {
                if (strcmp(unidadeLista->unidade, unidade) == 0) {
                    if (!encontrou) {
                        printf("\nPalavra em Portugues: %s\n", raiz->Info2);
                        encontrou = 1;
                    }
                    printf("  - Ingles: %s\n", atualIngles->palavra);
                }
                unidadeLista = unidadeLista->prox;
            }
            atualIngles = atualIngles->esq ? atualIngles->esq : atualIngles->dir;
        }
    }

    // Percorre a subárvore direita
    imprimePorUnidade(raiz->dir, unidade);
}

//(ii)informar uma palavra em português e então imprima todas as palavras em inglês equivalente a palavra em português dada, independente da unidade.
// Função para converter uma string para minúsculas e remover espaços extras
void normalizarString(char* str) {
    int i = 0, j = 0;
    int len = strlen(str);

    // Remove espaços no início
    while (isspace(str[i])) {
        i++;
    }

    // Converte para minúsculas e remove espaços internos
    while (i < len) {
        if (!isspace(str[i]) || (j > 0 && !isspace(str[j - 1]))) {
            str[j++] = tolower(str[i]);
        }
        i++;
    }

    // Remove espaços no final
    if (j > 0 && isspace(str[j - 1])) {
        j--;
    }

    str[j] = '\0';
}

// Função para imprimir traduções em inglês evitando vírgula inicial
int contarPalavrasABB(NoPalavrasUnidadesInglesABB* arvore) {
    if (arvore == NULL) return 0;
    return 1 + contarPalavrasABB(arvore->esq) + contarPalavrasABB(arvore->dir);
}

void imprimeTraducoesABB_Ingles(NoPalavrasUnidadesInglesABB* arvore) {
    int quantidadePalavras = contarPalavrasABB(arvore);
    static int contador = 0;

    if (arvore != NULL) {
        // Percorre a subárvore esquerda
        imprimeTraducoesABB_Ingles(arvore->esq);

        // Incrementa o contador de palavras
        contador++;

        // Imprime a palavra atual com vírgula e espaço antes, se houver mais de uma palavra
        if (quantidadePalavras > 1 && contador > 1) {
            printf(", ");
        }

        printf("%s ", arvore->palavra);

        // Percorre a subárvore direita
        imprimeTraducoesABB_Ingles(arvore->dir);
    }

    // Reseta o contador ao final
    if (arvore == NULL) {
        contador = 0;
    }
}

// Função para buscar uma palavra em português na árvore 2-3 e imprimir suas traduções em inglês
void buscaPorPalavraArv23(NoPalavrasPortuguesArv23* raiz, char* palavra) {
    normalizarString(palavra);

    if (raiz == NULL) {
        printf("Palavra em Português não encontrada: %s\n", palavra);
        return;
    }

    NoPalavrasPortuguesArv23* atual = raiz;
    int encontrada = 0;

    while (atual != NULL && !encontrada) {
        char info1Normalizada[TAM];
        char info2Normalizada[TAM];

        strcpy(info1Normalizada, atual->Info1);
        normalizarString(info1Normalizada);

        if (atual->numeroChaves == 2) {
            strcpy(info2Normalizada, atual->Info2);
            normalizarString(info2Normalizada);
        }

        if (strcmp(palavra, info1Normalizada) == 0) {
            printf("\nPalavra em Português: %s\n", atual->Info1);
            printf("Traduções em Inglês: ");
            imprimeTraducoesABB_Ingles(atual->arvIngles);
            printf("\n");
            encontrada = 1;
        } else if (atual->numeroChaves == 2 && strcmp(palavra, info2Normalizada) == 0) {
            printf("\nPalavra em Português: %s\n", atual->Info2);
            printf("Traduções em Inglês: ");
            imprimeTraducoesABB_Ingles(atual->arvIngles);
            printf("\n");
            encontrada = 1;
        } else {
            if (strcmp(palavra, info1Normalizada) < 0) {
                atual = atual->esq;
            } else if (atual->numeroChaves == 1 || strcmp(palavra, info2Normalizada) < 0) {
                atual = atual->cen;
            } else {
                atual = atual->dir;
            }
        }
    }

    if (!encontrada) {
        printf("Palavra em Português não encontrada: %s\n", palavra);
    }
}

/*
(iii)informar uma palavra em inglês e a unidade a qual a mesma pertence remove-la das árvores binárias das quais ela pertence. 
Caso ela seja a única palavra em uma das árvores binárias, remover também da árvore 2-3
 */
// Função para remover um nó com palavra em inglês na ABB e arvore 2-3 
void removerPalavraInglesUnidade(NoPalavrasPortuguesArv23** raiz, char* palavraIngles, char* unidade) {
    NoPalavrasPortuguesArv23* noAtual = *raiz;
    int removidoDaABB = 0;

    while (noAtual != NULL) {
        // Verifica a ABB associada à chave esquerda
        if (noAtual->Info1[0] != '\0' && noAtual->arvIngles != NULL) {
            printf("Verificando ABB associada a palavra em portugues: %s\n", noAtual->Info1);
            noAtual->arvIngles = removePalavraInglesABB(noAtual->arvIngles, palavraIngles, unidade);

            // Se a ABB ficar vazia, remover o nó da árvore 2-3
            if (noAtual->arvIngles == NULL) {
                printf("ABB associada a palavra '%s' ficou vazia. Removendo da arvore 2-3.\n", noAtual->Info1);
                removerArv23(raiz, noAtual->Info1, NULL);
                removidoDaABB = 1;
            }
        }

        // Verifica a ABB associada à chave direita, se existir
        if (!removidoDaABB && noAtual->numeroChaves == 2 && noAtual->Info2[0] != '\0' && noAtual->arvIngles != NULL) {
            printf("Verificando ABB associada a palavra em portugues: %s\n", noAtual->Info2);
            noAtual->arvIngles = removePalavraInglesABB(noAtual->arvIngles, palavraIngles, unidade);

            // Se a ABB ficar vazia, remover o nó da árvore 2-3
            if (noAtual->arvIngles == NULL) {
                printf("ABB associada a palavra '%s' ficou vazia. Removendo da árvore 2-3.\n", noAtual->Info2);
                removerArv23(raiz, noAtual->Info2, NULL);
            }
        }

        // Continua percorrendo a árvore 2-3
        if (!removidoDaABB) {
            if (strcmp(palavraIngles, noAtual->Info1) < 0) {
                noAtual = noAtual->esq;
            } else if (noAtual->numeroChaves == 1 || strcmp(palavraIngles, noAtual->Info2) < 0) {
                noAtual = noAtual->cen;
            } else {
                noAtual = noAtual->dir;
            }
        } else {
            noAtual = NULL; // Finaliza o loop se a palavra foi removida
        }
    }

    if (!removidoDaABB) {
        printf("Palavra '%s' nao encontrada na arvore 2-3.\n", palavraIngles);
    }
}

/* 
(iv)informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto deve remover a palavra em inglês da árvore binária 
correspondente a palavra em português da mesma unidade. 
Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da árvore 2-3. 
*/
//Função para remover palavras em português 
void removerPalavraPortugues(NoPalavrasPortuguesArv23** raiz, char* palavraPortugues, char* unidade) {
    NoPalavrasPortuguesArv23* atual = *raiz;
    int palavraEncontrada = 0;

    while (atual != NULL && !palavraEncontrada) {
        if (strcmp(palavraPortugues, atual->Info1) < 0) {
            atual = atual->esq;
        } else if (atual->numeroChaves == 1 || strcmp(palavraPortugues, atual->Info2) < 0) {
            atual = atual->cen;
        } else if (atual->numeroChaves == 2 && strcmp(palavraPortugues, atual->Info2) > 0) {
            atual = atual->dir;
        } else {
            // Verifica se a palavra corresponde a Info1 ou Info2
            if (strcmp(palavraPortugues, atual->Info1) == 0 || 
                (atual->numeroChaves == 2 && strcmp(palavraPortugues, atual->Info2) == 0)) {

                printf("Removendo palavra em portugues: %s\n", palavraPortugues);
                atual->arvIngles = removePalavraInglesABB(atual->arvIngles, palavraPortugues, unidade);

                // Se a ABB associada ficar vazia, remove o nó da árvore 2-3
                if (atual->arvIngles == NULL) {
                    if (strcmp(palavraPortugues, atual->Info1) == 0) {
                        printf("Removendo no da arvore 2-3: %s\n", atual->Info1);
                        removerArv23(raiz, atual->Info1, NULL);
                    } else if (atual->numeroChaves == 2 && strcmp(palavraPortugues, atual->Info2) == 0) {
                        printf("Removendo no da arvore 2-3: %s\n", atual->Info2);
                        removerArv23(raiz, atual->Info2, NULL);
                    }
                }
                palavraEncontrada = 1;
            }
        }
    }

    if (!palavraEncontrada) {
        printf("Palavra em portugues nao encontrada: %s\n", palavraPortugues);
    }
}


                                                           ////////FUNÇÕES PARA O EXPERIMENTO

// Função para buscar uma palavra na árvore 2-3 e imprimir o caminho percorrido
void buscarPalavraComCaminho(NoPalavrasPortuguesArv23* raiz, char* palavra, int* encontrado) {
    NoPalavrasPortuguesArv23* atual = raiz;
    printf("Caminho: ");

    while (atual != NULL) {
        printf("[%s", atual->Info1);
        if (atual->numeroChaves == 2) {
            printf(", %s", atual->Info2);
        }
        printf("] -> ");

        if (strcmp(palavra, atual->Info1) == 0) {
            *encontrado = 1;
            atual = NULL;
        } else if (atual->numeroChaves == 2 && strcmp(palavra, atual->Info2) == 0) {
            *encontrado = 1;
            atual = NULL;
        } else {
            if (strcmp(palavra, atual->Info1) < 0) {
                atual = atual->esq;
            } else if (atual->numeroChaves == 1 || strcmp(palavra, atual->Info2) < 0) {
                atual = atual->cen;
            } else {
                atual = atual->dir;
            }
        }
    }

    if (*encontrado) {
        printf("Palavra encontrada!\n");
    } else {
        printf("Palavra não encontrada.\n");
    }
}

// Função principal para executar o experimento
void experimentoBusca(Arv23 raiz) {
    printf("\n=== Experimento de Busca na Árvore 2-3 ===\n");
    clock_t inicio, fim;
    double tempoGasto;

    // Busca em ordem
    printf("\n--- Busca em Ordem ---\n");
    inicio = clock();
    imprimeArv23_EmOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pré-ordem
    printf("\n--- Busca em Pré-Ordem ---\n");
    inicio = clock();
    imprimeArv23_PreOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pós-ordem
    printf("\n--- Busca em Pós-Ordem ---\n");
    inicio = clock();
    imprimeArv23_PosOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);
}


// Função para exibir o menu principal
void exibirMenu() {
    printf("\n==== MENU PRINCIPAL ====\n");
    printf("1 - Cadastrar palavra e unidade\n");
    printf("2 - Informar uma unidade e imprimir palavras (Portugues -> Ingles)\n");
    printf("3 - Buscar uma palavra em Portugues e listar equivalentes em Ingles\n");
    printf("4 - Remover uma palavra em Ingles e a unidade a qual pertence\n");
    printf("5 - Remover uma palavra em Portugues e suas palavras associadas\n");
    printf("6 - Imprimir toda a arvore 2-3 (Portugues)\n");
    printf("7 - Imprimir toda a arvore binaria (ingles)\n");
    printf("8 - Experimento: busca de 30 palavras em portugues\n");
    printf("0 - Sair\n");
    printf("=======================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    NoPalavrasPortuguesArv23* arvorePortugues = NULL;
    char nomeArquivo[] = "C:/Users/MEU CAMINHO DO ARQUIVO/vocabulario.txt";
    int opcao;
    char unidade[TAM];
    char palavra[TAM];

    setlocale(LC_ALL, ""); // Suporte a UTF-8

    // Loop do menu principal
    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            fprintf(stderr, "Erro: entrada inválida.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }
        getchar(); // Limpar o buffer do teclado

        switch (opcao) {
            case 1:  // Cadastrar nova palavra e unidade a partir de um arquivo txt
                printf("Carregando vocabulario inicial do arquivo...\n");
			    cadastrarPalavra(&arvorePortugues, nomeArquivo);
			    printf("Vocabulario carregado com sucesso!\n");
                break;

            case 2:  // Informar uma unidade e imprimir palavras
			    printf("Informe a unidade: ");
			    fgets(unidade, TAM, stdin);
			    unidade[strcspn(unidade, "\n")] = '\0';
			    printf("\nPalavras da Unidade: %s\n", unidade);
			    imprimePorUnidade(arvorePortugues, unidade);
			    break;

            case 3:  // Buscar uma palavra em Português
                printf("Informe a palavra em Português: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                buscaPorPalavraArv23(arvorePortugues, palavra);
                printf("\b\b.\n"); // Corrige a vírgula extra no final
                break;

            case 4:  // Remover uma palavra em Inglês e a unidade
                printf("Informe a palavra em Inglês: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0';
                removerPalavraInglesUnidade(&arvorePortugues, palavra, unidade);
                break;

            case 5:  // Remover uma palavra em Português
                printf("Informe a palavra em Português: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0';
                removerPalavraPortugues(&arvorePortugues, palavra, unidade);
                break;

            case 6:  // Imprimir toda a árvore 2-3
				printf("\nImprimindo a árvore 2-3 (Português):\n");
			    imprimeArv23_Portugues(arvorePortugues);
			    printf("\b\b.\n"); // Corrige a vírgula extra no final
			    break;
            
            case 7:
			    printf("\nImprimindo a árvore binária de palavras em inglês:\n");
			    imprimeArv23_Ingles(arvorePortugues);
			    printf(".\n");
			    break;
			
			case 8:
			    cadastrarPalavra(&arvorePortugues, nomeArquivo);
    			experimentoBusca(arvorePortugues);
			    break;

            case 0:  // Sair do programa
                printf("\nEncerrando o programa...\n");
                liberarArv23(arvorePortugues);
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}



