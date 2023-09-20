#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#define STACKSIZE 2000
#define SLEEPTIME 1000

void count_loop_iteration(struct k_timer* timer, char* context_name, struct k_sem* semaphore, int* counter);
void deadlock_a(struct k_sem* sem1, struct k_sem* sem2, struct k_timer* timer);