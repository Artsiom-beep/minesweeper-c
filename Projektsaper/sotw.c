#include "sotw.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void spawnmines(int** a, int n, int m, int level, int miny) {
    int mines = 0;
    int minesmax;
    if (level == 1)minesmax = 10;
    if (level == 2)minesmax = 40;
    if (level == 3)minesmax = 99;
    if (level == 4)minesmax = miny;
    while (mines < minesmax) {
        int x = rand() % m + 1;
        int y = rand() % n + 1;

        if (a[y][x] == 0) {
            a[y][x] = -1;
            mines++;
        }
    }
}


	
void write_b(char** b, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (b[i][j] == '.') printf("\033[32m%c \033[0m", b[i][j]);
            if (b[i][j] == '1') printf("\033[33m%c \033[0m", b[i][j]);
            if (b[i][j] == '2') printf("\033[34m%c \033[0m", b[i][j]);
            if (b[i][j] == '3') printf("\033[36m%c \033[0m", b[i][j]);
            if (b[i][j] == '4') printf("\033[1;32m%c \033[0m", b[i][j]);
            if (b[i][j] == '5') printf("\033[1;33m%c \033[0m", b[i][j]);
            if (b[i][j] == '6') printf("\033[1;34m%c \033[0m", b[i][j]);
            if (b[i][j] == '7') printf("\033[1;36m%c \033[0m", b[i][j]);
            if (b[i][j] == '8') printf("\033[35m%c \033[0m", b[i][j]);
            if (b[i][j] == 'F') printf("\033[31m%c \033[0m", b[i][j]);
            if (b[i][j] == '#') printf("%c ", b[i][j]);            
        }
        printf("\n");
    }
}
void wheremines(int** a, int n, int m) {

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i][j] == -1) {
                if (a[i - 1][j - 1] >= 0) a[i - 1][j - 1]++;
                if (a[i - 1][j] >= 0) a[i - 1][j]++;
                if (a[i - 1][j + 1] >= 0) a[i - 1][j + 1]++;
                if (a[i][j - 1] >= 0) a[i][j - 1]++;
                if (a[i][j + 1] >= 0) a[i][j + 1]++;
                if (a[i + 1][j - 1] >= 0) a[i + 1][j - 1]++;
                if (a[i + 1][j] >= 0) a[i + 1][j]++;
                if (a[i + 1][j + 1] >= 0) a[i + 1][j + 1]++;
            }

        }
    }

}

void rasp(int** a, char** b, int n, int m, int o, int p) {
    
    if (a[o][p] == 0) {
        a[o][p] = -2; 
        b[o - 1][p - 1] = '.'; 

       
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; 
                int new_o = o + dx;
                int new_p = p + dy;

               
                if (new_o >= 1 && new_o <= n && new_p >= 1 && new_p <= m) {
                    if (a[new_o][new_p] == 0) {
                        rasp(a, b, n, m, new_o, new_p); 
                    }
                    else if (a[new_o][new_p] > 0) {
                        b[new_o - 1][new_p - 1] = a[new_o][new_p] + '0'; 
                        a[new_o][new_p] = -3; 
                    }
                }
            }
        }
    }
    else if (a[o][p] > 0) {
        
        b[o - 1][p - 1] = a[o][p] + '0';
        a[o][p] = -3; 
    }
}

int check(int** a, int n, int m, int *points) {
    (*points) = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i][j] == -2 || a[i][j] == -3) (*points)++;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i][j] > 0) return 0;
            
        }
    }
    return 1;
}
