#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "rtos/sema.h"

/*/ Thread-Stapel und Steuerblöcke
#define STACK_SIZE 512
#define PRIORITY 5

K_THREAD_STACK_DEFINE(thread_1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_2_stack, STACK_SIZE);

static struct k_thread thread_1_data;
static struct k_thread thread_2_data;

static Sema ts(0);

void thread_entry1(void *p1, void *p2, void *p3)
{
	int rc;

	k_sleep(K_MSEC(10));

	rc = ts.wait();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(ts.count(), 0, "count != 0");
	printk("Thread1 wait:  count = %d\n\n", ts.count());

	k_sleep(K_MSEC(20));

	rc = ts.wait();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(ts.count(), 1, "count != 1");
	printk("Thread1 wait:  count = %d\n\n", ts.count());

	rc = ts.wait();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(ts.count(), 0, "count != 0");
	printk("Thread1 wait:  count = %d\n\n", ts.count());

	rc = ts.wait_timed(100);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(ts.count(), 0, "count != 0");
	printk("Thread1 wait_timed:  count = %d\n\n", ts.count());

	rc = ts.wait_timed(100);
	zassert_equal(rc, -11, "rc != -11");
	zassert_equal(ts.count(), 0, "count != 0");
	printk("Thread1 wait_timed:  rc = %d, count = %d\n\n", rc, ts.count());
}

void thread_entry2(void *p1, void *p2, void *p3)
{
	int rc;

	ts.signal();
	zassert_equal(ts.count(), 1, "count != 1");
	printk("Thread2 signal:  count = %d\n\n", ts.count());

	k_sleep(K_MSEC(10));

	ts.signal();
	zassert_equal(ts.count(), 1, "count != 1");
	printk("Thread2 signal:  count = %d\n\n", ts.count());

	ts.signal();
	zassert_equal(ts.count(), 2, "count != 2");
	printk("Thread2 signal:  count = %d\n\n", ts.count());

	k_sleep(K_MSEC(50));

	ts.signal();
	// count 0, because execution of wait_timed in thread 1 directly after ts.signal();
	zassert_equal(ts.count(), 0, "count != 0");
	printk("Thread2 signal:  count = %d\n\n", ts.count());

}*/

ZTEST_SUITE(tests_sema, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_sema, sema_test1)
{
	int rc;
	uint32_t cnt = 0;
	Sema* s = new Sema(0);
	zassert_not_null(s, "memory alloc failed");

	s->signal();
	//zassert_equal(s.count(), 2, "count != 2");
	cnt = s->count();
	printk("signal count = %d\n\n", cnt);

	rc = s->wait();
	zassert_equal(rc, 0, "rc != 0");
	cnt = s->count();
	printk("wait count = %d\n\n", cnt);

	s->signal();
	//zassert_equal(s.count(), 2, "count != 2");
	cnt = s->count();
	printk("signal count = %d\n\n", cnt);

	rc = s->wait_timed(10);
	zassert_equal(rc, 0, "rc != 0");
	cnt = s->count();
	printk("wait_timed count = %d\n\n", cnt);

	delete s;
}

/*ZTEST(tests_sema, sema_test2)
{
    k_thread_create(&thread_1_data, thread_1_stack, STACK_SIZE, thread_entry1, 
		NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&thread_2_data, thread_2_stack, STACK_SIZE, thread_entry2, 
		NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

	// Warten, bis beide Threads fertig sind
	k_thread_join(&thread_1_data, K_FOREVER);
	k_thread_join(&thread_2_data, K_FOREVER);
}*/