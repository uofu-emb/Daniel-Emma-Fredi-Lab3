#include <unity.h>
#include "counter.h"

struct k_sem test_semaphore;
int test_counter;
struct k_timer timer;

struct k_thread coop_thread, coop_thread2, orphan_thread, adopted_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(coop_stack2, STACKSIZE);
K_THREAD_STACK_DEFINE(orphan_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(adopted_stack, STACKSIZE);

void setUp() {
    k_sem_init(&test_semaphore, 1, 1);
    k_timer_init(&timer, NULL, NULL);
    test_counter = 0;
}

void tearDown() {}


void test_counter_normal(void) {
    count_loop_iteration(&timer, "test", &test_semaphore, &test_counter);

    TEST_ASSERT_EQUAL(test_counter, 1);
    TEST_ASSERT_EQUAL(k_sem_count_get(&test_semaphore), 1);
}

void test_counter_blocked() {
    k_sem_take(&test_semaphore, K_FOREVER);
    count_loop_iteration(&timer, "test", &test_semaphore, &test_counter);

    
    TEST_ASSERT_EQUAL(test_counter, 0);
    TEST_ASSERT_EQUAL(k_sem_count_get(&test_semaphore), 0);
}

void test_deadlock() {
    struct k_sem sem1, sem2;
    k_sem_init(&sem1, 1, 1);
    k_sem_init(&sem2, 1, 1);

    k_thread_create(&coop_thread,
                    coop_stack,
                    STACKSIZE,
                    (k_thread_entry_t) deadlock_a,
                    &sem1,
                    &sem2,
                    &timer,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    k_thread_create(&coop_thread2,
                    coop_stack2,
                    STACKSIZE,
                    (k_thread_entry_t) deadlock_a,
                    &sem2,
                    &sem1,
                    &timer,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    struct k_timer short_timer;
    k_timer_init(&short_timer, NULL, NULL);
    k_timer_start(&short_timer, K_MSEC(1), K_NO_WAIT);
    k_timer_status_sync(&short_timer);

    TEST_ASSERT_EQUAL(0, k_sem_count_get(&sem1));
    TEST_ASSERT_EQUAL(0, k_sem_count_get(&sem2));

    k_thread_abort(&coop_thread);
    k_thread_abort(&coop_thread2);
}

void test_orphan() {
    k_thread_create(&orphan_thread,
                    orphan_stack,
                    STACKSIZE,
                    (k_thread_entry_t) orphaned_lock,
                    &test_semaphore,
                    &test_counter,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    
    struct k_timer short_timer;
    k_timer_init(&short_timer, NULL, NULL);
    k_timer_start(&short_timer, K_MSEC(1), K_NO_WAIT);
    k_timer_status_sync(&short_timer);

    TEST_ASSERT_EQUAL(0, k_sem_count_get(&test_semaphore));
    TEST_ASSERT_EQUAL(1, test_counter);

    k_thread_abort(&orphan_thread);
}

void test_adopted() {
    k_thread_create(&adopted_thread,
                    adopted_stack,
                    STACKSIZE,
                    (k_thread_entry_t) adopted_lock,
                    &test_semaphore,
                    &test_counter,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    
    struct k_timer short_timer;
    k_timer_init(&short_timer, NULL, NULL);
    k_timer_start(&short_timer, K_MSEC(1), K_NO_WAIT);
    k_timer_status_sync(&short_timer);

    TEST_ASSERT_EQUAL(1, k_sem_count_get(&test_semaphore));
    TEST_ASSERT_EQUAL(10, test_counter);

    k_thread_abort(&adopted_thread);
}

int main() {
    UNITY_BEGIN();
    // RUN_TEST(test_counter_normal);
    // RUN_TEST(test_counter_blocked);
    // RUN_TEST(test_deadlock);
    RUN_TEST(test_orphan);
    RUN_TEST(test_adopted);
    return UNITY_END();
}