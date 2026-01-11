#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <semaphore.h>

sem_t semaphore;

//i value, through a, is received through args
void* routine(void* args) {
    //with the sem_wait and sem_post combo, the number of threads specified by semaphore value (limits) can access the sleep and print statements at a time
    sem_wait(&semaphore);

    // int id = *(int*)args
    printf("Entering thread: %d\n", *(int*)args);
    sleep(1);
    printf("Exiting thread: %d\n", *(int*)args);
    sem_post(&semaphore);

    free(args);
    return NULL;
} 

int main(int argc, char* argv[]) {
    int thread_num, limit;
    printf("Provide a number for the total number of threads created:\n");
    if (scanf("%d", &thread_num) != 1 || thread_num <= 0) {
        fprintf(stderr, "Invalid entry.\n");
        return 1;
    }
    printf("Provide a number for the limit to the threads allowed for concurrent accesses to a resource:\n");
    if (scanf("%d", &limit) != 1 || limit <= 0) {
        fprintf(stderr, "Invalid entry.\n");
        return 1;
    }

    pthread_t th[thread_num];
    sem_init(&semaphore, 0, limit);

    for (int i = 0; i < thread_num; i++) {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            return 1;
        }
    }
    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
    }
    sem_destroy(&semaphore);

    return 0;
}