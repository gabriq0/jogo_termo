//ainda falta organizar melhor esse código e adicionar mais coisas no jogo!
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

int parada = 0;
#define tamPalavra 6
#define totalPalavras 30

void cores(){
    //eu usei um define para ficar mais fácil de chamar essa função no código. essa função que eu tô chamando,
    //é a função que atribui alguma coisa ao texto, o hConsole é a variável que eu tô usando como argumento,
    //e nela tem outra função(eu expliquei isso lá na função do gameTermo). o número depois, é a cor.
    #define cPadrao SetConsoleTextAttribute(hConsole, 0x07)
    #define cVerde SetConsoleTextAttribute(hConsole, 0x02)
    #define cVermelho SetConsoleTextAttribute(hConsole, 0x0C)
    #define cAmarelo SetConsoleTextAttribute(hConsole, 0x0E)
}
void repet(){
    printf("\n\ndeseja repetir?(y/qqlr botao) ");
    char r = getch();
    if(r == 'y') parada = 0;
    else parada = 1;
}
void gameTermo(){
    char *palavras[totalPalavras] = {"artes", "termo", "jogos", "verde", "couro", "porco", "vizao", "porta", "hotel", "coroa",
    "metal", "morte", "corte", "risco", "letra", "disco", "falso", "entra", "grato", "certo", "ziper", "sapos",
    "torre", "temor", "terno", "boina", "vidro", "raios", "funde", "finta"};

        char termo[tamPalavra];
        int tentativas=6;
        int rng = rand() % (29);

        strcpy(termo, palavras[rng]);

        //isso é coisa da biblioteca do <windows.h>, o handle é uma estrutura(struct), o hConsole é o nome
        //da variável que vai ser chamada sempre que eu quiser colorir a palavra e o que eu armazenei nela,
        //é a função que deve ser ativada sempre que eu chamar o hConsole. 

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
        cores();

        while(tentativas != 0){
            int i, j, correto[tamPalavra] = {0, 0, 0, 0, 0, 0}; 
            int pos[tamPalavra], letras=0;
            char guess[tamPalavra];

            printf("\ndigite alguma palavra(%d): ", rng);
            fgets(guess, tamPalavra, stdin);
            getchar();
            
            for(i=0;i<6;i++){ //caso a letra esteja no lugar certo.
                if(termo[i] == guess[i]){
                    pos[i] = 0;
                    correto[i] = 1;
                    letras++;
                } 
            }
            for(i=0;i<tamPalavra;i++){ //não tem esta letra!
                if(termo[i] != guess[i] && correto[i] == 0) pos[i] = 2;
            }
            for(i=0;i<tamPalavra;i++){ //caso tenha esta letra na palavra, porem, está no lugar errado.
                for(j=0;j<tamPalavra;j++){
                    if(termo[i] == guess[j] && correto[j] != 1){
                        pos[j] = 1; 
                        correto[j] = 1;
                        break;
                    } 
                }
            }
            
            for(i=0;i<tamPalavra;i++){ //caso pos=1(pinta de verde), pos=2(pinta de amarelo), pos=3(pinta de vermelho).
                if(pos[i] == 0){
                    cVerde;
                    printf("%c ", guess[i]);
                    cPadrao;
                }
                else if(pos[i] == 1){
                    cAmarelo;
                    printf("%c ", guess[i]);
                    cPadrao;
                }
                else if(pos[i] == 2){
                    cVermelho;
                    printf("%c ", guess[i]);
                    cPadrao;
                }
            }
            if(letras == tamPalavra){
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
    while(!parada){
        gameTermo();
        repet();
    }
    return 0;
}