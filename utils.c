#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

vec3_t vcross(vec3_t a, vec3_t b) {
  vec3_t out;
  out.e[0] = a.e[1] * b.e[2] - b.e[1] * a.e[2];
  out.e[1] = -a.e[0] * b.e[2] + b.e[0] * a.e[2];
  out.e[2] = a.e[0] * b.e[1] - b.e[0] * a.e[1];

  return out;
}

double vecdot(vec3_t a, vec3_t b) {
  return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

double vnorm2(vec3_t in) {
  return vecdot(in, in);
}

double vnorm(vec3_t in) {
  return sqrt(vnorm2(in));
}

vec3_t vmul(double a, vec3_t b) {
  vec3_t out;
  for(int i = 0; i < 3; i++) {
    out.e[i] = a * b.e[i];
  }

  return out;
}

vec3_t vecsub(vec3_t a, vec3_t b) {
  vec3_t out;
  for(int i = 0; i < 3; i++) {
    out.e[i] = a.e[i] - b.e[i];
  }

  return out;
}

vec3_t vecadd(vec3_t a, vec3_t b) {
  vec3_t out;
  for(int i = 0; i < 3; i++) {
    out.e[i] = a.e[i] + b.e[i];
  }

  return out;
}

vec3_t vnormed(vec3_t in) {
  return vmul(1.0 / vnorm(in), in);
}

double mdet(mat3_t a) {
  return a.c[0].e[0] * (a.c[1].e[1] * a.c[2].e[2] - a.c[1].e[2] * a.c[2].e[1]) -
         a.c[1].e[0] * (a.c[0].e[1] * a.c[2].e[2] - a.c[0].e[2] * a.c[2].e[1]) +
         a.c[2].e[0] * (a.c[0].e[1] * a.c[1].e[2] - a.c[0].e[2] * a.c[1].e[1]);
}

mat3_t matinv(mat3_t a) {
  mat3_t out;
  double det = mdet(a);
  out.c[0].e[0] = (a.c[1].e[1] * a.c[2].e[2] - a.c[1].e[2] * a.c[2].e[1]) / det;
  out.c[0].e[1] = (a.c[2].e[1] * a.c[0].e[2] - a.c[2].e[2] * a.c[0].e[1]) / det;
  out.c[0].e[2] = (a.c[0].e[1] * a.c[1].e[2] - a.c[0].e[2] * a.c[1].e[1]) / det;
  out.c[1].e[0] = (a.c[2].e[0] * a.c[1].e[2] - a.c[2].e[2] * a.c[1].e[0]) / det;
  out.c[1].e[1] = (a.c[0].e[0] * a.c[2].e[2] - a.c[0].e[2] * a.c[2].e[0]) / det;
  out.c[1].e[2] = (a.c[1].e[0] * a.c[0].e[2] - a.c[1].e[2] * a.c[0].e[0]) / det;
  out.c[2].e[0] = (a.c[1].e[0] * a.c[2].e[1] - a.c[1].e[1] * a.c[2].e[0]) / det;
  out.c[2].e[1] = (a.c[2].e[0] * a.c[0].e[1] - a.c[2].e[1] * a.c[0].e[0]) / det;
  out.c[2].e[2] = (a.c[0].e[0] * a.c[1].e[1] - a.c[0].e[1] * a.c[1].e[0]) / det;

  return out;
}

vec3_t mvmul(mat3_t a, vec3_t b) {
  vec3_t out;
  for(int i = 0; i < 3; i++) {
    out.e[i] = a.c[0].e[i] * b.e[0] + a.c[1].e[i] * b.e[1] + a.c[2].e[i] * b.e[2];
  }
  return out;
}

vec3_t mtvmul(mat3_t a, vec3_t b) {
  vec3_t out;
  for(int i = 0; i < 3; i++) {
    out.e[i] = a.c[i].e[0] * b.e[0] + a.c[i].e[1] * b.e[1] + a.c[i].e[2] * b.e[2];
  }
  return out;
}


void vprint(vec3_t a) {
  printf("%f %f %f\n", a.e[0], a.e[1], a.e[2]);
}

void mprint(mat3_t a) {
  printf("%f %f %f\n%f %f %f\n %f %f %f\n\n", a.c[0].e[0], a.c[1].e[0], a.c[2].e[0],
         a.c[0].e[1], a.c[1].e[1], a.c[2].e[1], a.c[0].e[2], a.c[1].e[2], a.c[2].e[2]);
}

mat3_t rot3y(double theta) { 
  mat3_t roty;
  roty.c[0].e[0] = cos(theta);
  roty.c[2].e[0] = sin(theta);
  roty.c[1].e[1] = 1;
  roty.c[0].e[2] = -sin(theta);
  roty.c[2].e[2] = cos(theta);

  return roty;
}
  
mat3_t rot3z(double theta) { 
  mat3_t rotz;
  rotz.c[0].e[0] = cos(theta);
  rotz.c[0].e[1] = sin(theta);
  rotz.c[2].e[2] = 1;
  rotz.c[1].e[0] = -sin(theta);
  rotz.c[1].e[1] = cos(theta);

  return rotz;
}

vec3_t constv(double val) {
  vec3_t out;
  out.e[0] = val;
  out.e[1] = val;
  out.e[2] = val;

  return out;
}

int isconst(vec3_t v, double val) {
  return (v.e[0] == val && v.e[1] == val && v.e[2] == val);
}

vec3_t vclamp(vec3_t v, double min, double max) {
  vec3_t out;
  out.e[0] = MIN(max, MAX(min, v.e[0]));
  out.e[1] = MIN(max, MAX(min, v.e[1]));
  out.e[2] = MIN(max, MAX(min, v.e[2]));

  return out;
}

vec3_t rot3v(double theta, vec3_t v, vec3_t n) {
  vec3_t nn = vnormed(n);

  return vecadd(vmul(cos(theta), v),
                vecadd(vmul(sin(theta), vcross(nn, v)),
                       vmul((1 - cos(theta)) * vecdot(nn, v), nn)));
}

void free_model(model_t model) {
  for(size_t i = 0; i < model.num_polys; i++) {
    if(model.polys[i].v) {
      free(model.polys[i].v);
    }
  }
  if(model.polys) {
    free(model.polys);
  }
}

void printpoly(poly_t p) {
  for(int i = 0; i < p.num_sides; i++) {
    printf("[%d]: ", i + 1);
    vprint(p.v[i].p);
  }
}
