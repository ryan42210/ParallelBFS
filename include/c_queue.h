#ifndef C_QUEUE_H
#define C_QUEUE_H

#include <vector>
#include <string.h>

// template <typename T>
// class Queue {
//  public:
//   Queue() = default;

//   Queue(int max_v_num) { init(max_v_num); }

//   void init(int max_v_num) {
//     queue_.resize(max_v_num);
//     head_ = 0;
//     tail_ = 0;
//     cap_ = max_v_num;
//   }

//   void reset() {
//     head_ = 0;
//     tail_ = 0;
//   }

//   T front() { return queue_[head_]; }

//   void pop() { head_ = (head_ + 1) % cap_; }

//   void push(const T& item) {
//     queue_[tail_] = item;
//     tail_ = (tail_ + 1) % cap_;
//   }

//   bool empty() { return head_ == tail_; }

//   int get_head() { return head_; }

//   int get_cap() { return cap_; }

//   int get_size() {
//     int dist = tail_ - head_;
//     if (dist >= 0) return dist;
//     else {
//       return cap_ + dist;
//     }
//   }

//   T& operator[](int i) {
//     return queue_[i];
//   }

//  private:
//   std::vector<T> queue_;
//   int head_, tail_;
//   int cap_;
// };

template <typename T>
class AtomQueue {
 public:
  AtomQueue() = default;

  void push(T i) {
    queue_.push_back(i);
    tail_++;
  }

  int size() {
    return queue_.size();
  }

  int empty() {
    return queue_.empty();
  }

  T operator[](int i) {
    return queue_[i];
  }

  void clear() {
    queue_.clear();
    tail_ = 0;
  }

 private:
  std::vector<T> queue_;
  int tail_;
};


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