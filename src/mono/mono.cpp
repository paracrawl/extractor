
#include "pcqueue.h"
#include "producer.h"

#include <iostream>
#include <thread>
#include <sstream>


namespace mono {

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
}


int main(int argc, char *argv[]) {
  util::PCQueue<int> pcq(3);

  for (std::string path; std::getline(std::cin, path);) {
    std::stringstream ss = mono::producer(path);
    std::cout << ss.str();
  }
  

  return 0;
}