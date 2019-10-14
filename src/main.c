#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int len, col, num, seed, visi=0, init=0;
// Length，Column，Mines, Seed, Visible Grid, Initialized or not
unsigned char *m;
// 00000000
//        ^ Is mine or not
//       ^  Is visible or not
//     ^~   Mark and type
// ^~~~     Mines number around this grid
char p[]={' ', 'O', 'X', '_'};
// Mark characters

#define NONE 0
#define SAFE 1
#define MINE 2
#define QUERY 3

#define M(x, y) m[(x)*col+(y)]
#define IS_MINE(x, y) (M(x, y)&1)
#define IS_VISI(x, y) (M(x, y)&2)
// Visible or not
#define IS_MARK(x, y) (M(x, y)&12)
#define IS_NUM(x, y) (M(x, y)&240)
#define IS_OUT(x, y) (x>=len||y>=col||x<0||y<0)
// Out of range or not
#define GET_MARK(x, y) ((M(x, y)&12)>>2)
#define GET_NUM(x, y) ((M(x, y)&240)>>4)
#define SET_MINE(x, y, s) M(x, y)=s?M(x, y)|1:M(x, y)&254
#define SET_VISI(x, y, s) M(x, y)=s?M(x, y)|2:M(x, y)&254
#define SET_MARK(x, y, s) M(x, y)=s&2?s&1?M(x, y)|12:(M(x, y)&243)|8:s&1?M(x, y)&243|4:M(x, y)&243
#define INC_NUM(x, y) if (!IS_OUT(x, y)) M(x, y)=(M(x, y)&15)|(GET_NUM(x, y)+1)<<4

void bfs(int x, int y) {
    if (IS_OUT(x, y)||IS_VISI(x, y))
        return;
    ++visi;
    SET_VISI(x, y, 1);
    if (!GET_NUM(x, y)) {
        bfs(x-1, y-1);
        bfs(x-1, y);
        bfs(x-1, y+1);
        bfs(x, y-1);
        bfs(x, y+1);
        bfs(x+1, y-1);
        bfs(x+1, y);
        bfs(x+1, y+1);
    }
}

void print(int c) {
    int x, y;
    printf("  ");
    for (x=0; x<len; ++x) {
        printf("%-2d", x);
    }
    printf("\n");
    for (y=col-1; y>=0; --y) {
        printf("%-2d", y);
        if (c) 
            for (x=0; x<len; ++x)
                printf("%-2c", IS_MINE(x, y)?'*':GET_NUM(x, y)+'0');
        else
            for (x=0; x<len; ++x)
                printf("%-2c", IS_VISI(x, y)?IS_MINE(x, y)?'*':GET_NUM(x, y)+'0':
                    p[GET_MARK(x, y)]);
        printf("%-2d", y);
        printf("\n");
    }
    printf("  ");
    for (x=0; x<len; ++x) {
        printf("%-2d", x);
    }
    printf("\n");
    if (c)
        printf("WARNING: CHAETING DETECTED\n");
}

void clear() {
    for (int i=0; i<len*col; ++i)
        m[i]|=2;
}

int input() {
    int x, y, s;
    printf("Enter X coordinate, Y coordinate, and instruction\n");
    scanf("%d %d %d", &x, &y, &s);
    if (IS_OUT(x, y)) {
        print(0);
        printf("Invaild Command: Out of range\n");
        return 0;
    }
    if (!init) {
        for (int i=0, a, b; i<num; ++i) {
            a=rand()%len;
            b=rand()%col;
            if (IS_MINE(a, b)||(a==x&&b==y))
                continue;
            else {
                INC_NUM(a-1, b-1);
                INC_NUM(a-1, b);
                INC_NUM(a-1, b+1);
                INC_NUM(a, b-1);
                SET_MINE(a, b, 1);
                INC_NUM(a, b+1);
                INC_NUM(a+1, b-1);
                INC_NUM(a+1, b);
                INC_NUM(a+1, b+1);
            }
        }
        init=1;
    }   
    if (s) {
        if (s==-seed) {
            print(1);
        }
        else if (s<=4&&s>0) {
            if (!IS_VISI(x, y))
                SET_MARK(x, y, s-1);
            else {
                print(0);
                printf("Invaild Command: Already visible\n");
                return 0;
            }
            print(0);
        }
        else {
            print(0);
            printf("Invaild Command: Command does not exist\n");
        }
    }
    else {
        SET_MARK(x, y, 0);
        if (IS_MINE(x, y)) {
            printf("You have lost\n");
            clear();
            print(0);
            return 1;
        }
        bfs(x, y);
        print(0);
    }
    if (visi==len*col-num) {
        printf("You win\n");
        clear();
        print(0);
        return 2;
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc>3) {
        printf("Getting information from argument\n");
        len=atoi(argv[1]);
        col=atoi(argv[2]);
        num=atoi(argv[3]);
        if (argc>4)
            seed=atoi(argv[4]);
        else seed=time(NULL);
    }
    else {
        printf("Enter length, width, mineage, and seed(random seeds fill in -1)\n");
        scanf("%d %d %d %d", &len, &col, &num, &seed);
        if (seed<0)
            seed=time(NULL);
    }
    srand(seed);
    printf("Length: %d\n", len);
    printf("Column: %d\n", col);
    printf("Mines: %d\n", num);
    printf("Seed：%d\n", seed);
    m=(char *)malloc(len*col);
    memset(m, 0, len*col);
    print(0);
    for (; !input(););
}