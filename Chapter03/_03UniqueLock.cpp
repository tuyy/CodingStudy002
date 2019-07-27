/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <mutex>
#include <thread>

struct CriticalData
{
    std::mutex mut;
};

void deadLockWithDeferLock(CriticalData &a, CriticalData &b)
{
    std::unique_lock<std::mutex> guard1(a.mut, std::defer_lock);
    std::cout << "Thread: " << std::this_thread::get_id() << " first mutex" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::unique_lock<std::mutex> guard2(b.mut, std::defer_lock);
    std::cout << "  Thread: " << std::this_thread::get_id() << " second mutex" << std::endl;
    std::cout << "  Thread: " << std::this_thread::get_id() << " both mutex" << std::endl;

    // 한꺼번에 동시에 lock을 걸기 위해 std::unique_lock을 사용한다. (RAII 적용됨!)
    std::lock(guard1, guard2);

    for (size_t i = 0; i < 3; ++i)
    {
        std::cout << " G Hello? : " << i << std::endl;
    }
}

// void deadLockWithAdoptLock(CriticalData &a, CriticalData &b)
// {
//     // deadLockWithDeferLock() 과 동일하다.(lock 순서 주의) std::adopt_lock 인자를 주면 이미 lock이 걸렸다고 가정한다.
//     std::lock(guard1, guard2);
//     std::unique_lock<std::mutex> guard1(a.mut, std::adopt_lock);
//     std::unique_lock<std::mutex> guard2(b.mut, std::adopt_lock);
//
//     std::cout << "Thread: " << std::this_thread::get_id() << " first mutex" << std::endl;
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//     std::cout << "  Thread: " << std::this_thread::get_id() << " second mutex" << std::endl;
//     std::cout << "  Thread: " << std::this_thread::get_id() << " both mutex" << std::endl;
//
//     for (size_t i = 0; i < 3; ++i)
//     {
//         std::cout << " G Hello? : " << i << std::endl;
//     }
// }

// void deadLockWIthScopedLock(CriticalData &a, CriticalData &b)
// {
//     std::scoped_lock sLock(a.mut, b.mut);
//     // ..
// }

auto main() -> int32_t
{
    CriticalData a;
    CriticalData b;

    std::thread t1([&]{
        deadLockWithDeferLock(a, b);});
    std::thread t2([&]{
        deadLockWithDeferLock(b, a);});
    std::thread t3([&]{
        deadLockWithDeferLock(b, a);});
    std::thread t4([&]{
        deadLockWithDeferLock(a, b);});

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "END!" << std::endl;

    return 0;
}
