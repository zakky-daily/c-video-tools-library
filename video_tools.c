#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32  // Windows向け
    #include <direct.h>
    #define mkdir(folder, mode) _mkdir(folder)
    #define stat_struct _stat
#else  // POSIX向け
    #include <sys/types.h>
    #define stat_struct stat
#endif
#include "video_tools.h"

// フレームの各要素を返す
int width(Frame* frame) { return frame->width; }
int height(Frame* frame) { return frame->height; }

// ビデオの各要素を返す
double duration(Video* video) { return video->duration; }
int fps(Video* video) { return video->fps; }
int frame_amount(Video* video) { return video->duration * video->fps; }

// 指定フレームでの秒数を計算する
double calc_second(Video* video, int frame_index) { return (double)frame_index / fps(video); }

// フレームの作成
Frame* create_frame(int width, int height) {
    Frame* frame = (Frame*)malloc(sizeof(Frame));
    *(int*)&frame->width = width;
    *(int*)&frame->height = height;
    frame->pixels = (Color**)malloc(height * sizeof(Color*));
    for (int y = 0; y < height; y++) {
        frame->pixels[y] = (Color*)malloc(width * sizeof(Color));
        for (int x = 0; x < width; x++) {
            frame->pixels[y][x] = (Color){255, 255, 255};
        }
    }
    return frame;
}

// 動画の作成
Video* create_video(double duration, int fps, int width, int height) {
    Video* video = (Video*)malloc(sizeof(Video));
    *(double*)&video->duration = duration;
    *(int*)&video->fps = fps;
    video->frames = (Frame**)malloc(frame_amount(video) * sizeof(Frame*));
    for (int f = 0; f < frame_amount(video); f++) {
        video->frames[f] = create_frame(width, height);
    }
    return video;
}

// フレーム編集を終了する(メモリ解放)
void exit_frame(Frame* frame) {
    for (int y = 0; y < height(frame); y++) {
        free(frame->pixels[y]);
    }
    free(frame);
}

//　動画編集を終了する(メモリ解放)
void exit_video(Video* video) {
    for (int f = 0; f < frame_amount(video); f++) {
        exit_frame(video->frames[f]);
    }
    free(video->frames);
    free(video);
}

// 動画の指定位置にフレームを設定
void set_frame(Video* video, Frame* frame, int frame_index) {
    if (frame_index >= 0 && frame_index < frame_amount(video)) {
        video->frames[frame_index] = frame;
    }
}
// 動画の指定位置のフレームを取得
Frame* get_frame(Video* video, int frame_index) {
    if (frame_index >= 0 && frame_index < frame_amount(video)) {
        return video->frames[frame_index];
    } else {
        printf("Error: frame_index out of range\n");
        return NULL;
    }
}

// フレームの色を設定
void set_pixel(Frame* frame, int x, int y, Color color) {
    if (x >= 0 && x < width(frame) && y >= 0 && y < height(frame)) {
        frame->pixels[y][x] = color;
    }
}
// フレームの色を取得
Color get_pixel(Frame* frame, int x, int y) {
    if (x >= 0 && x < width(frame) && y >= 0 && y < height(frame)) {
        return frame->pixels[y][x];
    } else {
        printf("Error: pixel pos out of range\n");
        return (Color){0, 0, 0};
    }
}

// フレームの書き出し
void export_frame(Frame* frame, char* frame_name) {
    FILE* frame_file = fopen(frame_name, "w");
    if (!frame_file) {
        printf("Error: folder not found\n");
        return;
    }
    fprintf(frame_file, "P3 %d %d 255\n", width(frame), height(frame));
    for (int y = 0; y < height(frame); y++) {
        for (int x = 0; x < width(frame); x++) {
            Color color = frame->pixels[y][x];
            fprintf(frame_file, "%d %d %d ", color.r, color.g, color.b);
        }
        fprintf(frame_file, "\n");
    }
    fclose(frame_file);
}

// 動画の書き出し
void export_video(Video* video, char* video_name) {

    // 拡張子の除去
    char video_folder[strlen(video_name)+1];
    strcpy(video_folder, video_name);
    char* dot = strrchr(video_folder, '.');
    if (dot != NULL) *dot = '\0';

    // フォルダ中身の消去またはフォルダの作成
    struct stat_struct st;
    if (!(stat(video_folder, &st) == 0 && (st.st_mode & S_IFDIR))) {
        mkdir(video_folder, 0777);
    }

    // フレームごとに描画
    char frame_name[1024];
    for (int f = 0; f < frame_amount(video); f++) {
        sprintf(frame_name, "%s/%04d.ppm", video_folder, f);
        export_frame(video->frames[f], frame_name);
    }
    printf("Success: now run the following code: convert -delay %d $(for i in $(seq -f \"%s/%%04g.ppm\" 0 %d); do echo $i; done) %s\n", 100 / fps(video), video_folder, frame_amount(video) - 1, video_name);
}