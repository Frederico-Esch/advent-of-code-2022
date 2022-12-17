#include <stdio.h>
#include <stdlib.h>

typedef enum {
    LOSE = 0,
    DRAW = 3,
    WIN = 6
} game_state;

typedef enum  {
    ROCK = 1,
    PAPER = 2,
    SCISORS = 3
} play;

play predict_play(play opponent, game_state objective){
    if (objective == DRAW) return opponent;

    opponent -= 1;
    if (objective == WIN)
        return (opponent+1)%3+1;
    else
        return (opponent+2)%3+1;

}

game_state calculate_state(play player, play opponent){
    if ( player == opponent) return DRAW;

    play lose_case;
    switch (player) {
        case ROCK:
            lose_case = PAPER;
            break;

        case PAPER:
            lose_case = SCISORS;
            break;

        case SCISORS:
            lose_case = ROCK;
            break;
    }

    if (opponent == lose_case) return LOSE;
    else return WIN;
}

play opponent_char_to_play(char opponent_char){
    switch (opponent_char) {
        case 'A': return ROCK   ;
        case 'B': return PAPER  ;
        case 'C': return SCISORS;
    }

    return 0;
}

void first_challenge(){
    FILE* file = fopen("./input", "r");
    char player_char, opponent_char;
    play player, opponent;
    int score = 0;

    while (!feof(file)) {
        fscanf(file, "%c %c\n", &opponent_char, &player_char);
        opponent = opponent_char_to_play(opponent_char);

        switch (player_char) {
            case 'X': player = ROCK   ; break;
            case 'Y': player = PAPER  ; break;
            case 'Z': player = SCISORS; break;
        }

        game_state result = calculate_state(player, opponent);
        score += result + player;
    }

    fclose(file);
    printf("RESULT: %d\n", score);
}

void second_challenge(){
    FILE* file = fopen("./input", "r");
    char opponent_char, objective_char;
    play opponent;
    game_state objective;
    int score = 0;

    while (!feof(file)) {
        fscanf(file, "%c %c\n", &opponent_char, &objective_char);

         opponent = opponent_char_to_play(opponent_char);

         switch (objective_char) {
            case 'X':
                objective = LOSE;
            break;
            case 'Y':
                objective = DRAW;
            break;
            case 'Z':
                objective = WIN;
            break;
         }

        play my_play = predict_play(opponent, objective);

        score += my_play + objective;
    }
    printf("SUM: %d\n", score);
    fclose(file);
}

int main(){
    second_challenge();
}
