#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 10
#define TIME_WINDOW 120
#define KEEP_PLAYING 1
#define STOP_PLAYING 0
#define NEW_OFFER_MAX_PLUS 10
#define AUCTION_ID_SIZE 16
#define PASSWORD_SIZE 20
#define IMEI_SIZE 50


int8_t counter;
time_t start_time;


void fflush_stdin(){
    while ((getchar()) != '\n');
}

void custom_read(char * buffer, size_t size){
    int i;

    for (i = 0; i < size - 1; i++)
        if ((buffer[i] = getchar()) == '\n')
            break;

    if (i == size - 1)
        fflush_stdin();

    buffer[i]='\0';
}

int64_t read_int64_t(){
    char input[16];
    memset(input, 0, sizeof(char) * 16);

    custom_read(input, 16);

    return (int64_t) strtol(input, NULL, 10); 
}

void rand_string(char *str, size_t size){
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t n;

    if (size) {
        for (n = 0; n < (size-1); n++) {
            /* sizeof charset - 1: '\0' is NOT part of the charset */
            int key = rand() % (int) (sizeof(charset) - 1);
            str[n] = charset[key];
        }
        str[n] = '\0';
    }
}

int make_new_offer(int64_t player_bet, int64_t max_value){
    int64_t tmp = rand() % (max_value * 2 / 3) + max_value / 3;

    return tmp > player_bet;
}

int play(int64_t *player_bet, int64_t *server_bet, int64_t *last_bets){
    
    do{
        printf("Best offer by now is %ld$. You have to offer more!\n", *server_bet);
        printf("How much do you want to offer?\n");
        fflush(stdout);
    
        *player_bet = read_int64_t();

        counter++;
        last_bets[counter % N] = *player_bet;

    } while(*player_bet <= *server_bet);

    printf("You offered %ld$.\n", *player_bet);
    
    if (time(NULL) - start_time < TIME_WINDOW){
        *server_bet = *player_bet + 1;
        printf("\nA mysterious man made a new offer: %ld$.\n", *server_bet);

    }
    else{
        printf("Time is over!\n");    
        return STOP_PLAYING;
    }

    fflush(stdout);

    return KEEP_PLAYING;

}

void welcome(){
    printf("Wonderful (hano)iFon on sale.\n");
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

void print_menu(){
    printf("\n1) Make an offer\n");
    printf("2) Quit\n");
    printf("?: ");
    fflush(stdout);
}

void participate(){
    char auction_id[AUCTION_ID_SIZE];
    int64_t server_bet;
    int64_t i;
    int64_t keep_playing;
    int64_t player_bet;
    char choice;
    int64_t last_bets[N];

    memset(last_bets, 0, sizeof(int64_t) * N);
    
    printf("Insert the auction ID: ");
    fflush(stdout);
    custom_read(auction_id, AUCTION_ID_SIZE);
    
    if( access(auction_id, F_OK) == -1 ) {
        printf("Auction does not exists\n\n");
        fflush(stdout);
        return;    
    }

    welcome();
    start_time = time(NULL);

    server_bet = 100 + (rand() % 50);
    keep_playing = 1;

    while(time(NULL) - start_time < TIME_WINDOW && keep_playing){
        print_menu();
        read(0, &choice, 1);
        fflush_stdin();

        switch (choice) {
            case '1':
                keep_playing = play(&player_bet, &server_bet, last_bets);
                break;
            case '2':
                keep_playing = STOP_PLAYING;
                break;
            default:
                break;
        }
    }

    if (player_bet > server_bet){
        printf("\nCongratulations! You bought the (hano)iFon for %ld$\n", player_bet);
        printf("We will send your device together with the IMEI in a couple of days.\n");
    }
    else
        printf("\nA mysterious man bought the (hano)iFon for %ld$\n", server_bet);
    fflush(stdout);

    i = 0;
    printf("Your last bets:\n");
    while (i < N && last_bets[counter % N]){
        printf("- %ld\n", last_bets[counter % N]);
        counter--;
        i++;
    }
    fflush(stdout);
    
}

void new_auction(){
    char imei[IMEI_SIZE];
    char passwd[PASSWORD_SIZE];
    char auction_id[AUCTION_ID_SIZE];

    memset(imei, 0, sizeof(char)*IMEI_SIZE);
    memset(passwd, 0, sizeof(char)*PASSWORD_SIZE);

    printf("Insert the IMEI of the (hano)iFon: ");
    fflush(stdout);

    custom_read(imei, IMEI_SIZE);
    
    rand_string(auction_id, AUCTION_ID_SIZE);
    rand_string(passwd, PASSWORD_SIZE);

    FILE *fp = fopen(auction_id, "w");
    fprintf(fp, "%s\n%s\n", passwd, imei);
    fclose(fp);

    printf("Auction ID: %s\n", auction_id);
    printf("Your Password: %s\n\n", passwd);
    fflush(stdout);

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
    custom_read(auction_id, AUCTION_ID_SIZE);
    
    if ((fp = fopen(auction_id, "r"))){        
        fgets(correct_passwd, PASSWORD_SIZE, fp);
    
    } else {
        printf("Auction does not exist\n");
        fflush(stdout);
        return;    
    }

    printf("Insert the password: ");
    fflush(stdout);
    custom_read(passwd, PASSWORD_SIZE);
    
    if (!strncmp(passwd, correct_passwd, PASSWORD_SIZE)){
        fseek(fp, 1, SEEK_CUR);
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
    char choice;
    counter = 0;
    struct timespec t;

    clock_gettime(CLOCK_REALTIME, &t);
    srand(t.tv_nsec);

    printf("Hi! This is the ToH auction platform.\n");
    printf("We have wonderful (hano)iFones on sale.\n\n");

    do{
        printf("1) Create new auction\n");
        printf("2) Admin your auction\n");
        printf("3) Participate in an auction\n");
        printf("4) Exit\n");
        printf("?: ");
        fflush(stdout);

        read(0, &choice, 1);
        fflush_stdin();
        
        switch (choice){
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
    }while(choice != '4');
    return 0;
}
