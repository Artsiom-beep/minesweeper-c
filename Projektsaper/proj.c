#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "sotw.h"

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Skips the rest of the current input line; exits when input is closed
void flush_line() {
    int c;
    while ((c = getchar()) != '\n') {
        if (c == EOF) exit(1);
    }
}

typedef struct {
    char name[255];
    int score;
} player;

player top[6];


int main(int argc, char *argv[]) {

    int opt;
	int miny = -1;
int jp = 1;
    char* testfile = NULL;

    while ((opt = getopt(argc, argv, "a:")) != -1) {
        switch (opt) {
            case 'a':
		jp = 2;
                testfile = optarg;
                break;
            default:
                printf("Unknown option: -%c\n", optopt);
                return 1;
        }
    }





    if(jp == 1){
    int points = 0;
    for (int i = 0; i < 5; i++) {
        strcpy(top[i].name, "-");
        top[i].score = 0;
    }
    FILE* text = fopen("Score.txt", "rt");
    if (text != NULL) {
        for (int i = 0; i < 5; i++) {
            char t[255];
            int y;
            if (fscanf(text, "%254s", t) != 1) break;
            if (fscanf(text, "%d", &y) != 1) break;
            strcpy(top[i].name, t);
            top[i].score = y;
        }
        fclose(text);
    }
    printf("Welcome Player! Choose difficulty: \nFirst level: 9x9 10 mines     To choose print 1\nSecond level: 16x16 40 mines      To choose print 2\nThird level: 16x30 99 mines      To choose print 3\nForth level: Your own map! Points not multiply by 4!	    To choose print 4\nLevel:");
    int level, n = 9, m = 9;
    while (scanf("%d", &level) != 1 || level > 4 || level < 1) {
        flush_line();
        printf("Choose from 1 to 4!");
    }
    if (level == 1) {
        n = 9;
        m = 9;
    }
    else if (level == 2) {
        n = 16;
        m = 16;
    }
    else if (level == 3) {
        n = 16;
        m = 30;
    }
else if( level == 4){
	L1:
	printf("Insert rows: ");
	if (scanf("%d", &n) != 1) { flush_line(); n = 0; }
	printf("Insert column: ");
	if (scanf("%d", &m) != 1) { flush_line(); m = 0; }
	printf("Insert mines: ");
	if (scanf("%d", &miny) != 1) { flush_line(); miny = 0; }
	if(n<=0 || m<=0 || miny<=0){printf("Insert natural positive numbers numbers\n"); goto L1;}
	if(n*m-1 < miny){printf("Map is invalid"); return 0;}
}
    // Own map (level 4) is not multiplied
    int multiplier = (level == 4) ? 1 : level;

    printf("Hello Player! Insert your first position: ");

    srand((unsigned)time(NULL));




    int** a = (int**)malloc((n + 2) * sizeof(int*));
    for (int i = 0; i < n + 2; i++) {
        a[i] = (int*)malloc((m + 2) * sizeof(int));
        for (int j = 0; j < m + 2; j++) {
            a[i][j] = -4;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            a[i][j] = 0;
        }
    }


    int o, p;

    while (scanf("%d%d", &o, &p) != 2 || o < 1 || o > n || p < 1 || p > m) {
        flush_line();
        printf("Choose from 1 to %d and from 1 to %d!", n, m);
    }
    a[o][p] = -2;
    spawnmines(a, n, m, level, miny);
    a[o][p] = 0;
    wheremines(a, n, m);

    char** b = (char**)malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        b[i] = (char*)malloc(m * sizeof(char));
        for (int j = 0; j < m; j++) {
            b[i][j] = '#';
        }
    }

printf("\n");
rasp(a, b, n, m, o, p);



printf("\n");
write_b(b, n, m);
char symb = '!';
bool winner = true;
while (check(a, n, m, &points) == 0) {
    clear_console();
    printf("Current map:\n");
    write_b(b, n, m);
    printf("Score: %d\n", points * multiplier);
    printf("Flag (f) or dig (r)?\n");

    A1:
    if (scanf(" %c", &symb) != 1 || scanf("%d%d", &o, &p) != 2) {
        flush_line();
        printf("Illegal move!\n");
        goto A1;
    }
    if (o < 1 || o > n || p < 1 || p > m) {
        printf("Illegal move!\n");
        goto A1;
    }
    if (symb == 'r') {
        if (b[o - 1][p - 1] == 'F') {
            b[o - 1][p - 1] = '#';
            continue;
        }
        else if (a[o][p] == -1) {
            winner = false;
           break;
        }
         else if (a[o][p] == -2 || a[o][p] == -3) {
            printf("Illegal command! Try enter command again:\n");
            goto A1;
        }


        printf("\n");
        rasp(a, b, n, m, o, p);

    }
    else if (symb == 'f') {
        if (b[o - 1][p - 1] == '#')
            b[o - 1][p - 1] = 'F';
        else if (b[o - 1][p - 1] == 'F')
            b[o - 1][p - 1] = '#';
        else {
            printf("Illegal command! Try enter command again:\n");
            goto A1;
        }
    }
    else {
        printf("Illegal command! Try enter command again:\n");
        goto A1;
    }

}

points *= multiplier;

if (winner) {
    printf("YOU WON!\nEnter your name:\n");
}
else {
    printf("GAME OVER!\nEnter your name:\n");
}

char im[254];
if (scanf("%253s", im) != 1) strcpy(im, "Player");
for (int i = 0; i < 5; i++) {
    if (top[i].score < points) {

        for (int j = 4; j > i; j--) {
            strcpy(top[j].name, top[j - 1].name);
            top[j].score = top[j - 1].score;

        }

        strcpy(top[i].name, im);

        top[i].score = points;
        break;
    }
}
    for (int i = 0; i < n+2; i++) {
        free(a[i]);
    }
    free(a);


    for (int i = 0; i < n; i++) {
        free(b[i]);
    }
    free(b);



	FILE* trum = fopen("Score2.txt", "a");
	if (trum != NULL) {
		fprintf(trum, "%s ", im);
		fprintf(trum, "%d\n", points);
		fclose(trum);
	}

    FILE* text2 = fopen("Score.txt", "wt");
    for (int i = 0; i < 5; i++) {

        if (text2 != NULL) {
            fprintf(text2, "%s\n", top[i].name);
            fprintf(text2, "%d\n", top[i].score);
        }

        printf("%15s %6d \n", top[i].name, top[i].score);

    }
    if (text2 != NULL) fclose(text2);

    }
	else if(jp == 2){
		// Test mode: map size, map (-1 = mine, 0 = empty), then moves "row column"
		FILE* cin = fopen(testfile, "r");
		if (cin == NULL) { printf("Cannot open %s\n", testfile); return 1; }
		int n, m;
		if (fscanf(cin, "%d%d", &n, &m) != 2 || n <= 0 || m <= 0) { printf("Invalid map size\n"); fclose(cin); return 1; }
	    int** a = (int**)malloc((n + 2) * sizeof(int*));
    	for (int i = 0; i < n + 2; i++) {
        	a[i] = (int*)malloc((m + 2) * sizeof(int));
       	for (int j = 0; j < m + 2; j++) {
        	    a[i][j] = -4;
       		 }
		}
		char** b = (char**)malloc(n * sizeof(char*));
		for(int i = 0; i<n; i++)
			b[i] = (char*)malloc(m*sizeof(char));

for(int i = 0; i<n; i++)
for(int j = 0; j<m; j++)
b[i][j] = '#';

for(int i = 1; i<=n; i++){
	for(int j = 1; j<=m; j++){
		if (fscanf(cin, "%d", &a[i][j]) != 1) a[i][j] = 0;
	}
}


wheremines(a, n, m);


int x, y, score = 0, goodmoves = 0;
int bomb = 1;
while (fscanf(cin, "%d%d", &x, &y) == 2) {
	if (x < 1 || x > n || y < 1 || y > m) continue; // skip moves outside the map
	if (a[x][y] == -1) { bomb = 0; break; }

	rasp(a, b, n, m, x, y);
	goodmoves++;
	if(check(a, n, m, &score) == 1) break;
}
check(a, n, m, &score);
printf("%d %d %d", goodmoves, score, bomb);
fclose(cin);

for (int i = 0; i < n + 2; i++) {
    free(a[i]);
}

free(a);
for (int i = 0; i < n; i++) {
    free(b[i]);
}
free(b);
}

    return 0;
}
