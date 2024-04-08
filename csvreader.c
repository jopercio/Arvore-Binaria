#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.csv>\n", argv[0]);
        return 1;
    }

    char *filename = strdup(argv[1]); /*aloca espaço necessário*/

    Arquivo *dados = read_csv(filename);
    if (dados == NULL) {
        return 1;
    }

    int opcao;
    do {
        printf("\n1) Sumario do arquivo\n");
        printf("2) Mostrar\n");
        printf("3) Filtros\n");
        printf("4) Descrição dos Dados\n");
        printf("5) Ordenação\n");
        printf("6) Seleção\n");
        printf("7) Dados Faltantes\n");
        printf("8) Salvar Dados\n");
        printf("9) Fim\n");
        printf("Digite sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibe_sumario(dados);
                printf("\nPressione Enter para continuar\n");
                getchar(); /*Limpar o buffer do teclado e espera o enter*/
                getchar(); 
                break;
            case 2:
                imprime(dados);
                printf("\nPressione Enter para continuar\n");
                getchar(); 
                getchar(); 
                break;
            case 3:
                filtros(dados);
                break;
            case 4:
                descricao_dados(dados);
                printf("\nPressione Enter para continuar\n");
                getchar();
                getchar();
                break;
            case 5:
                ordena(dados);
                break;
            case 6:
                selecao(dados);
                break;
            case 7:
                faltantes(dados);
                break;
            case 8:
                printf("Entre com o nome do arquivo para salvar os dados: ");
                char salva_filename[100];
                scanf("%s", salva_filename);
                salva_dados(dados, salva_filename);
                break;
            case 9:
                printf("Fim\n");
                break;
        }
    } while (opcao != 9);

    libera(dados);
    free(filename); /*libera a memória alocada*/

    return 0;
}
