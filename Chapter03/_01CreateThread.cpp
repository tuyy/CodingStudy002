/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <thread>

auto main() -> int32_t
{
    std::cout << "Hello" << std::endl;
    std::thread t([](){ std::cout << std::this_thread::get_id() << std::endl;});
    t.join();
    std::cout << "END" << std::endl;

    std::thread t2([](){ std::cout << std::this_thread::get_id() << std::endl;});
    t2.detach();

    // join or detach를 하지 않은 경우 joinable 한 thread라 한다.
    // joinable한 thread는 std::terminate 예외를 유발한다.
    // 또한 josin or detach를 여러번 호출한 경우 std::system_error를 유발한다.

    std::cout << std::thread::hardware_concurrency() << std::endl;

    return 0;
}
