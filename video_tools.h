#pragma once
#ifndef VIDEO_TOOLS
#define VIDEO_TOOLS

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color;

typedef struct {
    Color** pixels;
    const int width;
    const int height;
} Frame;

typedef struct {
    Frame** frames;
    const double duration;
    const int fps;
} Video;

int width(Frame* frame);
int height(Frame* frame);
double duration(Video* video);
int fps(Video* video);
int frame_amount(Video* video);
double calc_second(Video* video, int frame_index);

Frame* create_frame(int width, int height);
Video* create_video(double duration, int fps, int width, int height);
void exit_frame(Frame* frame);
void exit_video(Video* video);
void set_frame(Video* video, Frame* frame, int frame_index);
Frame* get_frame(Video* video, int frame_index);
void set_pixel(Frame* frame, int x, int y, Color color);
Color get_pixel(Frame* frame, int x, int y);
void export_frame(Frame* frame, char* frame_name);
void export_video(Video* video, char* video_name);

#endif