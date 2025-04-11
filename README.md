

# Análise de Processos da Justiça Eleitoral

Este projeto é um sistema desenvolvido em C para analisar dados de processos da Justiça Eleitoral. O programa lê dados de um arquivo CSV, permitindo que o usuário realize operações como ordenar processos, contar e listar informações sobre classes e assuntos.

## Project Overview

O sistema implementa funcionalidades para:
- Ler processos de um arquivo CSV.
- Ordenar processos por ID ou por data de ajuizamento.
- Contar quantidade de processos por classe.
- Listar IDs de assuntos únicos.
- Listar processos que têm múltiplos assuntos.
- Calcular o número de dias que os processos estão em tramitação.

## Installation

Para compilar e executar o projeto, você precisa de um compilador C como o GCC. Abaixo estão os passos para cada sistema operacional:

### Linux/Mac

1. Clone o repositório:
   ```bash
   git clone https://github.com/seu_usuario/seu_projeto.git
   cd seu_projeto
   ```

2. Compile os arquivos:
   ```bash
   gcc -o analiseProcessos main.c trabalho.c -lm
   ```

3. Execute o programa:
   ```bash
   ./analiseProcessos
   ```

### Windows

1. Clone o repositório.
2. Utilize um terminal de compilação, como o MinGW.
3. Compile o projeto:
   ```bash
   gcc -o analiseProcessos.exe main.c trabalho.c -lm
   ```

4. Execute o programa:
   ```bash
   analiseProcessos.exe
   ```

## Usage

Após executar o programa, você verá um menu interativo onde poderá selecionar as operações a serem realizadas. O programa solicitará que você forneça um arquivo CSV com os dados dos processos, que deve estar no mesmo diretório do executável.

## Features

- **Ordenação de Processos**: Possibilidade de ordenar processos por ID (crescente) ou por data de ajuizamento (decrescente) e salvar os resultados em arquivos CSV.
- **Contagem de Processos por Classe**: O usuário pode inserir um ID de classe e obter a contagem de processos associados.
- **Listar Assuntos Únicos**: Exibe a quantidade de assuntos únicos encontrados nos processos.
- **Identificação de Processos com Múltiplos Assuntos**: Listagem de processos que têm mais de um assunto associado.
- **Cálculo de Dias em Tramitação**: Mostra quantos dias cada processo está tramitando.

## Dependencies

O projeto não possui dependências externas específicas além do compilador C padrão.

## Project Structure

```
.
├── main.c              # Arquivo principal do programa
├── trabalho.c          # Implementações das funções de manipulação de processos
└── trabalho.h          # Header com definições e estrutura do Processo
```

### Estruturas e Funções

- **Processo**: Estrutura que contém campos relevantes dos processos, como ID, número e data de ajuizamento.
- **ler_csv()**: Função para ler dados de um arquivo CSV e popular a estrutura de processos.
- **IdCrescente()**: Função para ordenar processos crescentemente por ID.
- **dataDecrescente()**: Função para ordenar processos decrescentemente por data de ajuizamento.
- **contar_por_classe()**: Contagem de processos por ID de classe.
- **listarIdAssuntosUnicos()**: Listagem de IDs de assuntos únicos e suas contagens.
- **listar_processos_multiplos_assuntos()**: Listagem de processos com múltiplos assuntos.
- **dias_em_tramitacao()**: Cálculo da quantidade de dias em tramitação para cada processo.

## Contribuição

Sinta-se à vontade para contribuir com o projeto! Você pode relatar problemas, sugerir melhorias ou enviar pull requests.

## Licença

Este projeto está licenciado sob a MIT License - consulte o arquivo `LICENSE` para mais detalhes.
