#include <iostream>
#include <random>
#include <functional>
#include <chrono>
using namespace std;
class SortAlgo{
public:
SortAlgo(int length);
~SortAlgo();

enum Algo {
insertSort1,
insertSort2,
};

int* insertSortFunc1();
void generateRandom();
void printContent();
string getAlgoName(Algo algo);
void benchmark( Algo algo);

private:
int* data;
int len;
};


