#include "counter.h"

void count_loop_iteration(struct k_timer* timer, char* context_name, struct k_sem* semaphore, int* counter) {
    int res = k_sem_take(semaphore, K_MSEC(SLEEPTIME*2));
    if (res != 0){
        return;
    }
    {
        *counter = *counter + 1;
        printk("hello world from %s! Count %d\n", context_name, *counter);
        k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
        k_timer_status_sync(timer);
    }
    k_sem_give(semaphore);
}