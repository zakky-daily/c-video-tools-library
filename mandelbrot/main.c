#include <stdio.h>
#include <stdlib.h>
#include "video_tools.h"
#include "mandelbrot_zoom.c"

#define DURATION 15  // 動画の秒数
#define FPS 10       // 1秒間に更新する回数
#define WIDTH 960    // 動画の横幅
#define HEIGHT 640   // 動画の縦幅

extern void mandelbrot_zoom(Video* video, double frame_delay);

int main() {

    Video* video = create_video(DURATION, FPS, WIDTH, HEIGHT);  // 動画の作成

    mandelbrot_zoom(video, 0);

    export_video(video, "output.gif");  // 動画の出力
    exit_video(video);  // 動画編集を終了する
    return 0;
}
