#include <unity.h>
#include "counter.h"

struct k_sem test_semaphore;
int test_counter;
struct k_timer timer;

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

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_counter_normal);
    RUN_TEST(test_counter_blocked);
    return UNITY_END();
}