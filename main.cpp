#include "matrix.h"
#include <chrono>
#include <iostream>

#define MATRIX_TRANSPOSE_TEST

int main() {

#ifdef MATRIX_MULTIPLY_TEST
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

  std::cout << '\n';
  result->print();

#endif // MATRIX_MULTIPLY_TEST

#ifdef MATRIX_TRANSPOSE_TEST
  const int columns = 2000;
  const int rows = 4500;
  Matrix *a = new Matrix(rows, columns);
  Matrix *result = new Matrix(columns, rows);

  a->set_all(1);
  a->set_value(1, 1, 2);
  a->set_value(2, 2, 2);
  a->set_value(3, 3, 2);

  auto start_time = std::chrono::high_resolution_clock::now();
  Matrix::transpose(a, result);
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
      end_time - start_time);
  std::cout << "sequential transpose: " << duration.count() / 1000000.f
            << ".ms\n";

  auto start_time2 = std::chrono::high_resolution_clock::now();
  Matrix::parallel_transpose(a, result);
  auto end_time2 = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time2 -
                                                                  start_time2);
  std::cout << "\n parallel " << duration.count() / 1000000.f << ".ms\n";

#endif // MATRIX_TRANSPOSE_TEST

  return 0;
}
