//ainda falta organizar melhor esse código e adicionar mais coisas no jogo!
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define tamPalavra 6
#define totalPalavras 30
int gameLoop = 0;

void cores(){
    //eu usei um define para ficar mais fácil de chamar essa função no código. essa função que eu tô chamando,
    //é a função que atribui alguma coisa ao texto, o hConsole é a variável que eu tô usando como argumento,
    //e nela tem outra função(eu expliquei isso lá na função do gameTermo). o número depois, é a cor.
    #define cor_padrao SetConsoleTextAttribute(hConsole, 0x07)
    #define cor_verde SetConsoleTextAttribute(hConsole, 0x02)
    #define cor_vermelho SetConsoleTextAttribute(hConsole, 0x0C)
    #define cor_amarelo SetConsoleTextAttribute(hConsole, 0x0E)
}

void repetir(){
    printf("\n\ndeseja repetir?(s/n) ");
    char r = tolower(getch());
    
    if(r == 's') gameLoop = 0;
    else if(r == 'n'){
        gameLoop = 1;
        printf("\nobrigado por jogar!\n");
        getchar();
    } else {
        printf("\ncaractere invalido! tente de novo!");
        repetir();
    }
}

void gameTermo(){
    char *lista_palavras[totalPalavras] = {"artes", "termo", "jogos", "verde", "couro", "porco", "vizao", "porta",
    "hotel", "coroa", "metal", "morte", "corte", "risco", "letra", "disco", "falso", "entra", "grato", "certo",
    "ziper", "sapos", "torre", "temor", "terno", "boina", "vidro", "raios", "funde", "finta"};

        char termo[tamPalavra];
        int tentativas=6;
        int rng = rand() % (totalPalavras - 1); //escolhe um número aleatório, que sera usado como o index na lista,
                                                //e pega uma palavra aleatória!
        strcpy(termo, lista_palavras[rng]);

        //isso é coisa da biblioteca do <windows.h>, o handle é uma estrutura(struct), o hConsole é o nome
        //da variável que vai ser chamada sempre que eu quiser colorir a palavra e o que eu armazenei nela,
        //é a função que deve ser ativada sempre que eu chamar o hConsole. 

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
        cores();

        while(tentativas != 0){
            int i, j, letras_corretas=0; 
            int cor_da_letra[tamPalavra], posicao_da_letra[tamPalavra] = {0, 0, 0, 0, 0, 0};
            
            char guess[tamPalavra];

            printf("\ndigite alguma palavra(%d): ", rng);
            strlwr(fgets(guess, tamPalavra, stdin));
            getchar();
            
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
                if(cor_da_letra[i] == 0){
                    cor_verde;
                    printf("%c ", guess[i]);
                    cor_padrao;
                }
                else if(cor_da_letra[i] == 2){
                    cor_vermelho;
                    printf("%c ", guess[i]);
                    cor_padrao;
                }
                else if(cor_da_letra[i] == 1){
                    cor_amarelo;
                    printf("%c ", guess[i]);
                    cor_padrao;
                }
            }
            if(letras_corretas == tamPalavra){
                printf("\n=============================================");
                printf("\nparabens, voce acertou! palavra: %s", termo);
                break;
            } else {
                tentativas--;
            }
            if(tentativas == 0){
                printf("\n=============================================");
                printf("\ntriste, voce perdeu!\n");
            }
        }
}

int main(){
    srand(time(NULL));

    printf("bem vindo ao termo!");
    printf("\n=============================================");
    while(!gameLoop){
        gameTermo();
        repetir();
    }
    return 0;
}