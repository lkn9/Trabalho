#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trabalho.h"

int ler_csv(const char *arquivo, Processo *processo, int *tamanho) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) return 0;
    printf("Arquivo '%s' aberto com sucesso.\n", arquivo);

    char linha[512];
    int linha_atual = 0;

    while (fgets(linha, sizeof(linha), fp)) {
        if (linha_atual++ == 0) continue; // Pula o cabeçalho

        Processo p;
        char *token;
        char *rest = linha;
        int campo = 0;

        // Remove aspas e quebra de linha
        char *newline = strchr(linha, '\n');
        if (newline) *newline = '\0';

        while ((token = strtok_r(rest, ",", &rest))) {
            // Remove aspas se presentes
            if (token[0] == '"') {
                token++;
                char *end_quote = strchr(token, '"');
                if (end_quote) *end_quote = '\0';
            }

            switch (campo) {
                case 0:
                    p.Id = atoll(token);
                    break;
                case 1:
                    strncpy(p.Numero, token, sizeof(p.Numero) - 1);
                    p.Numero[sizeof(p.Numero) - 1] = '\0';
                    break;
                case 2:
                    strncpy(p.Ajuizamento, token, sizeof(p.Ajuizamento) - 1);
                    p.Ajuizamento[sizeof(p.Ajuizamento) - 1] = '\0';
                    break;
                case 3: {
                    // Remove chaves e extrai número
                    char *start = strchr(token, '{');
                    char *end = strchr(token, '}');
                    if (start && end) {
                        *end = '\0';
                        p.Classe = atoi(start + 1);
                    }
                    break;
                }
                case 4: {
                    // Remove chaves e extrai número
                    char *start = strchr(token, '{');
                    char *end = strchr(token, '}');
                    if (start && end) {
                        *end = '\0';
                        p.Assunto = atoi(start + 1);
                    }
                    break;
                }
                case 5:
                    p.EleicaoAno = atoi(token);
                    break;
            }
            campo++;
        }

        if (campo == 6) { // Verifica se todos os campos foram lidos
            processo[(*tamanho)++] = p;
            if (*tamanho >= numerodeProcessos) break;
        }
    }

    fclose(fp);
    return 1;
}

void IdCrescente(Processo *processo, int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (processo[j].Id > processo[j + 1].Id) {
                Processo temp = processo[j];
                processo[j] = processo[j + 1];
                processo[j + 1] = temp;
            }
        }
    }
}

int compararDatas(const char *d1, const char *d2) {
    struct tm tm1 = {0}, tm2 = {0};
    
    // Parse first date
    sscanf(d1, "%d-%d-%d %d:%d:%d",
           &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday,
           &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;

    // Parse second date
    sscanf(d2, "%d-%d-%d %d:%d:%d",
           &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday,
           &tm2.tm_hour, &tm2.tm_min, &tm2.tm_sec);
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    return (t1 < t2) - (t1 > t2);
}

void dataDecrescente(Processo *vetor, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (compararDatas(vetor[j].Ajuizamento, vetor[j + 1].Ajuizamento) < 0) {
                Processo temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

int contar_por_classe(Processo *lista, int tamanho, int id_classe) {
    int contador = 0;
    for (int i = 0; i < tamanho; i++) {
        if (lista[i].Classe == id_classe) contador++;
    }
    return contador;
}

void salvar_csv(const char *nome_arquivo, Processo *processos, int tamanho) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
    for (int i = 0; i < tamanho; i++) {
        fprintf(fp, "%lld,\"%s\",\"%s\",{%d},{%d},%d\n",
                processos[i].Id, processos[i].Numero, processos[i].Ajuizamento,
                processos[i].Classe, processos[i].Assunto, processos[i].EleicaoAno);
    }
    fclose(fp);
}

void listarIdAssuntosUnicos(Processo *vetor, int tamanho) {
    int *unicos = malloc(tamanho * sizeof(int));
    int *contadores = malloc(tamanho * sizeof(int));
    int totalUnicos = 0;

    if (!unicos || !contadores) {
        printf("Erro de alocação de memória.\n");
        free(unicos);
        free(contadores);
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        int atual = vetor[i].Assunto;
        int encontrado = -1;

        for (int j = 0; j < totalUnicos; j++) {
            if (unicos[j] == atual) {
                encontrado = j;
                break;
            }
        }

        if (encontrado != -1) {
            contadores[encontrado]++;
        } else {
            unicos[totalUnicos] = atual;
            contadores[totalUnicos] = 1;
            totalUnicos++;
        }
    }

    printf("Total de Assuntos únicos encontrados: %d\n", totalUnicos);
    for (int i = 0; i < totalUnicos; i++) {
        printf("ID Assunto: %d (%d processos)\n", unicos[i], contadores[i]);
    }

    free(unicos);
    free(contadores);
}

void listar_processos_multiplos_assuntos(Processo *lista, int tamanho) {
    printf("\n--- Processos com múltiplos assuntos ---\n");

    // Usar uma estrutura para armazenar IDs únicos e seus assuntos
    struct ProcessoAssuntos {
        long long id;
        int assuntos[100];  // Assumindo máximo de 100 assuntos por processo
        int num_assuntos;
    };

    struct ProcessoAssuntos *pa = malloc(tamanho * sizeof(struct ProcessoAssuntos));
    int total_processos = 0;

    if (!pa) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    // Primeiro passo: agrupar assuntos por ID
    for (int i = 0; i < tamanho; i++) {
        int encontrado = -1;
        for (int j = 0; j < total_processos; j++) {
            if (pa[j].id == lista[i].Id) {
                encontrado = j;
                break;
            }
        }

        if (encontrado == -1) {
            // Novo processo
            pa[total_processos].id = lista[i].Id;
            pa[total_processos].assuntos[0] = lista[i].Assunto;
            pa[total_processos].num_assuntos = 1;
            total_processos++;
        } else {
            // Verificar se o assunto já existe
            int assunto_existe = 0;
            for (int k = 0; k < pa[encontrado].num_assuntos; k++) {
                if (pa[encontrado].assuntos[k] == lista[i].Assunto) {
                    assunto_existe = 1;
                    break;
                }
            }
            if (!assunto_existe && pa[encontrado].num_assuntos < 100) {
                pa[encontrado].assuntos[pa[encontrado].num_assuntos++] = lista[i].Assunto;
            }
        }
    }

    // Segundo passo: listar processos com múltiplos assuntos
    int encontrou = 0;
    for (int i = 0; i < total_processos; i++) {
        if (pa[i].num_assuntos > 1) {
            encontrou = 1;
            // Encontrar o processo original para imprimir seus dados
            for (int j = 0; j < tamanho; j++) {
                if (lista[j].Id == pa[i].id) {
                    printf("ID: %lld | Numero: %s | Data: %s | Classe: %d | Assuntos: ",
                           lista[j].Id, lista[j].Numero, lista[j].Ajuizamento,
                           lista[j].Classe);
                    for (int k = 0; k < pa[i].num_assuntos; k++) {
                        printf("%d", pa[i].assuntos[k]);
                        if (k < pa[i].num_assuntos - 1) printf(", ");
                    }
                    printf(" | Ano: %d\n", lista[j].EleicaoAno);
                    break;
                }
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum processo com múltiplos assuntos encontrado.\n");
    }

    free(pa);
}

void dias_em_tramitacao(Processo *processos, int tamanho) {
    printf("\n--- Dias em tramitação ---\n");
    time_t agora = time(NULL);
    
    for (int i = 0; i < tamanho; i++) {
        struct tm tm_ajuiz = {0};
        int ano, mes, dia, hora, min, seg;
        
        if (sscanf(processos[i].Ajuizamento, "%d-%d-%d %d:%d:%d",
                   &ano, &mes, &dia, &hora, &min, &seg) == 6) {
            
            tm_ajuiz.tm_year = ano - 1900;
            tm_ajuiz.tm_mon = mes - 1;
            tm_ajuiz.tm_mday = dia;
            tm_ajuiz.tm_hour = hora;
            tm_ajuiz.tm_min = min;
            tm_ajuiz.tm_sec = seg;
            
            time_t t_ajuizamento = mktime(&tm_ajuiz);
            double segundos = difftime(agora, t_ajuizamento);
            int dias = (int)(segundos / (60 * 60 * 24));
            
            printf("Processo ID %lld está em tramitação há %d dias.\n", 
                   processos[i].Id, dias);
        }
    }
}

int main() {
    Processo processos[numerodeProcessos];
    int tamanho = 0;

    if (!ler_csv("processo_043_202409032338.csv", processos, &tamanho)) {
        printf("Erro ao ler o arquivo CSV.\n");
        return 1;
    }

    // Ordenar por ID e salvar em CSV
    IdCrescente(processos, tamanho);
    salvar_csv("processosOrdenadosId.csv", processos, tamanho);
    printf("Processos ordenados por ID em ordem crescente e salvos em 'processosOrdenadosId.csv'.\n");

    // Ordenar por data e salvar em CSV
    dataDecrescente(processos, tamanho);
    salvar_csv("processosOrdenadosData.csv", processos, tamanho);
    printf("Processos ordenados por data em ordem decrescente e salvos em 'processosOrdenadosData.csv'.\n");

    // Contar processos por classe
    int id_classe;
    printf("Digite o ID da classe a ser buscado: ");
    scanf("%d", &id_classe);
    int total_classe = contar_por_classe(processos, tamanho, id_classe);
    printf("Total de processos com ID de classe %d: %d\n", id_classe, total_classe);

    // Listar assuntos únicos
    listarIdAssuntosUnicos(processos, tamanho);
    // Listar processos com múltiplos assuntos
    listar_processos_multiplos_assuntos(processos, tamanho);
    // Calcular dias em tramitação
    dias_em_tramitacao(processos, tamanho);

    return 0;
}
