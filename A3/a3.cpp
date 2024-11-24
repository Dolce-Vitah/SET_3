#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int partition(std::vector<int> &container, int left, int right) {
  int pivot = container[right];
  int p_index = left;

  for (int i = left; i < right; i++) {
    if (container[i] <= pivot) {
      std::swap(container[i], container[p_index]);
      p_index++;
    }
  }

  std::swap(container[p_index], container[right]);

  return p_index;
}

int rand_partition(std::vector<int> &container, int left, int right) {
  int pivot_index = rand() % (right - left + 1) + left;

  std::swap(container[pivot_index], container[right]);

  return partition(container, left, right);
}

void heapify(std::vector<int> &container, int left, int N, int level) {

  int left_child = 2 * (level - left) + 1 + left;
  int right_child = 2 * (level - left) + 2 + left;
  int largest = level;

  if (left_child < N && container[largest] < container[left_child]) {
    largest = left_child;
  }

  if (right_child < N && container[largest] < container[right_child]) {
    largest = right_child;
  }

  if (largest != level) {
    std::swap(container[level], container[largest]);
    heapify(container, left, N, largest);
  }
}

void heap_sort(std::vector<int> &container, int left, int right) {
  int N = right - left + 1;

  for (int i = left + N / 2 - 1; i >= left; --i) {
    heapify(container, left, right + 1, i);
  }

  for (int i = right; i > left; --i) {
    std::swap(container[left], container[i]);
    heapify(container, left, i, left);
  }
}

void insertion_sort(std::vector<int> &container, int left, int right) {
  for (int i = left + 1; i <= right; i++) {
    int key = container[i];
    int j = i - 1;

    while (j >= left && container[j] > key) {
      container[j + 1] = container[j];
      j--;
    }
    container[j + 1] = key;
  }
}

void introsort(std::vector<int> &container, int left, int right,
               int recur_depth) {
  if (right - left + 1 <= 16) {
    insertion_sort(container, left, right);
  } else if (recur_depth == 0) {
    heap_sort(container, left, right);
  } else {
    int pivot = rand_partition(container, left, right);
    introsort(container, left, pivot - 1, recur_depth - 1);
    introsort(container, pivot + 1, right, recur_depth - 1);
  }
}

void quick_sort(std::vector<int> &container, int left, int right) {
  if (right <= left) {
    return;
  }
  int pivot = rand_partition(container, left, right);
  quick_sort(container, left, pivot - 1);
  quick_sort(container, pivot + 1, right);
}

class SortTester {
public:
  double test_quick_sort(std::vector<int> &test_array, int trials) {

    std::int64_t total_time = 0;

    for (int trial = 0; trial < trials; trial++) {
      total_time += measure_execution_time(
          [&](std::vector<int> &a) { quick_sort(a, 0, a.size() - 1); },
          test_array);
    }

    return total_time / (double)trials;
  }

  double test_introsort(std::vector<int> &test_array, int trials) {
    std::int64_t total_time = 0;
    const int recur_depth = 2 * static_cast<int>(std::log2(test_array.size()));

    for (int trial = 0; trial < trials; trial++) {
      total_time += measure_execution_time(
          [&](std::vector<int> &a) {
            introsort(a, 0, a.size() - 1, recur_depth);
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
  ArrayGenerator generator = ArrayGenerator();
  std::vector<int> data_set = generator.generate_reversed_array();

  SortTester tester;

  for (auto iter = data_set.begin() + 500; iter != data_set.end() + 100;
       iter += 100) {
    std::vector<int> sliced_set(data_set.begin(), iter);

    double result = tester.test_quick_sort(sliced_set, 100);

    std::cout << result << std::endl;
  }
}