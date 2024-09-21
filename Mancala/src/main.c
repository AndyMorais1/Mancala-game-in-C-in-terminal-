#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    char *name;
    int wins;
    int games_played;
    int draws;
    int loses;
    bool playing_status;
    int side;
    bool turn;
    bool advanced;
} tPlayer, *Player;

struct GameController
{
    tPlayer *players;
    int num_players;
    bool game_status;
    tPlayer *player_1, *player_2, CPU;
    int board[14];
    int num_games;
};

bool has_players(struct GameController *controller)
{
    return controller->num_players != 0;
}

void free_players(struct GameController *controller) {
    for (int i = 0; i < controller->num_players; i++) {
        free(controller->players[i].name);
    }
    free(controller->players);
}

bool is_player_registered(const struct GameController *controller, const char *name)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if (strcmp(name, controller->players[i].name) == 0)
        {
            return true;
        }
    }
    return false;
}

bool is_player_playing(struct GameController *controller, char *name)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if ((strcmp(controller->players[i].name, name)) == 0)
        {
            return controller->players[i].playing_status;
        }
    }
    return false;
}

bool is_player_turn(struct GameController *controller, char *name)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if ((strcmp(controller->players[i].name, name)) == 0)
        {
            return controller->players[i].turn;
        }
    }
    return false;
}

void change_turn(struct GameController *controller)
{
    if (controller->player_2){
    controller->player_1->turn = !controller->player_1->turn;
    controller->player_2->turn = !controller->player_2->turn;
    }
    else{
        controller->player_1->turn = !controller->player_1->turn;
    }
}

bool is_game_being_played(const struct GameController *controller)
{
    return controller->game_status;
}

tPlayer *get_player(struct GameController *controller, char *name)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if (strcmp(controller->players[i].name, name) == 0)
        {
            return &(controller->players[i]);
        }
    }
    return NULL;
}

void sort_players(struct GameController *controller)
{
    tPlayer temp;
    for (int i = 0; i < controller->num_players - 1; i++)
    {
        for (int j = i + 1; j < controller->num_players; j++)
        {
            if (controller->players[i].wins < controller->players[j].wins)
            {
                temp = controller->players[i];
                controller->players[i] = controller->players[j];
                controller->players[j] = temp;
            }
            else if (controller->players[i].wins == controller->players[j].wins)
            {
                if ((strcmp(controller->players[i].name, controller->players[j].name)) > 0)
                {
                    temp = controller->players[i];
                    controller->players[i] = controller->players[j];
                    controller->players[j] = temp;
                }
            }
        }
    }
}

void initialize_board(struct GameController *controller)
{
    int size = sizeof(controller->board) / sizeof(controller->board[0]);
    int half = size / 2;
    for (int i = 1; i <= half; i++)
    {
        controller->board[i] = 4;
    }

    for (int i = half; i < size; i++)
    {
        controller->board[i] = 4;
    }
    controller->board[0] = controller->board[13] = 0;
}

bool is_game_ending(struct GameController *controller)
{
    if (controller->player_2)
    {
        controller->player_1->side = 0;
        controller->player_2->side = 0;
        for (int i = 0; i < 6; i++)
        {
            controller->player_1->side += controller->board[i + 7];
            controller->player_2->side += controller->board[i + 1];
        }
        if (controller->player_1->side == 0 || controller->player_2->side == 0)
        {
            controller->game_status = false;
            controller->num_games++;
            controller->board[13] += controller->player_1->side;
            controller->board[0] += controller->player_2->side;
            if (controller->board[13] > controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].wins++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->player_2->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].loses++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                }
            }
            else if (controller->board[13] == controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].draws++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->player_2->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].draws++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                }
            }
            else if (controller->board[13] < controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].loses++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->player_2->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].wins++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                }
            }
            return true;
        }
    }
    else
    {
        controller->player_1->side = 0;
        controller->CPU.side = 0;
        for (int i = 0; i < 6; i++)
        {
            controller->player_1->side += controller->board[i + 7];
            controller->CPU.side += controller->board[i + 1];
        }

         if (controller->player_1->side == 0 || controller->CPU.side == 0)
        {
            controller->game_status = false;
            controller->num_games++;
            controller->board[13] += controller->player_1->side;
            controller->board[0] += controller->CPU.side;
            if (controller->board[13] > controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].wins++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->players[i].name, "CPU\0")) == 0)
                    {
                        controller->players[i].loses++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;  
                    }
                }
            }
            else if (controller->board[13] == controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].draws++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->players[i].name, "CPU\0")) == 0)
                    {
                        controller->players[i].draws++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;    
                    }
                }
            }
            else if (controller->board[13] < controller->board[0])
            {
                for (int i = 0; i < controller->num_players; i++)
                {
                    if ((strcmp(controller->player_1->name, controller->players[i].name)) == 0)
                    {
                        controller->players[i].loses++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;
                    }
                    if ((strcmp(controller->players[i].name, "CPU\0")) == 0)
                    {
                        controller->players[i].wins++;
                        controller->players[i].turn = false;
                        controller->players[i].playing_status = false;   
                    }
                }
            }
            return true;
        }  
    }
    return false;
}

void normal_cpu(struct GameController *controller)
{
    new_play:
    int lastReceptor, receptor, chosenBin, giveAwaySeeds;
        for (int i = 1; i <= 6; i++){
            if (controller->board[7 - i] > 0){
                chosenBin = 7 - i;
                break;
            }
        }
        giveAwaySeeds = controller->board[chosenBin];
        controller->board[chosenBin] = 0;
        receptor = chosenBin - 1;
        while (giveAwaySeeds > 0)
        {
            if (receptor == 13)
            {
                receptor = 6;
            }
            controller->board[receptor]++;
            giveAwaySeeds--;
            if (giveAwaySeeds == 0)
            {
                lastReceptor = receptor;
            }
            while (receptor >= 7)
            {
                receptor++;
                break;
            }
            if (receptor < 7)
            {
                receptor--;
            }
            if (receptor < 0)
            {
                receptor = 7;
            }
        }
        if (lastReceptor == 0)
        {
            goto new_play;
        }
        else if (controller->board[lastReceptor] == 1 && lastReceptor <= 6)
        {
            controller->board[0] += controller->board[lastReceptor] + controller->board[lastReceptor + 6];
            controller->board[lastReceptor] = 0;
            controller->board[lastReceptor + 6] = 0;
            change_turn(controller);
        }
        else
        {
            change_turn(controller);
        }
}

void cpu_brule(struct GameController *controller, int *chosenBin, bool *preference){
int lastReceptor, giveAwaySeeds;
    for (int i = 1; i <= 6; i++)
    {
        if (controller->board[7 - i] > 0)
        { 
            giveAwaySeeds = controller->board[7- i];
            lastReceptor = (7 - i) - giveAwaySeeds;
            if (lastReceptor > 0 && lastReceptor <=6)
            {
                if (controller->board[lastReceptor] == 0)
                {
                    *chosenBin = 7 - i;
                    *preference = true;
                    return;
                }                
            }
        }  
    }   
}

void cpu_arule(struct GameController *controller, int *chosenBin, bool *preference){
int lastReceptor,giveAwaySeeds;
    for (int i = 1; i <= 6; i++)
    {
        if (controller->board[7 - i] > 0)
        { 
            giveAwaySeeds = controller->board[7- i];
            lastReceptor = (7 - i) - giveAwaySeeds;
            if (lastReceptor == 0 )
            {
                *chosenBin = 7 - i;
                *preference = true;
                return;   
            }
        }
    }  
}

void cpu_rightRule(struct GameController *controller, int *chosenBin, bool *preference){
        for (int i = 6; i >= 1; i--){
            if (controller->board[7 - i] > 0){
                *chosenBin = 7 - i;
                *preference = true;
                return;
            }
        } 
}

void advanced_cpu(struct GameController *controller)
{
    if(!controller->player_1->turn)
    {
        new_play:
        int lastReceptor, receptor, chosenBin, giveAwaySeeds;
        bool preference = false;
        cpu_brule(controller, &chosenBin, &preference);
        if (!preference){
            cpu_arule(controller, &chosenBin, &preference);
        }
        if (!preference){
            cpu_rightRule(controller, &chosenBin, &preference);
        }
        giveAwaySeeds = controller->board[chosenBin];
        controller->board[chosenBin] = 0;
        receptor = chosenBin - 1;
        while (giveAwaySeeds > 0)
        {
            if (receptor == 13)
            {
                receptor = 6;
            }
            controller->board[receptor]++;
            giveAwaySeeds--;
            if (giveAwaySeeds == 0)
            {
                lastReceptor = receptor;
            }
            while (receptor >= 7)
            {
                receptor++;
                break;
            } 
            if (receptor < 7)
            {
                receptor--;
            }
            if (receptor < 0)
            {
                receptor = 7;
            }
        }
        if (lastReceptor == 0)
        {
           
            goto new_play;
        }
        else if (controller->board[lastReceptor] == 1 && lastReceptor <= 6)
        {
            controller->board[0] += controller->board[lastReceptor] + controller->board[lastReceptor + 6];
            controller->board[lastReceptor] = 0;
            controller->board[lastReceptor + 6] = 0;
            change_turn(controller);
        }
        else
        {
            change_turn(controller);
        }
    }    
}

bool is_level_advanced(struct GameController *controller){
    return controller->CPU.advanced;
}

void register_player(struct GameController *controller, const char *name)
{
    controller->num_players++;
    controller->players = realloc(controller->players, sizeof(tPlayer) * controller->num_players);
    controller->players[controller->num_players - 1].wins = 0;
    controller->players[controller->num_players - 1].loses = 0;
    controller->players[controller->num_players - 1].games_played = 0;
    controller->players[controller->num_players - 1].draws = 0;
    controller->players[controller->num_players - 1].side = 0;
    controller->players[controller->num_players - 1].name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(controller->players[controller->num_players - 1].name, name);
    controller->players[controller->num_players - 1].playing_status = false;
    controller->players[controller->num_players - 1].turn = false;
}

void list_players(struct GameController *controller)
{
    sort_players(controller);
    for (int i = 0; i < controller->num_players; i++)
    {
        printf("%s %d %d %d %d\n", controller->players[i].name,
               controller->players[i].games_played, controller->players[i].wins,
               controller->players[i].draws, controller->players[i].loses);
    }
}

void start_game(struct GameController *controller, char *player1, char *player2)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if ((strcmp(controller->players[i].name, player1)) == 0)
        {
            controller->players[i].playing_status = true;
            controller->players[i].turn = true;
            controller->players[i].games_played++;
        }
        if ((strcmp(controller->players[i].name, player2)) == 0)
        {
            controller->players[i].playing_status = true;
            controller->players[i].turn = false;
            controller->players[i].games_played++;
        }
    }
    controller->game_status = true;
    controller->player_1 = get_player(controller, player1);
    controller->player_2 = get_player(controller, player2);
    initialize_board(controller);
}

void start_automatic_game(struct GameController *controller, char *player1, char *level)
{
    for (int i = 0; i < controller->num_players; i++)
    {
        if ((strcmp(controller->players[i].name, player1)) == 0)
        {
            controller->players[i].playing_status = true;
            controller->players[i].turn = true;
            controller->players[i].games_played++;
        }
        if ((strcmp(controller->players[i].name, "CPU\0")) == 0)
        {
            controller->players[i].games_played++;
            if ((strcmp(level, "Avançado") == 0))
            {
                controller->CPU.advanced = true;
            }
            else
            {
                controller->CPU.advanced = false;
            }                
        }   
    }
    controller->game_status = true;
    controller->player_1 = get_player(controller, player1);
    controller->player_2 = NULL;
    initialize_board(controller);
}

void show_game_details(struct GameController *controller)
{
    if (controller->player_1 && controller->player_2)
    {
        int size = sizeof(controller->board) / sizeof(controller->board[0]);
        int half = size / 2;
        printf("%s X %s\n", controller->player_1->name, controller->player_2->name);
        for (int i = 0; i < half; i++)
        {
            if (i == 0)
            {
                printf("[%d]\t", controller->board[i]);
            }
            else
            {
                printf("(%d)\t", controller->board[i]);
            }
        }
        printf("\n\t");
        for (int i = half; i < size; i++)
        {
            if (i == 13)
            {
                printf("[%d]\n", controller->board[i]);
            }
            else
            {
                printf("(%d)\t", controller->board[i]);
            }
        }
    }
    else
    {
        int size = sizeof(controller->board) / sizeof(controller->board[0]);
        int half = size / 2;
        printf("%s X CPU\n", controller->player_1->name);
        for (int i = 0; i < half; i++)
        {
            if (i == 0)
            {
                printf("[%d]\t", controller->board[i]);
            }
            else
            {
                printf("(%d)\t", controller->board[i]);
            }
        }
        printf("\n\t");
        for (int i = half; i < size; i++)
        {
            if (i == 13)
            {
                printf("[%d]\n", controller->board[i]);
            }
            else
            {
                printf("(%d)\t", controller->board[i]);
            }
        }
    }    
}

int play(struct GameController *controller, int position)
{
    int lastReceptor, receptor, chosenBin, giveAwaySeeds;
    if ((controller->player_1->turn))
    {
        if (position >= 1 && position <= 6)
        {
            chosenBin = position + 6;
            if (controller->board[chosenBin] > 0)
            {
                giveAwaySeeds = controller->board[chosenBin];
                controller->board[chosenBin] = 0;
                receptor = chosenBin + 1;
                while (giveAwaySeeds > 0)
                {
                    if (receptor == 0)
                    {
                        receptor = 7;
                    }
                    controller->board[receptor]++;
                    giveAwaySeeds--;
                    if (giveAwaySeeds == 0)
                    {
                        lastReceptor = receptor;
                    }
                    while (receptor <= 6)
                    {
                        receptor--;
                        break;
                    }
                    if (receptor > 6)
                    {
                        receptor++;
                    } 
                    if (receptor > 13)
                    {
                        receptor = 6;
                    } 
                }
                if (lastReceptor == 13)
                {
                    return 2; 
                }
                else if (controller->board[lastReceptor] == 1 && lastReceptor > 6)
                {
                    controller->board[13] += controller->board[lastReceptor] + controller->board[lastReceptor - 6];
                    controller->board[lastReceptor] = 0;
                    controller->board[lastReceptor - 6] = 0;
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;   
            }
        }
        else
        {
            return 0;
        }
    }
    else if (controller->player_2->turn)
    {
        if (position >= 1 && position <= 6)
        {
            chosenBin = 7 - position;
            if (controller->board[chosenBin] > 0)
            {
                giveAwaySeeds = controller->board[chosenBin];
                controller->board[chosenBin] = 0;
                receptor = chosenBin - 1;
                while (giveAwaySeeds > 0)
                {
                    if (receptor == 13)
                    {
                        receptor = 6;
                    } 
                    controller->board[receptor]++;
                    giveAwaySeeds--;
                    if (giveAwaySeeds == 0)
                    {
                        lastReceptor = receptor;
                    }
                    while (receptor >= 7)
                    {
                        receptor++;
                        break;
                    }
                    if (receptor < 7)
                    {
                        receptor--;
                    }
                    if (receptor < 0)
                    {
                        receptor = 7;
                    }
                }
                if (lastReceptor == 0)
                {
                    return 2;
                }
                else if (controller->board[lastReceptor] == 1 && lastReceptor <= 6) 
                {
                    controller->board[0] += controller->board[lastReceptor] + controller->board[lastReceptor + 6];
                    controller->board[lastReceptor] = 0;
                    controller->board[lastReceptor + 6] = 0;
                    return 1;
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

void quit(struct GameController *controller, char *player1, char *player2)
{
    if (player1 && player2)
    {
        for (int i = 0; i < controller->num_players; i++)
        {
            if ((strcmp(controller->players[i].name, player1)) == 0)
            {
                controller->players[i].loses++;
                controller->players[i].playing_status = false;
            }
            if ((strcmp(controller->players[i].name, player2)) == 0)
            {
                controller->players[i].loses++;
                controller->players[i].playing_status = false;
            }
        }
        controller->num_games++;
        controller->game_status = false;
    }
    else
    {
        for (int i = 0; i < controller->num_players; i++)
        {
            if (controller->player_2)
            {
                if ((strcmp(controller->players[i].name, player1)) == 0)
                {
                    controller->players[i].loses++;
                    controller->players[i].playing_status = false;
                }
                if (((strcmp(controller->players[i].name, controller->player_1->name)) == 0) && (controller->players[i].playing_status))
                {
                    controller->players[i].wins++;
                    controller->players[i].playing_status = false;
                }
                else if (((strcmp(controller->players[i].name, controller->player_2->name)) == 0) && (controller->players[i].playing_status))
                {
                    controller->players[i].wins++;
                    controller->players[i].playing_status = false;
                }
            }
            else
            {
                if ((strcmp(controller->players[i].name, player1)) == 0)
                {
                    controller->players[i].loses++;
                    controller->players[i].playing_status = false;
                }
                if ((strcmp(controller->players[i].name, "CPU\0")) == 0)
                {
                    controller->players[i].wins++;
                    controller->players[i].playing_status = false;
                }
            }
        }
        controller->num_games++;
        controller->game_status = false;
    }  
}

void save_game(struct GameController *controller, char *file_name)
{
    FILE *arq = fopen(file_name, "w");
    fprintf(arq,"%d\n",controller->num_games);
    for (int i = 0; i < controller->num_players; i++)
    {
        fprintf(arq, "%s;", controller->players[i].name);
        fprintf(arq, "%d;", controller->players[i].games_played);
        fprintf(arq, "%d;", controller->players[i].wins);
        fprintf(arq, "%d;", controller->players[i].draws);
        fprintf(arq, "%d\n", controller->players[i].loses);
    }
    fclose(arq);
}

void read_game(struct GameController *controller, char *file_name)
{
    FILE *arq = fopen(file_name, "r");
    controller->players = NULL;
    controller->num_players = 0;
    controller->num_games=0;
    char *line = NULL;
    size_t len = 0;
    fscanf(arq,"%d\n",&controller->num_games);
    while (getline(&line, &len, arq) != -1)
    {
        line[strlen(line) - 1] = '\0';
        controller->players = realloc(controller->players, (controller->num_players + 1) * sizeof(tPlayer));
        char *name = strtok(line, ";");
        controller->players[controller->num_players].name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(controller->players[controller->num_players].name, name);
        controller->players[controller->num_players].games_played = atoi(strtok(NULL, ";"));
        controller->players[controller->num_players].wins = atoi(strtok(NULL, ";"));
        controller->players[controller->num_players].draws = atoi(strtok(NULL, ";"));
        controller->players[controller->num_players].loses = atoi(strtok(NULL, ";"));
        controller->num_players++;
        controller->num_games=atoi(strtok("NULL"," "));
        free(line);
        line = NULL;
    }
    if (line != NULL){
        free(line);
        fclose(arq);
    }
}

int main()
{
    struct GameController controller;
    controller.players = NULL;
    controller.num_players = 0;
    controller.game_status = false;
    char *line = NULL;
    size_t len = 0;
    register_player(&controller, "CPU\0");
    while (true)
    {
        getline(&line, &len, stdin);
        line[strlen(line) - 1] = '\0';
        if (strlen(line) == 0)
        {
            break;
        }
        char *instruction = strtok(line, " ");
        if ((strcmp(instruction, "RJ") == 0))
        {
            char *player1 = strtok(NULL, " ");
            if (player1)
            {
                if (is_player_registered(&controller, player1))
                {
                    printf("Jogador existente.\n");
                }
                else
                {
                    register_player(&controller, player1);
                    printf("Jogador registado com sucesso.\n");
                }
            }
        }
        else if (strcmp(instruction, "LJ") == 0)
        {
            if (has_players(&controller))
            {
                list_players(&controller);
            }
            else
            {
                printf("Sem jogadores registados.\n");
            }
        }
        else if (strcmp(instruction, "IJ") == 0)
        {
            char *player1 = strtok(NULL, " ");
            char *player2 = strtok(NULL, " ");
            if ((player1) && (player2))
            {
                if (is_game_being_played(&controller))
                {
                    printf("Existe um jogo em curso.\n");
                }
                else if ((!is_player_registered(&controller, player1) || (!is_player_registered(&controller, player2))))
                {
                    printf("Jogador inexistente.\n");
                }
                else
                {
                    start_game(&controller, player1, player2);
                    printf("Jogo iniciado com sucesso.\n");
                }
            }
        }
        else if (strcmp(instruction, "IJA") == 0)
        {
            char *player1 = strtok(NULL, " ");
            char *level = strtok(NULL, " ");
            if ((player1) && (level) && (((strcmp(level, "Normal") == 0)) || ((strcmp(level, "Avançado") == 0))))
            {
                if (is_game_being_played(&controller))
                {
                    printf("Existe um jogo em curso.\n");
                }
                else if (!is_player_registered(&controller, player1))
                {
                    printf("Jogador inexistente.\n");
                }
                else if ((strcmp(level, "Normal") == 0))
                {
                    start_automatic_game(&controller, player1, level);
                    printf("Jogo automático de nível %s iniciado com sucesso.\n", level);
                }
                else
                {
                    start_automatic_game(&controller, player1, level);
                    printf("Jogo automático de nível %s iniciado com sucesso.\n", level);
                }
            }
        }
        else if (strcmp(instruction, "DJ") == 0)
        {
            if (is_game_being_played(&controller))
            {
                show_game_details(&controller);
            }
            else
            {
               printf("Não existe jogo em curso.\n");
            }
        }
        else if (strcmp(instruction, "J") == 0)
        {
            char *player1 = strtok(NULL, " ");
            int position = atoi(strtok(NULL, " "));
            if (player1 && position)
            {
                if (controller.player_2){    
                    if (is_game_being_played(&controller))
                    {
                        if (is_player_registered(&controller, player1))
                        {
                            if (is_player_playing(&controller, player1))
                            {
                                if (is_player_turn(&controller, player1))
                                {
                                    if (controller.player_1->turn)
                                    {
                                        int done_play = play(&controller, position);
                                        if (!is_game_ending(&controller))
                                        {
                                            if (done_play == 1)
                                            {
                                                change_turn(&controller);
                                                printf("Jogada efetuada com sucesso.\n");
                                            }
                                            else if (done_play == 2)
                                            {
                                                printf("O jogador %s tem direito a outra jogada.\n", controller.player_1->name);
                                            }
                                        }
                                        else
                                        {
                                            printf("Jogo terminado.\n%s %d\n%s %d\n", controller.player_1->name, controller.board[13], controller.player_2->name, controller.board[0]);
                                        }
                                    }
                                    else if (controller.player_2->turn)
                                    {
                                        int done_play = play(&controller,position);
                                        if (!is_game_ending(&controller))
                                        {
                                            if (done_play == 1)
                                            {
                                                change_turn(&controller);
                                                printf("Jogada efetuada com sucesso.\n");
                                            }
                                            else if (done_play == 2)
                                            {
                                                printf("O jogador %s tem direito a outra jogada.\n", controller.player_2->name);
                                            }
                                        }
                                        else
                                        {
                                            printf("Jogo terminado.\n%s %d\n%s %d\n", controller.player_1->name, controller.player_1->side, controller.player_2->name, controller.player_2->side);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                printf("Jogador não participa no jogo em curso.\n");
                            }
                        }
                        else
                        {
                            printf("Jogador inexistente.\n");
                        }
                    }
                    else
                    {
                        printf("Não existe jogo em curso.\n");
                    }
                }
                else
                {
                    if (is_game_being_played(&controller))
                    {
                        if (is_player_registered(&controller, player1))
                        {
                            if (is_player_playing(&controller, player1))
                            {
                                if (is_player_turn(&controller, player1))
                                {
                                    if (controller.player_1->turn)
                                    {
                                        int done_play = play(&controller, position);
                                        if (!is_game_ending(&controller))
                                        {
                                            if (done_play == 1)
                                            {
                                                change_turn(&controller);
                                                printf("Jogada efetuada com sucesso.\n");
                                                if (is_level_advanced(&controller)){
                                                    advanced_cpu(&controller);
                                                }else{
                                                    normal_cpu(&controller);
                                                }
                                            }
                                            else if (done_play == 2)
                                            {
                                                printf("O jogador %s tem direito a outra jogada.\n", controller.player_1->name);
                                            }
                                        }
                                        else
                                        {
                                            printf("Jogo terminado.\n%s %d\nCPU %d\n", controller.player_1->name, controller.board[13], controller.board[0]);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                printf("Jogador não participa no jogo em curso.\n");
                            }
                        }
                        else
                        {
                            printf("Jogador inexistente.\n");
                        }
                    }
                    else
                    {
                        printf("Não existe jogo em curso.\n");
                    }
                }
            }  
        }    
        else if (strcmp(instruction, "D") == 0)
        {
            char *player1 = strtok(NULL, " ");
            char *player2 = strtok(NULL, " ");
            if (player1)
            {
                if (is_game_being_played(&controller))
                {
                    if (is_player_playing(&controller, player1))
                    {
                        if (player2)
                        {
                            if (is_player_playing(&controller, player2))
                            {
                                quit(&controller, player1, player2);
                                printf("Jogo terminado com sucesso.\n");
                            }
                            else
                            {
                                printf("Jogador não participa no jogo em curso.\n");
                            }
                        }
                        else
                        {
                            quit(&controller, player1, player2);
                            printf("Jogo terminado com sucesso.\n");
                        }
                    }
                    else
                    {
                        printf("Jogador não participa no jogo em curso.\n");
                    }
                }
                else
                {
                    printf("Não existe jogo em curso.\n");
                }
            }
        }
        else if (strcmp(instruction, "G") == 0)
        {
            char *file_name = strtok(NULL, " ");
            if (file_name)
            {
                save_game(&controller, file_name);
                printf("Jogo gravado com sucesso.\n");
            }
        }
        else if (strcmp(instruction, "L") == 0)
        {
        
            char *file_name = strtok(NULL, " ");
            if (file_name)
            {
                if (!is_game_being_played(&controller))
                {
                    free_players(&controller);
                    read_game(&controller, file_name);
                    printf("Jogo lido com sucesso.\n");
                }else{
                    printf("Existe um jogo em curso.\n");
                }
            }
        }
        else
        {
            printf("Instrução inválida.\n");
        }
        free(line);
        line = NULL;
    }

    if (line != NULL)
    {
        free(line);
    }
    free_players(&controller);
    return 0;
}
