#include "../../src/alg/sort.hpp"

int main()
{
  int cpu_num = port::NumSchedulableCPUs();
  SortAlgo s = SortAlgo(100000);
  s.generateRandom();
  //s.printContent();
  s.benchmark(SortAlgo::Algo::insertSort1);
  return 0;
}
