#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#define N 10
#define TIME_WINDOW 120
#define MAX_VALUE 1500
#define KEEP_PLAYING 1
#define STOP_PLAYING 0
#define NEW_OFFER_MAX_PLUS 10
#define STOP_PLAYING 0


float read_float(void){
    char input[16];
    int size;

    size = read(0, input, 15);
    input[15] = '\0'; 

    return (float) atof(input); 
}


int make_new_offer(int player_bet){
    float tmp = rand() % (MAX_VALUE * 2 / 3) + MAX_VALUE / 3;

    return tmp > player_bet;
}


int play(float *player_bet, float *user_bet){

    do{
        printf("Best offer by now is %.2f$. You have to offer more!\n", *user_bet);
        printf("How much do you want to offer?\n");
        fflush(stdout);
    
        *player_bet = read_float();

    } while(*player_bet <= *user_bet);

    printf("You offered %.2f$.\n", *player_bet);
    
    if (*player_bet >= MAX_VALUE)
        return STOP_PLAYING;

    if (make_new_offer(*player_bet)){
        *user_bet = *player_bet + (rand() % NEW_OFFER_MAX_PLUS) + 1;
        printf("\nA mysterious man made a new offer: %.2f$.\n", *user_bet);
        fflush(stdout);

        if (*user_bet >= MAX_VALUE)
            return STOP_PLAYING;
    }
    else
        return STOP_PLAYING;

    return KEEP_PLAYING;

}

void welcome(void){
    printf("Hi! This is the ToH auction platform.\n");
    printf("Today we have a wonderful (hano)iFon on sale.\n");
    printf("You can look at the picture. Perfect conditions.\n");
    printf("\t __________\n");
    printf("\t| ________ |\n");
    printf("\t||        ||\n");
    printf("\t||        ||\n");
    printf("\t||        ||\n");
    printf("\t||        ||\n");
    printf("\t||        ||\n");
    printf("\t||        ||\n");
    printf("\t||________||\n");
    printf("\t|    __    |\n");
    printf("\t|   |__|   |\n");
    printf("\t|__________|\n");
    printf("\nThe sale is going to finish in %d minutes.\n", TIME_WINDOW / 60);
    printf("There is someone else who already made an offer.\n");
    fflush(stdout);
}

void print_menu(void){
    printf("\n1) Make an offer\n");
    printf("2) Quit\n");
    printf("?: ");
    fflush(stdout);
}

int main(){
    float last_bets[N];
    float player_bet;
    float user_bet;
    int16_t counter;
    int8_t keep_playing;
    time_t start_time;
    char choice;
    int8_t i;

    srand(time(NULL));
    memset(last_bets, 0, sizeof(float) * N);
    
    welcome();
    start_time = time(NULL);

    user_bet = 100 + (rand() % 50);
    keep_playing = 1;

    counter = 0;
    while(time(NULL) - start_time < TIME_WINDOW && keep_playing){
        print_menu();
        choice = getchar();
        fflush(stdin);

        switch (choice) {
            case '1':
                keep_playing = play(&player_bet, &user_bet);
                counter++;
                last_bets[counter % N] = player_bet;
                break;
            case '2':
                keep_playing = STOP_PLAYING;
                break;
            default:
                break;
        }
    }

    if (player_bet > user_bet)
        printf("\nCongratulations! You bought the (hano)iFon for %.2f$\n", player_bet);
    else
        printf("\nA mysterious man bought the (hano)iFon for %.2f$\n", user_bet);
    fflush(stdout);

    i = 0;
    printf("Your last bets:\n");
    while (i < N && last_bets[counter % N]){
        printf("- %.2f\n", last_bets[counter % N]);
        counter--;
        i++;
    }
    fflush(stdout);
    
    return 0;
}
