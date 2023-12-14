#include "matrix.h"
#include <chrono>
#include <iostream>

int main() {
  const int matrix_size = 200;
  Matrix *a = new Matrix(matrix_size, matrix_size);
  Matrix *b = new Matrix(matrix_size, matrix_size);

  a->set_all(1);
  b->set_all(1);

  Matrix *result = new Matrix(matrix_size, matrix_size);

  Matrix *result_parallel = new Matrix(matrix_size, matrix_size);

  auto start_time = std::chrono::high_resolution_clock::now();
  Matrix::multiply(a, b, result);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);
  std::cout << "sequential " << duration.count() / 1000000.f << ".ms\n";

  auto start_time2 = std::chrono::high_resolution_clock::now();
  Matrix::parallel_multiply(a, b, result);
  auto end_time2 = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 -
                                                                  start_time2);
  std::cout << "\n parallel " << duration.count() / 1000000.f << ".ms\n";

  return 0;
}
