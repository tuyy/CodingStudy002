/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <condition_variable>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::mutex mutex_;
std::condition_variable condVar;

bool dataReady{false};

void doWithAtomicBool();

void waitingForWork()
{
    std::cout << "Waiting" << std::endl;
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck, []{return dataReady;});
    mySharedWork[1] = 2;
    std::cout << "Work done" << std::endl;
}

void setDataRead()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    mySharedWork = {1, 0, 3};
    {
        std::lock_guard<std::mutex> lck(mutex_);
        dataReady = true;
    }
    std::cout << "Data prepared" << std::endl;
    condVar.notify_one();
}

void doWithConVal()
{
    std::thread t1(waitingForWork);
    std::thread t2(setDataRead);

    t1.join();
    t2.join();

    for (const auto v : mySharedWork)
    {
        std::cout << v << " ";
    }

    std::cout << '\n' << std::endl;
    mySharedWork.clear();
}

std::atomic<bool> atomicDataReady(false);

void waitingForWorkWithAtomicBool()
{
    std::cout << "Waiting" << std::endl;
    while (!atomicDataReady.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    mySharedWork[1] = 2;
    std::cout << "Work done" << std::endl;
}

void setDataReadWithAtomicBool()
{
    mySharedWork = {1, 0, 3};
    atomicDataReady = true;
    std::cout << "Data prepared" << std::endl;
}

void doWithAtomicBool()
{
    std::thread t1(waitingForWorkWithAtomicBool);
    std::thread t2(setDataReadWithAtomicBool);

    t1.join();
    t2.join();

    for (const auto v : mySharedWork)
    {
        std::cout << v << " ";
    }

    std::cout << '\n' << std::endl;
    mySharedWork.clear();
}

auto main() -> int32_t
{
    // 조건 변수는 동기화시 push(notify)
    std::cout << "do with 조건변수" << std::endl;
    doWithConVal();

    // atomic bool 방식은 대기 스레드가 계속 확인(pull)
    std::cout << "do with 조건변수" << std::endl;
    doWithAtomicBool();

    return 0;
}
