/*Projeto Recurso LP1
Created by Frederico Azevedo*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NAME_DIM 64

#define ERR_ALLOC   "Error: Couldn't allocate memory."
#define ERROO   "Invalid command!"
#define ERR_CORD	"Error: invalid coordinate"
#define MSG_NOBOMB	"Error: no bomb at specified coordinate"
#define MSG_FNAME	"Error: missing file name"
#define MSG_FILE_CRP	"Error: file is corrupted"
#define MSG_FILE_OPN	"Error: could not open file"
#define MSG_FILE_INVDIM	"Error: invalid map dimensions"


/*Estrutura*/
typedef struct _matrizbombasfinal
{
    int x;
    int y;
    int time;
    struct _matrizbombasfinal * next;
}Matrizbombasfinal;


/*Fila*/
typedef struct {
    Matrizbombasfinal * head;
    Matrizbombasfinal * tail;
}Fila_matrizbombasfinal;



/*Declarações de Funções*/
void fila_init(Fila_matrizbombasfinal * lista_matrizbombasfinal);
char ** read_ficheiro(FILE * fp, int *dimx, int *dimy, char **matriz);
Fila_matrizbombasfinal * inserir_na_fila_ordenada(Fila_matrizbombasfinal * lista_bombasfinal, Matrizbombasfinal BombasFinais_aux, char **matriz,int DIMX, int DIMY, int value);
void menu(void);
char ** destruir_matriz(char **matriz, int dimx);
char **  criar_matriz(char **matriz, int dimx, int dimy);
void log_reben(char **matriz,Fila_matrizbombasfinal * lista_matrizbombasfinal,int cordx, int cordy, int * timer, int value,int DIMX, int DIMY);
void print_matriz_bombas_rebentadas(Fila_matrizbombasfinal * lista_matrizbombasfinal, int *tempo, int value);
void continuacao_log_reben(char **matriz,Fila_matrizbombasfinal * lista_matrizbombasfinal,int cordx, int cordy, int * tempo, int value,int DIMX, int DIMY);
void print_matriz(char **matriz,int DIMX, int DIMY);
void escrevermatriz(char **matriz,FILE* fp1, int dimx, int dimy);
Fila_matrizbombasfinal * deletelist(Fila_matrizbombasfinal * lista_matrizbombasfinal);



int main(int argc , const char * argv[]) {

    char **matriz = NULL;

    FILE * fp_bombas = NULL;
    FILE * fp1 = NULL;

    Fila_matrizbombasfinal lista_matrizbombasfinal;
   
    char filename[NAME_DIM];
    char option[NAME_DIM];
    char vez[NAME_DIM];
    char ficheiro1[NAME_DIM];
    int value = 0;
    int timer = 0;


    int DIMX = 0;
    int DIMY = 0;
    int cordenadax = 0;
    int cordenaday = 0;
    int sair = 0;

    fila_init(&lista_matrizbombasfinal);

    /*Verifica se o utilizador colocou o nome do ficheiro*/
    if(argc > 1)
    {
    
        strcpy(filename, argv[1]);

        fp_bombas = fopen(filename, "r");
        if(fp_bombas == NULL)
        {
            puts(MSG_FILE_OPN);
            exit(0);
        }
        matriz = read_ficheiro(fp_bombas,&DIMX,&DIMY,matriz);
        fclose(fp_bombas);

    }
    else{
        puts(MSG_FNAME);
        exit(0);
    }

    menu();

/*ciclo infinito que só termina com a opçao quit*/

    do
    {
        printf(">");
        scanf(" %s",option);

        if (strcmp(option,"show")==0)
        {
          print_matriz(matriz,DIMX,DIMY); 
        }

        else if (strcmp(option,"plant")==0)
        {
            scanf("%d %d", &cordenadax, &cordenaday);
            if (cordenadax < 0 || cordenadax >= DIMX || cordenaday<0 || cordenaday >= DIMY)
            {
                puts(ERR_CORD);
                continue;
            }
            else
            {
                if (cordenadax <= DIMX && cordenaday <= DIMY && cordenadax >= 0 && cordenaday >= 0)
                {
                    matriz[cordenadax][cordenaday] = '.';

                }
                else
                {
                    puts(MSG_NOBOMB);
                }
            }
        }
        else if (strcmp(option,"propagate")==0)
        {
            scanf("%d %d", &cordenadax, &cordenaday);

            if (cordenadax < 0 || cordenadax > DIMX || cordenaday<0 || cordenaday>DIMY)
            {
                puts(ERR_CORD);
                continue;
            }
            else
            {
                if (matriz[cordenadax][cordenaday] == '.')
                {
                    
                    value=0;
                    log_reben(matriz,&lista_matrizbombasfinal,cordenadax,cordenaday,&timer,value,DIMX,DIMY);
                   
                }
                else if (matriz[cordenadax][cordenaday] == '*')
                {
                    continue;
                }
                
                else
                {
                    puts(MSG_NOBOMB);
                }
            }
        }

        else if (strcmp(option,"log")==0)
        {
            scanf("%d %d", &cordenadax, &cordenaday);

            if (cordenadax < 0 || cordenadax > DIMX || cordenaday<0 || cordenaday>DIMY)
            {
                puts(ERR_CORD);
                continue;
            }
            else
            {
                value = 1;
                if (matriz[cordenadax][cordenaday] == '.')
                {
                    log_reben(matriz,&lista_matrizbombasfinal,cordenadax,cordenaday,&timer,value,DIMX,DIMY);
                }
                else if (matriz[cordenadax][cordenaday] == '*')
                {
                    continue;
                }
                
                else
                {
                    puts(MSG_NOBOMB);
                }
            }
        }

        else if(strcmp(option,"export")==0)
        {
            scanf(" %s", ficheiro1);
            fp1 = fopen(ficheiro1, "w");
            if (fp1 == NULL)
            {
                fprintf(stderr,"Error opening file\n");
            }
                        
            escrevermatriz(matriz,fp1,DIMX,DIMY);

        }


        else if (strcmp(option,"quit")==0)
        {
            matriz = destruir_matriz(matriz, DIMX);
            sair = 1;
        }
        

        else if (strcmp(option,"sos")==0)
        {
            menu();
        }


        else
        {
            scanf("%[^\n]s\n", vez);
            puts(ERROO);
        }

    }while(sair!=1);

    exit(0);
}



/* Função que irá apresentar o menu*/
void menu(){
    printf("+-----------------------------------------------------+\n");
    printf("show                - show the mine map\n");
    printf("propagate <x> <y>   - explode bomb at <x> <y>\n");
    printf("log <x> <y>         - explode bomb at <x> <y>\n");
    printf("plant <x> <y>       - place bomb at <x> <y>\n");
    printf("export <filename>   - save file with current map\n");
    printf("quit                - exit program\n");
    printf("sos                 - show menu\n");
    printf("+-----------------------------------------------------+\n");
}


/*Esta função inicia a lista*/
void fila_init(Fila_matrizbombasfinal * lista_matrizbombasfinal)
{
    lista_matrizbombasfinal -> head = NULL;
    lista_matrizbombasfinal -> tail = NULL;
}


/*Esta função lê o ficheiro: lemos linha por linha para a variavel input, até atingir o EOF; Insere os dados nas respectiva lista*/
char ** read_ficheiro(FILE * fp, int *dimx, int *dimy, char **matriz)
{
    char input[256];
    int quantidade = 0;
    int verificarlinha = 0;
    char *resultado_leitura ;

    char valor;
    int cordx = 0;
    int cordy = 0;

    do
    {
        resultado_leitura = fgets(input, 256, fp);
        if (resultado_leitura == NULL)
        {
            break;
        }
        quantidade = sscanf(input, "%c ", &valor);

        if(valor == '#' || valor == '\n' || quantidade == 0 || valor =='\r')
        {
            continue;
        }
        else
        {
            if(verificarlinha == 0)
            {
                quantidade = sscanf(input, "%d %d ", dimx , dimy);
                if (*dimx <= 0 || *dimy <= 0 || quantidade != 2)
                {
                    puts(MSG_FILE_INVDIM);
                    exit(0);
                }
                verificarlinha++;

                matriz = criar_matriz(matriz, *dimx, *dimy);
            }
            else
            {
                quantidade = sscanf(input, "%c %d %d ", &valor, &cordx, &cordy);
                if ((quantidade != 3 ) || (cordx >= *dimx ) || (cordy >= *dimy ) || (cordx < 0) || (cordy < 0) || (valor != '.' && valor != '*'))
                {
                    puts(MSG_FILE_CRP);
                    exit(0);
                }
                else
                {
                    matriz[cordx][cordy] = valor;
                }
                
            }
        }

    }   while(!feof(fp)); 
    return matriz;
}


/*Função que cria uma matriz com os respetivos valores retirados da estrutura*/
char **  criar_matriz(char **matriz, int dimx, int dimy)
{
    int i = 0;
    int x,y;

    /*Pai*/
    matriz = (char **) malloc(dimx * sizeof(char *));
    /*Filho*/
    for (i = 0; i < dimx; i++)
    {
        matriz[i] = (char *) malloc((dimy) * sizeof(char));
    }

    /*Inicializo a matriz*/
    for (x = 0; x < dimx; x++)
    {
        for (y = 0; y < dimy; y++)
        {
            matriz[x][y] = '_';
        }
    }

    return matriz;
}

/*Função para destruir matriz*/
char ** destruir_matriz(char **matriz, int dimx)
{
    int i = 0;
    for ( i = 0; i < dimx; i++)
    {
        free(matriz[i]);
    }

    free(matriz);
    matriz = NULL;
    return matriz;
}

/*Função para fazer o print da matriz, já com as bombas*/
void print_matriz(char **matriz,int DIMX, int DIMY)
{
    int linhas = 0;
    int colunas = 0;

    for (linhas = 0; linhas <= DIMX-1; linhas++)
    {
        for (colunas = 0; colunas <= DIMY-1; colunas++)
        {        
            printf("%c", matriz[linhas][colunas]); 
        }
        printf("\n");
    }
}


Fila_matrizbombasfinal * deletelist(Fila_matrizbombasfinal * lista_matrizbombasfinal)
{
 
    Matrizbombasfinal* current = lista_matrizbombasfinal->head;
    Matrizbombasfinal* next = NULL;
 
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
 
    lista_matrizbombasfinal->head = NULL;

    return lista_matrizbombasfinal;
}


/*Função que ira rebentar as bombas à sua volta, caso o value seja =1 (estamos em log) e caso seja  = 0(estamos em propagate) */
void log_reben(char **matriz,Fila_matrizbombasfinal * lista_matrizbombasfinal,int cordx, int cordy, int * tempo, int value,int DIMX, int DIMY)
{
    Matrizbombasfinal bombafinal_aux;
    Matrizbombasfinal * fila_aux;
    int tempo_aux = *tempo;

    
    bombafinal_aux.time = tempo_aux;
    bombafinal_aux.x = cordx;
    bombafinal_aux.y = cordy;
    lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value); 
    
    fila_aux = lista_matrizbombasfinal->head;

    while (fila_aux != NULL)
    {
        cordx = fila_aux -> x;
        cordy = fila_aux -> y;
        tempo_aux = fila_aux -> time;
        
        

        /* processa vizinhança */
        continuacao_log_reben(matriz,lista_matrizbombasfinal,cordx,cordy,&tempo_aux,value,DIMX,DIMY);
        fila_aux = fila_aux->next;
    }

    /* imprimir lista */
    print_matriz_bombas_rebentadas(lista_matrizbombasfinal, tempo, value);
    /* destroi lista*/
    lista_matrizbombasfinal = deletelist(lista_matrizbombasfinal);
}


/*Função para fazer o print da estrutura com as bombas rebentadas estrutura com tempos*/
void print_matriz_bombas_rebentadas(Fila_matrizbombasfinal * lista_matrizbombasfinal, int *tempo, int value){
    Matrizbombasfinal *bombafinal_aux;

    bombafinal_aux = lista_matrizbombasfinal->head;


    while (bombafinal_aux->next != NULL)
    {
        if (value == 1) /* log */
            printf("%d [%d, %d]\n", bombafinal_aux->time, bombafinal_aux->x, bombafinal_aux->y);
        bombafinal_aux = bombafinal_aux -> next;
    }

    if (value == 1) /* log */
    {
        printf("%d [%d, %d]\n", bombafinal_aux->time, bombafinal_aux->x, bombafinal_aux->y);
        *tempo = bombafinal_aux->time;
    }
    else
        *tempo = bombafinal_aux->time;

    return;
}


/*Função que insere as cordenadas e o tempo em que rebentam e coloca na lista*/
Fila_matrizbombasfinal * inserir_na_fila_ordenada(Fila_matrizbombasfinal * lista_bombasfinal, Matrizbombasfinal BombasFinais_aux, char **matriz,int DIMX, int DIMY, int value)
{
    Matrizbombasfinal * previous = lista_bombasfinal -> head;
    Matrizbombasfinal * novo_no = (Matrizbombasfinal*) malloc (sizeof(Matrizbombasfinal));
    if (novo_no == NULL)
    {
        puts(ERR_ALLOC);
        exit(0);
    }
    
    novo_no -> time = BombasFinais_aux.time;  
    novo_no -> x = BombasFinais_aux.x;
    novo_no -> y = BombasFinais_aux.y;
    novo_no -> next = NULL;

    /* rebenta a bomba e faz show no caso de estar em propagate */
    matriz[BombasFinais_aux.x][BombasFinais_aux.y] = '*';
    if (value == 0)
    {
        print_matriz(matriz,DIMX,DIMY);
        printf("\n");

    }

    /*Colocar na lista*/
    

    if ((lista_bombasfinal -> head == NULL) || BombasFinais_aux.time < novo_no -> time)
    {
        novo_no -> next = lista_bombasfinal -> head;
        /*Primeira posição*/
        lista_bombasfinal -> head = novo_no;
    }
    else
    {
        while((previous -> next != NULL) && (novo_no -> time >= previous -> next -> time))
        {
            if (novo_no -> time == previous -> next -> time && novo_no -> x < previous -> next -> x)
            {
                break;
            } 
            else if (novo_no -> time == previous -> next -> time && novo_no -> x == previous -> next -> x && novo_no -> y < previous -> next -> y )
            {
                break;
            }
                   
            previous = previous -> next;
        }

        
        novo_no -> next = previous -> next;
        previous -> next = novo_no;
            
        
        

    }

    

    return lista_bombasfinal;
}


/*continuação da função log que vai ver se existem bombas ao pé das bombas rebentadas*/
void continuacao_log_reben(char **matriz,Fila_matrizbombasfinal * lista_matrizbombasfinal,int cordx, int cordy, int * tempo, int value,int DIMX, int DIMY){
    Matrizbombasfinal bombafinal_aux;

    if ((cordx-1 >= 0) && (cordy >= 0)  && (cordx-1 < DIMX) && (cordy < DIMY) && (matriz[cordx-1][cordy] == '.'))
    {
        bombafinal_aux.time = *tempo + 10;
        bombafinal_aux.x = cordx - 1;
        bombafinal_aux.y = cordy;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz,DIMX,DIMY, value);
        
        
    }

    if (cordx-1 >= 0 && cordy-1 >=0  &&  cordx-1 < DIMX && cordy-1 < DIMY  &&  matriz[cordx-1][cordy-1] ==  '.')
    {
        bombafinal_aux.time = *tempo + 11;
        bombafinal_aux.x = cordx - 1;
        bombafinal_aux.y = cordy - 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz,DIMX,DIMY, value);

            
    }

    if (cordx >= 0 && cordy-1 >=0  &&  cordx < DIMX && cordy-1 < DIMY  &&  matriz[cordx][cordy-1] ==  '.')
    {
        bombafinal_aux.time = *tempo + 12;
        bombafinal_aux.x = cordx;
        bombafinal_aux.y = cordy - 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz,DIMX,DIMY, value);

    }

    if (cordx+1 >= 0 && cordy-1 >=0  && cordx+1 < DIMX && cordy-1 < DIMY  &&  matriz[cordx+1][cordy-1] ==  '.')
    {
        bombafinal_aux.time = *tempo + 13;
        bombafinal_aux.x = cordx + 1;
        bombafinal_aux.y = cordy - 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value);

    }

    if (cordx+1 >= 0 && cordy >=0  && cordx+1 < DIMX && cordy < DIMY  && matriz[cordx+1][cordy] ==  '.')
    {
        bombafinal_aux.time = *tempo + 14;
        bombafinal_aux.x = cordx+1;
        bombafinal_aux.y = cordy;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value);

    }

    if (cordx+1 >= 0 && cordy+1 >=0  && cordx+1 < DIMX && cordy+1 < DIMY  &&  matriz[cordx+1][cordy+1] ==  '.')
    {
        bombafinal_aux.time = *tempo + 15;
        bombafinal_aux.x = cordx + 1;
        bombafinal_aux.y = cordy + 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value);

    }

    if (cordx >= 0 &&  cordy+1 >=0  && cordx < DIMX && cordy+1 < DIMY  &&  matriz[cordx][cordy+1] =='.')
    {
        bombafinal_aux.time = *tempo + 16;
        bombafinal_aux.x = cordx;
        bombafinal_aux.y = cordy + 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value);

    }

    if (cordx-1 >= 0 && cordy+1 >=0  && cordx-1 < DIMX && cordy+1 < DIMY  && matriz[cordx-1][cordy+1] == '.')
    {
        bombafinal_aux.time = *tempo + 17;
        bombafinal_aux.x = cordx - 1;
        bombafinal_aux.y = cordy + 1;
        lista_matrizbombasfinal = inserir_na_fila_ordenada(lista_matrizbombasfinal, bombafinal_aux, matriz, DIMX, DIMY, value);

    }
    return;

}


/* Criar ficheiro com a matriz nova*/
void escrevermatriz(char **matriz,FILE* fp1, int dimx, int dimy)
{
    int linhas = 0;

    int colunas = 0;

    /*Por no main*/
    if (fp1 == NULL)
    {
	    fputs("Error opening file\n", stderr);
        return;
    }
    else
    {        
        fprintf(fp1,"%d %d\n",dimx,dimy);

        for ( linhas = 0; linhas < dimx; linhas++)
        {
            for ( colunas = 0; colunas < dimy; colunas++)
            {

                if (matriz[linhas][colunas]=='.')
                {
                    fprintf(fp1,"%c %d %d\n",matriz[linhas][colunas], linhas, colunas);
                }
                if (matriz[linhas][colunas]=='*')
                {                                               
                    fprintf(fp1,"%c %d %d\n",matriz[linhas][colunas],linhas, colunas);
                }                                                            
            }                                                        
        } 
    }
    fclose(fp1);
    return;            
}



/*Verificar se mudar o valor do dimx e dimy do main para apontador*/
