#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <curses.h>

#include "utils.h"
#include "stddef.h"

void render(vec3_t* buffer, double* depthbuffer, poly_t* polys, size_t num_polys, render_params_t* params, WINDOW* win);

#endif
