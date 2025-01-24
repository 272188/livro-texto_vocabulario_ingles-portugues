/*
                                                                           �RVORE 2-3
Cada unidade em um livro-texto em ingl�s cont�m um vocabul�rio Ingl�s-Portugu�s de palavras que s�o usadas pela primeira vez em uma unidade em particular. 
Desejamos converter um conjunto de tais vocabul�rios armazenados em um arquivo Ingl�s em um conjunto de vocabul�rios Portugu�s-Ingl�s. 
Para isso considere que: 
(a) Os nomes das unidades s�o precedidos por um s�mbolo de porcentagem. 
(b) H� somente uma entrada por linha. 
(c) Uma palavra em Ingl�s � separada por dois pontos de sua(s) equivalente(s) portuguesa(s); se h� mais de 
uma equivalente, elas s�o separadas por v�rgula. 
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
 
Para produzir as palavras em ordem alfab�tica, crie uma �rvore 2-3 contendo todas as palavras em portugu�s de todas as unidades e uma �rvore 
bin�ria de busca com as equivalentes em ingl�s e em cada n� da lista encadeada contenha tamb�m a unidade que pertence aquela palavra em ingl�s. 
Assegure-se de que exista apenas um n� para cada palavra em portugu�s na �rvore. 
Al�m disso, permita ao usu�rio: 
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>


#define TAM 100

// N� para representar uma unidade em uma lista encadeada
typedef struct NoUnidade {
    char unidade[TAM];                 // Nome da unidade
    struct NoUnidade* prox;            // Ponteiro para a pr�xima unidade
} NoUnidade;

// N� para palavras em ingl�s (ABB), com lista de unidades associadas e tradu��es em portugu�s
typedef struct NoPalavrasUnidadesInglesABB {
    char palavra[TAM];                 // Palavra em ingl�s
    NoUnidade* unidades;               // Lista encadeada de unidades
    struct NoPalavrasUnidadesInglesABB* esq, *dir; // Filhos da ABB
} NoPalavrasUnidadesInglesABB;

// N� para palavras em portugu�s (na �rvore 2-3)
typedef struct NoPalavrasPortuguesArv23{
    char Info1[TAM];  // chave esquerda
    char Info2[TAM];  // chave direita
    int numeroChaves;       // N�mero de chaves no n� (1 ou 2)
    struct NoPalavrasPortuguesArv23 *esq, *cen, *dir; // Filhos a �rvore 2-3
    NoPalavrasUnidadesInglesABB *arvIngles; // �rvore bin�ria associada
} NoPalavrasPortuguesArv23;

typedef NoPalavrasPortuguesArv23* Arv23;
typedef NoPalavrasUnidadesInglesABB* ABB;

 
                                                             //BLOCO LISTA ENCADEADA PARA UNIDADES
															    

//Fun��o para criar uma nova unidade na lista encadeada																    
NoUnidade* criaUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* nova = (NoUnidade*)malloc(sizeof(NoUnidade));
    if (nova!= NULL) {
    	strcpy(nova->unidade, unidade);
    	nova->prox = lista;
	}
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
        atual = atual->prox;
    }

    if (!existe) {
        lista = criaUnidade(lista, unidade); // Chama a fun��o para criar uma nova unidade
        printf("Inserindo nova unidade: %s\n", unidade);
    } else {
        printf("Unidade ja existente: %s\n", unidade);
    }

    return lista;
}

//Fun��o para buscar uma unidade na lista encadeada
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


//Fun��o para remover uma unidade na lista encadeada
NoUnidade* removeUnidade(NoUnidade* lista, char* unidade) {
    NoUnidade* atual = lista;
    NoUnidade* anterior = NULL;
    NoUnidade* novaLista = lista;

    while (atual != NULL) {
        if (strcmp(atual->unidade, unidade) == 0) {
            if (anterior == NULL) {
                // Se a unidade a ser removida est� no in�cio da lista
                novaLista = atual->prox;
            } else {
                // Ajusta o ponteiro do n� anterior para ignorar o n� removido
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
        printf("Unidade n�o encontrada para remo��o: %s\n", unidade);
    }

    return novaLista;
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


                                                    ////BLOCO �RVORE BIN�RIA DE BUSCA - ABB(palavras Ingl�s)


// Fun��o para criar um novo n� de palavras na �rvore bin�ria de busca - ABB (palavras em ingl�s)
NoPalavrasUnidadesInglesABB* criaNoIngles(char* palavra, char* unidade) {
    NoPalavrasUnidadesInglesABB* novo = (NoPalavrasUnidadesInglesABB*)malloc(sizeof(NoPalavrasUnidadesInglesABB));
    if (novo != NULL) {
        strcpy(novo->palavra, palavra);
	    novo->unidades = criaUnidade(NULL, unidade);
	    novo->esq = novo->dir = NULL;
    }
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
        printf("Palavra '%s' j� existe na ABB. Unidade '%s' adicionada.\n", palavraIngles, unidade);
    }
    return raiz;
}




// Fun��o para buscar uma palavra em ingles na ABB
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


//Fun��o para Remover uma palavra em ingl�s na �rvore Bin�ria
NoPalavrasUnidadesInglesABB* removePalavraInglesABB(NoPalavrasUnidadesInglesABB* raiz, char* palavraIngles, char* unidade) {
    if (raiz != NULL) {
        if (strcmp(palavraIngles, raiz->palavra) < 0) {
            raiz->esq = removePalavraInglesABB(raiz->esq, palavraIngles, unidade);
        } else if (strcmp(palavraIngles, raiz->palavra) > 0) {
            raiz->dir = removePalavraInglesABB(raiz->dir, palavraIngles, unidade);
        } else {
            printf("Removendo unidade '%s' da palavra '%s'.\n", unidade, raiz->palavra);
            raiz->unidades = removeUnidade(raiz->unidades, unidade);

            // Se a lista de unidades ficar vazia, remover o n� da ABB
            if (raiz->unidades == NULL) {
                printf("Lista de unidades vazia. Removendo o n� '%s'.\n", raiz->palavra);

                // Caso n� folha
                if (ehfolha(raiz)) {
                    free(raiz);
                    raiz = NULL;
                }
                // Caso com um �nico filho
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


// Fun��o auxiliar para imprimir palavras em ingl�s na ABB associada

void imprimeABB_EmOrdem(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimeABB_EmOrdem(raiz->esq);

        // Imprime a palavra em ingl�s e as unidades associadas
        printf("Palavra em Ingles: %s\n", raiz->palavra);
        printf("Unidades associadas: ");
        NoUnidade* atual = raiz->unidades;
        while (atual != NULL) {
            printf("%s ", atual->unidade);
            atual = atual->prox;
        }
        printf("\n");

        // Percorre a sub�rvore direita
        imprimeABB_EmOrdem(raiz->dir);
    }
}

// Fun��o para percorrer a �rvore 2-3 e imprimir as ABBs associadas
void imprimeArv23ABB(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimeArv23ABB(raiz->esq);

        // Imprime a ABB associada � chave Info1
        printf("Palavra em Portugu�s: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Percorre a sub�rvore central
        imprimeArv23ABB(raiz->cen);

        // Imprime a ABB associada � chave Info2, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Portugu�s: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a sub�rvore direita
        imprimeArv23ABB(raiz->dir);
    }
}


//Fun��o para imprimir todas as palavras em ingl�s na ABB
void imprimeABB_Ingles(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        static int first = 1; // Flag para evitar v�rgula no in�cio
        imprimeABB_Ingles(raiz->esq);
        if (!first) {
            printf(", ");
        }
        printf("%s", raiz->palavra);
        first = 0;
        imprimeABB_Ingles(raiz->dir);
    }
}



// Fun��o para imprimir a �rvore de palavras em ingl�s a partir da �rvore 2-3
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


//Fun��o para Liberar a �rvore Bin�ria de Busca (ABB) de Palavras em Ingl�s
void liberarABB(NoPalavrasUnidadesInglesABB* raiz) {
    if (raiz != NULL) {
        liberarABB(raiz->esq); // Libera a sub�rvore esquerda
        liberarABB(raiz->dir); // Libera a sub�rvore direita
        liberarListaUnidades(raiz->unidades); // Libera a lista de unidades
        free(raiz); // Libera o n� atual
    }
}

                                                          //////BLOCO �RVORE 2-3(palavras portugu�s)


//Fun��o para criar um novo n� da �rvore 2-3
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
        fprintf(stderr, "Erro ao alocar mem�ria para o novo n�.\n");
    }

    printf("Criado novo n� com Info1=%s\n", chave);
    return novoNo;
}


// Fun��o para adicionar uma nova chave ao n�
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

    printf("Chave adicionada ao n�: Info1=%s, Info2=%s\n", no->Info1, no->numeroChaves == 2 ? no->Info2 : "(vazio)");
    return no;
}


//Fun��o quebraNo
//Fun��o que quebra um n� da �rvore 2-3 quando ele est� cheio e precisa ser dividido
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
        fprintf(stderr, "Erro: Ponteiro para a raiz � NULL na quebra de n�.\n");
    }

    return novoNo;
}



// Fun��o que verifica se o n� � folha na �rvore 2-3
int isLeaf(NoPalavrasPortuguesArv23 *no) {
    return (no->esq == NULL && no->cen == NULL && no->dir == NULL);
}

//Fun��o para inserir uma nova palavra na �rvore 2-3
NoPalavrasPortuguesArv23* insereArv23(NoPalavrasPortuguesArv23** no, char* palavra, char** sobe, NoPalavrasPortuguesArv23** pai) {
    NoPalavrasPortuguesArv23* MaiorNo = NULL;
    char* chavePromovida = NULL;

    // Caso base: o n� atual � nulo (inser��o em um n� vazio)
    if (*no == NULL) {
        *no = criaNo(palavra, NULL, NULL);
    } else {
        // Verifica duplica��o de palavras nas chaves Info1 e Info2
        if (strcmp(palavra, (*no)->Info1) == 0 || (strcmp(palavra, (*no)->Info2) == 0 && (*no)->numeroChaves == 2)) {
            printf("Palavra j� existente: %s\n", palavra);
        } else {
            // Inser��o em um n� folha
            if ((*no)->numeroChaves == 1) {
                // Navega para a sub�rvore esquerda ou central
                if (strcmp(palavra, (*no)->Info1) < 0) {
                    MaiorNo = insereArv23(&((*no)->esq), palavra, &chavePromovida, no);
                } else {
                    MaiorNo = insereArv23(&((*no)->cen), palavra, &chavePromovida, no);
                }
            } else {
                // Inser��o em um n� com duas chaves
                if (strcmp(palavra, (*no)->Info1) < 0) {
                    MaiorNo = insereArv23(&((*no)->esq), palavra, &chavePromovida, no);
                } else if (strcmp(palavra, (*no)->Info2) < 0) {
                    MaiorNo = insereArv23(&((*no)->cen), palavra, &chavePromovida, no);
                } else {
                    MaiorNo = insereArv23(&((*no)->dir), palavra, &chavePromovida, no);
                }
            }

            // Ajusta a �rvore ap�s a inser��o
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




//Fun��es relacionadas a remo��o da �rvore 2-3

// Fun��o para encontrar o n� com a menor chave/palavra
NoPalavrasPortuguesArv23* encontrarMinimo(NoPalavrasPortuguesArv23 *No) {
    // Continua descendo � esquerda at� encontrar o n� mais � esquerda
    while (No != NULL && No->esq != NULL) {
        No = No->esq;
    }
    return No;
}

/*
O sucessor de uma chave em uma �rvore 2-3 � o menor valor na sub�rvore central ou direita do n�.
Se estamos procurando o sucessor de Info1, o sucessor est� no menor valor da sub�rvore central.
Se estivermos removendo a Info2, o sucessor est� na sub�rvore direita.
*/
// Fun��o para encontrar o sucessor
NoPalavrasPortuguesArv23* encontrarSucessor(NoPalavrasPortuguesArv23 *No) {
    if (No == NULL) return NULL;
    
    NoPalavrasPortuguesArv23 *atual = No->cen; // O sucessor de Info1 est� na sub�rvore central
    if (No->numeroChaves == 2) {
        atual = No->dir; // Se o n� tem duas chaves, o sucessor de Info2 est� na sub�rvore direita
    }

    // Encontrar o menor valor na sub�rvore
    while (atual != NULL && atual->esq != NULL) {
        atual = atual->esq;
    }

    return atual;
}

/*A fus�o/jun��o de n�s ocorre quando um n� perde uma chave e fica com 0 chaves, o que � proibido na �rvore 2-3. 
Quando isso acontece, � necess�rio fundir o n� com um irm�o.
*/
//Fun��o para juntar n�s ap�s remo��o
void fundirNos(NoPalavrasPortuguesArv23* raiz, NoPalavrasPortuguesArv23* Info1, NoPalavrasPortuguesArv23* Info2) {
    // Valida��o inicial dos ponteiros
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
    // Caso 3: Filho esquerdo, central e direito (n� completo)
    else if (raiz->esq == Info1 && raiz->dir == Info2) {
        strcpy(Info1->Info2, raiz->Info1);
        Info1->numeroChaves = 2;
        Info1->cen = Info2->esq;
        Info1->dir = Info2->cen;
        raiz->numeroChaves--;
        raiz->cen = raiz->dir;
        raiz->dir = NULL;
    }

    printf("Fusao conclu�da. No Info1 atualizado: %s, %s\n", Info1->Info1, Info1->Info2);

    // Libera o espa�o de mem�ria de Info2
    free(Info2);
}

/*
A fun��o ajustarNo � respons�vel por corrigir a estrutura de um n� na �rvore 2-3 ap�s a remo��o de uma chave, redistribuindo as chaves entre o n�, 
o pai e o irm�o.
*/
void ajustarNo(NoPalavrasPortuguesArv23* pai, NoPalavrasPortuguesArv23* no, NoPalavrasPortuguesArv23* irmao) {
    if (pai->numeroChaves == 1) {
        // Caso em que o pai tem apenas uma chave e dois filhos (esquerdo e central)
        if (pai->esq == no) {
            // No � o filho esquerdo, irmao � o central
            strcpy(no->Info2, pai->Info1); // Move a chave do pai para o n�
            strcpy(pai->Info1, irmao->Info1); // Move a chave do irm�o para o pai
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        } else if (pai->cen == no) {
            // No � o filho central, irmao � o esquerdo
            strcpy(no->Info2, pai->Info1);
            strcpy(pai->Info1, irmao->Info2);
            irmao->Info2[0] = '\0';
            no->numeroChaves = 2;
            irmao->numeroChaves = 1;
        }
    } else if (pai->numeroChaves == 2) {
        // Caso em que o pai tem duas chaves e tr�s filhos (esquerdo, central e direito)
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


// Fun��o para remover a chave de um n� folha
void removeNoFolha(NoPalavrasPortuguesArv23* no, char* palavra) {
    printf("Removendo a palavra %s de um no folha.\n", palavra);

    if (strcmp(palavra, no->Info1) == 0) {
        strcpy(no->Info1, no->Info2);
    }

    no->Info2[0] = '\0';
    no->numeroChaves--;

    printf("N� atualizado ap�s remo��o: Info1=%s, Info2=%s, Chaves=%d\n", no->Info1, no->Info2, no->numeroChaves);
}

// Fun��o principal de remo��o
void removerArv23(NoPalavrasPortuguesArv23** raiz, char* palavra, NoPalavrasPortuguesArv23* pai) {
    NoPalavrasPortuguesArv23* no = *raiz;
    NoPalavrasPortuguesArv23* sucessor = NULL;

    if (no != NULL) {
        // **Caso 1: A palavra est� no n� atual**
        if (strcmp(palavra, no->Info1) == 0 || strcmp(palavra, no->Info2) == 0) {
            // **Remo��o de uma chave em uma folha**
            if (isLeaf(no)) {
                removeNoFolha(no, palavra);

                if (no->numeroChaves == 0) {
                    free(no);
                    *raiz = NULL;
                }
            } else {
                // **Remo��o de uma chave em um n� interno**
                sucessor = (strcmp(palavra, no->Info1) == 0) ? encontrarMinimo(no->cen) : encontrarMinimo(no->dir);
                strcpy((strcmp(palavra, no->Info1) == 0) ? no->Info1 : no->Info2, sucessor->Info1);
                removerArv23((strcmp(palavra, no->Info1) == 0) ? &(no->cen) : &(no->dir), sucessor->Info1, no);
            }
        }
        // **Caso 2: Navegar pela �rvore para encontrar a palavra**
        else if (strcmp(palavra, no->Info1) < 0) {
            removerArv23(&(no->esq), palavra, no);
        } else if (no->numeroChaves == 1 || strcmp(palavra, no->Info2) < 0) {
            removerArv23(&(no->cen), palavra, no);
        } else {
            removerArv23(&(no->dir), palavra, no);
        }

        // **Ajustar a �rvore ap�s a remo��o**
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

//Fun��o para buscar uma palavra espec�fica na �rvore 2-3 de palavras em portugu�s
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


// Fun��o para imprimir palavras em portugu�s em ordem na �rvore 2-3
void imprimeArv23_EmOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimeArv23_EmOrdem(raiz->esq);

        // Imprime a chave esquerda
        printf("Palavra em Portugu�s: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Tradu��es associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Percorre a sub�rvore central
        imprimeArv23_EmOrdem(raiz->cen);

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Portugu�s: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Tradu��es associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a sub�rvore direita
        imprimeArv23_EmOrdem(raiz->dir);
    }
}

// Fun��o para imprimir palavras em portugu�s em pr�-ordem na �rvore 2-3
void imprimeArv23_PreOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Imprime a chave esquerda
        printf("Palavra em Portugu�s: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Tradu��es associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Portugu�s: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Tradu��es associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }

        // Percorre a sub�rvore esquerda
        imprimeArv23_PreOrdem(raiz->esq);

        // Percorre a sub�rvore central
        imprimeArv23_PreOrdem(raiz->cen);

        // Percorre a sub�rvore direita
        imprimeArv23_PreOrdem(raiz->dir);
    }
}

// Fun��o para imprimir palavras em portugu�s em p�s-ordem na �rvore 2-3
void imprimeArv23_PosOrdem(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Percorre a sub�rvore esquerda
        imprimeArv23_PosOrdem(raiz->esq);

        // Percorre a sub�rvore central
        imprimeArv23_PosOrdem(raiz->cen);

        // Percorre a sub�rvore direita
        imprimeArv23_PosOrdem(raiz->dir);

        // Imprime a chave esquerda
        printf("Palavra em Portugu�s: %s\n", raiz->Info1);
        if (raiz->arvIngles != NULL) {
            printf("Tradu��es associadas:\n");
            imprimeABB_EmOrdem(raiz->arvIngles);
        }

        // Imprime a chave direita, se existir
        if (raiz->numeroChaves == 2) {
            printf("Palavra em Portugu�s: %s\n", raiz->Info2);
            if (raiz->arvIngles != NULL) {
                printf("Tradu��es associadas:\n");
                imprimeABB_EmOrdem(raiz->arvIngles);
            }
        }
    }
}

// Fun��o para imprimir todas as palavras em portugu�s na �rvore 2-3
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



// Fun��o para liberar a mem�ria da �rvore 2-3
void liberarArv23(NoPalavrasPortuguesArv23* raiz) {
    if (raiz != NULL) {
        // Libera sub�rvores
        liberarArv23(raiz->esq);
        liberarArv23(raiz->cen);
        liberarArv23(raiz->dir);

        // Libera a ABB associada
        liberarABB(raiz->arvIngles);

        // Libera o n� atual
        printf("Liberando no com Info1: %s\n", raiz->Info1);
        if (raiz->numeroChaves == 2) {
            printf("Liberando no com Info2: %s\n", raiz->Info2);
        }
        free(raiz);
    }
}

// Fun��o para limpar espa�os corretamente
void limparEspacos(char* str) {
    char* fim = str + strlen(str) - 1;
    while (fim > str && isspace(*fim)) *fim-- = '\0'; // Remove espa�os � direita
    while (*str && isspace(*str)) str++;             // Remove espa�os � esquerda
}


                                                     ////////FUN��ES ESPEC�FICAS PARA O MENU NA MAIN
                                                       
// Fun��o de cadastro a partir de um arquivo txt
// Fun��o para cadastrar palavras e suas tradu��es corretamente na �rvore 2-3 e ABB associada
void cadastrarPalavra(NoPalavrasPortuguesArv23** raiz, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    char linha[TAM];
    char unidadeAtual[TAM] = "";
    char palavraIngles[TAM];
    char palavrasPortugues[TAM];

    if (arquivo != NULL) {
        printf("Carregando vocabul�rio inicial do arquivo...\n");
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
        printf("Vocabul�rio carregado com sucesso.\n");
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
}

//(i) informar uma unidade e ent�o imprima todas as palavras da unidade em portugu�s seguida das equivalentes em ingl�s;
//fun��o auxiliar de busca de unidade na ABB
/*void buscarNaArvoreBinaria(NoPalavrasUnidadesInglesABB* raiz, char* unidade, char* palavraPortugues) {
    if (raiz == NULL) return;

    buscarNaArvoreBinaria(raiz->esq, unidade, palavraPortugues);

    NoUnidade* unidadeAtual = raiz->unidades;
    while (unidadeAtual != NULL) {
        if (strcmp(unidadeAtual->unidade, unidade) == 0) {
            printf("  - Palavra em Ingl�s: %s (Unidade: %s)\n", raiz->palavra, unidadeAtual->unidade);
        }
        unidadeAtual = unidadeAtual->prox;
    }

    buscarNaArvoreBinaria(raiz->dir, unidade, palavraPortugues);
}
*/

// Fun��o para imprimir palavras de uma unidade espec�fica na �rvore 2-3
void imprimePorUnidade(NoPalavrasPortuguesArv23* raiz, const char* unidade) {
    if (raiz == NULL) return;

    // Percorre a sub�rvore esquerda
    imprimePorUnidade(raiz->esq, unidade);

    // Imprime as tradu��es associadas � Info1
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

    // Percorre a sub�rvore central
    imprimePorUnidade(raiz->cen, unidade);

    // Imprime as tradu��es associadas � Info2, se existir
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

    // Percorre a sub�rvore direita
    imprimePorUnidade(raiz->dir, unidade);
}

//(ii)informar uma palavra em portugu�s e ent�o imprima todas as palavras em ingl�s equivalente a palavra em portugu�s dada, independente da unidade.
// Fun��o para converter uma string para min�sculas e remover espa�os extras
void normalizarString(char* str) {
    int i = 0, j = 0;
    int len = strlen(str);

    // Remove espa�os no in�cio
    while (isspace(str[i])) {
        i++;
    }

    // Converte para min�sculas e remove espa�os internos
    while (i < len) {
        if (!isspace(str[i]) || (j > 0 && !isspace(str[j - 1]))) {
            str[j++] = tolower(str[i]);
        }
        i++;
    }

    // Remove espa�os no final
    if (j > 0 && isspace(str[j - 1])) {
        j--;
    }

    str[j] = '\0';
}

// Fun��o para imprimir tradu��es em ingl�s evitando v�rgula inicial
int contarPalavrasABB(NoPalavrasUnidadesInglesABB* arvore) {
    if (arvore == NULL) return 0;
    return 1 + contarPalavrasABB(arvore->esq) + contarPalavrasABB(arvore->dir);
}

void imprimeTraducoesABB_Ingles(NoPalavrasUnidadesInglesABB* arvore) {
    int quantidadePalavras = contarPalavrasABB(arvore);
    static int contador = 0;

    if (arvore != NULL) {
        // Percorre a sub�rvore esquerda
        imprimeTraducoesABB_Ingles(arvore->esq);

        // Incrementa o contador de palavras
        contador++;

        // Imprime a palavra atual com v�rgula e espa�o antes, se houver mais de uma palavra
        if (quantidadePalavras > 1 && contador > 1) {
            printf(", ");
        }

        printf("%s ", arvore->palavra);

        // Percorre a sub�rvore direita
        imprimeTraducoesABB_Ingles(arvore->dir);
    }

    // Reseta o contador ao final
    if (arvore == NULL) {
        contador = 0;
    }
}

// Fun��o para buscar uma palavra em portugu�s na �rvore 2-3 e imprimir suas tradu��es em ingl�s
void buscaPorPalavraArv23(NoPalavrasPortuguesArv23* raiz, char* palavra) {
    normalizarString(palavra);

    if (raiz == NULL) {
        printf("Palavra em Portugu�s n�o encontrada: %s\n", palavra);
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
            printf("\nPalavra em Portugu�s: %s\n", atual->Info1);
            printf("Tradu��es em Ingl�s: ");
            imprimeTraducoesABB_Ingles(atual->arvIngles);
            printf("\n");
            encontrada = 1;
        } else if (atual->numeroChaves == 2 && strcmp(palavra, info2Normalizada) == 0) {
            printf("\nPalavra em Portugu�s: %s\n", atual->Info2);
            printf("Tradu��es em Ingl�s: ");
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
        printf("Palavra em Portugu�s n�o encontrada: %s\n", palavra);
    }
}

/*
(iii)informar uma palavra em ingl�s e a unidade a qual a mesma pertence remove-la das �rvores bin�rias das quais ela pertence. 
Caso ela seja a �nica palavra em uma das �rvores bin�rias, remover tamb�m da �rvore 2-3
 */
// Fun��o para remover um n� com palavra em ingl�s na ABB e arvore 2-3 
void removerPalavraInglesUnidade(NoPalavrasPortuguesArv23** raiz, char* palavraIngles, char* unidade) {
    NoPalavrasPortuguesArv23* noAtual = *raiz;
    int removidoDaABB = 0;

    while (noAtual != NULL) {
        // Verifica a ABB associada � chave esquerda
        if (noAtual->Info1[0] != '\0' && noAtual->arvIngles != NULL) {
            printf("Verificando ABB associada a palavra em portugues: %s\n", noAtual->Info1);
            noAtual->arvIngles = removePalavraInglesABB(noAtual->arvIngles, palavraIngles, unidade);

            // Se a ABB ficar vazia, remover o n� da �rvore 2-3
            if (noAtual->arvIngles == NULL) {
                printf("ABB associada a palavra '%s' ficou vazia. Removendo da arvore 2-3.\n", noAtual->Info1);
                removerArv23(raiz, noAtual->Info1, NULL);
                removidoDaABB = 1;
            }
        }

        // Verifica a ABB associada � chave direita, se existir
        if (!removidoDaABB && noAtual->numeroChaves == 2 && noAtual->Info2[0] != '\0' && noAtual->arvIngles != NULL) {
            printf("Verificando ABB associada a palavra em portugues: %s\n", noAtual->Info2);
            noAtual->arvIngles = removePalavraInglesABB(noAtual->arvIngles, palavraIngles, unidade);

            // Se a ABB ficar vazia, remover o n� da �rvore 2-3
            if (noAtual->arvIngles == NULL) {
                printf("ABB associada a palavra '%s' ficou vazia. Removendo da �rvore 2-3.\n", noAtual->Info2);
                removerArv23(raiz, noAtual->Info2, NULL);
            }
        }

        // Continua percorrendo a �rvore 2-3
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
(iv)informar uma palavra em portugu�s e a unidade a qual a mesma pertence e ent�o remove-la, para isto deve remover a palavra em ingl�s da �rvore bin�ria 
correspondente a palavra em portugu�s da mesma unidade. 
Caso ela seja a �nica palavra na �rvore bin�ria, a palavra em portugu�s deve ser removida da �rvore 2-3. 
*/
//Fun��o para remover palavras em portugu�s 
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

                // Se a ABB associada ficar vazia, remove o n� da �rvore 2-3
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


                                                           ////////FUN��ES PARA O EXPERIMENTO

// Fun��o para buscar uma palavra na �rvore 2-3 e imprimir o caminho percorrido
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
        printf("Palavra n�o encontrada.\n");
    }
}

// Fun��o principal para executar o experimento
void experimentoBusca(Arv23 raiz) {
    printf("\n=== Experimento de Busca na �rvore 2-3 ===\n");
    clock_t inicio, fim;
    double tempoGasto;

    // Busca em ordem
    printf("\n--- Busca em Ordem ---\n");
    inicio = clock();
    imprimeArv23_EmOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em pr�-ordem
    printf("\n--- Busca em Pr�-Ordem ---\n");
    inicio = clock();
    imprimeArv23_PreOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);

    // Busca em p�s-ordem
    printf("\n--- Busca em P�s-Ordem ---\n");
    inicio = clock();
    imprimeArv23_PosOrdem(raiz);
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nTempo gasto: %.3f ms\n", tempoGasto);
}


// Fun��o para exibir o menu principal
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
            fprintf(stderr, "Erro: entrada inv�lida.\n");
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

            case 3:  // Buscar uma palavra em Portugu�s
                printf("Informe a palavra em Portugu�s: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                buscaPorPalavraArv23(arvorePortugues, palavra);
                printf("\b\b.\n"); // Corrige a v�rgula extra no final
                break;

            case 4:  // Remover uma palavra em Ingl�s e a unidade
                printf("Informe a palavra em Ingl�s: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0';
                removerPalavraInglesUnidade(&arvorePortugues, palavra, unidade);
                break;

            case 5:  // Remover uma palavra em Portugu�s
                printf("Informe a palavra em Portugu�s: ");
                fgets(palavra, TAM, stdin);
                palavra[strcspn(palavra, "\n")] = '\0';
                printf("Informe a unidade: ");
                fgets(unidade, TAM, stdin);
                unidade[strcspn(unidade, "\n")] = '\0';
                removerPalavraPortugues(&arvorePortugues, palavra, unidade);
                break;

            case 6:  // Imprimir toda a �rvore 2-3
				printf("\nImprimindo a �rvore 2-3 (Portugu�s):\n");
			    imprimeArv23_Portugues(arvorePortugues);
			    printf("\b\b.\n"); // Corrige a v�rgula extra no final
			    break;
            
            case 7:
			    printf("\nImprimindo a �rvore bin�ria de palavras em ingl�s:\n");
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
                printf("\nOp��o inv�lida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}



