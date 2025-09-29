#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "lstr.h"
#include "str.h"

// ================= Harness =================

typedef struct {
    const char* test;
    const char* detail;
    const char* file;
    int line;
} Failure;

static Failure fails[1024];
static int n_fail = 0, n_checks = 0, n_tests = 0;
static const char* cur_test = "(none)";

static void record_fail(const char* file, int line, const char* fmt, ...) {
    if (n_fail >= 1024) return;
    Failure* f = &fails[n_fail++];
    f->test = cur_test;
    f->file = file;
    f->line = line;
    static char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    f->detail = strdup(buf);
    fprintf(stderr, "[FAIL] %s:%d %s: %s\n", file, line, cur_test, buf);
}

#define CHECK(cond, msg) do{ n_checks++; if(!(cond)) record_fail(__FILE__,__LINE__,"%s",msg);}while(0)
#define CHECK_STREQ(s,exp,ctx) do{ n_checks++; char* c=s_strc(s); if(strcmp(c,exp)!=0) record_fail(__FILE__,__LINE__,"%s exp=%s got=%s",ctx,exp,c); free(c);}while(0)
#define RUN_TEST(fn) do{ cur_test=#fn; n_tests++; fn(); }while(0)

// ================= Helpers =================

static void free_vec(char**v,int n){for(int i=0;i<n;i++)free(v[i]);free(v);}
static char** coleta_fwd(Lstr L,int*pn){
    int n=ls_tam(L); char**v=calloc(n?n:1,sizeof(char*)); int k=0;
    for(ls_inicio(L);ls_avanca(L);) v[k++]=s_strc(ls_item(L));
    *pn=k; return v;
}
static char** coleta_rev(Lstr L,int*pn){
    int n=ls_tam(L); char**v=calloc(n?n:1,sizeof(char*)); int k=0;
    ls_posiciona(L,ls_tam(L));
    while(ls_recua(L)) v[k++]=s_strc(ls_item(L));
    *pn=k; return v;
}

// ================= Tests =================

static void test_cria_vazia(){
    Lstr L=ls_cria();
    CHECK(ls_vazia(L),"nova lista deve ser vazia");
    CHECK(ls_tam(L)==0,"tam==0");
    CHECK(!ls_item_valido(L),"nenhum item válido");
    ls_destroi(L);
}

static void test_inserir_e_percorrer(){
    Lstr L=ls_cria();
    ls_insere_depois(L,s_("A"));
    ls_insere_depois(L,s_("B"));
    ls_insere_depois(L,s_("C"));
    CHECK(ls_tam(L)==3,"tam==3");
    int n; char**v=coleta_fwd(L,&n);
    CHECK(n==3,"3 itens");
    if(n==3){CHECK(strcmp(v[0],"A")==0,"[0]=A");CHECK(strcmp(v[1],"B")==0,"[1]=B");}
    free_vec(v,n);
    v=coleta_rev(L,&n);
    if(n==3){CHECK(strcmp(v[0],"C")==0,"rev[0]=C");}
    free_vec(v,n);
    ls_destroi(L);
}

static void test_posiciona(){
    const char* arr[]={"A","B","C","D","E"};
    Lstr L=ls_cria();
    for(int i=0;i<5;i++){ls_final(L);ls_insere_antes(L,s_((char*)arr[i]));}
    ls_posiciona(L,0); CHECK_STREQ(ls_item(L),"A","pos0");
    ls_posiciona(L,3); CHECK_STREQ(ls_item(L),"D","pos3");
    ls_posiciona(L,-1);CHECK_STREQ(ls_item(L),"E","pos-1");
    ls_posiciona(L,10);CHECK(!ls_item_valido(L),"pos>=tam inválido");
    ls_destroi(L);
}

static void test_remove(){
    Lstr L=ls_cria();
    ls_insere_depois(L,s_("A")); ls_insere_depois(L,s_("B")); ls_insere_depois(L,s_("C"));
    ls_posiciona(L,1);
    str r=ls_remove(L); CHECK_STREQ(r,"B","remove B");
    CHECK(ls_tam(L)==2,"tam==2");
    ls_destroi(L);
}

static void test_sublista(){
    Lstr L=ls_cria();
    ls_insere_depois(L,s_("a")); ls_insere_depois(L,s_("b")); ls_insere_depois(L,s_("c"));
    ls_posiciona(L,1); // em "b"
    Lstr S=ls_sublista(L,2);
    int n; char**v=coleta_fwd(S,&n);
    CHECK(n==2,"sublista=2");
    if(n==2){CHECK(strcmp(v[0],"b")==0,"S[0]=b");}
    free_vec(v,n);
    ls_destroi(S); ls_destroi(L);
}

static void test_junta_e_str(){
    Lstr L=ls_cria();
    ls_insere_depois(L,s_("AA")); ls_insere_depois(L,s_("BB"));
    str j=ls_junta(L,s_(","));
    CHECK_STREQ(j,"AA,BB","join");
    str ap=s_apara(s_("  teste. "),s_(" ."));
    CHECK_STREQ(ap,"teste","apara");
    ls_destroi(L);
}

// ================= Main =================

int main(){
    RUN_TEST(test_cria_vazia);
    RUN_TEST(test_inserir_e_percorrer);
    RUN_TEST(test_posiciona);
    RUN_TEST(test_remove);
    RUN_TEST(test_sublista);
    RUN_TEST(test_junta_e_str);

    printf("\n=== RELATÓRIO FINAL ===\n");
    printf("Testes: %d  Checks: %d  Falhas: %d\n",n_tests,n_checks,n_fail);
    if(n_fail){
        for(int i=0;i<n_fail;i++)
            printf(" %d) [%s] %s (%s:%d)\n",i+1,fails[i].test,fails[i].detail,fails[i].file,fails[i].line);
        printf("RESULTADO: FALHOU ❌\n");
    }else{
        printf("RESULTADO: SUCESSO ✔\n");
    }
}
