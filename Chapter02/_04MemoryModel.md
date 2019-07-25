### atomic free functions
#### std::shared_ptr
* atomic 연산을 적용할 수 있는 유일한 비 atomic 데이터 타입이다.
* 레퍼런스 카운터를 늘릴떄 원자성이 보장된다.
* atomic 스마트포인터는 C++20 등장 예정
### memory_order
* 기본 모델은 std::memory_order_seq_cst (sequential consistency)
    * 우리가 보통 생각하는 thread 동작 방식. 모델들 중 가장 비싼 방식이다.
* CPU가 thread memory를 어떻게 바라볼지 나타낸다. 순서가 바뀐다.(잘 알고쓰자)
* acquire-release 모델은 생산자-소비자 모델에 적합하다
* relax 모델은 순서 제약이 없다. 그만큼 싼 방식이다.
    * add 만 더하는 count 연산은 relax 메모리 모델을 써도 무방하다.

### latch
* TODO
