#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <curses.h>
#include "ui.h"
#include "data.h"
#include "kernel.h"

render_params_t get_params(WINDOW* win) {
  render_params_t p;
  p.cd = 1.0;
  p.ml = 0.1;
  p.al = 0.15;
  p.rotz = 1;
  p.l.e[0] = 0.5;
  p.l.e[1] = -0.3;
  p.l.e[2] = -1;
  p.cs.e[0] = 15;
  p.cs.e[1] = 15;
  p.cs.e[2] = 15;
  p.cv.e[0] = -1;
  p.cv.e[1] = -1;
  p.cv.e[2] = -1;
  p.cz.e[0] = 0;
  p.cz.e[1] = 0;
  p.cz.e[2] = 1;

  double char_ratio = 2.0;
  getmaxyx(win, p.screen_y, p.screen_x);

  p.cw = ((double)p.screen_x / (double)p.screen_y) / char_ratio;
  p.ch = 1.0;

  return p;
}

int main(int argc, char** argv) {
  model_t m;
  if(argc > 1) {
    m = load_model(argv[1]);
  } else {
    fprintf(stderr, "Enter an .obj file\n");
    return 1;
  }

  initscr();
  cbreak();
  noecho();

  init_colors();

  WINDOW* win = newwin(0, 0, 0, 0);
  nodelay(win, true);
  
  render_params_t p = get_params(win);
  vec3_t* buffer = malloc(sizeof(vec3_t) * p.screen_x * p.screen_y);
  double* depthbuffer = malloc(sizeof(double) * p.screen_x * p.screen_y);
  
  const double thetaz = 0.2;
  struct timeval stop, start;
  wclear(win);
  while(1) {
    if(p.rotz) {
      for(int t = 0; t < m.num_polys; t++) {
        for(int i = 0; i < m.polys[t].num_sides; i++) {
          m.polys[t].v[i].p = mvmul(rot3y(thetaz), m.polys[t].v[i].p);
        }
      }
    }

    gettimeofday(&start, NULL);    
    render(buffer, depthbuffer, &m.polys[0], m.num_polys, &p, win);
    gettimeofday(&stop, NULL);    
    
//    wclear(win);
    draw(buffer, p.screen_x, p.screen_y, win);
    mvwprintw(win, p.screen_y - 1, 0,
      "[%ld, %ld]   cs: [%f %f %f]   cv: [%f %f %f]   render time: %f s", p.cs.e[0], p.cs.e[1], p.cs.e[2],
      p.screen_x, p.screen_y, p.cv.e[0], p.cv.e[1], p.cv.e[2],
      (double)(stop.tv_sec - start.tv_sec) + 0.000001 * (double)(stop.tv_usec - start.tv_usec));
    wrefresh(win);

    char c = wgetch(win);
    while(c != -1) {
      if(c == '\033') {
        getch();
        char c2 = getch();
        if(c2 >= 65 && c2 <= 68) {
          handleinput(24 + (int)c2 - 65, &p);
        }
      } else {
        handleinput(c, &p);
      }

      c = wgetch(win);
    }
  }

  free_model(m);
  free(buffer);
  free(depthbuffer);

  delwin(win);
  endwin();

  return 0;
}
