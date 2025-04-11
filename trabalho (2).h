// trabalho.h
#ifndef TRABALHO_H
#define TRABALHO_H

#define numerodeProcessos 20000

typedef struct {
    long long int Id;
    char Numero[32];
    char Ajuizamento[32];
    int Classe;
    int Assunto;
    int EleicaoAno;
} Processo;

int ler_csv(const char *nome_arquivo, Processo *processo, int *tamanho);
void IdCrescente(Processo *processos, int qtd);
void dataDecrescente(Processo *vetor, int tamanho);
void contar_por_classe(Processo *lista, int tamanho);
void listarIdAssuntosUnicos(Processo *vetor, int tamanho);
void listar_processos_multiplos_assuntos(Processo *lista, int tamanho);
void dias_em_tramitacao(Processo *processos, int tamanho);

#endif
