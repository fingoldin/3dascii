#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "ui.h"

char charmap(double val) {
  const char map[] = ".`'-:/*+=a&#@";
  size_t l = strlen(map);

  if(val > 0.0) {
    return map[MIN(l - 1, (int)(val * (double)l))];
  }

  return ' ';
}

static const int num_colors = 7;
static const int color_data[7][5] = {
  { COLOR_RED, COLOR_BLACK, 255, 0, 0 },
  { COLOR_GREEN, COLOR_BLACK, 0, 255, 0 },
  { COLOR_YELLOW, COLOR_BLACK, 255, 255, 0 },
  { COLOR_BLUE, COLOR_BLACK, 0, 0, 255 },
  { COLOR_MAGENTA, COLOR_BLACK, 255, 0, 255 },
  { COLOR_CYAN, COLOR_BLACK, 0, 255, 255 },
  { COLOR_WHITE, COLOR_BLACK, 255, 255, 255 }
};

void init_colors() {
  start_color();

  for(int i = 0; i < num_colors; i++) {
    init_pair(i + 1, color_data[i][0], color_data[i][1]);
  }
}

int color_map(vec3_t raw_rgb) {
  vec3_t rgb = vnormed(raw_rgb);
  double best_dist = 10.0;
  int out = 0;

  for(int i = 0; i < num_colors; i++) {
    vec3_t color_rgb;
    color_rgb.e[0] = color_data[i][2];
    color_rgb.e[1] = color_data[i][3];
    color_rgb.e[2] = color_data[i][4];
    color_rgb = vmul(1.0 / 255.0, color_rgb);

    double dist = vnorm2(vecsub(rgb, color_rgb));
    if(dist < best_dist) {
      out = i + 1;
      best_dist = dist;
    }
  }

  return out;
}

void draw(vec3_t* buffer, int screen_x, int screen_y, WINDOW* window) {
  wmove(window, 0, 0);
  for(int y = screen_y - 1; y >= 0; y--) {
    for(int x = 0; x < screen_x; x++) {
      double intensity = vnorm(buffer[x * screen_y + y]);
      int color = color_map(buffer[x * screen_y + y]);

      wattron(window, COLOR_PAIR(color));
      waddch(window, charmap(intensity));
      wattroff(window, COLOR_PAIR(color));
    }
  }
}

void handleinput(char c, render_params_t* p) {
  double dx = 0.5;
  double dtheta = 0.05;
  vec3_t forward = vnormed(p->cv);
  vec3_t right = vnormed(vcross(p->cv, p->cz));
  vec3_t up = vnormed(vcross(right, forward));
  switch(c) {
    case 'w':
      p->cs = vecadd(p->cs, vmul(dx, forward));
      break;
    case 's':
      p->cs = vecadd(p->cs, vmul(-dx, forward));
      break;
    case 'a':
      p->cs = vecadd(p->cs, vmul(-dx, right));
      break;
    case 'd':
      p->cs = vecadd(p->cs, vmul(dx, right));
      break;
    case 24:
      p->cv = rot3v(dtheta, p->cv, right);
      break;
    case 25:
      p->cv = rot3v(-dtheta, p->cv, right);
      break;
    case 26:
      p->cv = rot3v(-dtheta, p->cv, up);
      break;
    case 27:
      p->cv = rot3v(dtheta, p->cv, up);
      break;
    case 'r':
      p->rotz = !p->rotz;
      break;
    default:
      break;
  };
}
