#include <iostream>
#include <algorithm>


class BaseOp {
  generateRandom();
  resetData();

protect:
  int* data_orig_;
  int* data_;
  int len_;
  int seedLimit_;
}

class Query {
public:

private:
};

void Query::generateRandom() {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> dis(0, seedLimit);
  auto random = std::bind(dis, generator);
  for (int i = 0; i < len; i++)
    data_orig[i] = random();
  resetData()
}

int main()
{

}
