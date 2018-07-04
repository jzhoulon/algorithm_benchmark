#include <iostream>
#include <random>
#include <string.h>
#include <algorithm>
#include <chrono>
using namespace std;
const int cacheLineWidth = 16//byte
bool isCheckAccuracy;
class Matrix {
public:
  Matrix();
  ~Matrix();
  enum method{
    matrixSingleThread,
    matrixOmpThread,
  };
  void matrixSingleThreadFunc();
  void matrixOmpThreadFunc();
  void generateRandom();
  void setDataLen(int m, int n, int k);
  void benchmark(method mtd);
  void printContent();
private:
  string getMethodName(method mtd);
  float** createMatrix(int rows, int cols);
  void destroyMatrix(float** matrix, int rows);
  float** input1_;
  float** input2_;
  float** output_;
  int m_len_;
  int n_len_;
  int k_len_;
};

Matrix::Matrix() {

}

Matrix::~Matrix() {
    destroyMatrix(input1_, m_len_);
    destroyMatrix(input2_, n_len_);
    destroyMatrix(output_, m_len_);
}

float** Matrix::createMatrix(int rows, int cols) { 
  float ** matrix  = new float* [rows];
  for (int i = 0; i < rows; i++) {
    matrix[i] =  new float[cols];
  }
  return matrix;
}

void Matrix::destroyMatrix(float ** matrix, int rows) {
  if (matrix) {
    for (int i = 0; i < rows; i++)
        delete[] matrix[i];
    delete[] matrix;
    matrix = nullptr;
  }
}
void Matrix::setDataLen(int m, int n, int k) {

  input1_ = createMatrix(m, n);
  input2_ = createMatrix(n, k);
  output_ = createMatrix(m, k);
  m_len_ = m;
  n_len_ = n;
  k_len_ = k;
}

void Matrix::generateRandom() {
  std::default_random_engine generator;
  std::uniform_real_distribution <float> dis1(1.0, 1.0);
  std::uniform_real_distribution<float> dis2(0., 255.0);
  auto random = isCheckAccuracy? std::bind(dis1, generator)
                                : std::bind(dis2, generator);
  for (int i = 0; i < m_len_; i++)
    for (int j = 0; j < n_len_; j++) {
      input1_[i][j] = random();
    }
  for (int i = 0; i < n_len_; i++) 
    for (int j = 0; j < k_len_; j++) {
      input2_[i][j] = random();
    }
}

void Matrix::printContent() {
  std::cout << "input1:" << std::endl;
    for (int i = 0; i < m_len_; i++) {
      for (int j = 0; j < n_len_; j++) {
        std::cout << input1_[i][j]<<" ";
      }
      std::cout << std::endl;
    }
  std::cout << "input2:" << std::endl;
    for (int i = 0; i < m_len_; i++) {
      for (int j = 0; j < n_len_; j++) {
        std::cout << input2_[i][j]<<" ";
      }
      std::cout << std::endl;
    }
  std::cout << "output:" << std::endl;
    for (int i = 0; i < m_len_; i++) {
      for (int j = 0; j < n_len_; j++) {
        std::cout << output_[i][j]<<" ";
      }
      std::cout << std::endl;
    }

}

string Matrix::getMethodName(method mtd) {
  string name;
  switch(mtd) {
    case matrixSingleThread:
        name = "matrixSingleThread";
        break;
    case matrixOmpThread:
        name = "matrixOmpThread";
        break;
    default:
        name = "unsupported name";
  }
  return name;
}
void Matrix::benchmark(method mtd) {
  float time_us = 0;
  auto t_s = std::chrono::high_resolution_clock::now();
  switch (mtd){
    case matrixSingleThread:
        matrixSingleThreadFunc();
        break;
    case matrixOmpThread:
        matrixOmpThreadFunc();
        break;
    default:
        std::cout << "not supported yet"<<std::endl;
  }
  auto t_e = std::chrono::high_resolution_clock::now();
  time_us = std::chrono::duration_cast<std::chrono::microseconds>(t_e - t_s).count() / 1000.;
  std::cout << getMethodName(mtd) << "time: " << time_us<< "ms" << ", " <<
  m_len_ * n_len_ * k_len_ * 2/time_us/1000./1000.<<"flops"<< std::endl;
  if (isCheckAccuracy) {
    std::cout << getMethodName(mtd) << "content: ";
    printContent();
  }
}

void Matrix::matrixSingleThreadFunc() {
  for (int i = 0; i < m_len_; i++) {
    for (int j = 0; j < k_len_; j++) {
      output_[i][j] = 0.0;
      for (int n = 0;  n< n_len_; n++) {
        output_[i][j] += input1_[i][n] * input2_[n][j];
      }
    }
  }
}

void Matrix::matrixOmpThreadFunc() {
#pragma omp parallel for collapse(2) schedule(static) 
  for (int i = 0; i < m_len_; i++) {
    for (int j = 0; j < k_len_; j++) {
      output_[i][j] = 0.0;
      for (int n = 0;  n< n_len_; n++) {
        output_[i][j] += input1_[i][n] * input2_[n][j];
      }
    }
  }
}

void Matrix::matrixOmpThreadFunc2() {
#pragma omp parallel for collapse(2) schedule(static) 
  for (int i = 0; i < m_len_; i++) {
    for (int j = 0; j < k_len_; j+=cacheLineWidth) {
      output_[i][j] = 0.0;
      for (int n = 0;  n< n_len_; n++) {
        output_[i][j] += input1_[i][n] * input2_[n][j];
      }
    }
  }
}



int main(int argc, char** argv) {
  --argc, ++argv;

  while(argc > 0) {
    if (!strcmp("-p", argv[0])) isCheckAccuracy = false;
    else if (!strcmp("-c", argv[0])) isCheckAccuracy = true;
    else break;
    --argc;
    ++argv;
  }
  Matrix m;
  if (isCheckAccuracy)
    m.setDataLen(4, 4, 4);
  else
    m.setDataLen(10000, 2000, 1000);
  m.generateRandom();
  if (isCheckAccuracy)
    m.printContent();
 // m.benchmark(Matrix::method::matrixSingleThread); 
  m.benchmark(Matrix::method::matrixOmpThread); 
}

