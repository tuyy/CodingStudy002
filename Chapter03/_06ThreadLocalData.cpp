/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <thread>
#include <mutex>

std::mutex coutMutex;

// 스레드 로컬 문자열로 생성된다.
thread_local std::string s("hello from ");

void addThreadLocal(const std::string &s2)
{
    s += s2;
    //std::cout 보호
    std::lock_guard<std::mutex> guard(coutMutex);
    std::cout << s << std::endl;
    std::cout << "&s: " << &s << std::endl;
    std::cout << std::endl;
}

auto main() -> int32_t
{
    std::cout << std::endl;

    std::thread t1(addThreadLocal, "t1");
    std::thread t2(addThreadLocal, "t2");
    std::thread t3(addThreadLocal, "t3");
    std::thread t4(addThreadLocal, "t4");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
