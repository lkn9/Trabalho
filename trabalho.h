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

// Função para ler dados do arquivo CSV
int ler_csv(const char *nome_arquivo, Processo *processo, int *tamanho);

// 1. Função para ordenar por ID em ordem crescente
void IdCrescente(Processo *processos, int qtd);

// 2. Função para ordenar por data em ordem decrescente
void dataDecrescente(Processo *vetor, int tamanho);

// 3. Função para contar processos por classe
int contar_por_classe(Processo *lista, int tamanho, int id_classe);

// 4. Função para listar assuntos únicos
void listarIdAssuntosUnicos(Processo *vetor, int tamanho);

// 5. Função para listar processos com múltiplos assuntos
void listar_processos_multiplos_assuntos(Processo *lista, int tamanho);

// 6. Função para calcular dias em tramitação
void dias_em_tramitacao(Processo *processos, int tamanho);

// Função auxiliar para salvar em CSV
void salvar_csv(const char *nome_arquivo, Processo *processos, int tamanho);

// Função auxiliar para comparar datas
int compararDatas(const char *d1, const char *d2);

#endif
