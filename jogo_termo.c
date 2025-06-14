#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>

#define totalPalavras 80
#define tamPalavra 6

#define cor_vermelho "\e[0;31m"
#define cor_verde "\e[0;32m"
#define cor_amarelo "\e[0;33m"
#define cor_reset "\e[0m"

typedef struct
{
    char nome[10];
    int pontos, tentativas, vitorias, derrotas;
    time_t tempo;
}Jogador;

Jogador jogador_info()
{
    //essa função pega as informações do player!
    system("cls");
    Jogador player;
    
    while(1){
        printf("Digite seu nome: ");
        fgets(player.nome, sizeof(player.nome), stdin);
        player.nome[strcspn(player.nome, "\n")] = '\0';
        
        int check = 0;

        //vai checar se o nome do player tem alguma letra invalida.
        for(int i=0;player.nome[i] != '\0';i++){
            if(!(isalnum(player.nome[i]) || player.nome[i] == ' ' || player.nome[i] == '_') || player.nome[i] > 127){
                check = 1;
                break;
            }
        }

        if(strlen(player.nome) < 1 || strlen(player.nome) > 9) check = 1;

        if(!check){
            player.pontos = 0, player.tentativas = 0, player.vitorias = 0, player.derrotas = 0;
            player.tempo = time(NULL);
            break;
        } else {
            printf("nome invalido, tente de novo!");
            getch();
            system("cls");
        }

    }
    system("cls");
    return player;
}

void atualizarPontos(Jogador *plr, int pts)
{
    //essa função vai ser chamada para atualizar a pontuação do jogador atual dentro do jogo
    //ela vai receber as informações do jogador e atualizar os pontos do jogador na função "jogador_info".
    //por isso, ela tá recebendo um ponteiro.
    plr->pontos += pts;
}

void salvarJogador(const Jogador *plr, const char *jogadores)
{
    FILE *save = fopen(jogadores, "a");

    if(save == NULL){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    fprintf(save, "%s - %d pts\n", plr->nome, plr->pontos);
    fclose(save);
}

void mostrarRegras()
{
    system("cls");
    printf("Cada rodada, você deve digitar uma palavra:\n\n");
    printf("se a letra estiver verde, ela está na palavra e na posição correta: \n");
    printf(cor_verde "t e " cor_vermelho "r m o");
    printf(cor_reset);
    printf("\n\n");
    
    printf("se a letra estiver amarela, ela está na palavra, mas em outra posição: \n");
    printf(cor_vermelho "a l m " cor_amarelo "a s");
    printf(cor_reset);
    printf("\n\n");
    
    printf("se a letra estiver em vermelho, ela não está na palavra: \n");
    printf(cor_verde "c i " cor_vermelho "n c o");
    printf(cor_reset);
    printf("\n\n");
    getch();
    system("cls");
}

char* gerarPalavra(char palavras[totalPalavras][100])
{
    //vai escrever todas as palavras da lista e colocar num vetor, depois vai pegar uma palavra aleatória entre elas,
    //para cada rodada ter uma palavra diferente!
    FILE *lista = fopen("listadepalavras.txt", "r");
    
    if(lista == NULL){
        printf("A lista de palavras não foi encontrada!\n");
        getch();
        system("cls");
        return NULL;
    }

    int i=0;

    //também troca todos os "\n" por "\0" para finalizar a string, no lugar de pular uma linha.
    while(fgets(palavras[i], totalPalavras, lista)!=NULL){
        palavras[i][strcspn(palavras[i], "\n")] = '\0';
        i++;
    }
    
    fclose(lista);

    //pega um número aleatório e coloca ele como index do vetor de palavras, também retorna a palavra.
    int rng = rand() % totalPalavras;
    return palavras[rng];
}

void jogoTermo(Jogador *plr)
{
    system("cls");
    int gameloop = 0;
    char palavras[totalPalavras][100];
    plr->pontos = 0;
    plr->tentativas++;
    int winStreak = 0, pts = 10, check=0;

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
                strlwr(guess);

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

            for(i=0;i<tamPalavra;i++){ //caso a letra esteja no lugar certo.
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
                printf("=============================================");
                printf("\nParabéns, você acertou!");
                plr->vitorias++;

                winStreak++;
                if(winStreak>1) pts +=2; 
                int n = tentativas * pts;

                atualizarPontos(plr, n);
                printf("\n\npontuação atual: %d\n", plr->pontos);
                printf("sequência de vitórias atual: %d", winStreak);

                break;
            } else {
                tentativas--;
            }

            if(tentativas == 0){
                printf("=============================================");
                printf("\nQue pena, você perdeu! Palavra: %s", termo);
                plr->derrotas++;

                salvarJogador(plr, "jogadores.dat");
                check=1;
            }
        }

        printf("\n\ndeseja repetir?(s/outro) ");
        char r = tolower(getch());
        printf("\n");

        if(r == 's')
        {
            if(check == 1) plr->tentativas++;
            gameloop = 0;
            system("cls");
        }
        else
        {
            if(check == 0) salvarJogador(plr, "jogadores.dat"); 
            gameloop = 1;
            system("cls");
        }
    }
}

void mostrarJogadores(const char *jogadores)
{
    system("cls");
    FILE *load = fopen(jogadores, "r");
    char linha[50];
    int total=0;

    if(load == NULL){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    while (fgets(linha, sizeof(linha), load)) {
        linha[strcspn(linha, "\n")] = '\0';
        total++;
        printf(linha);
        printf("\n");
    }

    fclose(load);
    
    printf("\n\n=============================================");
    printf("\nTotal de jogadores / tentativas: %d", total);
    printf("\n");
    getch();
    system("cls");
}

void mostrarRanking(const char *jogadores, const char *ranking)
{
    system("cls");
    FILE *load = fopen(jogadores, "r");
    char nome[200][10], temp_str[10];
    int pontos[200], top15_pts[15], i=0, j=0, temp;

    if(load == NULL){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    while (fscanf(load, "%s - %d pts", nome[i], &pontos[i]) == 2) {
        i++;
    }
    int max = i;
    
    fclose(load);

    //isso de cima vai pegar a lista de jogadores e colocar num vetor.

    //bubble sort para organizar os jogadores e suas pontuações!!!
    for(i=0;i<max-1;i++){
        for(j=0;j<max-1-i;j++){
            if(pontos[j] < pontos[j+1]){
                temp = pontos[j];
                pontos[j] = pontos[j+1];
                pontos[j+1] = temp;

                strcpy(temp_str, nome[j]);
                strcpy(nome[j], nome[j+1]);
                strcpy(nome[j+1], temp_str);
            }
        }
    }

    FILE *write = fopen(ranking, "w");

    if(write == NULL){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    int limite = (max < 15) ? max : 15;
    
    //vai colocar os valores organizados no arquivo e depois mostrar eles!
    for(i=0;i<limite;i++){
        top15_pts[i] = pontos[i];
        fprintf(write, "(%d) %s - %d pts\n", i+1, nome[i], top15_pts[i]);
        printf("(%d) %s - %d pts\n", i+1, nome[i], top15_pts[i]);
    }

    fclose(write);

    printf("\n");
    getch();
    system("cls");
}

void mostrarInformacoes(const Jogador *plr)
{
    system("cls");
    printf("Jogador(a): %s\n", plr->nome);
    printf("=============================================\n");
    printf("tentativas: %d\n", plr->tentativas);
    printf("palavras descobertas: %d\n", plr->vitorias);
    printf("derrotas: %d\n\n", plr->derrotas);

    time_t atual = time(NULL);
    int total = (int)(atual - plr->tempo);
    int min = total / 60;
    int segundos = total % 60;
    printf("você jogou por %02d:%02d", min, segundos);
    
    getch();
    system("cls");
}

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "pt_PT.UTF8");

    int menuloop = 0;

    Jogador playerAtual = jogador_info();

    while(!menuloop){
        printf("Termo\n");
        printf("bem vindo(a), %s!\n\n", playerAtual.nome);

        printf("1 - Jogar Termo\n");
        printf("2 - Ver Regras\n");
        printf("3 - Ver Pontuações\n");
        printf("4 - Mostrar Informações do Jogador\n");
        printf("5 - Trocar de Jogador\n");
        printf("6 - Sair do Jogo\n");

        char r = getch();

        if(r == '1') jogoTermo(&playerAtual);
        else if(r == '2') mostrarRegras();
        else if(r == '3'){
            system("cls");
            printf("Escolha uma opção: \n");
            printf("1 - Ver lista dos jogadores\n");
            printf("2 - Ver ranking dos 15 melhores jogadores\n");

            char resp = getch();
            if(resp == '1') mostrarJogadores("jogadores.dat");
            else if(resp == '2') mostrarRanking("jogadores.dat", "ranking.dat");
            else{
                menuloop = 0;
                system("cls");
            }
        }
        else if(r == '4') mostrarInformacoes(&playerAtual);
        else if(r == '5') playerAtual = jogador_info();
        else if(r == '6') exit(0);
        else
        {
            menuloop = 0;
            system("cls");
        }
    }

    return 0;
}