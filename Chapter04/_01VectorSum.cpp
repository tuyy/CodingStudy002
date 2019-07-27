/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <iostream>
#include <random>
#include <vector>

constexpr long long size = 100000000;

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

static void testSingle(const std::vector<int> &randValues)
{
    unsigned long long sum = {};

    std::cout << "START!" << std::endl;
    const auto start = std::chrono::steady_clock::now();

    for (auto n: randValues)
    {
        sum += n;
    }

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl;
}

static void testAlgo(const std::vector<int> &randValues)
{
    std::cout << "START!" << std::endl;

    const auto start = std::chrono::steady_clock::now();

    unsigned long long sum = std::accumulate(randValues.begin(), randValues.end(), 0);

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl;
}

void testLock(const std::vector<int> &randValues)
{
    std::cout << "START!" << std::endl;

    const auto start = std::chrono::steady_clock::now();

    unsigned long long sum = std::accumulate(randValues.begin(), randValues.end(), 0);

    std::mutex m;
    for (auto n: randValues)
    {
        std::lock_guard<std::mutex> lock(m);
        sum += n;
    }

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl;
}

static void testAtomic(const std::vector<int> &randValues)
{
    std::atomic<unsigned long long> sum = {};

    std::cout << "START! is_lock_free? " << sum.is_lock_free() << std::endl;
    const auto start = std::chrono::steady_clock::now();

    for (auto n: randValues)
    {
        sum += n;
    }

    const std::chrono::duration<double> duration = std::chrono::steady_clock::now() - start;
    std::cout << "  Time for mySumition " << duration.count() << " seconds" << std::endl;
    std::cout << "  Result: " << sum << std::endl;
}

auto main() -> int32_t
{
    // 싱글쓰레드에서 벡터합 구하기

    auto sVec = getVector();

    // for : 0.09s
    testSingle(sVec);

    // std::accumulate : 0.08s
    testAlgo(sVec);

    // std::lock_guard : 3.5s
    testLock(sVec);

    // std::atomic<long long int> : 1.5s
    testAtomic(sVec);

    return 0;
}
