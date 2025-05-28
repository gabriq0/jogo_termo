//ainda falta organizar melhor esse código e adicionar mais coisas no jogo!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define tamPalavra 6
#define totalPalavras 30
int gameLoop = 0;
char nomeJogador[10];

void cores(){
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
        system("cls");
    }
    else {
        printf("\ncaractere invalido! tente de novo!");
        repetir();
    }
}

void gameTermo(){
    char *lista_palavras[] = {"artes", "termo", "jogos", "verde", "couro", "porco", "vizao", "porta",
    "hotel", "coroa", "metal", "morte", "corte", "risco", "letra", "disco", "falso", "entra", "grato", "certo",
    "ziper", "sapos", "torre", "temor", "terno", "boina", "vidro", "raios", "funde", "finta"};

    gameLoop = 0;
    while(!gameLoop){
        char termo[tamPalavra];
        int tentativas=6;
        int rng = rand() % (totalPalavras - 1); 
        
        strcpy(termo, lista_palavras[rng]); 

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
        cores();

        while(tentativas != 0){
            int i, j, letras_corretas=0, tamLetras=0; 
            int cor_da_letra[tamPalavra], posicao_da_letra[tamPalavra] = {0, 0, 0, 0, 0, 0};
            char guess[tamPalavra];
            
            while(1){
            printf("\n=============================================\n");
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
        repetir();
    }
}

void ler_regras(){
    NULL;
}

void mostrar_pts(){
    NULL;
}

int main(){
    srand(time(NULL));
   
    int tamNome = 0, menuLoop = 0;
    
    while(tamNome == 0){
    printf("Digite seu nome: ");
    scanf("%s", nomeJogador);
    
    if(strlen(nomeJogador) < 3) tamNome = 0;
    else tamNome = 1;
    }
    
    system("cls");
    
    while(menuLoop == 0){
        printf("Bem vindo(a) ao Termo!");
        printf("\nola %s! Por favor, escolha uma opcao!\n\n", nomeJogador);

        printf("1 - Jogar Termo\n");
        printf("2 - Ler regras\n");
        printf("3 - Mostrar pontuacaoo dos jogadores\n");
        printf("4 - Trocar de jogador\n");
        printf("5 - Sair do Jogo\n");

        char escolha = getch();
        system("cls");

        if(escolha == '1'){
            getchar();
            gameTermo();
        }
        else if (escolha == '2') ler_regras();
        else if(escolha == '3') mostrar_pts();
        else if(escolha == '4') NULL;
        else if(escolha == '5') exit(0);
        else{
            printf("opcao invalida, tente de novo!");
            menuLoop = 0;
        }
    }
    return 0;
}