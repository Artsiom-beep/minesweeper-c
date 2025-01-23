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

typedef struct {
    char name[255];
    int score;
} player;

player top[6];


int main(int argc, char *argv[]) {

    int opt;
	int miny = -1;
int jp = 1;

    while ((opt = getopt(argc, argv, "a:")) != -1) {
        switch (opt) {
            case 'a':
		jp = 2;
                printf("Option -a with value: %s\n", optarg);
                break;
            default:
                printf("Unknown option: -%c\n", optopt);
                return 1;
        }
    }





    if(jp == 1){
    int points = 0;
    FILE* text = fopen("Score.txt", "rt");
    for (int i = 0; i < 5; i++) {
        char t[255];
        int y;
        fscanf(text, "%s", t);
        fscanf(text, "%d", &y);
        strcpy(top[i].name, t);
        top[i].score = y;
        //printf("%15s %6d \n", top[i]->name, top[i]->score);

    }
    fclose(text);
    printf("Welcome Player! Choose difficulty: \nFirst level: 9x9 10 mines     To choose print 1\nSecond level: 16x16 40 mines      To choose print 2\nThird level: 16x30 99 mines      To choose print 3\nForth level: Your own map! Points not multiply by 4!	    To choose print 4\nLevel:");
    int level, n, m;
    scanf("%d", &level);
    if (level > 4 || level < 1) {
        printf("Choose from 1 to 4!");
        scanf("%d", &level);
	if(level > 4 || level < 1) return 0;
    }
    else if (level == 1) {
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
	printf("Insert rows: "); scanf("%d", &n);
	printf("Insert column: "); scanf("%d", &m);
	printf("Insert mines: "); scanf("%d", &miny);
	if(n<=0 || m<=0 || miny<=0){printf("Insert natural positive numbers numbers\n"); goto L1;}
	if(n*m-1 < miny){printf("Map is invalid"); return 0;}
}
    else {
	printf("WRONG VALUE!");
	return 0;
    }
    
    printf("Hello Player! Insert your first position: ");

    srand(time(NULL));
    

    

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

    scanf("%d%d", &o, &p);
    if (o < 1 || o>n || p < 1 || p>m){ 
        printf("Choose from 1 to %d and from 1 to %d!", n, m);
        scanf("%d%d", &o, &p);
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
    points *= level;
    printf("Score: %d\n", points);
    printf("Flag (f) or dig (r)?\n");
   
    A1:
    getchar();
    scanf("%c", &symb);
    scanf("%d%d", &o, &p);
    if (o > n || p > m) {
        printf("Illegal move!");
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
   

        /*for (int i = 0; i < n + 2; i++) {
            for (int j = 0; j < m + 2; j++) {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
        */

       
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



if (winner) {
    printf("YOU WON!\nEnter your name:\n");
}
else {
    printf("GAME OVER!\nEnter your name:\n");
}

char im[254];
//scanf_s("%[^\n]", &im, sizeof(im));
scanf("%s", im);
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
	fprintf(trum, "%s ", im);
	fprintf(trum, "%d\n", points);
	fclose(trum);

    FILE* text2 = fopen("Score.txt", "wt");
    for (int i = 0; i < 5; i++) {
       
        fprintf(text2, "%s\n", top[i].name);
        fprintf(text2, "%d\n", top[i].score);
        
        printf("%15s %6d \n", top[i].name, top[i].score);

    }

    
   
    
    }
	else if(jp == 2){
		FILE* cin = fopen(argv[2], "r");
		int n, m;
		fscanf(cin, "%d%d", &n, &m);
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
		fscanf(cin, "%d", &a[i][j]);
	}
}


wheremines(a, n, m);


int x, y, score = 0, goodmoves = 0;
while((fscanf(cin, "%d%d", &x, &y) == 2) && a[x][y] != -1){

rasp(a, b, n, m, x, y);
goodmoves++;
if(check(a, n, m, &score) == 1) break;


}
int bomb = 1;
if(a[x][y] == -1)bomb = 0;
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
