#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "io.h"

Arquivo *read_csv(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    Arquivo *data_file = malloc(sizeof(Arquivo));
    if (data_file == NULL) {
        fclose(file);
        return NULL;
    }

    /*conta o número de colunas*/
    char line[1024];
    fgets(line, sizeof(line), file);
    char *token = strtok(line, ",");
    data_file->num_col = 0;
    while (token != NULL) {
        data_file->num_col++;
        token = strtok(NULL, ",");
    }

    rewind(file);

    data_file->cabecalho = malloc(data_file->num_col * sizeof(char *));
    if (data_file->cabecalho == NULL) {
        fclose(file);
        free(data_file);
        return NULL;
    }

    fgets(line, sizeof(line), file);
    token = strtok(line, ",");
    for (int i = 0; i < data_file->num_col; i++) {
        data_file->cabecalho[i] = strdup(token);
        token = strtok(NULL, ",");
    }

    /*conta o número de linhas*/
    data_file->num_lin = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        data_file->num_lin++;
    }

    /*aloca memória para os dados*/
    data_file->dados = malloc(data_file->num_lin * sizeof(char **));
    if (data_file->dados == NULL) {
        printf("Erro ao alocar memória\n");
        fclose(file);
        free(data_file->cabecalho);
        free(data_file);
        return NULL;
    }
    for (int i = 0; i < data_file->num_lin; i++) {
        data_file->dados[i] = malloc(data_file->num_col * sizeof(char *));
        if (data_file->dados[i] == NULL) {
            printf("Erro ao alocar memória\n");
            fclose(file);
            free(data_file->cabecalho);
            for (int j = 0; j < i; j++) {
                free(data_file->dados[j]);
            }
            free(data_file->dados);
            free(data_file);
            return NULL;
        }
    }

    /*le os dados*/
    rewind(file);
    fgets(line, sizeof(line), file); 
    int lin = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        token = strtok(line, ",");
        int col = 0;
        while (token != NULL) {
            data_file->dados[lin][col] = strdup(token);
            token = strtok(NULL, ",");
            col++;
        }
        lin++;
    }

    fclose(file);

    return data_file;
}

bool is_numeric(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i]) && str[i] != '.') {
            return false;
        }
    }
    return true;
}

void exibe_sumario(const Arquivo *data_file) {
    printf("Sumário do Arquivo:\n");

    char tipos_variaveis[data_file->num_col];

    for (int i = 0; i < data_file->num_col; i++) {
        tipos_variaveis[i] = is_numeric(data_file->dados[0][i]) ? 'N' : 'S';
    }

    for (int i = 0; i < data_file->num_col; i++) {
        printf("%s [%c]\n", data_file->cabecalho[i], tipos_variaveis[i]);
    }

    printf("%d variaveis encontradas\n", data_file->num_col);
}

void filtros(Arquivo *data_file) {
    printf("Filtros\n");

    char variavel[100];
    char filtro[3];
    float valor;

    printf("Entre com a variavel: ");
    scanf("%s", variavel);
    printf("Escolha um filtro ( == > >= < <= != ): ");
    scanf("%s", filtro);
    printf("Digite um valor: ");
    scanf("%f", &valor);

    int coluna = -1;
    for (int i = 0; i < data_file->num_col; i++) {
        if (strcmp(data_file->cabecalho[i], variavel) == 0) {
            coluna = i;
            break;
        }
    }

    if (coluna == -1) {
        printf("Variavel nao encontrada.\n");
        return;
    }

    printf("\n");
    printf("%-6s", "Indice");
    for (int j = 0; j < data_file->num_col; j++) {
        printf("%-12s ", data_file->cabecalho[j]);
    }
    printf("\n");

    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(filtro, "==") == 0 && atof(data_file->dados[i][coluna]) == valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        } else if (strcmp(filtro, ">") == 0 && atof(data_file->dados[i][coluna]) > valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        } else if (strcmp(filtro, ">=") == 0 && atof(data_file->dados[i][coluna]) >= valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        } else if (strcmp(filtro, "<") == 0 && atof(data_file->dados[i][coluna]) < valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        } else if (strcmp(filtro, "<=") == 0 && atof(data_file->dados[i][coluna]) <= valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        } else if (strcmp(filtro, "!=") == 0 && atof(data_file->dados[i][coluna]) != valor) {
            printf("%-6d", i);
            for (int j = 0; j < data_file->num_col; j++) {
                printf("%-12s ", data_file->dados[i][j]);
            }
            printf("\n");
        }
    }

    printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
    char opcao;
    scanf(" %c", &opcao);

    if (toupper(opcao) == 'S') {
        char nome_arquivo[100];
        printf("Entre com o nome do arquivo: ");
        scanf("%s", nome_arquivo);

        FILE *arquivo_filtrado = fopen(nome_arquivo, "w");
        if (arquivo_filtrado == NULL) {
            printf("Erro ao criar arquivo.\n");
            return;
        }

        // Imprimir cabeçalho
        for (int j = 0; j < data_file->num_col; j++) {
            fprintf(arquivo_filtrado, "%s,", data_file->cabecalho[j]);
        }
        fprintf(arquivo_filtrado, "\n");

        // Imprimir dados filtrados
        for (int i = 0; i < data_file->num_lin; i++) {
            if (strcmp(filtro, "==") == 0 && atof(data_file->dados[i][coluna]) == valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            } else if (strcmp(filtro, ">") == 0 && atof(data_file->dados[i][coluna]) > valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            } else if (strcmp(filtro, ">=") == 0 && atof(data_file->dados[i][coluna]) >= valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            } else if (strcmp(filtro, "<") == 0 && atof(data_file->dados[i][coluna]) < valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            } else if (strcmp(filtro, "<=") == 0 && atof(data_file->dados[i][coluna]) <= valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            } else if (strcmp(filtro, "!=") == 0 && atof(data_file->dados[i][coluna]) != valor) {
                for (int j = 0; j < data_file->num_col; j++) {
                    fprintf(arquivo_filtrado, "%s,", data_file->dados[i][j]);
                }
                fprintf(arquivo_filtrado, "\n");
            }
        }

        fclose(arquivo_filtrado);
        printf("Arquivo gravado com sucesso.\n");
    }
}

void descricao_dados(const Arquivo *data_file) {
    printf("Descrição dos Dados\n");

    char variavel[100];
    printf("Entre com a variavel: ");
    scanf("%s", variavel);

    int coluna = -1;
    for (int i = 0; i < data_file->num_col; i++) {
        if (strcmp(data_file->cabecalho[i], variavel) == 0) {
            coluna = i;
            break;
        }
    }

    if (coluna == -1) {
        printf("Variavel nao encontrada.\n");
        return;
    }

    // Contagem de dados
    int total_dados = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            total_dados++;
        }
    }

    // Cálculo da média
    double soma = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            soma += atof(data_file->dados[i][coluna]);
        }
    }
    double media = soma / total_dados;

    // Cálculo da mediana
    double *valores = malloc(total_dados * sizeof(double));
    int index = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            valores[index] = atof(data_file->dados[i][coluna]);
            index++;
        }
    }
    qsort(valores, total_dados, sizeof(double), comparar_double);
    double mediana;
    if (total_dados % 2 == 0) {
        mediana = (valores[total_dados / 2 - 1] + valores[total_dados / 2]) / 2;
    } else {
        mediana = valores[total_dados / 2];
    }

    // Cálculo da moda
    int moda_frequencia = 0;
    double moda_valor = atof(data_file->dados[0][coluna]);
    int frequencia_atual = 0;
    double valor_atual = atof(data_file->dados[0][coluna]);
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            double valor = atof(data_file->dados[i][coluna]);
            if (valor == valor_atual) {
                frequencia_atual++;
            } else {
                if (frequencia_atual > moda_frequencia) {
                    moda_frequencia = frequencia_atual;
                    moda_valor = valor_atual;
                }
                frequencia_atual = 1;
                valor_atual = valor;
            }
        }
    }
    if (frequencia_atual > moda_frequencia) {
        moda_frequencia = frequencia_atual;
        moda_valor = valor_atual;
    }

    // Cálculo do desvio padrão
    double desvio_soma = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            double valor = atof(data_file->dados[i][coluna]);
            desvio_soma += pow(valor - media, 2);
        }
    }
    double desvio_padrao = sqrt(desvio_soma / total_dados);

    // Encontrar mínimo e máximo
    double minimo = atof(data_file->dados[0][coluna]);
    double maximo = atof(data_file->dados[0][coluna]);
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            double valor = atof(data_file->dados[i][coluna]);
            if (valor < minimo) {
                minimo = valor;
            }
            if (valor > maximo) {
                maximo = valor;
            }
        }
    }

    // Contar valores únicos
    int num_valores_unicos = 0;
    double *valores_unicos = malloc(total_dados * sizeof(double));
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][coluna], "NaN") != 0) {
            double valor = atof(data_file->dados[i][coluna]);
            bool encontrado = false;
            for (int j = 0; j < num_valores_unicos; j++) {
                if (valores_unicos[j] == valor) {
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) {
                valores_unicos[num_valores_unicos] = valor;
                num_valores_unicos++;
            }
        }
    }

    printf("Contador: %d\n", total_dados);
    printf("Media: %.2lf\n", media);
    printf("Desvio: %.2lf\n", desvio_padrao);
    printf("Mediana: %.2lf\n", mediana);
    printf("Moda: %.2lf (%d vezes)\n", moda_valor, moda_frequencia);
    printf("Min.: %.2lf\n", minimo);
    printf("Max.: %.2lf\n", maximo);
    printf("Valores unicos: [");
    for (int i = 0; i < num_valores_unicos; i++) {
        printf("%.2lf", valores_unicos[i]);
        if (i < num_valores_unicos - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    free(valores);
    free(valores_unicos);
}

void ordena(Arquivo *data_file) {
    printf("Ordenação\n");

    char variavel[100];
    char opcao;
    printf("Entre com a variavel: ");
    scanf("%s", variavel);
    printf("Selecione uma opcao [A]scendente ou [D]escrescente: ");
    scanf(" %c", &opcao);

    int coluna = -1;
    for (int i = 0; i < data_file->num_col; i++) {
        if (strcmp(data_file->cabecalho[i], variavel) == 0) {
            coluna = i;
            break;
        }
    }

    if (coluna == -1) {
        printf("Variavel nao encontrada.\n");
        return;
    }

    qsort(data_file->dados, data_file->num_lin, sizeof(char *), comparar_linhas);

    if (toupper(opcao) == 'D') {
        inverte_matriz(data_file->dados, data_file->num_lin, data_file->num_col);
    }

    printf("%-6s", "Indice");
    for (int j = 0; j < data_file->num_col; j++) {
        printf("%-12s ", data_file->cabecalho[j]);
    }
    printf("\n");

    for (int i = 0; i < data_file->num_lin; i++) {
        printf("%-6d", i);
        for (int j = 0; j < data_file->num_col; j++) {
            printf("%-12s ", data_file->dados[i][j]);
        }
        printf("\n");
    }

    printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
    char salvar;
    scanf(" %c", &salvar);

    if (toupper(salvar) == 'S') {
        char nome_arquivo[100];
        printf("Entre com o nome do arquivo: ");
        scanf("%s", nome_arquivo);

        FILE *arquivo_ordenado = fopen(nome_arquivo, "w");
        if (arquivo_ordenado == NULL) {
            printf("Erro ao criar arquivo.\n");
            return;
        }

        // Imprimir cabeçalho
        for (int j = 0; j < data_file->num_col; j++) {
            fprintf(arquivo_ordenado, "%s,", data_file->cabecalho[j]);
        }
        fprintf(arquivo_ordenado, "\n");

        // Imprimir dados ordenados
        for (int i = 0; i < data_file->num_lin; i++) {
            for (int j = 0; j < data_file->num_col; j++) {
                fprintf(arquivo_ordenado, "%s,", data_file->dados[i][j]);
            }
            fprintf(arquivo_ordenado, "\n");
        }

        fclose(arquivo_ordenado);
        printf("Arquivo gravado com sucesso.\n");
    }
}

void selecao(Arquivo *data_file) {
    printf("Seleção\n");

    printf("Entre com as variaveis que deseja selecionar (separadas por espaço): ");
    char variaveis_selecionadas[1000];
    scanf(" %[^\n]", variaveis_selecionadas);

    char *token = strtok(variaveis_selecionadas, " ");
    int num_variaveis_selecionadas = 0;
    char **cabecalho_selecionado = malloc(data_file->num_col * sizeof(char *));
    while (token != NULL) {
        cabecalho_selecionado[num_variaveis_selecionadas] = strdup(token);
        token = strtok(NULL, " ");
        num_variaveis_selecionadas++;
    }

    printf("\n");
    for (int i = 0; i < num_variaveis_selecionadas; i++) {
        printf("%-12s ", cabecalho_selecionado[i]);
    }
    printf("\n");

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < num_variaveis_selecionadas; j++) {
            printf("%-12s ", data_file->dados[i][encontra_coluna(data_file->cabecalho, data_file->num_col, cabecalho_selecionado[j])]);
        }
        printf("\n");
    }

    printf("Deseja gravar um arquivo com as variáveis selecionadas? [S|N] ");
    char opcao;
    scanf(" %c", &opcao);

    if (toupper(opcao) == 'S') {
        char nome_arquivo[100];
        printf("Entre com o nome do arquivo: ");
        scanf("%s", nome_arquivo);

        FILE *arquivo_selecionado = fopen(nome_arquivo, "w");
        if (arquivo_selecionado == NULL) {
            printf("Erro ao criar arquivo.\n");
            return;
        }

        // Imprimir cabeçalho selecionado
        for (int j = 0; j < num_variaveis_selecionadas; j++) {
            fprintf(arquivo_selecionado, "%s,", cabecalho_selecionado[j]);
        }
        fprintf(arquivo_selecionado, "\n");

        // Imprimir dados selecionados
        for (int i = 0; i < data_file->num_lin; i++) {
            for (int j = 0; j < num_variaveis_selecionadas; j++) {
                fprintf(arquivo_selecionado, "%s,", data_file->dados[i][encontra_coluna(data_file->cabecalho, data_file->num_col, cabecalho_selecionado[j])]);
            }
            fprintf(arquivo_selecionado, "\n");
        }

        fclose(arquivo_selecionado);
        printf("Arquivo gravado com sucesso.\n");
    }
}

void faltantes(Arquivo *data_file) {
    printf("Dados Faltantes\n");

    printf("1) Listar registros com NaN\n");
    printf("2) Substituir pela media\n");
    printf("3) Substituir pelo proximo valor valido\n");
    printf("4) Remover registros com NaN\n");
    printf("5) Voltar ao menu principal\n");

    int opcao;
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listar_registros_nan(data_file);
            break;
        case 2:
            substituir_media(data_file);
            break;
        case 3:
            substituir_proximo_valido(data_file);
            break;
        case 4:
            remover_registros_nan(data_file);
            break;
        case 5:
            printf("Voltando ao menu principal...\n");
            break;
        default:
            printf("Opção inválida.\n");
    }
}

void listar_registros_nan(Arquivo *data_file) {
    printf("Listar registros com NaN\n");

    bool encontrado = false;
    for (int i = 0; i < data_file->num_lin; i++) {
        for (int j = 0; j < data_file->num_col; j++) {
            if (strcmp(data_file->dados[i][j], "NaN") == 0) {
                if (!encontrado) {
                    // Imprime cabeçalho
                    printf("%-6s", "Indice");
                    for (int k = 0; k < data_file->num_col; k++) {
                        printf("%-12s ", data_file->cabecalho[k]);
                    }
                    printf("\n");
                    encontrado = true;
                }
                // Imprime dados com NaN
                printf("%-6d", i);
                for (int k = 0; k < data_file->num_col; k++) {
                    printf("%-12s ", data_file->dados[i][k]);
                }
                printf("\n");
                break;  // Para verificar a próxima linha
            }
        }
    }

    if (!encontrado) {
        printf("Não foram encontrados registros com NaN.\n");
    }
}

void substituir_media(Arquivo *data_file) {
    printf("Substituir valores NaN pela média\n");

    for (int i = 0; i < data_file->num_lin; i++) {
        for (int j = 0; j < data_file->num_col; j++) {
            if (strcmp(data_file->dados[i][j], "NaN") == 0) {
                double media = calcular_media_coluna(data_file, j);
                char media_str[100];
                sprintf(media_str, "%.2f", media);
                strcpy(data_file->dados[i][j], media_str);
            }
        }
    }

    printf("Valores NaN substituídos pela média.\n");
}

void substituir_proximo_valido(Arquivo *data_file) {
    printf("Substituir valores NaN pelo próximo valor válido\n");

    for (int i = 0; i < data_file->num_lin; i++) {
        for (int j = 0; j < data_file->num_col; j++) {
            if (strcmp(data_file->dados[i][j], "NaN") == 0) {
                // Procurar próximo valor válido na mesma coluna
                for (int k = i + 1; k < data_file->num_lin; k++) {
                    if (strcmp(data_file->dados[k][j], "NaN") != 0) {
                        strcpy(data_file->dados[i][j], data_file->dados[k][j]);
                        break;
                    }
                }
            }
        }
    }

    printf("Valores NaN substituídos pelo próximo valor válido.\n");
}

void remover_registros_nan(Arquivo *data_file) {
    printf("Remover registros com NaN\n");

    int novas_linhas = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        bool contem_nan = false;
        for (int j = 0; j < data_file->num_col; j++) {
            if (strcmp(data_file->dados[i][j], "NaN") == 0) {
                contem_nan = true;
                break;
            }
        }
        if (!contem_nan) {
            // Copiar linha atual para a nova matriz
            for (int j = 0; j < data_file->num_col; j++) {
                strcpy(data_file->dados[novas_linhas][j], data_file->dados[i][j]);
            }
            novas_linhas++;
        }
    }

    printf("%d registros com NaN removidos.\n", data_file->num_lin - novas_linhas);
    data_file->num_lin = novas_linhas;
}

double calcular_media_coluna(Arquivo *data_file, int col) {
    double soma = 0.0;
    int contador = 0;
    for (int i = 0; i < data_file->num_lin; i++) {
        if (strcmp(data_file->dados[i][col], "NaN") != 0) {
            soma += atof(data_file->dados[i][col]);
            contador++;
        }
    }
    return soma / contador;
}


void imprime(const Arquivo *data_file) {
    printf("       ");
    /*imprime o cabeçalho*/
    for (int j = 0; j < data_file->num_col; j++) {
        printf("%-12s ", data_file->cabecalho[j]);
    }
    printf("\n");

    /*imprime os dados*/
    for (int i = 0; i < data_file->num_lin; i++) {
        printf("%-6d ", i); /*imprime o número da linha*/
        for (int j = 0; j < data_file->num_col; j++) {
            printf("%-12s ", data_file->dados[i][j]); 
        }
        printf("\n");
    }

    /*imprime o total de linhas e colunas*/
    printf("\n[%d rows x %d columns]\n", data_file->num_lin, data_file->num_col);
}

void salva_dados(const Arquivo *data_file, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < data_file->num_col - 1; i++) {
        fprintf(file, "%s, ", data_file->cabecalho[i]);
    }
    fprintf(file, "%s\n", data_file->cabecalho[data_file->num_col - 1]);

    for (int i = 0; i < data_file->num_lin; i++) {
        for (int j = 0; j < data_file->num_col - 1; j++) {
            fprintf(file, "%s, ", data_file->dados[i][j]);
        }
        fprintf(file, "%s\n", data_file->dados[i][data_file->num_col -1]);
    }
    fclose(file);
}

void libera(Arquivo *data_file) {
    for (int i = 0; i < data_file->num_col; i++) {
        free(data_file->cabecalho[i]);
    }
    free(data_file->cabecalho);

    for (int i = 0; i < data_file->num_lin; i++) {
        for (int j = 0; j < data_file->num_col; j++) {
            free(data_file->dados[i][j]);
        }
        free(data_file->dados[i]);
    }
    free(data_file->dados);

    free(data_file);
}
