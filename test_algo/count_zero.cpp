#include <iostream>
#include <chrono>
#include <random>
#include <functional>
#include <string.h>
#include <algorithm>
using namespace std;

class CountZeroPattern {
public:

  enum Algo {
    naive,
  };
  CountZeroPattern();
  ~CountZeroPattern();
  void naive_func();
  void generateRandom();
  void setDataLen(int length);
  string getAlgoName(Algo a);
  void benchmark(Algo a);
  void printContent();
private:
  int* data_;
  int len_;
};

CountZeroPattern::CountZeroPattern() {

}
CountZeroPattern::~CountZeroPattern() {
  delete data_;
}

void CountZeroPattern::setDataLen(int length) {
  len_ = length;
  data_ =  new int[len_];
}

void CountZeroPattern::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(-20, 20);
  auto random = std::bind(dis, generator);
  for (int i =0; i < len_; i++) {
    data_[i] = random();
  }
}
void CountZeroPattern::naive_func() {
  int count = 0;
  for (int i = 0; i < len_; i++) {
    for (int j = i+1; j < len_; j++) {
      if (data_[i] + data_[j] == 0) {
        count++;
        std::cout << "(" << i << ":" << data_[i] <<',' << j << ":" << data_[j] << ")," ;
      }
          
    }
  } 

  std::cout<<std::endl<< "naive func = " << count << endl; 
} 
string CountZeroPattern::getAlgoName(Algo a) {
  string name;
  switch (a) {
    case naive:
        name = "naive algo";
        break;
    default:
        std::cout << "unsupported algo" << std::endl;
        return name;
  }
}
void CountZeroPattern::printContent() {
  std::cout << "data content: " << std::endl;
  for (int i = 0; i < len_; i++ ) {
    std::cout << data_[i] << ", ";
  }
  std::cout << std::endl;
}

void CountZeroPattern::benchmark(Algo a) {
  float time_us = 0.0;
  auto t_s = std::chrono::high_resolution_clock::now();
  switch(a) {
    case naive:
      naive_func();
      break;
    default:
      std::cout << "unsupported algo" << std::endl;
  }
  auto t_e = std::chrono::high_resolution_clock::now();
  std::cout << getAlgoName(a) << "time" <<
    std::chrono::duration_cast<std::chrono::microseconds>(t_e - t_s).count() /1000. << "ms" << std::endl;
}
 

int main(int argc, char **argv) {
  CountZeroPattern c;
  c.setDataLen(200);
  c.generateRandom();
  c.benchmark(CountZeroPattern::Algo::naive);
  c.printContent();
  return 0;
}


