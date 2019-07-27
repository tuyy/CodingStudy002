/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::mutex gMutex;

class Worker
{
public:
    explicit Worker(const std::string &aName)
        : mName(aName)
    {
    }

    void operator()(const bool aHasMutex)
    {
        for (size_t i = 1; i < 3; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (aHasMutex)
            {
                gMutex.lock();
            }

            std::cout << mName << ": " << i << " done !!!" << std::endl;

            if (aHasMutex)
            {
                gMutex.unlock();
            }
        }
    }

private:
    std::string mName;
};

// 예상대로 마구자비로 출력된다.
void printAsyncWrite()
{
    std::cout << "Boss: Let's start working.\n\n" << std::endl;

    std::vector<std::thread> sVec;
    sVec.emplace_back(Worker("k1"), false);
    sVec.emplace_back(Worker("k2"), false);
    sVec.emplace_back(Worker("k3"), false);

    std::for_each(sVec.begin(), sVec.end(), [](auto &t){t.join();});

    std::cout << "END!" << std::endl;
}

void printAsyncWriteWithMutex()
{
    std::cout << "\nBoss: Let's start working with Mutex.\n\n" << std::endl;

    std::vector<std::thread> sVec;
    sVec.emplace_back(Worker("jonh1"), true);
    sVec.emplace_back(Worker("jonh2"), true);
    sVec.emplace_back(Worker("jonh3"), true);

    std::for_each(sVec.begin(), sVec.end(), [](auto &t){t.join();});

    std::cout << "END!" << std::endl;
}

void printAsyncWriteWithLockGuard()
{
    auto sFunc = [](const auto &aName){
        std::lock_guard<std::mutex> sLockGuard(gMutex);
        for (size_t i = 0; i < 5; ++i)
        {
            std::cout << aName << " hello? " << i << std::endl;
        }
    };

    std::vector<std::thread> sVec;
    sVec.emplace_back(sFunc, "first");
    sVec.emplace_back(sFunc, "second");
    sVec.emplace_back(sFunc, "third");
    sVec.emplace_back(sFunc, "fourth");

    std::for_each(sVec.begin(), sVec.end(), [](auto &t){t.join();});
    std::cout << "END!" << std::endl;
}

auto main() -> int32_t
{
    // 1) std::cout에 비동기 쓰기
    printAsyncWrite();

    // 2) mutex 적용
    printAsyncWriteWithMutex();

    // 3) std::lock_guard 적용
    printAsyncWriteWithLockGuard();

    return 0;
}