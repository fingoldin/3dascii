#ifndef _UTILS_H_
#define _UTILS_H_

#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define MIN3(a, b, c)  ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

typedef struct vec3 {
  double e[3];
} vec3_t;

typedef struct vertex {
  vec3_t p;
  vec3_t c;
  vec3_t n;
} vertex_t;

typedef struct poly {
  vertex_t* v;
  size_t num_sides;
} poly_t;

typedef struct model {
  poly_t *polys;
  size_t num_polys;
} model_t;

typedef struct render_params {
  double cw, ch, cd, ml, rd, al;
  int screen_x, screen_y, rotz;
  vec3_t l, cs, cv, cz;
} render_params_t;

typedef struct mat3 {
  // Columns
  vec3_t c[3];
} mat3_t;

vec3_t vcross(vec3_t a, vec3_t b);

double vecdot(vec3_t a, vec3_t b);

double vnorm(vec3_t in);

double vnorm2(vec3_t in);

vec3_t vmul(double a, vec3_t b);

vec3_t vecsub(vec3_t a, vec3_t b);

vec3_t vecadd(vec3_t a, vec3_t b);

vec3_t vnormed(vec3_t in);

double mdet(mat3_t in);

mat3_t matinv(mat3_t in);

vec3_t mvmul(mat3_t a, vec3_t b);

vec3_t mtvmul(mat3_t a, vec3_t b);

void vprint(vec3_t a);

void mprint(mat3_t a);

mat3_t rot3y(double theta);

mat3_t rot3z(double theta);

vec3_t rot3v(double theta, vec3_t v, vec3_t n);

vec3_t constv(double val);

vec3_t vclamp(vec3_t v, double min, double max);

int isconst(vec3_t v, double val);

void free_model(model_t model);

void printpoly(poly_t p);

#endif
