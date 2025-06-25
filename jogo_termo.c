#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <locale.h>

#define totalPalavras 100
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

Jogador jogador_info(){
    //essa função pega as informações do player!
    system("cls");
    Jogador player;
    
    while(1){
        printf("Digite seu nome: ");
        fgets(player.nome, sizeof(player.nome), stdin);
        player.nome[strcspn(player.nome, "\n")] = '\0';
        
        int check = 0; //se o check continuar 0, as infos serão aceitas. caso se torne 1, serão recusados.

        //vai checar se o nome do player tem alguma letra invalida.
        for(int i=0;player.nome[i] != '\0';i++){
            if(!(isalnum(player.nome[i]) || player.nome[i] == '_') || player.nome[i] > 127){
                check = 1;
                break;
            }
        }

        if(strlen(player.nome) < 1 || strlen(player.nome) > 9) check = 1;

        //isso tudo aqui, vai ler a lista dos jogadores: se o nome já existir, não pode utilizá-lo.
        FILE *file = fopen("data/jogadores.dat", "r");
        char linha[100], nome[200][10];
        int i=0, pontos[200];

        while(fgets(linha, sizeof(linha), file)){
            if(sscanf(linha, "%s - %d pts", nome[i], &pontos[i]) == 2) {
                if(strcmp(nome[i], player.nome) == 0){
                    check = 1;
                    break;
                }

                i++;
                fgets(linha, sizeof(linha), file);
            }
        }
        fclose(file);

        if(!check){
            player.pontos = 0, player.tentativas = 0, player.vitorias = 0, player.derrotas = 0;
            player.tempo = time(NULL);
            break;
        } else {
            printf("nome invalido ou alguém já usou ele, tente de novo!");
            getch();
            system("cls");
        }

    }
    system("cls");
    return player;
}

void mostrarRegras(){
    //aqui explica as regras e dicas sobre este jogo.
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
    printf("==========================================================================================\n\n");

    printf("* Quanto maior mais palavras, você acertar em sequência sem perder, maior será sua pontuação!\n\n");
    printf("* Quando você salvar suas informações, apenas sua maior pontuação será salva.\n\n");
    printf("* O jogo não salva seus pontos automaticamente, você deve salvar pelo menu quando quiser atualizar suas informações.\n\n");

    getch();
    system("cls");
}

void atualizarPontos(Jogador *plr, int pontuacao){
    //vai atualizar os pontos ao final de uma jogatina caso a pontuação seja maior do que o "high score" anterior.
    if(pontuacao > plr->pontos) plr->pontos = pontuacao;
}

void salvarJogador(const char *filename, Jogador *plr){
    //essa função atualiza as informações dos jogadores na lista geral com todos os players.
    system("cls");
    
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("data/temp.dat", "w");
    
    if(!file){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    if (!temp) {
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    time_t atual = time(NULL);
    int total = (int)(atual - plr->tempo);
    int min = total / 60;
    int segundos = total % 60;

    char line1[1000], line2[1000];
    int found = 0;
    int newScore = plr->pontos;

    while (fgets(line1, sizeof(line1), file)) {
        fgets(line2, sizeof(line2), file);

        char nome[10];
        int pontos;

        if(sscanf(line1, "%s - %d pts", nome, &pontos) == 2) {
            if (strcmp(nome, plr->nome) == 0) {
                found = 1;
                if (newScore > pontos) {
                    fprintf(temp, "%s - %d pts\n", nome, newScore); //atualiza os novos pontos do player
                    fprintf(temp, "tentativas: %d / let. corretas: %d / derrotas: %d / tempo de jogo: %02d:%02d\n", plr->tentativas, plr->vitorias, plr->derrotas, min, segundos);
                } else {
                    fputs(line1, temp); //não atualiza os pontos do player
                    fprintf(temp, "tentativas: %d / let. corretas: %d / derrotas: %d / tempo de jogo: %02d:%02d\n", plr->tentativas, plr->vitorias, plr->derrotas, min, segundos);
                }
            } else {
                fputs(line1, temp); //mantém os outros jogadores
                fputs(line2, temp);
            }
        }
    }

    //caso o jogador não estivesse na lista antes, isso vai colocar ele
    if (!found) {
        fprintf(temp, "%s - %d pts\n", plr->nome, newScore);
        fprintf(temp, "tentativas: %d / let. corretas: %d / derrotas: %d / tempo de jogo: %02d:%02d\n", plr->tentativas, plr->vitorias, plr->derrotas, min, segundos);
    }

    fclose(file);
    fclose(temp);

    //troca a temp com o arquivo original
    remove(filename);
    rename("data/temp.dat", filename);

    system("cls");
}

char* gerarPalavra(char palavras[totalPalavras][100]){
    //essa função gera uma palavra da lista. ela é chamada em todas as rodadas do jogo.
    FILE *lista = fopen("data/listadepalavras.txt", "r");
    
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

void jogoTermo(Jogador *plr){
    //essa função é o jogo inteiro.
    system("cls");
    int gameloop = 0;
    char palavras[totalPalavras][100];
    int winStreak = 0, pontuacao=0, pts = 10;
    plr->tentativas++;

    while(!gameloop){
        int tentativas=6, check=0;
        char termo[tamPalavra];

        char *palavraGerada = gerarPalavra(palavras);
        if (palavraGerada == NULL) gameloop = 1;
        else strcpy(termo, palavraGerada);

        while(tentativas != 0){
            int i, j, letras_corretas=0, tamLetras=0; 
            int cor_da_letra[tamPalavra], posicao_da_letra[tamPalavra] = {0, 0, 0, 0, 0, 0};
            char guess[tamPalavra];

            //vai checar se a palavra escrita é do mesmo tamanho da resposta.
            while(1){
                printf("=============================================\n");
                fflush(stdin);
                printf("(%d | %d) ", tentativas, pontuacao);
                printf("Digite alguma palavra: ");
                scanf("%s", &guess);
                strlwr(guess);

                if(strlen(guess) == strlen(termo)){
                    getchar();
                    break;
                } else {
                    printf("\n----------------------------------\n");
                    printf("|apenas palavras de cinco letras!|");
                    printf("\n----------------------------------\n");
                    fflush(stdin);
                }
            }

            //checa se a letra está no lugar certo.
            for(i=0;i<tamPalavra;i++){
                if(termo[i] == guess[i]){
                    cor_da_letra[i] = 0;
                    posicao_da_letra[i] = 1;
                    letras_corretas++;
                } 
            }

            //checa se a letra não está na resposta.
            for(i=0;i<tamPalavra;i++){
                if(termo[i] != guess[i] && posicao_da_letra[i] == 0) cor_da_letra[i] = 2;
            }

            //checa se a letra na resposta, porem, está no lugar errado.
            for(i=0;i<tamPalavra;i++){
                for(j=0;j<tamPalavra;j++){
                    if(termo[i] == guess[j] && posicao_da_letra[j] != 1){
                        cor_da_letra[j] = 1;
                        break;
                    } 
                }
            }

            //caso = 0 (pinta de verde), = 1 (pinta de amarelo), = 2 (pinta de vermelho).
            for(i=0;i<tamPalavra;i++){ 
                if(cor_da_letra[i] == 0) printf(cor_verde "%c ", guess[i]);
                else if(cor_da_letra[i] == 2) printf(cor_vermelho "%c ", guess[i]);
                else if(cor_da_letra[i] == 1) printf(cor_amarelo "%c ", guess[i]);
                printf(cor_reset);
            }
            printf("\n");

            //checa se o player acertou.
            if(letras_corretas == tamPalavra){
                printf("=============================================");
                printf("\nParabéns, você acertou!");
                plr->vitorias++;

                winStreak++;
                if(winStreak>1) pts +=2; 
                pontuacao += tentativas * pts;
                
                printf("\n\npontuação atual: %d\n", pontuacao);
                printf("sequência de vitórias atual: %d\n", winStreak);
                atualizarPontos(plr, pontuacao);

                break;
            } else {
                tentativas--;
            }

            //checa se o player perdeu (esgotou as tentativas)
            if(tentativas == 0){
                printf("=============================================");
                printf("\nQue pena, você perdeu! Palavra: %s", termo);
                printf("\nPontuação final: %d\n", pontuacao);
                plr->derrotas++;
                atualizarPontos(plr, pontuacao);

                pontuacao=0, winStreak=0, pts=10;
                check=1;
            }
        }

        //checa se o player ainda quer continuar (caso sim, seus pontos continuam)
        printf("\ndeseja repetir?(s/outro) ");
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
            gameloop = 1;
            system("cls");
        }
    }
}

void mostrarJogadores(const char *jogadores){
    //essa função serve para mostrar a lista com todos os jogadores.
    system("cls");
    FILE *load = fopen(jogadores, "r");
    char linha[1000];
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
        if(total % 2 == 0) printf("==========================================================================================\n");
    }

    fclose(load);
    
    printf("\nTotal de jogadores: %d", total/2);
    printf("\n");
    getch();
    system("cls");
}

void mostrarRanking(const char *jogadores, const char *ranking){
    //essa função vai mostrar o ranking com os 15 melhores jogadores.
    system("cls");
    FILE *load = fopen(jogadores, "r");
    
    char nome[200][10], temp_str[10], ignorar[100], linha[100];
    int pontos[200]; 
    int i=0, j=0, temp;

    if(load == NULL){
        printf("Não foi possivel abrir o arquivo!\n");
        getch();
        system("cls");
        return;
    }

    //isso vai ler a lista e pegar apenas o nome e os pontos de cada player.
    while(fgets(linha, sizeof(linha), load)){
        if(sscanf(linha, "%s - %d pts", nome[i], &pontos[i]) == 2) {
            i++;
            fgets(linha, sizeof(linha), load);
        }
    }
    int max = i;
    
    fclose(load);

    //bubble sort para organizar os jogadores e suas pontuações.
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
    
    //vai escrever o ranking no arquivo e depois mostrar ele no jogo.
    for(i=0;i<limite;i++){
        fprintf(write, "(%d) %s - %d pts\n", i+1, nome[i], pontos[i]);
        printf("(%d) %s - %d pts\n", i+1, nome[i], pontos[i]);
    }

    fclose(write);

    printf("\n");
    getch();
    system("cls");
}

void mostrarInformacoes(const Jogador *plr){
    //essa função mostra as informações do player atual.
    system("cls");
    
    printf("Jogador(a): %s\n", plr->nome);
    printf("\n=============================================\n\n");
    
    printf("maior pontuação: %d\n", plr->pontos);
    printf("tentativas: %d\n", plr->tentativas);
    printf("palavras descobertas: %d\n", plr->vitorias);
    printf("derrotas: %d\n\n", plr->derrotas);
    
    printf("=============================================\n");

    time_t atual = time(NULL);
    int total = (int)(atual - plr->tempo);
    int min = total / 60;
    int segundos = total % 60;
    printf("você jogou por %02d:%02d", min, segundos);
    
    getch();
    system("cls");
}

int main(){
    //o main tem o menu do jogo, que vai chamar cada função dependendo do que for selecionado.
    srand(time(NULL));
    setlocale(LC_ALL, "pt_PT.UTF8");

    int menuloop = 0;

    Jogador playerAtual = jogador_info();

    while(!menuloop){
        printf(
        "  _____ _____ ____  __  __  ___   \n"
        " |_   _| ____|  _ \\|  \\/  |/ _ \\  \n"
        "   | | |  _| | |_) | |\\/| | | | | \n"
        "   | | | |___|  _ <| |  | | |_| | \n"
        "   |_| |_____|_| \\_\\_|  |_|\\___/  \n");

        printf("\nbem vindo(a), %s!\n\n", playerAtual.nome);

        printf("1 - Jogar Termo\n");
        printf("2 - Ver Regras\n");
        printf("3 - Ver Estatísticas\n");
        printf("4 - Salvar Jogo\n");
        printf("5 - Trocar de Jogador\n");
        printf("6 - Sair do Jogo\n");

        char r = getch();

        if(r == '1') jogoTermo(&playerAtual);
        else if(r == '2') mostrarRegras();
        else if(r == '3'){
            system("cls");
            printf("Escolha uma opção: \n\n");
            printf("1 - Ver estatísticas pessoais\n");
            printf("2 - Ver lista de todos os jogadores\n");
            printf("3 - Ver ranking dos 15 melhores jogadores\n");

            char resp = getch();
            if(resp == '1') mostrarInformacoes(&playerAtual);
            else if(resp == '2') mostrarJogadores("data/jogadores.dat");
            else if(resp == '3') mostrarRanking("data/jogadores.dat", "data/ranking.dat");
            else{
                menuloop = 0;
                system("cls");
            }
        }
        else if(r == '4') salvarJogador("data/jogadores.dat", &playerAtual);
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