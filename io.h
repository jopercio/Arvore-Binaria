#ifndef IO_H
#define IO_H

typedef struct {
    char **cabecalho; 
    char ***dados; 
    int num_col; 
    int num_lin; 
} Arquivo;

/*função para ler e processar o arquivo*/
Arquivo *read_csv(char *filename);

/*função para imprimir o sumário do arquivo*/
void exibe_sumario(const Arquivo *data_file);
void filtros(Arquivo *datafile);
void descricao_dados(const Arquivo *data_file);
void ordena(Arquivo *data_file);
void selecao(Arquivo *data_file);
void faltantes(Arquivo *data_file);
void imprime(const Arquivo *data_file);
void salva_dados(const Arquivo *datafile, char *filename);

/*função para liberar a memória alocada para os dados do arquivo*/
void libera(Arquivo *data_file);

#endif
