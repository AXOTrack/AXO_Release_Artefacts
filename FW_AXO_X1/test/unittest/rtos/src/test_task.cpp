#include <zephyr/ztest.h>
//#include <vector>
//#include <algorithm>

#define private public
#include "rtos/task.h"


#define STACK_SIZE 1024
#define PRIORITY 5
static K_THREAD_STACK_DEFINE(TASK_STACK, STACK_SIZE);


class TestTask : public Task {
 public:
    explicit TestTask(const char* name = "??");
    virtual ~TestTask();
    virtual void task(void);

 private:
};

TestTask::TestTask(const char* name) : Task(name) {
}

TestTask::~TestTask() { }

void TestTask::task(void) { 
}


ZTEST_SUITE(tests_task, NULL, NULL, NULL, NULL, NULL);

// test constructor and destructor
ZTEST(tests_task, tests_task_constructor)
{
	TestTask* pTask = new TestTask("test");

	zassert_not_null(pTask, "memory alloc failed");

    pTask->create(TASK_STACK, STACK_SIZE, PRIORITY);
    pTask->setId(17);
    pTask->start();
    pTask->configure();
    pTask->task();

	delete pTask; 
}

// test function taskCode
ZTEST(tests_task, tests_task_taskCode)
{
	TestTask* pTask = new TestTask("test");

	zassert_not_null(pTask, "memory alloc failed");

    pTask->create(TASK_STACK, STACK_SIZE, PRIORITY);
    pTask->start();
    Task::taskCode((void*)pTask,NULL,NULL);

	delete pTask; 
}
