#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define totalPalavras 80
#define tamPalavra 6

#define cor_vermelho "\e[0;31m"
#define cor_verde "\e[0;32m"
#define cor_amarelo "\e[0;33m"
#define cor_reset "\e[0m"

char* gerarPalavra(char palavras[totalPalavras][100])
{
    FILE *lista = fopen("listadepalavras.txt", "r");
    
    if(lista == NULL){
        printf("A lista de palavras não foi encontrada!\n");
        return NULL;
    }

    int i=0;

    //pega a lista de palavras e coloca em um vetor. também troca todos os "\n" por "\0" para finalizar
    //a string, no lugar de pular uma linha.
    while(fgets(palavras[i], totalPalavras, lista)!=NULL){
        palavras[i][strcspn(palavras[i], "\n")] = '\0';
        i++;
    }
    
    fclose(lista);

    //pega um número aleatório e coloca ele como index do vetor de palavras, também retorna a palavra.
    int rng = rand() % totalPalavras;
    return palavras[rng];
}

void jogoTermo(){
    int gameloop = 0;
    char palavras[totalPalavras][100];

    while(!gameloop){
        int tentativas=6;
        char termo[tamPalavra];

        strcpy(termo, gerarPalavra(palavras));

        while(tentativas != 0){
            int i, j, letras_corretas=0, tamLetras=0; 
            int cor_da_letra[tamPalavra], posicao_da_letra[tamPalavra] = {0, 0, 0, 0, 0, 0};
            char guess[tamPalavra];

            while(1){
                printf("=============================================\n");
                fflush(stdin);
                printf("(%d)", tentativas);
                printf(" digite alguma palavra: ");
                scanf("%s", &guess);
                    
                if(strlen(guess) == 5){
                    getchar();
                    break;
                } else {
                    printf("\n----------------------------------\n");
                    printf("|apenas palavras de cinco letras!|");
                    printf("\n----------------------------------\n");
                    fflush(stdin);
                }
            }

            for(i=0;i<6;i++){ //caso a letra esteja no lugar certo.
                if(termo[i] == guess[i]){
                    cor_da_letra[i] = 0;
                    posicao_da_letra[i] = 1;
                    letras_corretas++;
                } 
            }

            for(i=0;i<tamPalavra;i++){ //não tem esta letra!
                if(termo[i] != guess[i] && posicao_da_letra[i] == 0) cor_da_letra[i] = 2;
            }

            for(i=0;i<tamPalavra;i++){ //caso tenha esta letra na palavra, porem, está no lugar errado.
                for(j=0;j<tamPalavra;j++){
                    if(termo[i] == guess[j] && posicao_da_letra[j] != 1){
                        cor_da_letra[j] = 1;
                        break;
                    } 
                }
            }

            for(i=0;i<tamPalavra;i++){ //caso = 0(pinta de verde), = 1(pinta de amarelo), = 2(pinta de vermelho).
                if(cor_da_letra[i] == 0) printf(cor_verde "%c ", guess[i]);
                else if(cor_da_letra[i] == 2) printf(cor_vermelho "%c ", guess[i]);
                else if(cor_da_letra[i] == 1) printf(cor_amarelo "%c ", guess[i]);
                printf(cor_reset);
            }
            printf("\n");

            if(letras_corretas == tamPalavra){
                printf("\n=============================================");
                printf("\nparabens, voce acertou!");
                break;
            } else {
                tentativas--;
            }
            if(tentativas == 0){
                printf("\n=============================================");
                printf("\ntriste, voce perdeu! palavra: %s", termo);
            }
        }
        printf("\n\ndeseja repetir?(s/outro) ");
        char r = tolower(getch());
        printf("\n");
        if(r == 's') gameloop = 0;
        else gameloop = 1;
    }
}

int main(){
    srand(time(NULL));
    jogoTermo();

    return 0;
}