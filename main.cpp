#include <iostream>

#include "include/graph.h"
#include "include/io.h"
#include "include/test.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "input graph not specified\n");
    exit(1);
  }

  bool is_rmat = false;
  bool is_mm = false;
  if (argc >= 3) {
    if (argv[2] == "-h")
      is_rmat = true;
    else if (argv[2] == "-m")
      is_mm = true;
  }

  const char* filename = argv[1];

  Graph g;
  if (is_mm) {
    read_mm(filename, g);
  } else {
    read_edge(filename, g, is_rmat);
  }

  Test test;
  test.set_graph(g);
  test.gen_source();

  test.test_single_thread();
  test.print_result();
  // g.out_to_file("test.txt");

  int max_thread_num = 16;

  for (int th_num = 1; th_num <= max_thread_num; th_num++) {
    printf("=============== th_num = %d ==================\n", th_num);
    printf(" time(us) |  MTEPS  |  std_dv\n");
    for (int i = 0; i < 3; i++) {
      test.test_parallel(th_num);
      test.print_result();
    }
  }
  
  return 0;
}