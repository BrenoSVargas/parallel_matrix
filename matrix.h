#include <algorithm>
#include <cstddef>
#include <iostream>
#include <thread>
#include <threads.h>
#include <vector>

class Matrix {
  int *data;
  int rows;
  int columns;

public:
  Matrix(int n, int m) : rows(n), columns(m) {
    data = new int[rows * columns];
    std::fill(data, data + rows * columns, 0);
  }

  void set_value(int line_index, int column_index, int value) {
    data[line_index * columns + column_index] = value;
  }

  void set_all(int value) { std::fill(data, data + rows * columns, value); }

  void print() const {
    int length = rows * columns;
    for (size_t i = 0; i < length; i++) {
      std::cout << data[i] << " ";

      if ((i + 1) % columns == 0) {
        std::cout << "\n";
      }
    }
  }

  static void multiply(Matrix *x, Matrix *y, Matrix *result) {
    if (!(x->columns == y->rows) ||
        !((x->rows == result->rows) && (y->columns == result->columns))) {
      std::cout << "Error: unable to multiply incompatible Matrixes\n";
    }

    int result_length = result->rows * result->columns;

    for (size_t i = 0; i < result_length; i++) {
      for (size_t j = 0; j < x->columns; j++) {
        result->data[i] += x->data[(i / result->columns) + j] *
                           y->data[i % result->rows + j * y->columns];
      }
    }
  }

  static void parallel_multiply(Matrix *x, Matrix *y, Matrix *result) {
    if (!(x->columns == y->rows) ||
        !((x->rows == result->rows) && (y->columns == result->columns))) {
      std::cout << "Error: unable to parallel multiply incompatible Matrixes\n";
    }

    struct process_data_chunk {
      void operator()(Matrix *x, Matrix *y, Matrix *result, int start_index,
                      int end_index) {
        for (size_t i = start_index; i < end_index; i++) {
          for (size_t j = 0; j < x->columns; j++) {
            result->data[i] += x->data[(i / result->columns) + j] *
                               y->data[i % result->rows + j * y->columns];
          }
        }
      }
    };

    int result_length = result->rows * result->columns;

    if (!result_length) {
      return;
    }

    int min_per_thread = 1000;
    int max_threads = (result_length + min_per_thread - 1) / min_per_thread;
    int hardware_threads = std::thread::hardware_concurrency();
    int num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    int block_size = result_length / num_threads;

    std::vector<std::thread> threads(num_threads);
    int block_start = 0;
    int block_end = 0;

    {
      for (unsigned long i = 0; i < num_threads; i++) {
        block_start = i * block_size;
        block_end = block_start + block_size;
        threads[i] = std::thread(process_data_chunk(), x, y, result,
                                 block_start, block_end);
      }

      for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
      }

      process_data_chunk()(result, x, y, block_end, result_length);
    }
  }

  static void transpose(Matrix *x, Matrix *result) {
    if (!((x->columns == result->rows) && x->rows == result->columns)) {
      std::cout << "Error: invalid matrix sizes for transpose.\n";
    }

    int result_length = result->rows * result->columns;

    int result_row = 0;
    int result_column = 0;

    int input_row = 0;
    int input_column = 0;

    for (size_t i = 0; i < result_length; i++) {
      result_row = i / result->columns;
      result_column = i % result->columns;

      input_row = result_column;
      input_column = result_row;

      result->data[i] = x->data[input_row * x->columns + input_column];
    }
  }

  static void parallel_transpose(Matrix *x, Matrix *result) {
    if (!((x->columns == result->rows) && x->rows == result->columns)) {
      std::cout << "Error: invalid matrix sizes for transpose.\n";
    }

    struct process_data_chunk {
      void operator()(Matrix *x, Matrix *result, int start_index,
                      int end_index) {
        int result_row = 0;
        int result_column = 0;

        int input_row = 0;
        int input_column = 0;

        for (size_t i = start_index; i < end_index; i++) {
          result_row = i / result->columns;
          result_column = i % result->columns;

          input_row = result_column;
          input_column = result_row;

          result->data[i] = x->data[input_row * x->columns + input_column];
        }
      }
    };

    int result_length = result->rows * result->columns;

    if (!result_length) {
      return;
    }

    int min_per_thread = 10000;
    int max_threads = (result_length + min_per_thread - 1) / min_per_thread;
    int hardware_threads = std::thread::hardware_concurrency();
    int num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    int block_size = result_length / num_threads;

    std::vector<std::thread> threads(num_threads);
    int block_start = 0;
    int block_end = 0;

    {
      for (unsigned long i = 0; i < num_threads; i++) {
        block_start = i * block_size;
        block_end = block_start + block_size;
        threads[i] = std::thread(process_data_chunk(), x, result,
                                 block_start, block_end);
      }

      for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
      }

      process_data_chunk()(result, x, block_end, result_length);
    }
  }
};
