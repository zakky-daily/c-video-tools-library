#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "video_tools.h"

// 最大反復回数を120に設定
#define MAX_ITER 120

// グラデーションの調整(fromGPT)
Color lerp(Color start, Color end, double t) {  // ツールの仕様に合わせてColorを大文字にした
    Color ret;
    ret.r = start.r + (int)((end.r - start.r) * t);
    ret.g = start.g + (int)((end.g - start.g) * t);
    ret.b = start.b + (int)((end.b - start.b) * t);
    return ret;
}

// 反復回数に応じて色を決定する関数（グラデーション）

//   0   : 白     (0, 0, 0)
//   20  : 青     (0, 0, 255)
//   40  : 水色   (0, 255, 255)
//   60  : 緑     (0, 255, 0)
//   80  : 黄色   (255, 255, 0)
//   100 : 赤     (255, 0, 0)
//   119 : 白     (255, 255, 255)
//   120 : 黒     (0, 0, 0)
Color mandelbrot_Color(int iter) {
    Color c;
    
    if (iter <= 20) {
        // 0～20: 黒 → 青
        Color start = {0, 0, 0};
        Color end   = {0, 0, 255};
        double t = (double)iter / 20.0;
        c = lerp(start, end, t);
    }
    else if (iter <= 40) {
        // 20～40: 青 → 水色
        Color start = {0, 0, 255};
        Color end   = {0, 255, 255};
        double t = (double)(iter - 20) / 20.0;
        c = lerp(start, end, t);
    }
    else if (iter <= 60) {
        // 40～60: 水色 → 緑
        Color start = {0, 255, 255};
        Color end   = {0, 255, 0};
        double t = (double)(iter - 40) / 20.0;
        c = lerp(start, end, t);
    }
    else if (iter <= 80) {
        // 60～80: 緑 → 黄色
        Color start = {0, 255, 0};
        Color end   = {255, 255, 0};
        double t = (double)(iter - 60) / 20.0;
        c = lerp(start, end, t);
    }
    else if (iter <= 100) {
        // 80～100: 黄色 → 赤
        Color start = {255, 255, 0};
        Color end   = {255, 0, 0};
        double t = (double)(iter - 80) / 20.0;
        c = lerp(start, end, t);
    }
    else if (iter <= 119) { 
        // 100～120: 赤 → 黒
        Color start = {255, 0, 0};
        Color end   = {255, 255, 255};
        double t = (double)(iter - 100) / 20.0;
        c = lerp(start, end, t);
    }
    else {
        Color end   = {0, 0, 0};
        c = end;
    }
    
    return c;
}

// マンデルブロ集合を描画する関数
Color mandelbrot_set(double c_real, double c_imag) {  // 複素数を受け取るところから計算する, それ以前は他の人に任せている

    double z_real, z_imag;
    int iter;

    // ★ここまではズームの方と調整

    z_real = 0.0;
    z_imag = 0.0;
    iter = 0;
    
    // 反復計算（z = (z_{n-1})^2 + c）
    while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < MAX_ITER) {
        double z_real_temporary = z_real * z_real - z_imag * z_imag + c_real;
        z_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = z_real_temporary;
        iter++;
    }

    //★ここから

    // 色を決定してピクセルに設定
    Color c = mandelbrot_Color(iter);
    return c;  // 色を返す. 画像に書き込むのは他の人にやらせている
}

/*
int main(void) {
    // キャンバスの生成
    img_clear();
    
    // マンデルブロ集合を描画
    mandelbrot_set();
    
    // 画像を作成
    img_write();
    
    return 0;
}
*/
