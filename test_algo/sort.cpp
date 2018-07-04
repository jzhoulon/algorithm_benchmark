#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <string.h>
#include <algorithm>
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
 topMergeSort,
 bottomMergeSort,
 quickSort,
 std_quickSort,
 bucketSort,
};

 int* insertSortFunc();
 int* insertSortDichotomyFunc();
 int* bubbleSortFunc();
 int* cocktailSortFunc();
 int* chooseSortFunc();
 int* topMergeSortFunc();
 int* bottomMergeSortFunc();
 int* quickSortFunc();
 int* std_quickSortFunc();
 int* bucketSortFunc();
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
 int seedLimit;
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
  this->seedLimit = 1000;
}

void SortAlgo::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(0, seedLimit);
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

void mergeArray(int a[], int first, int mid, int last, int tmp[]) {
   int i  =  first;
   int j = mid+1;
   int k = 0;
   while(i <=mid && j <=last){
     if (a[i] < a[j])
       tmp[k++] = a[i++];
     else
       tmp[k++] = a[j++];
   }
   while(i<=mid)
     tmp[k++] = a[i++];
   while(j<=last) 
     tmp[k++] = a[j++];
   memcpy(a+first, tmp, (last - first+1)*sizeof(a[0]));
}

void mergeSortImpl(int a[], int first, int last, int tmp[]) {
  int mid = (first+last)/2 ;
  if (first>=last)
    return;
  mergeSortImpl(a, first, mid, tmp);
  mergeSortImpl(a, mid+1, last, tmp);
  mergeArray(a, first, mid, last, tmp);
  return;
}



int* SortAlgo::topMergeSortFunc() {
  int *p = new int[len];
  mergeSortImpl(data, 0, len -1, p);
  delete[] p;
}

int* SortAlgo::bottomMergeSortFunc() {
 int *tmp = new int[len];
 for (int width = 1; width < len; width = width*2) {
  for(int i = 0; i < len; i += width*2) {
    int low = i;
    int mid = low+width -1;
    int high = mid + width > len-1? len-1:mid + width;
    mergeArray(data, low, mid, high, tmp+low);
  }
 }
 delete [] tmp;
}


int partition(int a[], int low, int high) {
  int pivot = a[low];
  int tmp;
  int i = high;
  for (int j = high; j >low; j--) {
    if (a[j]>pivot) {
      tmp = a[i];
      a[i] = a[j];
      a[j] = tmp;
      i--;
    }
  }
  tmp = a[i];
  a[i] = a[low];
  a[low] = tmp;
  return i;
}
void quickSortImpl(int a[], int low, int high) {
  if (high-low < 1) {
    return;
  }
  int p = partition(a, low, high);
  quickSortImpl(a, low, p-1);
  quickSortImpl(a, p+1, high);
}

int* SortAlgo::quickSortFunc() {
  quickSortImpl(data, 0, len-1);
}

int* SortAlgo::std_quickSortFunc() {
  std::sort(data, data+len);
}

int* SortAlgo::bucketSortFunc() {
  int* bucket = new int[seedLimit+1];
  memset(bucket, 0, seedLimit);
  for (int i = 0; i< len; i++)
      bucket[data[i]]++;
  int j = 0;
  for (int i = 0; i <=seedLimit; i++)
    while(bucket[i]>0){
      data[j++] = i;
      bucket[i]--;
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
  case Algo::topMergeSort:
    name = "topMergeSort";
    break;
  case Algo::bottomMergeSort:
    name = "bottomMergeSort";
    break;
  case Algo::quickSort:
    name = "quickSort";
    break;
  case Algo::std_quickSort:
    name = "std_quickSort";
    break;
  case Algo::bucketSort:
    name = "bucketSort";
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
  case Algo::topMergeSort:
    topMergeSortFunc();
    break;
  case Algo::bottomMergeSort:
    bottomMergeSortFunc();
    break;
  case Algo::quickSort:
    quickSortFunc();
    break;
  case Algo::std_quickSort:
    std_quickSortFunc();
    break;
  case Algo::bucketSort:
    bucketSortFunc();
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
#if 0
  s.benchmark(SortAlgo::Algo::insertSort);
  s.benchmark(SortAlgo::Algo::insertSortDichotomy);
  s.benchmark(SortAlgo::Algo::bubbleSort);
  s.benchmark(SortAlgo::Algo::cocktailSort);
  s.benchmark(SortAlgo::Algo::chooseSort);
  s.benchmark(SortAlgo::Algo::topMergeSort);
#endif
  s.benchmark(SortAlgo::Algo::bottomMergeSort);
  s.benchmark(SortAlgo::Algo::quickSort);
  s.benchmark(SortAlgo::Algo::std_quickSort);
  s.benchmark(SortAlgo::Algo::bucketSort);
  return 1;
}
