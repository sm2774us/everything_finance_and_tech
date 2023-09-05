#include<core/concurrency/task.h>
#include<core/concurrency/actor.h>
#include<core/concurrency/thread.h>
#include<core/concurrency/spinlock.hpp>
#include<core/concurrency/thread_pool.h>
#include<core/concurrency/ring_buffer_spsc_lockfree.hpp>
#include<core/concurrency/queue_mpsc.hpp>

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>
using namespace std;
using namespace core;

class worker
{
    public :
        void* run(void* arg)
        {
            int* p_val = static_cast<int *>(arg);
            ++(*p_val);
            return nullptr;
        }
};

class fred : public core::Actor
{
    int foo = 0;
    public:

        void *run() override
        {
            foo++;
            cout << "fred" << endl;
            return nullptr;
        }

        int getFoo() const { return foo; }
};

TEST(Concurrency, Thread)
{
    worker w;
    int testVal = 665;

    // Building thread
    TaskPtr task(new Task(&worker::run, &w, static_cast<void *>(&testVal)));
    core::Thread t1;
    t1.setTask(std::move(task));

    t1.start();
    t1.join();

    EXPECT_EQ(666, testVal);
}

TEST(Concurrency, Spinlock)
{
    int sum = 10;
    int actual = 0;
    std::vector<std::thread> threads;
    core::SpinLock sharedLock;

    for (int i {0}; i < sum; i++)
    {
        threads.push_back(std::thread([&](){
                                                sharedLock.lock();
                                                actual++;
                                                sharedLock.unlock();
                                            }));
    }

    for (auto& elem : threads)
    {
        elem.join();
    }

    EXPECT_EQ(sum, actual);
}

TEST(Concurrency, Actor)
{
    fred f;
    f.start();
    f.join();
    int testVal = f.getFoo();
    EXPECT_EQ(1, testVal);
}

TEST(Concurrency, RingBufferSPSCLockFree)
{
    core::RingBufferSPSCLockFree<int> queue(19);
    std::vector<std::thread> threads;
    vector<int> testVector = {4,5,7,2,6,7,8,9};
    int sum = 0;

    for (auto i : testVector)
    {
        threads.push_back(std::thread([&](int n){ cout << "Pushing " << n << endl; queue.push(n); }, i));
        sum += i;
    }

    int testSum = 0;

    for (auto i : testVector)
    {
        threads.push_back(std::thread([&](){    int n = 0;
                                                while(true)
                                                {
                                                    if(queue.tryPop(&n) )
                                                    {
                                                        break;
                                                    }
                                                }
                                                cout << "Pulled " << n << endl; testSum += n;
                                            }));
    }

    for (auto& elem : threads)
    {
        elem.join();
    }

    EXPECT_EQ(sum, testSum);
}


TEST(Concurrency, QueueMPSC)
{
    core::QueueMPSC<int> mqueue;
    std::vector<std::thread> threads;
    vector<int> testVector = { 4,5,7,2,6,7,8,9 };
    int sum = 0;
    int testSum = 0;

    for (auto i : testVector)
    {
        threads.push_back(std::thread([&](int n){ mqueue.push(n); }, i));
        sum += i;
    }

    std::thread readThread([&]()
                    {
                        core::QueueMPSC<int>::QueueMPSCNode* node = nullptr;
                        node = mqueue.flush();
                        while (node)
                        {
                            testSum += node->m_data;
                            node = node->m_next;
                        }

                    });

    for (auto& elem : threads)
    {
        elem.join();
    }

    readThread.join();

    EXPECT_EQ(sum, testSum);
}

struct ThreadPoolJobArgs
{
    int* array;
    int index;
};

class ThreadPoolJob
{
    public:
        void* run(ThreadPoolJobArgs arg)
        {
            arg.array[arg.index] = 1;
            return nullptr;
        }
};

TEST(Concurrency, ThreadPool)
{
    vector<string> threadNames = { "a", "b", "c", "d" };

    core::ThreadPoolArguments args;
    args.m_hyperThreading = true;
    args.m_pinThreadsToCores = true;
    args.m_threadNames = threadNames;
    args.m_workQueueSizePerThread = 1024;
    core::ThreadPool pool;

    pool.initialise(args);

    int* workArray = new int[ threadNames.size() ];
    ThreadPoolJob job;

    ThreadPoolJobArgs job_args;
    job_args.array = workArray;

    int expectedSum = 0;

    for (size_t i = 0; i < threadNames.size(); i++)
    {
        job_args.index = i;
        expectedSum++;
        core::Task task(&ThreadPoolJob::run, &job, job_args);
        pool.submitTask(task, i);
        core::Thread::sleep(1000000); // Let`s wait long enough , 1 second , to make sure that all thread pool jobs invoked
    }

    pool.shutdown();

    int actualSum = 0;

    std::for_each(workArray, workArray + threadNames.size(), [&] (int val) { actualSum+= val;});

    delete[] workArray;

    EXPECT_EQ(actualSum, expectedSum);
}