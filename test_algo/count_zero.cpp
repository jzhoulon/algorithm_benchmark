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
    sort,
  };
  CountZeroPattern();
  ~CountZeroPattern();
  void naive_func();
  void sort_func();
  void generateRandom();
  void setDataLen(int length);
  void resetData();
  string getAlgoName(Algo a);
  void benchmark(Algo a);
  void printContent();
private:
  int* dataOrig_;
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
  dataOrig_ = new int[len_];
}

void CountZeroPattern::resetData() {
  memcpy(dataOrig_, data_, len_*sizeof(data_[0]));
}

void CountZeroPattern::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(-20, 20);
  auto random = std::bind(dis, generator);
  for (int i =0; i < len_; i++) {
    data_[i] = random();
  }
  resetData();
}
void CountZeroPattern::naive_func() {
  int count = 0;
  for (int i = 0; i < len_; i++) {
    for (int j = i+1; j < len_; j++) {
      if (dataOrig_[i] + dataOrig_[j] == 0) {
        count++;
      }
    }
  }
  std::cout << std::endl;

  std::cout<<std::endl<< "naive func = " << count << endl; 
}


void CountZeroPattern::sort_func() {
  int count = 0;
  int num_neg = 0;
  int num_pos = 0;
  int index_zero;
  std::sort(data_, data_+len_);

  int i = 0, j = len_ -1;
  while(i < j) {
    if (data_[i] > 0) break;
    else if (data_[i] == 0) {index_zero = i; break;}
    if (data_[j] <0) break;
    else if(data_[j] == 0) {index_zero == j; break;}
    if (data_[i] + data_[j] != 0) {
      if (data_[i] + data_[j] > 0) j--;
      else i++;
    } else {
      do {
        i++;
        num_neg++;
      } while(data_[i] == data_[i-1]);
      do {
        j--;
        num_pos++;
      } while(data_[j] == data_[j+1]);
      count+= num_neg*num_pos;
      num_neg = 0; num_pos= 0;
    }
  }
    int num_zero = 0;
     if (data_[index_zero+1] == 0) {
       while(data_[index_zero] == 0) {
        num_zero++;
        index_zero++;
       }
     } else if (data_[index_zero -1 ] == 0) {
       while(data_[index_zero] == 0) {
         num_zero++;
         index_zero--;
       }
     } else {
       num_zero++;
     }

    count += num_zero * (num_zero -1)/2;
  std::cout << std::endl<< "sort func " << count << std::endl;
} 


string CountZeroPattern::getAlgoName(Algo a) {
  string name;
  switch (a) {
    case naive:
        name = "naive algo";
        break;
    case sort:
        name = "sort algo";
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
    case sort:
      sort_func();
    default:
      std::cout << "unsupported algo" << std::endl;
  }
  auto t_e = std::chrono::high_resolution_clock::now();
  std::cout << getAlgoName(a) << "time" <<
    std::chrono::duration_cast<std::chrono::microseconds>(t_e - t_s).count() /1000. << "ms" << std::endl;
}
 

int main(int argc, char **argv) {
  CountZeroPattern c;
  c.setDataLen(100000);
  c.generateRandom();
  c.benchmark(CountZeroPattern::Algo::naive);
 // c.printContent();
  c.benchmark(CountZeroPattern::Algo::sort);
 //c.printContent();
  return 0;
}


