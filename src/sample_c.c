#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define N 15
#define TIME_WINDOW 120
#define MAX_VALUE 150000
#define KEEP_PLAYING 1
#define STOP_PLAYING 0
#define NEW_OFFER_MAX_PLUS 10
#define STOP_PLAYING 0
#define AUCTION_ID_SIZE 20
#define PASSWORD_SIZE 20
#define IMEI_SIZE 50


int64_t read_int64_t(void){
    char input[16];
    int size;

    size = read(0, input, 16);
    input[15] = '\0'; 

    return (int64_t) strtol(input, NULL, 10); 
}

void rand_string(char *str, size_t size){
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t n;

    if (size) {
        --size;
        for (n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof(charset) - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
}

int make_new_offer(int64_t player_bet){
    int64_t tmp = rand() % (MAX_VALUE * 2 / 3) + MAX_VALUE / 3;

    return tmp > player_bet;
}

int play(int64_t *player_bet, int64_t *user_bet, int64_t *last_bets, int8_t *counter){

    do{
        printf("Best offer by now is %ld$. You have to offer more!\n", *user_bet);
        printf("How much do you want to offer?\n");
        fflush(stdout);
    
        *player_bet = read_int64_t();

        (*counter)++;
        last_bets[*counter % N] = *player_bet;
        printf("%d - %p\n", *counter, last_bets + (*counter % N));
        fflush(stdout);

    } while(*player_bet <= *user_bet);

    printf("You offered %ld$.\n", *player_bet);
    
    if (*player_bet >= MAX_VALUE)
        return STOP_PLAYING;

    if (make_new_offer(*player_bet)){
        *user_bet = *player_bet + (rand() % NEW_OFFER_MAX_PLUS) + 1;
        printf("\nA mysterious man made a new offer: %ld$.\n", *user_bet);
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

void participate(){
    int64_t last_bets[N];
    int64_t player_bet;
    int64_t user_bet;
    int8_t x = 0;
    int8_t keep_playing;
    time_t start_time;
    char choice[128];
    int8_t i;
    int8_t counter;
    char auction_id[AUCTION_ID_SIZE];

    memset(last_bets, 0, sizeof(int64_t) * N);
    memset(choice, 0, sizeof(char) * 128);

    printf("Insert the auction ID: ");
    fflush(stdout);
    read(0, auction_id, AUCTION_ID_SIZE);
    auction_id[AUCTION_ID_SIZE-1] = '\0';
    fflush(stdin);

    if( access(auction_id, F_OK) == -1 ) {
        printf("Auction does not exits\n\n");
        fflush(stdout);
        return;    
    }

    welcome();
    start_time = time(NULL);

    printf("x %p\n", &x);
    printf("last_bets %p\n", last_bets);
    printf("player_bet %p\n", &player_bet);
    printf("counter %p\n", &counter);

    user_bet = 100 + (rand() % 50);
    keep_playing = 1;

    counter = 0;
    while(time(NULL) - start_time < TIME_WINDOW && keep_playing){
        print_menu();
        read(0, choice, 128);
        fflush(stdin);

        switch (choice[0]) {
            case '1':
                keep_playing = play(&player_bet, &user_bet, last_bets, &counter);
                break;
            case '2':
                keep_playing = STOP_PLAYING;
                break;
            default:
                break;
        }
    }

    if (player_bet > user_bet)
        printf("\nCongratulations! You bought the (hano)iFon for %ld$\n", player_bet);
    else
        printf("\nA mysterious man bought the (hano)iFon for %ld$\n", user_bet);
    fflush(stdout);

    i = 0;
    printf("Your last bets:\n");
    while (i < N && last_bets[counter % N]){
        printf("- %ld\n", last_bets[counter % N]);
        counter--;
        i++;
    }
    if (x)
        printf("ASDASDASDASD");
    fflush(stdout);
    
    return 0;
}

void new_auction(){
    char imei[IMEI_SIZE];
    char passwd[PASSWORD_SIZE];
    char auction_id[AUCTION_ID_SIZE];

    memset(imei, 0, sizeof(char)*IMEI_SIZE);
    memset(passwd, 0, sizeof(char)*PASSWORD_SIZE);

    printf("Insert the IMEI of the (hano)iFon:\n");
    fflush(stdout);
    read(0, imei, IMEI_SIZE);
    fflush(stdin);

    rand_string(auction_id, AUCTION_ID_SIZE);
    rand_string(passwd, PASSWORD_SIZE);

    FILE *fp = fopen(auction_id, "w");
    fprintf(fp, "%s\n%s\n", passwd, imei);
    fclose(fp);

    printf("Auction ID: %s\n", auction_id);
    printf("Your Password: %s\n\n", passwd);

}

void admin_auction(){
    char auction_id[AUCTION_ID_SIZE];
    char passwd[PASSWORD_SIZE];
    char correct_passwd[PASSWORD_SIZE];
    char imei[IMEI_SIZE];
    FILE *fp;

    memset(auction_id, 0, sizeof(char)*AUCTION_ID_SIZE);
    memset(passwd, 0, sizeof(char)*PASSWORD_SIZE);

    printf("Insert the auction ID: ");
    fflush(stdout);
    read(0, auction_id, AUCTION_ID_SIZE);
    auction_id[AUCTION_ID_SIZE-1] = '\0';
    fflush(stdin);

    if (fp = fopen(auction_id, "r")){        
        fgets(correct_passwd, PASSWORD_SIZE, fp);
    
    } else {
        printf("Auction does not exits\n");
        fflush(stdout);
        return;    
    }

    printf("Insert the password: ");
    fflush(stdout);
    read(0, passwd, PASSWORD_SIZE);
    passwd[PASSWORD_SIZE-1] = '\0';
    fflush(stdin);

    if (!strncmp(passwd, correct_passwd, PASSWORD_SIZE)){
        fgets(imei, IMEI_SIZE, fp);
        fgets(imei, IMEI_SIZE, fp);
        printf("Your IMEI: %s\n", imei);
    }
    else{
        printf("Wrong password!\n");
    }

    fflush(stdout);
    fclose(fp); 

}

int main(){
    char choice[4];
    
    srand(time(NULL));

    do{
        printf("1) Create new auction\n");
        printf("2) Admin your auction\n");
        printf("3) Participate in an auction\n");
        printf("4) Exit\n");
        printf("?: \n");
        fflush(stdout);

        read(0, choice, 4);
        fflush(stdin);
        
        switch (choice[0]){
            case '1':
                new_auction();
                break;
            case '2':
                admin_auction();
                break;
            case '3':
                participate();
                break;
            default:
                break;
        }
    }while(choice[0] != '4');
}
