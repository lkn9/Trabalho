// main.c
#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "trabalho.h"

int main() {
    setlocale(LC_ALL, "");

    Processo processos[numerodeProcessos];
    int qtd = 0;

    printf("Lendo arquivo 'dados.csv'...\n");
    if (!ler_csv("dados.csv", processos, &qtd)) {
        printf("Erro ao ler o arquivo.\n");
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
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                IdCrescente(processos, qtd);
                FILE *saidaId = fopen("processosOrdenadosId.csv", "w");
                if (!saidaId) {
                    printf("Erro ao criar o arquivo de saída.\n");
                    break;
                }
                fprintf(saidaId, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
                for (int i = 0; i < qtd; i++) {
                    fprintf(saidaId, "%lld,%s,%s,%d,%d,%d\n",
                        processos[i].Id, processos[i].Numero, processos[i].Ajuizamento,
                        processos[i].Classe, processos[i].Assunto, processos[i].EleicaoAno);
                }
                fclose(saidaId);
                printf("Dados salvos em 'processosOrdenadosId.csv'.\n");
                break;

            case 2:
                dataDecrescente(processos, qtd);
                FILE *saidaData = fopen("processosOrdenadosData.csv", "w");
                if (!saidaData) {
                    printf("Erro ao criar o arquivo de saída.\n");
                    break;
                }
                fprintf(saidaData, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
                for (int i = 0; i < qtd; i++) {
                    fprintf(saidaData, "%lld,%s,%s,%d,%d,%d\n",
                        processos[i].Id, processos[i].Numero, processos[i].Ajuizamento,
                        processos[i].Classe, processos[i].Assunto, processos[i].EleicaoAno);
                }
                fclose(saidaData);
                printf("Dados salvos em 'processosOrdenadosData.csv'.\n");
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