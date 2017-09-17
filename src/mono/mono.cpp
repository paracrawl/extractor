
#include <iostream>
#include <thread>
#include "pcqueue.h"

void thread_producer(util::PCQueue<int> *pcq) {
  int r = rand();
  (*pcq).Produce(r);
  std::cout << "Producing: " << r << std::endl;
}

void thread_consumer(util::PCQueue<int> *pcq) {
  std::chrono::milliseconds sleepDuration(100);
  std::this_thread::sleep_for(sleepDuration);

  int c = (*pcq).Consume();
  std::cout << "Consumed: " << c << std::endl;
}

int main() {
  srand(time(NULL));
  util::PCQueue<int> pcq(3);

  std::thread t1(thread_producer, &pcq);
  std::thread t2(thread_consumer, &pcq);

  t1.join();
  t2.join();

  return 0;
}