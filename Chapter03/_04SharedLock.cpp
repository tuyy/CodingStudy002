/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <mutex>
#include <thread>
#include <map>
#include <shared_mutex>
#include <string>

std::map<std::string, int> teleBook{{"Dijkstra", 1972}, {"Scott", 1927}, {"Ritchile", 1983}};

// or std::shared_mutex
std::shared_timed_mutex teleBookMutex;

void addToTeleBook(const std::string &na, int tele)
{
    // 쓰기는 하나의 쓰레드만 ok
    // 쓰는 동안엔 읽기가 막힌다. (당연한가?)
    std::lock_guard<std::shared_timed_mutex> writerLock(teleBookMutex);

    std::cout << "\nSTARTING UPDATE " << na << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    teleBook[na] = tele;
    std::cout << " ... ENDING UPDATE " << na << std::endl;
}

void printNumber(const std::string &na)
{
    // shared mutex는 동시에 읽기 작업 ok
    // 한 쓰레드라도 읽고 있는 중엔 쓰기 불가
    std::shared_lock<std::shared_timed_mutex> readerLock(teleBookMutex);

    if (teleBook.find(na) == teleBook.end())
    {
        std::cout << na << ": not found" << std::endl;
    }
    else
    {
        std::cout << na << ": " << teleBook[na] << std::endl;
    }
}

auto main() -> int32_t
{
    std::cout << std::endl;

    std::thread reader1([]{ printNumber("Scott"); });
    std::thread reader2([]{ printNumber("Ritchile"); });
    std::thread w1([]{ addToTeleBook("Scott", 1968); });
    std::thread reader3([]{ printNumber("Dijkstra"); });
    std::thread reader4([]{ printNumber("Scott"); });
    std::thread reader8([]{ printNumber("Bjarne"); });
    std::thread w2([]{ addToTeleBook("Bjarne", 1965); });
    std::thread reader5([]{ printNumber("Scott"); });
    std::thread reader6([]{ printNumber("Ritchile"); });
    std::thread reader7([]{ printNumber("Scott"); });

    reader1.join();
    reader2.join();
    reader3.join();
    reader4.join();
    reader5.join();
    reader6.join();
    reader7.join();
    reader8.join();
    w1.join();
    w2.join();

    std::cout << std::endl;

    std::cout << "\nThe new telephone book" << std::endl;
    for (const auto [k, v] : teleBook)
    {
        std::cout << k << ": " << v << std::endl;
    }

    return 0;
}
