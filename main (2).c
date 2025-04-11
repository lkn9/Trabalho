// main.c
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "trabalho.h"

void escrever_arquivo_ordenado(const char* nome_arquivo, Processo* processos, int qtd) {
    // Primeiro tenta abrir o arquivo
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir arquivo '%s' para escrita: %s\n", nome_arquivo, strerror(errno));
        return;
    }

    // Escreve o cabeçalho
    fprintf(arquivo, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    fflush(arquivo);

    // Escreve os dados
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%lld,\"%s\",%s,{%d},{%d},%d\n",
                processos[i].Id, processos[i].Numero, processos[i].Ajuizamento,
                processos[i].Classe, processos[i].Assunto, processos[i].EleicaoAno);
        
        if (i % 1000 == 0) {
            fflush(arquivo);
            printf("Processados %d registros...\n", i);
        }
    }

    // Fecha o arquivo
    fclose(arquivo);
    
    // Verifica se o arquivo foi criado
    if (access(nome_arquivo, F_OK) != -1) {
        printf("Arquivo '%s' criado com sucesso.\n", nome_arquivo);
    } else {
        printf("Erro: arquivo '%s' não foi criado.\n", nome_arquivo);
    }
}

int main() {
    setlocale(LC_ALL, "");

    Processo processos[numerodeProcessos];
    int qtd = 0;

    printf("Lendo arquivo 'processo_043_202409032338.csv'...\n");
    if (!ler_csv("processo_043_202409032338.csv", processos, &qtd)) {
        printf("Erro ao ler o arquivo: %s\n", strerror(errno));
        return 1;
    }

    printf("Total de processos lidos: %d\n", qtd);

    int opcao;
    do {
        printf("\nBem vindo ao sistema!!\n");
        printf("1. Ordenar por ID (crescente)\n");
        printf("2. Ordenar por Data de Ajuizamento (decrescente)\n");
        printf("3. Contar processos por ID de classe\n");
        printf("4. Contar ID Assuntos\n");
        printf("5. Listar processos com múltiplos assuntos\n");
        printf("6. Mostrar dias em tramitação\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Erro na leitura da opção.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                printf("Ordenando por ID...\n");
                IdCrescente(processos, qtd);
                printf("Ordenação concluída. Salvando arquivo...\n");
                escrever_arquivo_ordenado("processosOrdenadosId.csv", processos, qtd);
                break;

            case 2:
                printf("Ordenando por data...\n");
                dataDecrescente(processos, qtd);
                printf("Ordenação concluída. Salvando arquivo...\n");
                escrever_arquivo_ordenado("processosOrdenadosData.csv", processos, qtd);
                break;

            case 3:
                contar_por_classe(processos, qtd);
                break;

            case 4:
                listarIdAssuntosUnicos(processos, qtd);
                break;

            case 5:
                listar_processos_multiplos_assuntos(processos, qtd);
                break;

            case 6:
                dias_em_tramitacao(processos, qtd);
                break;

            case 0:
                printf("Obrigado por usar nosso sistema!!\n");
                break;

            default:
                printf("Digite uma opção válida.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}
