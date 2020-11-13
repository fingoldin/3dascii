#include <math.h>
#include <curses.h>
#include <stdio.h>
#include <string.h>
#include "kernel.h"

vec3_t shader(vec3_t fragPos, vec3_t normal, vec3_t color, render_params_t* params) {
  const double diffuse = fabs(vecdot(vnormed(params->l), normal));
  const double intensity = MAX(params->ml, params->al + diffuse);

  return vmul(intensity, color);
}

int kernel(vec3_t* target, vec3_t pc, vec3_t ts, mat3_t Ut, mat3_t Uc, double* min_a, poly_t poly, render_params_t* params) {
  vec3_t pcd = pc;
  pcd.e[2] -= params->cd;
  const vec3_t rv = mvmul(Uc, pcd);
  const vec3_t csmts = vecsub(params->cs, ts);

  const double a = -mtvmul(Ut, csmts).e[2] / mtvmul(Ut, rv).e[2];

  if(isnan(a) || a * vnorm(pcd) < 1 || (*min_a > 0.0 && a >= *min_a)) {
    return 0;
  }

  const vec3_t dt = mtvmul(Ut, vecadd(csmts, vmul(a, rv)));

  vec3_t prev_u = mtvmul(Ut, vecsub(poly.v[0].p, poly.v[poly.num_sides - 1].p));
  for(size_t i = 0; i < poly.num_sides; i++) { 
    const vec3_t u = mtvmul(Ut, vecsub(poly.v[(i + 1) % poly.num_sides].p, poly.v[i].p));
    const vec3_t pt = mtvmul(Ut, vecsub(poly.v[i].p, ts));
    const double s = vecdot(vcross(vecsub(dt, pt), u), vcross(u, prev_u));
    if(s < 0) {
      return 0;
    }
    prev_u = u;
  }

  *min_a = a;

  const vec3_t fragPos = vecadd(params->cs, vmul(a, rv));

  vec3_t color = constv(0.0);
  double total_dist = 0.0;
  for(size_t i = 0; i < poly.num_sides; i++) {
    double dist = vnorm(vecsub(poly.v[i].p, fragPos));
    color = vecadd(color, vmul(dist, poly.v[i].c));

    total_dist += dist;
  }

  if(isconst(color, 0.0)) {
    color = constv(1.0);
  } else {
    color = vmul(1.0 / total_dist, color);
  }

  *target = vclamp(shader(fragPos, Ut.c[2], color, params), 0.0, 1.0);

  return 1;
}

void render(vec3_t* buffer, double* depthbuffer, poly_t* polys, size_t num_polys, render_params_t* params, WINDOW* win) {
  mat3_t Uc;
  const vec3_t cvn = vnormed(params->cv);
  Uc.c[0] = vcross(cvn, vnormed(params->cz));
  Uc.c[1] = vnormed(vcross(Uc.c[0], cvn));
  Uc.c[0] = vnormed(Uc.c[0]);
  Uc.c[2] = vmul(-1.0, cvn);

  const double epsilon = 0.0000001;

  memset(buffer, 0, params->screen_x * params->screen_y * sizeof(vec3_t));
  memset(depthbuffer, 0, params->screen_x * params->screen_y * sizeof(double));

  for(size_t t = 0; t < num_polys; t++) {
    const vec3_t ts = polys[t].v[0].p;
    const vec3_t tp1mts = vecsub(polys[t].v[1].p, ts);
    const vec3_t tp2mts = vecsub(polys[t].v[2].p, ts);

    mat3_t Ut;
    Ut.c[0] = tp1mts;
    Ut.c[1] = vcross(Ut.c[0], vcross(tp2mts, Ut.c[0]));
    Ut.c[2] = vnormed(vcross(Ut.c[0], Ut.c[1]));
    Ut.c[1] = vnormed(Ut.c[1]);
    Ut.c[0] = vnormed(Ut.c[0]);

    if(fabs(vecdot(Ut.c[2], params->cv)) < epsilon) {
      continue;
    }

    double rmax_x = params->cw * 0.5;
    double rmin_x = -params->cw * 0.5;
    double rmax_y = params->ch * 0.5;
    double rmin_y = -params->ch * 0.5;

    int all_nb = 1;
    for(int v = 0; v < polys[t].num_sides; v++) {
      vec3_t ir = mtvmul(Uc, vecsub(polys[t].v[v].p, params->cs));
      double b = -params->cd / ir.e[2];
      if(b > 0.0) {
        double rx = b * ir.e[0];
        double ry = b * ir.e[1];

        if(rmax_x == params->cw * 0.5 || rx > rmax_x) {
          rmax_x = rx;
        }
        if(rmin_x == -params->cw * 0.5 || rx < rmin_x) {
          rmin_x = rx;
        }
        if(rmax_y == params->ch * 0.5 || ry > rmax_y) {
          rmax_y = ry;
        }
        if(rmin_y == -params->ch * 0.5 || ry < rmin_y) {
          rmin_y = ry;
        }
        all_nb = 0;
      }
    }
    if(all_nb) {
      continue;
    }

    int max_x = MIN(params->screen_x, 1 + (int)((rmax_x / params->cw + 0.5) * (double)(params->screen_x)));
    int min_x = MAX(0, (int)((rmin_x / params->cw + 0.5) * (double)(params->screen_x)));
    int max_y = MIN(params->screen_y, 1 + (int)((rmax_y / params->ch + 0.5) * (double)(params->screen_y)));
    int min_y = MAX(0, (int)((rmin_y / params->ch + 0.5) * (double)(params->screen_y)));

    for(int x = min_x; x < max_x; x++) {
      for(int y = min_y; y < max_y; y++) {
        vec3_t pc;
        pc.e[0] = params->cw * 0.5 *
                  MIN(MAX(-1.0, (double)(x - params->screen_x / 2) / (double)(params->screen_x / 2)), 1.0);
        pc.e[1] = params->ch * 0.5 * 
                  MIN(MAX(-1.0, (double)(y - params->screen_y / 2) / (double)(params->screen_y / 2)), 1.0);
        pc.e[2] = 0;
        vec3_t target;
        if(kernel(&target, pc, ts, Ut, Uc, &depthbuffer[x * params->screen_y + y], polys[t], params)) {
          buffer[x * params->screen_y + y] = target;
        }
      }
    }
    if((t + 1) % 100 == 0) {
      mvwprintw(win, 0, 0, "%ld/%ld ", t + 1, num_polys);
      wrefresh(win);
    }
  }
}
  
