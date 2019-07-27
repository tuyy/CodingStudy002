/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <vector>
#include <random>
#include <thread>

constexpr long long size = 100000000;

constexpr long long s1 = 25000000;
constexpr long long s2 = 50000000;
constexpr long long s3 = 75000000;
constexpr long long s4 = 100000000;

std::mutex m;

static std::vector<int> getVector()
{
    std::vector<int> randValues;

    std::random_device seed;
    std::mt19937 engine(seed());
    std::uniform_int_distribution<> uniformDist(1, 10);
    for (long long i = 0; i < size; ++i)
    {
        randValues.push_back(uniformDist(engine));
    }
    return randValues;
}

void testLockGuard(const std::vector<int> &randomValues)
{
    auto func = [](unsigned long long &sum, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        for (auto it = beg; it < end; ++it)
        {
            std::lock_guard<std::mutex> lock(m);
            sum += val[it];
        }
    };

    unsigned long long sum = 0;

    const auto start = std::chrono::steady_clock::now();

    std::thread t1(func, std::ref(sum), std::ref(randomValues), 0, s1);
    std::thread t2(func, std::ref(sum), std::ref(randomValues), s1, s2);
    std::thread t3(func, std::ref(sum), std::ref(randomValues), s2, s3);
    std::thread t4(func, std::ref(sum), std::ref(randomValues), s3, s4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl << std::endl;
}

void testAtomic(const std::vector<int> &randomValues)
{
    auto func = [](std::atomic<unsigned long long> &sum, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        for (auto it = beg; it < end; ++it)
        {
            sum += val[it];
        }
    };

    std::atomic<unsigned long long> sum = 0;

    const auto start = std::chrono::steady_clock::now();

    std::thread t1(func, std::ref(sum), std::ref(randomValues), 0, s1);
    std::thread t2(func, std::ref(sum), std::ref(randomValues), s1, s2);
    std::thread t3(func, std::ref(sum), std::ref(randomValues), s2, s3);
    std::thread t4(func, std::ref(sum), std::ref(randomValues), s3, s4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl << std::endl;
}

void testAtomicFetchAdd(const std::vector<int> &randomValues)
{
    auto func = [](std::atomic<unsigned long long> &sum, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        for (auto it = beg; it < end; ++it)
        {
            sum.fetch_add(val[it]);
        }
    };

    std::atomic<unsigned long long> sum = 0;

    const auto start = std::chrono::steady_clock::now();

    std::thread t1(func, std::ref(sum), std::ref(randomValues), 0, s1);
    std::thread t2(func, std::ref(sum), std::ref(randomValues), s1, s2);
    std::thread t3(func, std::ref(sum), std::ref(randomValues), s2, s3);
    std::thread t4(func, std::ref(sum), std::ref(randomValues), s3, s4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl << std::endl;
}

void testAtomicFetchAddRelaxed(const std::vector<int> &randomValues)
{
    auto func = [](std::atomic<unsigned long long> &sum, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        for (auto it = beg; it < end; ++it)
        {
            sum.fetch_add(val[it], std::memory_order_relaxed);
        }
    };

    std::atomic<unsigned long long> sum = 0;

    const auto start = std::chrono::steady_clock::now();

    std::thread t1(func, std::ref(sum), std::ref(randomValues), 0, s1);
    std::thread t2(func, std::ref(sum), std::ref(randomValues), s1, s2);
    std::thread t3(func, std::ref(sum), std::ref(randomValues), s2, s3);
    std::thread t4(func, std::ref(sum), std::ref(randomValues), s3, s4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl << std::endl;
}

auto main() -> int32_t
{
    // 멀티쓰레드 환경에서 벡터합 구하기

    auto sVec = getVector();

    // std::lock_guard
    testLockGuard(sVec);

    // std::atomic
    testAtomic(sVec);

    // std::atomic fetch_add(..)
    testAtomicFetchAdd(sVec);

    // std::atomic fetch_add(..) std::memory_order_relaxed
    testAtomicFetchAddRelaxed(sVec);

    return 0;
}
