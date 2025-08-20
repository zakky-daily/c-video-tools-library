#include <stdio.h>
#include <stdlib.h>
#include "video_tools.h"  // 動画作成ツール
#include "mandelbrot_set.c"  // 亀田くんが書いてくれた関数を読み込む

extern Color mandelbrot_set(double c_real, double c_imag);

void mandelbrot_frame(Frame* frame, double s, double cx, double cy) {

    double x_min = -480, x_max = 479;
    double y_min = -360, y_max = 359;

    for (double y = y_max; y >= y_min; y--) {

        for (double x = x_min; x <= x_max; x++) {

            //printf("(%f, %f)\n", (x - cx) / s, (y - cy) / s);

            Color c = mandelbrot_set(x / s + cx, y / s - cy);  // こう座標軸を設定したらうまくいった
            set_pixel(frame, x + 480, y + 360, c);  // 色を書き出す
        }
    }
}

/*
int main() {
    zoom(2, 100, 200);
    return 0;
}
*/
