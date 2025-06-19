#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846

int main() {
    double A, f;
    int n;
    
    // 標準入力からパラメータを読み込む
    printf("振幅A: ");
    scanf("%lf", &A);
    
    printf("周波数f (Hz): ");
    scanf("%lf", &f);
    
    printf("点数n: ");
    scanf("%d", &n);
    
    // 16ビット符号付き整数の最大値
    // 振幅を正規化するために使用
    const double MAX_AMP = 32767.0;
    
    // 各点を計算してRAWデータとして出力
    for (int i = 0; i < n; i++) {
        // 時間（秒）
        double t = (double)i / 44100.0;
        
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
        fwrite(&output, sizeof(short), 1, stdout);
    }
    
    return 0;
} 