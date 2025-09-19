#include "lstr.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
    struct no* ant;
    struct no* prox;
    str string;
} no;

struct lstr{
    no* primeiro;
    no* ultimo;
    no* corrente;
    int tam;
    int pos;
};

Lstr ls_cria(){
    Lstr new = malloc(sizeof(struct lstr));
    new->primeiro = NULL;
    new->ultimo = NULL;
    new->corrente = NULL;
    new->tam = 0;
    new->pos = -1;
    return new;
}

void ls_destroi(Lstr self){
    if(ls_vazia(self)){ 
        free(self);
        return;
    }
    ls_inicio(self);
    while(self->corrente->prox != NULL)
        ls_remove(self);
    free(self);
}

bool ls_vazia(Lstr self){
    return (self->primeiro==NULL)?1:0;
}

int ls_tam(Lstr self){
    return self->tam;
}

bool ls_item_valido(Lstr self){
    return (ls_vazia(self) || self->pos <= -1 || self->pos >= self->tam)? 0 : 1;
}

str ls_item(Lstr self){
    assert(ls_item_valido(self));
    return self->corrente->string;
}

void ls_inicio(Lstr self){
    self->corrente = NULL;
    printf("corrente = null\n");
    self->pos = -1;
    printf("pos = -1\n");
}

void ls_fim(Lstr self){
    self->corrente = NULL;
    self->pos = self->tam;
}

static void pos_depois_fim(Lstr self){
    // no *antTemp;
    // self->pos = self->tam;
    // self->corrente = NULL;
    // self->corrente->prox = NULL;
        
}

void ls_posiciona(Lstr self, int pos){
    if(pos<0) pos += self->tam;
    if(pos<0){
        ls_inicio(self);
        return;
    }
    if(pos>=self->tam){
        ls_fim(self);
        return;
    }
    int begin = (pos > (self->tam - pos))?1:0;
    if(begin == 0){
        ls_inicio(self);
        while(self->pos != pos)
            ls_avanca(self);
    }else{
        ls_fim(self);
        while(self->pos != pos)
            ls_recua(self);
    }
    self->pos = pos;
}

bool ls_avanca(Lstr self){
    if(self->pos == self->tam) return 0;
    self->pos += 1;
    s_imprime(self->corrente->string);
    printf("chegou no pos +1\n");
    self->corrente = (self->corrente->prox != NULL)?self->corrente->prox:NULL;
    printf("chegou no self->corrente\n");
    return (self->corrente == NULL)?0:1;
}

bool ls_recua(Lstr self){
    if(self->pos == -1) return 0;
    self->pos -= 1;
    self->corrente = (self->corrente->ant != NULL)?self->corrente->ant:NULL;
    return (self->corrente == NULL)?0:1;
}

void ls_insere_antes(Lstr self, str cad){
    if(ls_vazia(self)){
        printf("Chegou no lista vazia\n");
        no* novo = malloc(sizeof(no));
        printf("Chegou no malloc\n");
        novo->ant = NULL;
        printf("Chegou no ant\n");
        novo->prox = NULL;
        printf("Chegou no prox\n");
        novo->string = s_copia(cad);
        printf("Chegou no string\n");
        self->primeiro = novo;
        printf("Chegou no primeiro\n");
        self->ultimo = novo;
        printf("Chegou no ultimo\n");
        self->pos = 0;
        printf("Chegou no pos\n");
        self->tam += 1;
        printf("Chegou no tam\n");
        return;
    }
    printf("Passou do lista vazia no insere\n");
    no* proximo;
    no* anterior;
    if(self->pos <= 0){
        proximo = self->primeiro;
        anterior = NULL;
    }else{
        proximo = self->corrente;
        anterior = self->corrente->ant;
    }
    no* novo = malloc(sizeof(no));
    novo->ant = anterior;
    novo->prox = proximo;
    novo->string = s_copia(cad);
    // apagar o cad? acho que nao?
    if(self->pos <= 0){
        self->primeiro->ant = novo;
        self->primeiro = novo;
        self->pos = 0;
    }else{
        self->corrente->ant->prox = novo;
        proximo->ant = novo;
        ls_avanca(self);
    }
    self->tam += 1;
}

void ls_insere_depois(Lstr self, str cad){

}

str ls_remove(Lstr self){

}

Lstr ls_sublista(Lstr self, int tam){

}

str ls_junta(Lstr self, str separador){

}

void ls_imprime(Lstr self){
    if(self->tam == 0) return;
    for(ls_inicio(self);ls_avanca(self);){
        printf("%d\n",self->pos);
        s_imprime(self->corrente->string);
    }
}

int main(){
    printf("Inicio da main\n");
    Lstr lista = ls_cria();
    printf("Chegou no ls cria\n");
    ls_insere_antes(lista,s_("abacaxi"));
    // ls_insere_antes(lista,s_("berinjela"));
    // ls_insere_antes(lista,s_("cacto"));
    ls_imprime(lista);
}