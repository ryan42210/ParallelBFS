#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "mmio.h"

#include "graph.h"

#define IO_BUFFER_SIZE 20

int read_adj_list(const char *filename) {
  FILE* f = fopen(filename, "r");
  return 0;
}

int read_edge(const char *filename, Graph &g, bool is_rmat) {
  FILE* f = fopen(filename, "r");
  if (f == NULL) {
    printf("Fail to open file\n");
    exit(1);
  }
  if (is_rmat) {
    printf("rmat graph!\n");
    g.init(1e2);
    g.set_edge_num(1e3);
    int from, to;
    while(fscanf(f, "%d  %d\n", &from, &to) != EOF) {
      if (from != to) {
        g.add_edge(from, to);
      }
    }
  } else {
    char temp[1024];
    for (int i = 0; i < 2; i++) {
      char *ptr = fgets(temp, 1024, f);
    }
    int64_t nodes, edges;
    int res = fscanf(f, "# Nodes: %ld Edges: %ld\n", &nodes, &edges);
    g.set_edge_num(edges);
    printf("The graph has %ld nodes and %ld edges\n", nodes, edges);
    char * ptr = fgets(temp, 1024, f);
    g.init(nodes);
    char *buf = NULL;
    size_t size, read;
    while ((read = getline(&buf, &size, f)) != -1) {
      char *token_list = NULL;
      char *token = strtok_r(buf, "\t", &token_list);
      int from, to;
      from = (int)strtol(token, NULL, 10);
      token = strtok_r(NULL, " ", &token_list);
      to = (int)strtol(token, NULL, 10);
      if (to == from) continue;
      g.add_edge(from, to);
    }
    // int from, to;
    // while(fscanf(f, "%d\t%d\n", &from, &to) != EOF) {
    //   if (from != to) {
    //     g.add_edge(from, to);
    //   }
    // }
  }
  fclose(f);
  printf("Graph loaded...\n");
  return 0;
}

int read_mm(const char* filename, Graph &g) {
  int ret_code;
  MM_typecode matcode;
  FILE *f;
  if ((f = fopen(filename, "r")) == NULL) {
    printf("Fail to open file\n");
    exit(1);
  }

  if (mm_read_banner(f, &matcode) != 0) {
    printf("Could not process Matrix Market banner.\n");
    exit(1);
  }

  if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
      mm_is_sparse(matcode)) {
    printf("Sorry, this application does not support ");
    printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
    exit(1);
  }

  /* find out size of sparse matrix .... */
  int M, N, nz;
  if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) != 0) exit(1);
  if (M != N) {
    printf("Matrix is not square!\n");
    exit(1);
  }

  g.init(M);

  int from, to;
  float dist;
  for (int i = 0; i < nz; i++) {
    fscanf(f, "%d %d %g\n", &from, &to, &dist);
    g.add_edge(from, to);
  }

  if (f != stdin) fclose(f);
  return 0;
}


// int read() {
//   int num = 0;
//   char c = getchar();
//   while (c < '0' || c > '9') {
//     c = getchar();
//     if (c == EOF) return -1;
//   }
//   while (c > '0' || c < '9') {
//     num = (num << 1) + (num << 3) + c ^ 48;
//     c = getchar();
//     if (c == EOF) return -1;
//   }
//   return num;
// }


#endif