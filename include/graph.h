#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <iostream>
#include <atomic>
#include <omp.h>

#include "c_queue.h"


struct Result {
  Result() {
    parent_ = -1;
    dist_ = -1;
  }
  int parent_;
  int dist_;
};


class Graph {
 public:
  Graph() = default;

  explicit Graph(int num_v) {
    init(num_v);
  }

  void set_edge_num(int64_t edge_num) {
    edge_num_ = edge_num;
  }

  int64_t get_edge_num() {
    return edge_num_;
  }

  void init(int num_v) {
    graph_.resize(num_v+1);
    result_.resize(num_v+1);
    // visited_.resize(num_v+1, false);
    num_vertex_ = num_v;
    connected_v_num_ = 0;
  }
  // clear result and ready for next bfs
  void reset() {
    result_.clear();
    result_.resize(num_vertex_ + 1);
    // visited_.resize(num_vertex_+1, false);
    connected_v_num_ = 0;
  }

  int get_connected_count() {
    if (connected_v_num_ == 0) {
      for (auto &res : result_) {
        if (res.dist_ != -1) connected_v_num_++;
      }
    }
    return connected_v_num_;
  }

  int get_vertex_num() {
    return num_vertex_;
  }

  void add_edge(int from, int to) {
    graph_[from].push_back(to);
  }

  void print_result() {
    for (int i = 1; i < result_.size(); i++) {
      if (result_[i].dist_ != -1) {
        printf("node:%d, dist: %d, parent:%d\n", i, result_[i].dist_, result_[i].parent_);
      }
    }
  }

  void out_to_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    for (int i = 1; i < num_vertex_; i++) {
      if (result_[i].dist_ != -1)
        fprintf(f, "%d %d %d\n", i, result_[i].dist_, result_[i].parent_);
    }
  }

  int64_t sequencial_bfs(int source) {
    std::queue<int> fifo;
    fifo.push(source);
    result_[source].dist_ = 0;
    result_[source].parent_ = source;
    int64_t edge_cnt = 0;
    while (!fifo.empty()) {
      int current = fifo.front();
      // add neighbour of current vertex into queue
      for (auto &neighbour : graph_[current]) {
        edge_cnt++;
        if (result_[neighbour].dist_ == -1) {
          fifo.push(neighbour);
          result_[neighbour].dist_ = result_[current].dist_ + 1;
          result_[neighbour].parent_ = current;
        }
      }
      connected_v_num_++;
      fifo.pop();
    }
    return edge_cnt;
  }


  void parallel_bfs_local_q(int source, int th_num) {
    omp_set_num_threads(th_num);
    std::vector<std::vector<int>> local_q;
    local_q.resize(th_num);
    
    Array g_list(num_vertex_);

    g_list.push(source);
    result_[source].dist_ = 0;
    result_[source].parent_ = source;
    int dist = 0;
    std::atomic<unsigned int> g_counter;
    while (!g_list.empty()) {
      g_counter.store(0);
      #pragma omp parallel default(shared)
      {
        int tid = omp_get_thread_num();
        for (int i = tid; i < g_list.size(); i += th_num) {
          int curr_node = g_list[i];
          for (auto &neighbour : graph_[curr_node]) {
            if (result_[neighbour].dist_ == -1) {
              local_q[tid].push_back(neighbour);
              result_[neighbour].parent_ = curr_node;
              result_[neighbour].dist_ = dist + 1;
            }
          }
        }
        // combine all local_q
        int cnt = local_q[tid].size();
        unsigned int start_idx = g_counter.fetch_add(cnt);
        
        #pragma omp barrier
        if (cnt > 0) {
          g_list.copy_to(local_q[tid], start_idx, cnt);
          local_q[tid].clear();
        }
        if(local_q[tid].capacity() > num_vertex_ / th_num) local_q[tid].shrink_to_fit();
      }
      g_list.set_size(g_counter);
      dist++;
    }
  }

  void parallel_bfs_local_q2(int source, int th_num) {
    omp_set_num_threads(th_num);
    std::vector<std::vector<int>> local_q;
    local_q.resize(th_num);
    
    Array g_list(num_vertex_);

    g_list.push(source);
    result_[source].dist_ = 0;
    result_[source].parent_ = source;
    int dist = 0;
    std::atomic<unsigned int> g_counter;
    while (!g_list.empty()) {
      g_counter.store(0);
      #pragma omp parallel for default(shared) schedule(dynamic, 4)
      for (int i = 0; i < g_list.size(); i++) {
        int tid = omp_get_thread_num();
        int curr_node = g_list[i];
        for (auto &neighbour : graph_[curr_node]) {
          if (result_[neighbour].dist_ == -1) {
            local_q[tid].push_back(neighbour);
            result_[neighbour].parent_ = curr_node;
            result_[neighbour].dist_ = dist + 1;
          }
        }
      }
      #pragma omp parallel default(shared)
      {
        // combine all local_q
        int tid = omp_get_thread_num();
        int cnt = local_q[tid].size();
        unsigned int start_idx = g_counter.fetch_add(cnt);

        if (cnt > 0) {
          g_list.copy_to(local_q[tid], start_idx, cnt);
          local_q[tid].clear();
        }
        if(local_q[tid].capacity() > num_vertex_ / th_num) local_q[tid].shrink_to_fit();
      }
      g_list.set_size(g_counter);
      dist++;
    }
  }


 private:
  std::vector<std::vector<int>> graph_;
  std::vector<Result> result_;
  int64_t edge_num_;
  int num_vertex_;
  int connected_v_num_;
};

#endif