#include "counter.h"

// struct k_thread coop_thread;
// K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

// struct k_sem semaphore;
// int counter;



// void thread_entry(void) {
//     struct k_timer timer;
//     k_timer_init(&timer, NULL, NULL);

//     while (1) {
//         count_loop_iteration(&timer, "thread", &semaphore, &counter);
//     }
// }

// int main(void) {
//     counter = 0;
//     k_sem_init(&semaphore, 1, 1);
//     k_thread_create(&coop_thread,
//         coop_stack,
//         STACKSIZE,
//         (k_thread_entry_t)thread_entry,
//         NULL,
//         NULL,
//         NULL,
//         K_PRIO_COOP(7),
//         0,
//         K_NO_WAIT);

//     struct k_timer timer;
//     k_timer_init(&timer, NULL, NULL);

//     while (1) {
//         count_loop_iteration(&timer, "main", &semaphore, &counter);
//     }

//     return 0;
// }