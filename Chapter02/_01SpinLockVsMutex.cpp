#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

class SpinLock
{
public:
    SpinLock()
        : mFlag(false)
    {
    }

    void lock()
    {
        // 과도한 CPU 사용
        while(mFlag.test_and_set());
    }

    void unlock()
    {
        mFlag.clear();
    }

private:
    std::atomic_flag mFlag; // C++ atomic의 가장 기본 빌딩 블록이며 이것을 이용해서 고급 atomic 연산 가능(lock-free 보장)
};

static SpinLock gSpinLock;

void worksOnResourceWithSpinLock()
{
    gSpinLock.lock();

    // 공유변수 접근
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    gSpinLock.unlock();
}

static std::mutex gM;

void worksOnResourceWithMutex()
{
    gM.lock();
    // 공유변수 접근
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    gM.unlock();
}

auto main() -> int32_t
{
    std::cout << "Hello SpinLock!" << std::endl;

    std::thread t1(worksOnResourceWithSpinLock);
    std::thread t2(worksOnResourceWithSpinLock);

    t1.join();
    t2.join();

    std::cout << "Hello Mutex!" << std::endl;

    std::thread t3(worksOnResourceWithSpinLock);
    std::thread t4(worksOnResourceWithSpinLock);

    t3.join();
    t4.join();

    return 0;
}
