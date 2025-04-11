#include <stdio.h>
#include <stdlib.h>
#include "trabalho.h"

int main() {
    Processo processos[numerodeProcessos];
    int tamanho = 0;
    int opcao;

    printf("Sistema de Análise de Processos da Justiça Eleitoral\n");
    printf("==================================================\n\n");

    // Carregar dados do arquivo CSV
    printf("Carregando dados do arquivo 'processo_043_202409032338.csv'...\n");
    if (!ler_csv("processo_043_202409032338.csv", processos, &tamanho)) {
        printf("Erro ao ler o arquivo CSV.\n");
        return 1;
    }
    printf("Total de processos carregados: %d\n\n", tamanho);

    do {
        printf("\nOperações disponíveis:\n");
        printf("1. Ordenar por ID (crescente) e salvar\n");
        printf("2. Ordenar por data de ajuizamento (decrescente) e salvar\n");
        printf("3. Contar processos por classe\n");
        printf("4. Listar quantidade de assuntos únicos\n");
        printf("5. Listar processos com múltiplos assuntos\n");
        printf("6. Calcular dias em tramitação\n");
        printf("0. Sair\n");
        printf("\nEscolha uma operação: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpar buffer
            printf("Opção inválida!\n");
            continue;
        }

        printf("\n");

        switch (opcao) {
            case 1:
                printf("Ordenando por ID em ordem crescente...\n");
                IdCrescente(processos, tamanho);
                salvar_csv("processosOrdenadosId.csv", processos, tamanho);
                break;

            case 2:
                printf("Ordenando por data em ordem decrescente...\n");
                dataDecrescente(processos, tamanho);
                salvar_csv("processosOrdenadosData.csv", processos, tamanho);
                break;

            case 3: {
                int id_classe;
                printf("Digite o ID da classe para contagem: ");
                scanf("%d", &id_classe);
                int total = contar_por_classe(processos, tamanho, id_classe);
                printf("Total de processos com ID de classe %d: %d\n", id_classe, total);
                break;
            }

            case 4:
                printf("Identificando assuntos únicos...\n");
                listarIdAssuntosUnicos(processos, tamanho);
                break;

            case 5:
                printf("Listando processos com múltiplos assuntos...\n");
                listar_processos_multiplos_assuntos(processos, tamanho);
                break;

            case 6:
                printf("Calculando dias em tramitação...\n");
                dias_em_tramitacao(processos, tamanho);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpar buffer
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
