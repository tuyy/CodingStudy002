/*
 * vim 설정
 * vim:ts=4:shiftwidth=4:et:cindent:fileencoding=utf-8:
 */

#include <iostream>
#include <future>
#include <vector>

void testAsync()
{
    auto sFunc = []() {
        std::cout << "Hello std::async(..) : " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 2000 + 1;
    };

    // std::launch::deferred, std::launch::async 시동정책을 가진다.
    // 반환값 없는 std::async 함수 호출은 순차적으로 실행된다.
    auto sFut = std::async(sFunc);

    // promise에 값이 set될 때 까지 block 된다.
    std::cout << sFut.get() << std::endl;
}

int sum(int a, int b)
{
    std::cout << "SUM start.." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "SUM END.." << std::endl;
    return a + b;
}

void testPackagedTask()
{
    std::cout << "using std::packaged_task.." << std::endl;

    // thread에 std::promise 객체를 안넘겨도 쉽게 std::future로 리턴값을 받을 수 있다.
    std::packaged_task<int(int, int)> sPackaged(sum);
    auto sFut = sPackaged.get_future();
    std::thread sT(std::move(sPackaged), 3, 4);
    sT.detach();
    std::cout << sFut.get() << std::endl;
    std::cout << "END!" << std::endl;
}

void add(int a, int b, std::promise<int> &p)
{
    std::cout << "void add start.." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "end.." << std::endl;
    p.set_value(a + b);
}

void testPromiseAndFuture()
{
    // std::promise와 std::future를 이용한 쓰레드 간 데이터 전달! (가장 기본 task)
    // std::promise 를 통해 예외도 받을 수 있다.
    // std::future가 한 번 이상 결과를 요청하면 std::future_error 예외가 발생한다.
    // 기본적으로 std::promise와 std::future는 1:1 관계가 성립한다.

    std::cout << "using std::promise and std::future.." << std::endl;
    std::promise<int> sPromise;
    auto sFut = sPromise.get_future();

    std::thread sT(add, 1, 2, std::ref(sPromise));
    sT.join();
    std::cout << sFut.get() << std::endl;
}

void testSharedFuture()
{
    auto sFunc1 = [](std::promise<int>&& p, int b){
        std::cout << "Waiting.." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "Go!" << std::endl;
        p.set_value(b * 100);
    };

    auto sFunc2 = [](std::shared_future<int> fut, int a){
        int num = fut.get();
        std::cout << "result" << num+a << std::endl;
    };

    std::promise<int> p;
    // 공유가 가능하다. 동시에 실행할 때 좋은 것 같다.
    std::shared_future<int> fut = p.get_future();

    std::thread t1(sFunc1, std::move(p), 1);
    std::thread t2(sFunc2, fut, 1);
    std::thread t3(sFunc2, fut, 2);
    std::thread t4(sFunc2, fut, 3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void foo(std::promise<void> &&p, int a)
{
    if (a == 1)
    {
        try
        {
            throw std::runtime_error("exception..");
        }
        catch (...)
        {
            std::cout << "occured exception" << std::endl;
            p.set_exception(std::current_exception());
        }
    }
    std::cout << "END foo(..)" << std::endl;
}

void testPromiseException()
{
    std::promise<void> p;
    auto fut = p.get_future();
    std::thread t(foo, std::move(p), 1);
    t.join();

    try
    {
        fut.get();
    }
    catch (const std::runtime_error &e)
    {
        std::cout << "good?" << std::endl;
    }
}

void boo(int64_t milliseconds)
{
    std::cout << "  " << std::this_thread::get_id() << " start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    std::cout << "  " << std::this_thread::get_id() << " end" << std::endl;
}

void testVoidFuture()
{
    std::vector<std::future<void>> sVec;
    sVec.push_back(std::async(boo, 5000));
    sVec.push_back(std::async(boo, 4000));
    sVec.push_back(std::async(boo, 3000));
    sVec.push_back(std::async(boo, 2000));
    sVec.push_back(std::async(boo, 1000));

    std::cout << "BEGIN" << std::endl;
    std::for_each(sVec.begin(), sVec.end(), [](auto &&sFut){ (void)sFut.get(); });
    std::cout << "END" << std::endl;
}

auto main() -> int32_t
{
    // 1) std::async
    testAsync();

    // 2) std::packaged_task
    testPackagedTask();

    // 3) std::promise, std::future
    testPromiseAndFuture();

    // 4) std::shared_future
    testSharedFuture();

    // 5) std::promise exception
    testPromiseException();

    // 6) etc
    testVoidFuture();

    return 0;
}
