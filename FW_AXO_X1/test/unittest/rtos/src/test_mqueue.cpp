#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

#include "rtos/mbox.h"
#include "rtos/mpelem.h"
#include "rtos/mpool.h"
#include "rtos/mqueue.h"

/*#include <zephyr/ztest.h>
#include <vector>
#include <algorithm>

#define private public
#include "rtos/MessageQueue.h"*/

ZTEST_SUITE(tests_msgqueue, NULL, NULL, NULL, NULL, NULL);

ZTEST(tests_msgqueue, msgqueue_test)
{
	int32_t rc;
    uint16_t* out;
    uint16_t in;
	std::string s;
	std::string exp;

    MQueue<uint16_t, 3> q;

    in = 11;
	rc = q.put(&in);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(q.size(), 3, "size() != 3");
	zassert_equal(q.count(), 1,  "used() != 1");
	zassert_equal(q.avail(), 2, "available() != 2");
	zassert_equal(q.msg_size(), 4, "msg_size() != 4");
	printk("put:  used = %d, available = %d, enqued value = %d\n\n", q.count(), q.avail(), in);

    q.stats();

    in = 12;
	rc = q.put(&in);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(q.count(), 2, "used() != 2");
	zassert_equal(q.avail(), 1, "available() != 1");
	printk("put:  used = %d, available = %d, enqued value = %d\n\n", q.count(), q.avail(), in);

	out = q.get();
	//zassert_equal(*out, 11, "val != 0");
	zassert_equal(q.count(), 1, "used() != 1");
	zassert_equal(q.avail(), 1, "available() != 1");
	printk("put:  used = %d, available = %d, enqued value = %d\n\n", q.count(), q.avail(), in);

    in = 13;
	rc = q.put(&in);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(q.count(), 2, "used() != 2");
	printk("put:  used = %d, available = %d, enqued value = %d\n\n", q.count(), q.avail(), in);

    in = 14;
	rc = q.put(&in);
	zassert_equal(rc, 0, "rc != 0");
	zassert_equal(q.count(), 3, "used() != 3");
	printk("put:  used = %d, available = %d, enqued value = %d\n\n", q.count(), q.avail(), in);
	// Queue full

    in = 14;
	rc = q.put(&in);
	zassert_equal(rc, -35, "rc != -35");
}