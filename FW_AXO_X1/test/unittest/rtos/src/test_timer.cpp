#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "rtos/timer.h"


void timer_cb(k_timer* timer) {
    void* p1 = k_timer_user_data_get(timer);
    printk("Timer expired\n");
}


ZTEST_SUITE(tests_timer, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_timer, tests_timer_constructor)
{
    Timer* pTimer = new Timer(timer_cb, NULL, 1, 0);

    pTimer->start();
    printk("Timer start\n");
    pTimer->stop();
    printk("Timer stop\n");

    pTimer->reconfigure(2, 0);
    printk("Timer reconfigure\n");
    pTimer->start();
    printk("Timer start 2\n");

    pTimer->stop();
    printk("Timer stop\n");
    //k_sleep(K_MSEC(2000));

    pTimer->start(1, 0);
    printk("Timer start 1\n");
    pTimer->stop();
    printk("Timer stop\n");

    //k_sleep(K_MSEC(2000));

    pTimer->restart();
    printk("Timer restart 1\n");
    //k_sleep(K_MSEC(2000));
    delete pTimer;
}
