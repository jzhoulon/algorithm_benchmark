#include <iostream>
#include <random>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <functional>
using namespace std;
const int cacheLineWidth = 16;//byte
bool isCheckAccuracy;
class Matrix {
public:
  Matrix();
  ~Matrix();
  enum method{
    matrixSingleThread,
    matrixOmpThread,
    matrixOmpThread2,
    matrixOmpThreadBlock,
  };
  void matrixSingleThreadFunc();
  void matrixOmpThreadFunc();
  void matrixOmpThreadFunc2();
  void matrixOmpThreadBlockFunc();
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
    destroyMatrix(input2_, k_len_);
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
void Matrix::setDataLen(int m, int k, int n) {

  input1_ = createMatrix(m, k);
  input2_ = createMatrix(k, n);
  output_ = createMatrix(m, n);
  m_len_ = m;
  n_len_ = n;
  k_len_ = k;
}

void Matrix::generateRandom() {
  std::default_random_engine generator;
  std::uniform_real_distribution<float> dis(0., 255.0);
  auto random = std::bind(dis, generator);
  if (isCheckAccuracy) {
      for (int i = 0; i < m_len_; i++)
       for (int j = 0; j < k_len_; j++) {
         input1_[i][j] = i;
       }
     for (int i = 0; i < k_len_; i++) 
       for (int j = 0; j < n_len_; j++) {
         input2_[i][j] = j;
       } 
   } else {
    for (int i = 0; i < m_len_; i++)
      for (int j = 0; j < k_len_; j++) {
        input1_[i][j] = random();
      }
    for (int i = 0; i < k_len_; i++) 
      for (int j = 0; j < n_len_; j++) {
        input2_[i][j] = random();
      }
    }
}

void Matrix::printContent() {
  std::cout << "input1:" << std::endl;
    for (int i = 0; i < m_len_; i++) {
      for (int j = 0; j < k_len_; j++) {
        std::cout << input1_[i][j]<<" ";
      }
      std::cout << std::endl;
    }
  std::cout << "input2:" << std::endl;
    for (int i = 0; i < k_len_; i++) {
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
    case matrixOmpThread2:
        name = "matrixOmpThread2";
        break;
    case matrixOmpThreadBlock:
        name = "matrixOmpThreadBlock";
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
    case matrixOmpThread2:
        matrixOmpThreadFunc2();
        break;
    case matrixOmpThreadBlock:
        matrixOmpThreadBlockFunc();
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
for (int i = 0; i < m_len_; i++)
  for (int j = 0; j < n_len_; j++) {
    output_[i][j] = 0.0;
  }
  for (int i = 0; i < m_len_; i++) {
    for (int j = 0; j < n_len_; j++) {
      for (int k = 0;  k< k_len_; k++) {
        output_[i][j] += input1_[i][k] * input2_[k][j];
      }
    }
  }
}

void Matrix::matrixOmpThreadFunc() {
for (int i = 0; i < m_len_; i++)
  for (int j = 0; j < n_len_; j++) {
    output_[i][j] = 0.0;
 } 
#pragma omp parallel for collapse(2) schedule(static) 
  for (int i = 0; i < m_len_; i++) {
    for (int j = 0; j < n_len_; j++) {
      for (int k = 0;  k< k_len_; k++) {
        output_[i][j] += input1_[i][k] * input2_[k][j];
      }
    }
  }
}

void Matrix::matrixOmpThreadFunc2() {
for (int i = 0; i < m_len_; i++)
  for (int j = 0; j < n_len_; j++) {
    output_[i][j] = 0.0;
  }
#pragma omp parallel for collapse(2) schedule(static) 
  for (int i = 0; i < m_len_; i++) {
    for (int k = 0; k < k_len_; k++) {
      for (int j = 0;  j< n_len_; j++) {
        output_[i][j] += input1_[i][k] * input2_[k][j];
      }
    }
  }
}


void Matrix::matrixOmpThreadBlockFunc() {
  for (int i = 0; i < m_len_; i++)
    for (int j = 0; j < n_len_; j++)
      output_[i][j] = 0.0;

  const int block_size = 16;//cache line size = 64byte = 16 float
 
  int n_blen=  block_size* (n_len_ / block_size);
  int n_rest = n_len_ % block_size;
  int k_blen = block_size * (k_len_ / block_size);
  int k_rest = k_len_ % block_size;
#pragma omp parallel for collapse(2)  schedule(static) 
  for (int i = 0; i < m_len_; i++){
      for (int k = 0; k < k_blen; k+= block_size) {
      for (int j = 0; j < n_blen; j+= block_size)
        for (int kk = k; kk < k+block_size; kk++) {
           output_[i][j] += input1_[i][kk] * input2_[kk][j];
           output_[i][j+1] += input1_[i][kk] * input2_[kk][j+1];
           output_[i][j+2] += input1_[i][kk] * input2_[kk][j+2];
           output_[i][j+3] += input1_[i][kk] * input2_[kk][j+3];
           output_[i][j+4] += input1_[i][kk] * input2_[kk][j+4];
           output_[i][j+5] += input1_[i][kk] * input2_[kk][j+5];
           output_[i][j+6] += input1_[i][kk] * input2_[kk][j+6];
           output_[i][j+7] += input1_[i][kk] * input2_[kk][j+7];
           output_[i][j+8] += input1_[i][kk] * input2_[kk][j+8];
           output_[i][j+9] += input1_[i][kk] * input2_[kk][j+9];
           output_[i][j+10] += input1_[i][kk] * input2_[kk][j+10];
           output_[i][j+11] += input1_[i][kk] * input2_[kk][j+11];
           output_[i][j+12] += input1_[i][kk] * input2_[kk][j+12];
           output_[i][j+13] += input1_[i][kk] * input2_[kk][j+13];
           output_[i][j+14] += input1_[i][kk] * input2_[kk][j+14];
           output_[i][j+15] += input1_[i][kk] * input2_[kk][j+15];
          }
        }
      }


#pragma omp parallel for 
  for (int i = 0; i < m_len_; i++) {
    for (int k = k_blen; k< k_len_; k++) {
      for (int j = 0; j < n_len_; j++) {
        output_[i][j] += input1_[i][k] * input2_[k][j];
      }
    }
  }

#pragma omp parallel for 
  for (int i = 0; i < m_len_; i++) {
    for (int k = 0; k < k_blen; k++) {
      for (int j = n_blen; j < n_len_; j++) {
        output_[i][j] += input1_[i][k] * input2_[k][j];
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
    m.setDataLen(18, 18, 20);
  else
    m.setDataLen(10000, 2000, 1000);
  m.generateRandom();
  if (isCheckAccuracy)
    m.printContent();
 // m.benchmark(Matrix::method::matrixSingleThread); 
 // m.benchmark(Matrix::method::matrixOmpThread); 
  m.benchmark(Matrix::method::matrixOmpThread2);
  m.benchmark(Matrix::method::matrixOmpThreadBlock);
}

