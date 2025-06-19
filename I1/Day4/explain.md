### 課題4-1, 4-2
```
./sound/doremi.wav -t raw -b 16 -c 1 -e s -r 44100 - | ./downsample 5 | play -t raw -b 16 -c 1 -e s -r 8820 -
```

### 課題4-3
```
% ./sin 10000 3528 88200 > ../Day4/kadai4_3.raw
% ./read_data2 ../Day4/kadai4_3.raw > ../Day4/kadai4_3.txt
% gnuplot
> plot "kadai4_3.txt" with linespoints, "kadai4_3.txt" every 10 with line
> set xrange [0:50]
> replot
```

### 課題4-5
% sox ./sound/doremi.wav -t raw -c 1 - | ./bandpass 8192 300 2000 | play -t raw -b 16 -c 1 -e s -r 44100 -
% gnuplot plot_spectrum.gp

