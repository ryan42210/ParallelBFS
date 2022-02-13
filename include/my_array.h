#ifndef C_QUEUE_H
#define C_QUEUE_H

#include <vector>
#include <string.h>

class Array {
 public:
  Array(int cap) {
    data_.resize(cap, 0);
    // p_data = new int[cap];
    capacity = cap;
    size_ = 0;
  }

  void copy_to(std::vector<int> &input, int start_idx, int size) {
    // for (int i = 0; i < size; i++) {
    //   data_[i + start_idx] = input[i];
    // }
    memcpy(&data_[start_idx], input.data(), size * sizeof(int));
  }

  int size() {
    return size_;
  }

  void set_size(int new_size) {
    size_ = new_size;
  }

  int operator[](int id) {
    // return p_data[id];
    return data_[id];
  }

  int get(int id) {
    return data_[id];
  }

  void push(int item) {
    // p_data[size_++] = item;
    int idx = __sync_fetch_and_add(&size_, 1);
    data_[idx] = item;
  }

  bool empty() {
    return size_ == 0;
  }

  void clear() {
    size_ = 0;
  }
 private:
  std::vector<int> data_;
  int capacity;
  int size_;
};


#endif