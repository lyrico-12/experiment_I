#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>


#define PI 3.14159265358979323846
#define SAMPLE_RATE 44100
#define SAMPLES_PER_NOTE 13230

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "使用法: %s 振幅A 音の数n\n", argv[0]);
        return 1;
    }

    double A = atof(argv[1]);
    int n = atoi(argv[2]);

    double f = 261.63;
    
    // 16ビット符号付き整数の最大値
    const double MAX_AMP = 32767.0;
    int count = 0;
    // n音分の音階を生成
    for (int note = 0; note < n; note++) {
        // 現在の音階のインデックス（ドレミファソラシドの周期）
        int note_index = note % 8;
        
        // 1音分（13230標本）の波形を生成
        for (int i = 0; i < SAMPLES_PER_NOTE; i++) {
            // 時間（秒）
            double t = (double)i / SAMPLE_RATE;
            
            // サイン波計算: x = A * sin(2πft)
            double sample = A * sin(2.0 * PI * f * t);
            
            // 16ビット符号付き整数に変換
            short output;
            
            // 振幅が最大値を超えないように調整
            if (sample > MAX_AMP) {
                output = 32767;
            } else if (sample < -MAX_AMP) {
                output = -32768;
            } else {
                output = (short)sample;
            }
            
            // バイナリデータとして出力
            write(1, &output, sizeof(short));
        }

        if (count % 7 == 2 || count % 7 == 6) {
            f = pow(2.0, 1.0/12.0) * f;
        } else {
            f = pow(2.0, 1.0/6.0) * f;
        }
        count++;
    }
    
    return 0;
}