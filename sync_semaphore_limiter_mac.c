#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dispatch/dispatch.h>

dispatch_semaphore_t semaphore;

//i value, through a, is received through args
void* routine(void* args) {
    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);

    // int id = *(int*)args
    printf("Entering thread: %d\n", *(int*)args);
    sleep(1);
    printf("Exiting thread: %d\n", *(int*)args);
    dispatch_semaphore_signal(semaphore);

    free(args);
    return NULL;
}

int main(void) {
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

    pthread_t *th = malloc(sizeof(*th) * thread_num);
    if (!th) return 1;
    semaphore = dispatch_semaphore_create(limit);

    for (int i = 0; i < thread_num; i++) {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, routine, a) != 0) {
            return 1;
        }
    }

    for (int i = 0; i < thread_num; i++) {
       if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
    }

    free(th);
    return 0;
}
