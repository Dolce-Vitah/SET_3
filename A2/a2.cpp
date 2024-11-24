#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

void merge(std::vector<int> &container, int left, int middle, int right) {
  std::vector<int> temp(right - left + 1);

  int i = left;
  int j = middle + 1;
  int k = 0;

  while (i <= middle && j <= right) {
    if (container[i] <= container[j]) {
      temp[k++] = container[i++];
    } else {
      temp[k++] = container[j++];
    }
  }

  while (i <= middle) {
    temp[k++] = container[i++];
  }

  while (j <= right) {
    temp[k++] = container[j++];
  }

  for (int i = 0; i < k; i++) {
    container[left + i] = temp[i];
  }
}

void insertion_sort(std::vector<int> &container, int left, int right) {
  for (int i = left + 1; i < right + 1; i++) {
    int key = container[i];
    int j = i - 1;

    while (j >= left && container[j] > key) {
      container[j + 1] = container[j];
      j--;
    }
    container[j + 1] = key;
  }
}

void merge_insert_sort(std::vector<int> &container, int left, int right,
                       int threshold) {
  if (right - left + 1 <= threshold) {
    insertion_sort(container, left, right);
  } else {
    int middle = left + (right - left) / 2;
    merge_insert_sort(container, left, middle, threshold);
    merge_insert_sort(container, middle + 1, right, threshold);

    merge(container, left, middle, right);
  }
}

void merge_sort(std::vector<int> &container, int left, int right) {
  if (right <= left) {
    return;
  }
  int middle = left + (right - left) / 2;
  merge_sort(container, left, middle);
  merge_sort(container, middle + 1, right);

  merge(container, left, middle, right);
}

class SortTester {
public:
  double test_merge_sort(std::vector<int> &test_array, int trials) {

    std::int64_t total_time = 0;

    for (int trial = 0; trial < trials; trial++) {
      total_time += measure_execution_time(
          [&](std::vector<int> &a) { merge_sort(a, 0, a.size() - 1); },
          test_array);
    }

    return total_time / (double)trials;
  }

  double test_hybrid_merge_insertion_sort(std::vector<int> &test_array,
                                          int trials) {
    std::int64_t total_time = 0;
    const int threshold = 20;

    for (int trial = 0; trial < trials; trial++) {
      total_time += measure_execution_time(
          [&](std::vector<int> &a) {
            merge_insert_sort(a, 0, a.size() - 1, threshold);
          },
          test_array);
    }

    return total_time / (double)trials;
  }

private:
  template <typename Function>
  std::int64_t measure_execution_time(Function sort_function,
                                      std::vector<int> &container) {

    std::vector<int> temp = container;

    auto start = std::chrono::high_resolution_clock::now();
    sort_function(temp);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
        .count();
  }
};

class ArrayGenerator {
public:
  ArrayGenerator(int size = 10000, int max_value = 6000)
      : size_(size), max_value_(max_value), generator(std::random_device{}()) {}

  std::vector<int> generate_random_array() {
    std::vector<int> test_array(size_);
    std::uniform_int_distribution<> dis(0, max_value_);

    for (int &value : test_array) {
      value = dis(generator);
    }
    return test_array;
  }

  std::vector<int> generate_reversed_array() {
    std::vector<int> test_array = generate_random_array();
    std::sort(test_array.begin(), test_array.end(), std::greater<int>());

    return test_array;
  }

  std::vector<int> generate_nearly_sorted_array() {
    std::vector<int> test_array = generate_random_array();
    std::sort(test_array.begin(), test_array.end());

    int swaps = static_cast<int>(0.005 * size_);
    for (int i = 0; i < swaps; i++) {
      int index_1 = rand() % size_;
      int index_2 = rand() % size_;
      std::swap(test_array[index_1], test_array[index_2]);
    }

    return test_array;
  }

private:
  int max_value_;
  int size_;
  std::mt19937 generator;
};

int main() {
  ArrayGenerator test = ArrayGenerator(10000, 6000);
  std::vector<int> data_set = test.generate_nearly_sorted_array();

  SortTester tester;

  for (auto iter = data_set.begin() + 500; iter != data_set.end() + 100;
       iter += 100) {
    std::vector<int> sliced_set(data_set.begin(), iter);

    double result = tester.test_merge_sort(sliced_set, 100);

    std::cout << result << std::endl;
  }
}