/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <functional>
#include <thread>
#include <condition_variable>

std::mutex mutex_;
std::condition_variable condVar;

bool dataReady{false};

void doTheWork()
{
    std::cout << "Processing shared data." << std::endl;
}

void waitingForWork()
{
    std::cout << "Worker: Waiting for work." << std::endl;
    std::unique_lock<std::mutex> lck(mutex_);
    condVar.wait(lck, [](){ return dataReady; });
    doTheWork();
    std::cout << "Work done." << std::endl;
}

void setDataReady()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        std::lock_guard<std::mutex> lck(mutex_);
        dataReady = true;
    }
    std::cout << "Sender: Data is ready." << std::endl;
    condVar.notify_one();
}

void setInvalidDataReady()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    std::cout << "Sender: Data is invalid ready." << std::endl;
    condVar.notify_one();
}

void testConVal()
{
    std::cout << std::endl;

    std::thread t1(setDataReady);
    std::thread t2(waitingForWork);

    t1.join();
    t2.join();

    std::cout << "END.." << std::endl;
}

void testInvalidConVal()
{
    std::cout << std::endl;

    std::thread t1(setInvalidDataReady);
    std::thread t2(waitingForWork);
    std::thread t3([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        dataReady = true; // true해도 꺠어나진 않내?
        // condVar.notify_all();
        std::cout << "dataReady is true" << std::endl;
    });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "END.." << std::endl;
}

auto main() -> int32_t
{
    // testConVal();
    testInvalidConVal();

    return 0;
}
