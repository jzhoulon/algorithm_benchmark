#include "sort.hpp"
using namespace std;

SortAlgo::SortAlgo(int length) {
  this->data = new int[length];
  this->len = length;
}

SortAlgo::~SortAlgo() {
  delete data;
}

void SortAlgo::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(0, 1000);
  auto random = std::bind(dis, generator);
  for (int i=0; i < len; i++) {
    this->data[i] = random();
  } 
}

void SortAlgo::printContent() {
  std::cout<< "data content, " ; 
  for (int i = 0; i < len; i++) {
    std::cout<< this->data[i] << ",";
  }
  std::cout << std::endl;

}

int* SortAlgo::insertSortFunc1() {
  for (int j = 1; j < len; j++) {
    auto key = data[j];
	int i = j-1;
	while(i > 0 && data[i] > key) {
	  data[i+1] = data[i];
	  i = i - 1;
	}
	data[i+1] = key;
  }
}



string SortAlgo::getAlgoName(Algo algo) {
  string name;
  switch(algo) {
    case Algo::insertSort1:
	  name = "insertSort1";
	  break;
	case Algo::insertSort2:
	  name = "insertSort2";
	  break;
    default:
	  std::cout << "not supported name" << std::endl;
	  name = "not supported algo";
  }
  return name;
}
void SortAlgo::benchmark(Algo algo) {
  float time_us = 0;
  auto t_s = std::chrono::high_resolution_clock::now();

  switch(algo) {
  case Algo::insertSort1:
      insertSortFunc1();
	  break;
  case Algo::insertSort2:
      insertSortFunc1();
	  break;
  default:
     std::cout<<"not support yet!!" << std::endl;
  
  }
  auto t_e = std::chrono::high_resolution_clock::now();
  std::cout << getAlgoName(algo) << "time:" <<
    std::chrono::duration_cast<std::chrono::microseconds>(t_e-t_s).count() / 1000. << "ms" <<std::endl;
  //printContent();
}

