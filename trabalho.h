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

// Carrega processos de um arquivo CSV
int carregarProcessosDoArquivo(const char *nome_arquivo, Processo *processo, int *tamanho);

// Ordena processos por ID em ordem crescente
void ordenarProcessosPorId(Processo *processos, int qtd);

// Ordena processos por data de ajuizamento em ordem decrescente
void ordenarPorDataAjuizamento(Processo *vetor, int tamanho);

// Conta quantidade de processos de uma determinada classe
int contarProcessosClasse(Processo *lista, int tamanho, int id_classe);

// Exibe lista de assuntos únicos e suas quantidades
void mostrarAssuntosUnicos(Processo *vetor, int tamanho);

// Exibe processos que possuem mais de um assunto
void exibirProcessosMultiplosAssuntos(Processo *lista, int tamanho);

// Calcula e exibe o tempo de tramitação de cada processo
void calcularTempoTramitacao(Processo *processos, int tamanho);

// Salva lista de processos em arquivo CSV
void salvarProcessosEmArquivo(const char *nome_arquivo, Processo *processos, int tamanho);

// Compara datas de ajuizamento
int compararDataAjuizamento(const char *d1, const char *d2);

#endif
