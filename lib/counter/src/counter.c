#include "counter.h"

void count_loop_iteration(struct k_timer* timer, char* context_name, struct k_sem* semaphore, int* counter) {
    int res = k_sem_take(semaphore, K_MSEC(SLEEPTIME * 2));
    if (res != 0) {
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

void deadlock_a(struct k_sem* sem1, struct k_sem* sem2, struct k_timer* timer) {
    k_sem_take(sem1, K_FOREVER);
    k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
    k_timer_status_sync(timer);

    k_sem_take(sem2, K_FOREVER);
    k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
    k_timer_status_sync(timer);

    k_sem_give(sem2);
    k_timer_start(timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
    k_timer_status_sync(timer);
    
    k_sem_give(sem1);
}