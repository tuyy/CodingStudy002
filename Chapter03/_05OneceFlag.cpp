/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <mutex>
#include <thread>

static std::once_flag onceFlag;

void do_once()
{
    std::call_once(onceFlag, [](){ std::cout << "Only once." << std::endl; });
}

auto main() -> int32_t
{
    // 상수식은 사용자 정의 타입도 가능하다.
    // 단, 아래 조건을 만족해야한다.
    // 1) 가상 메서드나 가상 기본 클래스가 없어야한다.
    // 2) 생성자는 비어 있어야 하며, 그 자체로 상수식이어야 한다.
    // 3) 컴파일 타임에 호출될 수 있어야 하는 메서드도 상수식이어야 한다.
    constexpr double pi = 3.14;
    std::cout << "pi: " << pi << std::endl;

    // std::call_one와 std::once_flag
    // std::call_once를 사용하면 callable 함수를 등록할 수 있다.
    // std::call_flag는 하나의 등록된 함수만이 호출될 수 있다고 가정한다.

    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << std::endl;

    return 0;
}
