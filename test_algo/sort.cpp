#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <string.h>

using namespace std;

bool isCheckAccuracy;
class SortAlgo{
public:
 SortAlgo();
 ~SortAlgo();

enum Algo {
 insertSort,
 insertSortDichotomy,
 bubbleSort,
 cocktailSort,
 chooseSort,
};

 int* insertSortFunc();
 int* insertSortDichotomyFunc();
 int* bubbleSortFunc();
 int* cocktailSortFunc();
 int* chooseSortFunc();
 void generateRandom();
 void resetData();
 void setDataLen(int length);
 void printContent();
 string getAlgoName(Algo algo);
 void benchmark( Algo algo);

private:
 int* data_orig;
 int* data;
 int len;
};


SortAlgo::SortAlgo() {

}


SortAlgo::~SortAlgo() {
  delete data_orig;
  delete data;
}

void SortAlgo::setDataLen(int length) {
  this->data_orig = new int[length];
  this->data = new int[length];
  this->len = length;
}

void SortAlgo::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(0, 1000);
  auto random = std::bind(dis, generator);
  for (int i=0; i < len; i++) {
    this->data_orig[i] = random();
  } 
  resetData();
}

void SortAlgo::resetData() {
  memcpy(data, data_orig, len * sizeof(data_orig[0]));
}
void SortAlgo::printContent() {
  for (int i = 0; i < len; i++) {
    std::cout<< this->data[i] << ",";
  }
  std::cout << std::endl;
}

int* SortAlgo::insertSortFunc() {
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

int* SortAlgo::insertSortDichotomyFunc() {
  for (int j = 1; j < len; j++) {
  auto key = data[j];
  int index_found;
  bool found = false;
  int start = 0;
  int end = j;
  while(start <=end) {
    int mid = (start + end)/2;
    if (data[mid] < key)
      start = mid+1;
    else
      end = mid-1;
   }
  index_found = start;
  for(int i = j; i > index_found; i--) {
    data[i] =  data[i - 1];
  }
  data[index_found]  = key;
  }
}

int* SortAlgo::bubbleSortFunc() {
  auto tmp = data[0];
  bool flag = false;
  for (int j = 0; j < len; j++) {
    flag = false;
    for (int i = 0; i < len-j -1 ; i++) {
      if (data[i] > data[i+1]) {
        tmp = data[i+1];
        data[i+1] = data[i];
        data[i] = tmp;
        flag  = true;
      }
    }
    if (flag ==false)
      break;
  }
}

int* SortAlgo::cocktailSortFunc() {
  auto tmp = data[0];
  bool flag = false;
  int start = 0;
  int end = len - 1;
  while (start < end) {
    for (int i = start; i < end; i++) {
      if (data[i] > data[i+1]) {
        tmp = data[i+1];
        data[i+1] = data[i];
        data[i] = tmp;
        flag = true;
      }
    }

    for(int i = end -1; i > start; i--) {
      if(data[i]< data[i -1 ]) {
        tmp = data[i-1];
        data[i-1] = data[i];
        data[i] = tmp;
        flag = true;
      }
    }
    if (!flag) break;
    end --;
    start ++;
  }
}

int* SortAlgo::chooseSortFunc() {
  auto tmp = data[0];
  for (int i = 0; i < len -1; i++) {
    int min_index = i;
    for (int j = i+1; j < len; j++) {
      if (data[j] <  data[min_index])
        min_index = j;
    }
    if (min_index != i) {
      tmp = data[i];
      data[i] = data[min_index];
      data[min_index] = tmp;
    }
  }
}



string SortAlgo::getAlgoName(Algo algo) {
  string name;
  switch(algo) {
  case Algo::insertSort:
	  name = "insertSort";
	  break;
  case Algo::insertSortDichotomy:
    name = "insertSortDichotomy" ;
    break;
	case Algo::bubbleSort:
	  name = "bubbleSort";
	  break;
  case Algo::cocktailSort:
    name = "cocktailSort";
    break;
  case Algo::chooseSort:
    name = "chooseSort";
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
  case Algo::insertSort:
    insertSortFunc();
	  break;
  case Algo::insertSortDichotomy:
    insertSortDichotomyFunc();
    break;
  case Algo::bubbleSort:
    bubbleSortFunc();
	  break;
  case Algo::cocktailSort:
    cocktailSortFunc();
    break;
  case Algo::chooseSort:
    chooseSortFunc();
    break;
  default:
     std::cout<<"not support yet!!" << std::endl;
  
  }
  auto t_e = std::chrono::high_resolution_clock::now();
  std::cout << getAlgoName(algo) << "time:" <<
  std::chrono::duration_cast<std::chrono::microseconds>(t_e-t_s).count() / 1000. << "ms" <<std::endl;
  if (isCheckAccuracy){
    std::cout<< getAlgoName(algo) << "content:  ";
    printContent();
  }
  resetData();
}

int main(int argc, char **argv)
{
  --argc; ++argv;

  while(argc > 0) {
    if (!strcmp("-p", argv[0])) isCheckAccuracy = false;
    else if (!strcmp("-c", argv[0])) isCheckAccuracy = true;
    else break;
    --argc;
    ++argv;
  }
  
  //int cpu_num = port::NumSchedulableCPUs();
  SortAlgo s;
  if (isCheckAccuracy)
    s.setDataLen(100);
  else 
    s.setDataLen(100000);
  s.generateRandom();
  if (isCheckAccuracy)
    s.printContent();
  s.benchmark(SortAlgo::Algo::insertSort);
  s.benchmark(SortAlgo::Algo::insertSortDichotomy);
  s.benchmark(SortAlgo::Algo::bubbleSort);
  s.benchmark(SortAlgo::Algo::cocktailSort);
  s.benchmark(SortAlgo::Algo::chooseSort);
  return 0;
}
