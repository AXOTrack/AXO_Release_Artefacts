#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "rtos/mutex.h"


/*/ Thread-Stapel und Steuerblöcke
#define STACK_SIZE 512
#define PRIORITY 5

K_THREAD_STACK_DEFINE(thread_1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_2_stack, STACK_SIZE);

static struct k_thread thread_1_data;
static struct k_thread thread_2_data;

static Mutex tm;

void thread_entry1(void *p1, void *p2, void *p3)
{
	int rc;

	rc = tm.lock();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(tm.mutex_handle.lock_count, 1, "lock_count != 1");
	printk("Thread1 lock:  count = %d\n", tm.mutex_handle.lock_count);

	k_sleep(K_MSEC(10));
	printk("Thread1 unlock\n");
	tm.unlock();

	rc = tm.lock_timed(100);     // warte 100 ms maximal bis frei (unlocked)
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(tm.mutex_handle.lock_count, 1, "lock_count != 1");
	printk("Thread1 lock_timed:  count = %d\n", tm.mutex_handle.lock_count);
}

void thread_entry2(void *p1, void *p2, void *p3)
{
	int rc;

	rc = tm.lock_timed(100);     // warte 100 ms maximal bis frei (unlocked)
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(tm.mutex_handle.lock_count, 1, "lock_count != 1");
	printk("Thread2 lock_timed:  count = %d\n", tm.mutex_handle.lock_count);

	printk("Thread2 unlock\n");
	tm.unlock();
	k_sleep(K_MSEC(10));

	rc = tm.lock_timed(100);     // warte 100 ms maximal bis frei (unlocked)
	zassert_equal(rc, -11, "rc != -11");
	zassert_equal(tm.mutex_handle.lock_count, 1, "lock_count != 1");
	printk("Thread2 lock_timed:  count = %d, rc = %d\n", tm.mutex_handle.lock_count, rc);
}

*/

ZTEST_SUITE(tests_mutex, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_mutex, mutex_test1)
{
	int rc;
	Mutex* m = new Mutex();

	zassert_not_null(m, "memory alloc failed");

	rc = m->lock();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(m->mutex_handle.lock_count, 1, "lock_count != 1");
	printk("lock:  count = %d\n\n", m->mutex_handle.lock_count);

	m->unlock();
	zassert_equal(m->mutex_handle.lock_count, 0, "lock_count != 0");
	printk("unlock:  count = %d\n\n", m->mutex_handle.lock_count);

	rc = m->lock();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(m->mutex_handle.lock_count, 1, "lock_count != 1");
	printk("lock:  count = %d\n\n", m->mutex_handle.lock_count);

	rc = m->lock();
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(m->mutex_handle.lock_count, 2, "lock_count != 2");
	printk("lock:  count = %d\n\n", m->mutex_handle.lock_count);

	m->unlock();
	zassert_equal(m->mutex_handle.lock_count, 1, "lock_count != 1");
	printk("unlock:  count = %d\n\n", m->mutex_handle.lock_count);

	m->unlock();
	zassert_equal(m->mutex_handle.lock_count, 0, "lock_count != 0");
	printk("unlock:  count = %d\n\n", m->mutex_handle.lock_count);

	delete m;
}

/*ZTEST(tests_mutex, mutex_test2)
{
    k_thread_create(&thread_1_data, thread_1_stack, STACK_SIZE, thread_entry1, 
		NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

	k_thread_create(&thread_2_data, thread_2_stack, STACK_SIZE, thread_entry2, 
		NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

	// Warten, bis beide Threads fertig sind
	k_thread_join(&thread_1_data, K_FOREVER);
	k_thread_join(&thread_2_data, K_FOREVER);
}*/

ZTEST(tests_mutex, mutex_test3)
{
	int rc;
	Mutex* m = new Mutex();

	zassert_not_null(m, "memory alloc failed");

	rc = m->lock_timed(10);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(m->mutex_handle.lock_count, 1, "lock_count != 1");
	printk("lock:  count = %d\n\n", m->mutex_handle.lock_count);

	//m->unlock();
	//zassert_equal(m->mutex_handle.lock_count, 0, "lock_count != 0");
	//printk("unlock:  count = %d\n\n", m->mutex_handle.lock_count);
}