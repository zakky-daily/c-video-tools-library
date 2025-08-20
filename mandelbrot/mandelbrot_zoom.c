#include <stdio.h>
#include <stdlib.h>
#include "video_tools.h"
#include "mandelbrot_frame.c"

extern void mandelbrot_frame(Frame* frame, double s, double cx, double cy);

void mandelbrot_zoom(Video* video, double frame_delay) {

    for (int f = 0; f < frame_amount(video); f++ ) {

        Frame* frame = get_frame(video, f);
        double sec = calc_second(video, f);
        double zoom = 1000 * sec * sec * sec * sec * sec * sec;

        mandelbrot_frame(frame, zoom, -1.26222162762384535370226702572022420406, -0.04591700163513884695098681782544085357512);
    }
}
