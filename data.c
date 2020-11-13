#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

model_t load_model(const char* path) {
  model_t out;
  out.num_polys = 0;

  FILE* fp = fopen(path, "r");
  if(!fp) {
    printf("ERROR opening model file\n");
    return out;
  }

  size_t linecount = 1;
  for(char c = getc(fp); c != EOF; c = getc(fp)) {
    if(c == '\n' || c == '\r') {
      linecount += 1;
    }
  }

  out.polys = malloc(sizeof(poly_t) * linecount);
  vertex_t* vertices = malloc(sizeof(vertex_t) * linecount);
  memset(vertices, 0, sizeof(vertex_t) * linecount);

  size_t num_vertices = 0;
  size_t face_idx = 0;
  char* line = NULL;
  size_t n = 0;
  char* original_line = NULL;

  rewind(fp);
  while(getline(&line, &n, fp) != -1) {
    original_line = line;
    if(line[0] == 'v' && num_vertices < linecount) {
      int i = 0;
      while(strsep(&line, " ") && i < 6) {
        if(!line) {
          break;
        }
        char* endptr = line;
        double val = strtod(line, &endptr);
        if(endptr != line && !isnan(val)) {
          if(i > 2) {
            vertices[num_vertices].c.e[(i++ - 3)] = val;
          } else {
            vertices[num_vertices].p.e[i++] = val;
          }
        }
      }
      num_vertices++;
    }
    
    free(original_line);
    line = NULL;
    n = 0;
  }

  rewind(fp);
  while(getline(&line, &n, fp) != -1) {
    original_line = line;
    if(line[0] == 'f' && face_idx < linecount) {
      int face_vertices = 0;
      const int alloc_step = 3;
      int num_alloced = alloc_step;
      out.polys[face_idx].v = malloc(sizeof(vertex_t) * num_alloced);
      while(strsep(&line, " ")) {
        if(!line) {
          break;
        }
        char* endptr = line;
        size_t x = strtol(line, &endptr, 10);
        if(endptr != line && x > 0 && x <= num_vertices) {
          if(face_vertices >= num_alloced) {
            out.polys[face_idx].v = realloc(out.polys[face_idx].v, sizeof(vertex_t) * (num_alloced + alloc_step));
            num_alloced += alloc_step;
          }
          out.polys[face_idx].v[face_vertices++] = vertices[x - 1];
        }
      }
      out.polys[face_idx].num_sides = face_vertices;
      face_idx++;
    }

    free(original_line);
    line = NULL;
    n = 0;
  }
  
  if(original_line) {
    free(original_line);
  }
  fclose(fp);

  out.num_polys = face_idx;

  return out;
}
