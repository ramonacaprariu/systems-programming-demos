#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//desired output:
//1 AB 2 CD 3 EF 4 GH 5 IJ 6 KL 6 MN 8 OP 9 QR 10 ST 11 UV 12 WX 13 YZ

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_condition = PTHREAD_COND_INITIALIZER;

int turn = 0; //0 is numbers, 1 is letters and they alternate 

void* print_numbers() {
    for (int i = 1; i < 14; ++i) {
        pthread_mutex_lock(&mutex); //acquiring lock
        while (turn != 0) { //therefore not numbers' turn
            pthread_cond_wait(&mutex_condition, &mutex);
        }
        printf("%d ", i);
        turn = 1; //passing the turn over
        pthread_cond_signal(&mutex_condition);
        pthread_mutex_unlock(&mutex); //releasing lock
    }
    return NULL;
}

void* print_letters() {
    for (char c = 'A'; c <= 'Z'; c+=2) {
        pthread_mutex_lock(&mutex); //acquiring lock
        while (turn != 1) { //therefore not letters' turn
            pthread_cond_wait(&mutex_condition, &mutex);
        }
        printf("%c%c ", c, c+1); //c+1 computed as an int
        turn = 0;
        pthread_cond_signal(&mutex_condition);
        pthread_mutex_unlock(&mutex); //releasing lock
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, &print_numbers, NULL) != 0) {
        return 1;
    }
    if (pthread_create(&t2, NULL, &print_letters, NULL) != 0) {
        return 2;
    }
    if (pthread_join(t1, NULL) != 0) {
        return 3;
    }
    if (pthread_join(t2, NULL) != 0) {
        return 4;
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}