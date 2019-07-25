/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <atomic>

template <typename T>
T fetch_mult(std::atomic<T> &shared, T mult)
{
    T oldValue = shared.load();
    while(!shared.compare_exchange_strong(oldValue, oldValue * mult));
    return oldValue;
}

auto main() -> int32_t
{
    int intArray[5];
    std::atomic<int*> p(intArray);
    p++;
    assert(p.load() == &intArray[1]);

    p += 1;
    assert(p.load() == &intArray[2]);

    p--;
    assert(p.load() == &intArray[1]);

    std::atomic<int> myInt{5};
    std::cout << myInt << std::endl;

    fetch_mult(myInt, 5);
    std::cout << myInt << std::endl;

    // 사용자 정의 atomic은 제약이 많다. 사용시 주의하자.

    return 0;
}
