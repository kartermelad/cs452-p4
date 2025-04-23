#include "harness/unity.h"
#include "../src/lab.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

// NOTE: Due to the multi-threaded nature of this project. Unit testing for this
// project is limited. I have provided you with a command line tester in
// the file app/main.cp. Be aware that the examples below do not test the
// multi-threaded nature of the queue. You will need to use the command line
// tester to test the multi-threaded nature of your queue. Passing these tests
// does not mean your queue is correct. It just means that it can add and remove
// elements from the queue below the blocking threshold.

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_create_destroy(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    queue_destroy(q);
}

void test_queue_dequeue(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    enqueue(q, &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    queue_destroy(q);
}

void test_queue_dequeue_multiple(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    queue_destroy(q);
}

void test_queue_dequeue_shutdown(void)
{
    queue_t q = queue_init(10);
    TEST_ASSERT_TRUE(q != NULL);
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    enqueue(q, &data);
    enqueue(q, &data2);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    queue_shutdown(q);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);
    TEST_ASSERT_TRUE(is_shutdown(q));
    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_dequeue_empty(void)
{
    queue_t q = queue_init(2);
    TEST_ASSERT_TRUE(q != NULL);

    TEST_ASSERT_TRUE(dequeue(q) == NULL);

    queue_shutdown(q);

    queue_destroy(q);
}

void test_shutdown_behavior(void)
{
    queue_t q = queue_init(3);
    TEST_ASSERT_TRUE(q != NULL);

    int data1 = 1, data2 = 2;
    enqueue(q, &data1);
    enqueue(q, &data2);

    queue_shutdown(q);

    TEST_ASSERT_TRUE(dequeue(q) == &data1);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);

    int data3 = 3;
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(is_empty(q));

    queue_destroy(q);
}

void test_large_number_of_items(void)
{
    queue_t q = queue_init(1000);
    TEST_ASSERT_TRUE(q != NULL);

    for (int i = 0; i < 1000; i++) {
        enqueue(q, (void *)(long)i);
    }

    for (int i = 0; i < 1000; i++) {
        TEST_ASSERT_TRUE(dequeue(q) == (void *)(long)i);
    }

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_queue_size_one(void)
{
    queue_t q = queue_init(1);
    TEST_ASSERT_TRUE(q != NULL);

    int data = 42;
    enqueue(q, &data);
    TEST_ASSERT_TRUE(dequeue(q) == &data);

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

void test_multiple_shutdown_calls(void)
{
    queue_t q = queue_init(5);
    TEST_ASSERT_TRUE(q != NULL);

    queue_shutdown(q);
    queue_shutdown(q);

    TEST_ASSERT_TRUE(is_shutdown(q));
    queue_destroy(q);
}

void test_mixed_operations(void)
{
    queue_t q = queue_init(5);
    TEST_ASSERT_TRUE(q != NULL);

    int data1 = 1, data2 = 2, data3 = 3;
    enqueue(q, &data1);
    enqueue(q, &data2);
    TEST_ASSERT_TRUE(dequeue(q) == &data1);
    enqueue(q, &data3);
    TEST_ASSERT_TRUE(dequeue(q) == &data2);
    TEST_ASSERT_TRUE(dequeue(q) == &data3);

    TEST_ASSERT_TRUE(is_empty(q));
    queue_destroy(q);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_destroy);
  RUN_TEST(test_queue_dequeue);
  RUN_TEST(test_queue_dequeue_multiple);
  RUN_TEST(test_queue_dequeue_shutdown);
  RUN_TEST(test_dequeue_empty);
  RUN_TEST(test_shutdown_behavior);
  RUN_TEST(test_large_number_of_items);
  RUN_TEST(test_queue_size_one);
  RUN_TEST(test_multiple_shutdown_calls);
  RUN_TEST(test_mixed_operations);
  return UNITY_END();
}
