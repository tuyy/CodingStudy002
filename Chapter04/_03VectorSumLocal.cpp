/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <future>

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
        unsigned long long tmpSum = 0;
        for (auto it = beg; it < end; ++it)
        {
            tmpSum += val[it];
        }
        std::lock_guard<std::mutex> lock(m);
        sum += tmpSum;
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
        unsigned long long tmpSum = 0;
        for (auto it = beg; it < end; ++it)
        {
            tmpSum += val[it];
        }
        sum += tmpSum;
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
        unsigned long long tmpSum = 0;
        for (auto it = beg; it < end; ++it)
        {
            tmpSum += val[it];
        }
        sum.fetch_add(tmpSum, std::memory_order_relaxed);
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

thread_local unsigned long long tmpSum2 = 0;

void testThreadLoacal(const std::vector<int> &randomValues)
{
    auto func = [](std::atomic<unsigned long long> &sum, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        for (auto it = beg; it < end; ++it)
        {
            tmpSum2 += val[it];
        }
        sum.fetch_add(tmpSum2, std::memory_order_relaxed);
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

void testTask(const std::vector<int> &randomValues)
{
    auto func = [](std::promise<unsigned long long> &&p, const std::vector<int> &val, unsigned long long beg, unsigned long long end)
    {
        unsigned long long sum = 0;
        for (auto it = beg; it < end; ++it)
        {
            sum += val[it];
        }
        p.set_value(sum);
    };

    std::promise<unsigned long long> p1;
    std::promise<unsigned long long> p2;
    std::promise<unsigned long long> p3;
    std::promise<unsigned long long> p4;

    auto fut1 = p1.get_future();
    auto fut2 = p2.get_future();
    auto fut3 = p3.get_future();
    auto fut4 = p4.get_future();

    const auto start = std::chrono::steady_clock::now();

    std::thread t1(func, std::move(p1), std::ref(randomValues), 0, s1);
    std::thread t2(func, std::move(p2), std::ref(randomValues), s1, s2);
    std::thread t3(func, std::move(p3), std::ref(randomValues), s2, s3);
    std::thread t4(func, std::move(p4), std::ref(randomValues), s3, s4);

    auto sum = fut1.get() + fut2.get() + fut3.get() + fut4.get();

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl << std::endl;

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void testTask2(const std::vector<int> &randomValues)
{
    auto func = [](const std::vector<int> &val, unsigned long long beg, unsigned long long end) -> unsigned long long
    {
        unsigned long long sum = 0;
        for (auto it = beg; it < end; ++it)
        {
            sum += val[it];
        }
        return sum;
    };

    const auto start = std::chrono::steady_clock::now();

    std::vector<std::future<unsigned long long>> futs;
    for (size_t i = 0; i < size; i += 10000000)
    {
        futs.push_back(std::async(func, std::ref(randomValues), i, i+10000000));
    }

    std::cout << "  SIZE:" << futs.size() << std::endl;
    std::cout << "  CORE CNT:" << std::thread::hardware_concurrency() << std::endl;

    unsigned long long sum = 0;
    for (auto &fut : futs)
    {
       sum += fut.get();
    }

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

    // std::atomic fetch_add(..) std::memory_order_relaxed
    testAtomicFetchAddRelaxed(sVec);

    // thread local
    testThreadLoacal(sVec);

    // task
    testTask(sVec);

    // task2
    testTask2(sVec);

    return 0;
}
