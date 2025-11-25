#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "expressao.h"
#include <string.h>
#define _USE_MATH_DEFINES

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


typedef struct no{
    float dado;
    struct no *proximo;
} No;

typedef struct{
    No *topo;
}Pilha;

void iniciar(Pilha *p){
    p->topo = NULL;
}

void empilhar(Pilha *p, float valor){
    No *novo = (No*) malloc(sizeof(No));

    if(novo == NULL){
        printf("Erro ao alocar memoria!\n");
        return;
    }

    novo->dado = valor;
    novo->proximo = p->topo;
    p->topo = novo;
}

float desempilhar(Pilha *p){
    if(p->topo == NULL){
        printf("tem algo errado ai patrao\n");
        return 0;
    }
    No * del = p->topo;
    float valor = del->dado;

    p->topo = del->proximo;
    free(del);

    return valor;
}

float getValorPosFixa(char *StrPosFixa){
    Pilha p;
    iniciar(&p);

    char copia[512];
    strcpy(copia, StrPosFixa);

    char *tok = strtok(copia, " ");
    while (tok != NULL){
        if((tok[0] >= '0' && tok[0] <= '9') || tok[0] == '.'){
            float valor = atof(tok);
            empilhar(&p, valor);
        }else if(strcmp(tok, "+") == 0 || strcmp(tok, "-") == 0 ||
                strcmp(tok, "*") == 0 || strcmp(tok, "/") == 0 ||
                strcmp(tok, "%") == 0 || strcmp(tok, "^") == 0){

            float b = desempilhar(&p);
            float a = desempilhar(&p);
            float r = 0;

            if(strcmp(tok, "+") == 0) r = a + b;
            if(strcmp(tok, "-") == 0) r = a - b;
            if(strcmp(tok, "*") == 0) r = a * b;
            if(strcmp(tok, "/") == 0) r = a / b;
            if(strcmp(tok, "%") == 0) r = fmod(a, b);
            if(strcmp(tok, "^") == 0) r = pow(a, b);

            empilhar(&p, r);
        
        }else if(strcmp(tok, "sen") == 0 ||
                strcmp(tok, "cos") == 0 ||
                strcmp(tok, "tg") == 0  ||
                strcmp(tok, "log") == 0 ||
                strcmp(tok, "raiz") == 0){

            float a = desempilhar(&p);
            float r = 0;

            if(strcmp(tok, "sen") == 0)  r = sin(a * M_PI / 180.0);
            if(strcmp(tok, "cos") == 0)  r = cos(a * M_PI / 180.0);
            if(strcmp(tok, "tg")  == 0)  r = tan(a * M_PI / 180.0);
            if(strcmp(tok, "log") == 0)  r = log10(a);
            if(strcmp(tok, "raiz") == 0) r = sqrt(a);

            empilhar(&p, r);
        }
                tok = strtok(NULL, " ");
    }
        return desempilhar(&p);
}

typedef struct noS{
    char dado[512];
    struct noS *proximo;
} NoS;

typedef struct {
    NoS *topo;
} Pilhas;

void iniciarS(Pilhas *p){
    p->topo = NULL;
}

void empilharS(Pilhas *p, char *valor){
    NoS *novo = (NoS *) malloc(sizeof(NoS));
    if(novo == NULL){
        printf("Deu ruim chefia!\n");
        return;
    }

    strcpy(novo->dado, valor);
    novo->proximo = p->topo;
    p->topo = novo;
}

char *desempilharS(Pilhas *p){
    if(p->topo == NULL)
        return NULL;

    NoS *del = p->topo;

    // copia a string real para uma nova área
    char *valor = malloc(strlen(del->dado) + 1);
    strcpy(valor, del->dado);

    p->topo = del->proximo;
    free(del);

    return valor;
}


char * getFormaInFixa(char *Str){
    static char resultado[512];
    Pilhas p;
    iniciarS(&p);

    char copia[512];
    strcpy(copia, Str);

    char *tok = strtok(copia, " ");

    while(tok != NULL){
        if((tok[0] >= '0' && tok[0] <= '9') || tok[0] == '.'){
            empilharS(&p, tok);
        }else if(strcmp(tok, "+") == 0 || strcmp(tok, "-") == 0 ||
                strcmp(tok, "*") == 0 || strcmp(tok, "/") == 0 ||
                strcmp(tok, "%") == 0 || strcmp(tok, "^") == 0){

            char *b = desempilharS(&p);
            char *a = desempilharS(&p);

            if(a == NULL || b == NULL)
                return NULL;

            char nova[512];
            sprintf(nova, "(%s%s%s)", a, tok, b);

            empilharS(&p, nova);
        }else if(strcmp(tok, "sen") == 0 || strcmp(tok, "cos") == 0 ||
                strcmp(tok, "tg")  == 0 || strcmp(tok, "log") == 0 ||
                strcmp(tok, "raiz") == 0){

            char *a = desempilharS(&p);
            if(a == NULL)
                return NULL;

            char nova[512];
            sprintf(nova, "%s(%s)", tok, a);

            empilharS(&p, nova);
        }else {
            return NULL;
        }

        tok = strtok(NULL, " ");
    }
    char *resp = desempilharS(&p);
    if(resp == NULL)
        return NULL;

    strcpy(resultado, resp);
    return resultado;
}
