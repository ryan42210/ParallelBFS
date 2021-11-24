#ifndef TEST_H
#define TEST_H

#include <chrono>
#include <vector>
#include <cmath>

#include "graph.h"

#define TRIAL_NUM 20


class Test {
 public:
  Test() = default;

  void set_graph(Graph &g) {
    graph_ = g;
    edge_cnt.resize(TRIAL_NUM, 0);
  }

  void gen_source() {
    for (int i = 0; i < TRIAL_NUM ; i++) {
      source_.push_back(rand() % graph_.get_vertex_num());
      // source_.push_back(i+1);
    }
  }

  void test_single_thread() {
    int64_t total_time = 0;
    int64_t total_edge_num = 0;
    for (int i = 0; i < TRIAL_NUM; i++) {
      int source = source_[i];
      auto start = std::chrono::steady_clock::now();
      edge_cnt[i] = graph_.sequencial_bfs(source);
      auto end = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      time_.push_back(duration.count());

      graph_.reset();
    }
  }

  void test_parallel(int th_num) {
    graph_.reset();
    time_.clear();
    int64_t total_time = 0;
    int64_t total_edge_num = 0;
    for (int i = 0; i < TRIAL_NUM; i++) {
      int source = source_[i];
      auto start = std::chrono::steady_clock::now();
      graph_.parallel_bfs_local_q2(source, th_num);
      auto end = std::chrono::steady_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
      time_.push_back(duration.count());

      graph_.reset();
    }
  }

  void print_result() {
    int64_t time = 0;
    std::vector<float> mteps_v(TRIAL_NUM, 0);
    for (int i = 0; i < TRIAL_NUM; i++) {
      if (time_[i] < 1) time_[i] = 1;
      time += time_[i];
      // mteps_v[i] = (double)graph_.get_edge_num() / time_[i];
      mteps_v[i] = (double)edge_cnt[i] / time_[i];
    }

    float avg_mteps = 0;
    int cnt = 0;
    for (int i = 0; i < TRIAL_NUM; i++) {
      if (mteps_v[i] < 5 || mteps_v[i] > 10000) continue;
      cnt++;
      avg_mteps += mteps_v[i];
      // printf(" %f |", mteps_v[i]);
    }
    // printf("\n");
    avg_mteps /= cnt;

    cnt = 0;
    float std_deviation = 0;
    for (int i = 0; i < TRIAL_NUM; i++) {
      float diff = (float)mteps_v[i] - avg_mteps;
      if (diff > 100 || diff < -100) continue;
      std_deviation += diff * diff;
      cnt++;
    }
    std_deviation /= cnt;;
    std_deviation = std::sqrt(std_deviation);

    printf(" %10ld | %8.2f | %7.3f \n", time, avg_mteps, std_deviation);

  }
 private:
  Graph graph_;
  std::vector<int64_t> edge_cnt;
  std::vector<int> source_;
  std::vector<int64_t> time_;
};

#endif