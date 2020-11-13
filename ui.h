#ifndef _UI_H_
#define _UI_H_

#include <curses.h>
#include "utils.h"

void draw(vec3_t* buffer, int screen_x, int screen_y, WINDOW* window);

void handleinput(char c, render_params_t* p);

void init_colors();

#endif
