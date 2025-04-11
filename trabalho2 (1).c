    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>
    #include "trabalho.h"

    int carregarProcessosDoArquivo(const char *arquivo, Processo *processo, int *tamanho) {
        FILE *fp = fopen(arquivo, "r");
        if (!fp) {
            printf("Erro ao abrir o arquivo '%s'.\n", arquivo);
            return 0;
        }
        printf("Arquivo '%s' aberto com sucesso.\n", arquivo);

        char linha[1024];
        int linha_atual = 0;
        *tamanho = 0;

        // Ler linha por linha
        while (fgets(linha, sizeof(linha), fp) && *tamanho < numerodeProcessos) {
            if (linha_atual++ == 0) continue; // Pula o cabeçalho

            // Remover newline se presente
            char *newline = strchr(linha, '\n');
            if (newline) *newline = '\0';

            Processo p = {0}; // Inicializa com zeros
            char buffer[256];
            int pos = 0, campo = 0;
            int dentro_aspas = 0;
            
            // Parser manual para lidar corretamente com aspas e chaves
            for (int i = 0; linha[i]; i++) {
                if (linha[i] == '"') {
                    dentro_aspas = !dentro_aspas;
                    continue;
                }
                
                if (!dentro_aspas && linha[i] == ',') {
                    buffer[pos] = '\0';
                    // Processar o campo
                    switch (campo) {
                        case 0: // id
                            p.Id = atoll(buffer);
                            break;
                        case 1: // numero
                            strncpy(p.Numero, buffer, sizeof(p.Numero) - 1);
                            break;
                        case 2: // data_ajuizamento
                            strncpy(p.Ajuizamento, buffer, sizeof(p.Ajuizamento) - 1);
                            break;
                        case 3: // id_classe
                            if (buffer[0] == '{') {
                                char *end = strchr(buffer, '}');
                                if (end) {
                                    *end = '\0';
                                    p.Classe = atoi(buffer + 1);
                                }
                            }
                            break;
                        case 4: // id_assunto
                            if (buffer[0] == '{') {
                                char *end = strchr(buffer, '}');
                                if (end) {
                                    *end = '\0';
                                    p.Assunto = atoi(buffer + 1);
                                }
                            }
                            break;
                    }
                    pos = 0;
                    campo++;
                    continue;
                }
                
                if (pos < sizeof(buffer) - 1) {
                    buffer[pos++] = linha[i];
                }
            }
            
            // Processar último campo (ano_eleicao)
            if (pos > 0) {
                buffer[pos] = '\0';
                p.EleicaoAno = atoi(buffer);
                processo[(*tamanho)++] = p;
            }
        }

        fclose(fp);
        return 1;
    }

    void ordenarProcessosPorId(Processo *processo, int qtd) {
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

    int compararDataAjuizamento(const char *d1, const char *d2) {
        struct tm tm1 = {0}, tm2 = {0};
        
        sscanf(d1, "%d-%d-%d %d:%d:%d",
            &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday,
            &tm1.tm_hour, &tm1.tm_min, &tm1.tm_sec);
        tm1.tm_year -= 1900;
        tm1.tm_mon -= 1;

        sscanf(d2, "%d-%d-%d %d:%d:%d",
            &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday,
            &tm2.tm_hour, &tm2.tm_min, &tm2.tm_sec);
        tm2.tm_year -= 1900;
        tm2.tm_mon -= 1;

        time_t t1 = mktime(&tm1);
        time_t t2 = mktime(&tm2);

        return (t1 < t2) - (t1 > t2);
    }

    void ordenarPorDataAjuizamento(Processo *vetor, int tamanho) {
        for (int i = 0; i < tamanho - 1; i++) {
            for (int j = 0; j < tamanho - i - 1; j++) {
                if (compararDataAjuizamento(vetor[j].Ajuizamento, vetor[j + 1].Ajuizamento) < 0) {
                    Processo temp = vetor[j];
                    vetor[j] = vetor[j + 1];
                    vetor[j + 1] = temp;
                }
            }
        }
    }

    int contarProcessosClasse(Processo *lista, int tamanho, int id_classe) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (lista[i].Classe == id_classe) contador++;
        }
        return contador;
    }

    void mostrarAssuntosUnicos(Processo *vetor, int tamanho) {
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

    void exibirProcessosMultiplosAssuntos(Processo *lista, int tamanho) {
        printf("\n--- Processos com múltiplos assuntos ---\n");

        struct ProcessoAssuntos {
            long long id;
            int assuntos[100];
            int num_assuntos;
        };

        struct ProcessoAssuntos *pa = malloc(tamanho * sizeof(struct ProcessoAssuntos));
        int total_processos = 0;

        if (!pa) {
            printf("Erro de alocação de memória.\n");
            return;
        }

        for (int i = 0; i < tamanho; i++) {
            int encontrado = -1;
            for (int j = 0; j < total_processos; j++) {
                if (pa[j].id == lista[i].Id) {
                    encontrado = j;
                    break;
                }
            }

            if (encontrado == -1) {
                pa[total_processos].id = lista[i].Id;
                pa[total_processos].assuntos[0] = lista[i].Assunto;
                pa[total_processos].num_assuntos = 1;
                total_processos++;
            } else {
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

        int encontrou = 0;
        for (int i = 0; i < total_processos; i++) {
            if (pa[i].num_assuntos > 1) {
                encontrou = 1;
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

    void calcularTempoTramitacao(Processo *processos, int tamanho) {
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

    void salvarProcessosEmArquivo(const char *nome_arquivo, Processo *processos, int tamanho) {
        FILE *fp = fopen(nome_arquivo, "w");
        if (!fp) {
            printf("Erro ao abrir o arquivo para escrita.\n");
            return;
        }
        
        // Escrever cabeçalho exatamente como no arquivo original
        fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");
        
        // Escrever cada processo no formato especificado
        for (int i = 0; i < tamanho; i++) {
            fprintf(fp, "%lld,\"%s\",\"%s\",{%d},{%d},%d\n",
                processos[i].Id,
                processos[i].Numero,
                processos[i].Ajuizamento,
                processos[i].Classe,
                processos[i].Assunto,
                processos[i].EleicaoAno);
        }
        
        fclose(fp);
        printf("Arquivo '%s' salvo com sucesso.\n", nome_arquivo);
    }
