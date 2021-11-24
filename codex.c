#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

#define MAX_WORD_LEN 12
#define NUM_WORDS 1000

int GenerateSample(char* sample, int len);

int main(int argc, char* argv[]){

    WINDOW* win = initscr();
    start_color();
    noecho();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    // wbkgd(win, COLOR_PAIR(5));
    int wrds = 10;
    char* sample = malloc(sizeof(char) * wrds * MAX_WORD_LEN);
    GenerateSample(sample, wrds);
    // char sample[] = "And as he rose from the black of night his cold smile pierced like light";
    printw("%s \n", sample);
    
    refresh();
    int w, h;
    getmaxyx(win, h, w);

    int len = strlen(sample);
    int input_y = len / w + 1; 
    int index = 0;
    int wordcount = 0;
    char ch;

    struct timespec start, finish; 

    short first = 1;
    while(index < len){
        ch = getch();
        
        if(first){
            clock_gettime(CLOCK_MONOTONIC, &start);
            first = 0;
        }

        if(ch == 127){    //Backspace
            if(wordcount > 0){
                wordcount--;
                index--;
            }
            attron(COLOR_PAIR(4));
            mvaddch(index/w, index%w, sample[index]);
            move(index/w, index%w);
            continue;
        }else if(ch == sample[index]){
            attron(COLOR_PAIR(1));
            mvaddch(index/w, index%w, ch);
        }else{
            attron(COLOR_PAIR(2));
            if(sample[index] == ' ')
                ch = '_';
            else
                ch = sample[index];
            mvaddch(index/w, index%w, ch);
        }
        wordcount++;
        index++;
        refresh();
    }

    endwin();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double delta = (finish.tv_sec - start.tv_sec);
    delta += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Delta:  %.2fs\n", delta);
    printf("WPM:    %d\n", (int) round(wrds/delta*60));
    return 0;
}

int GenerateSample(char* sample, int len){
    char words[NUM_WORDS][MAX_WORD_LEN];
    
    FILE* file = fopen("words.txt", "r");
    for(int i = 0; i < NUM_WORDS; i++){
        // words[i] = malloc(sizeof(char) * MAX_WORD_LEN);
        fscanf(file, "%[^\n] ", words[i]);
        // printf("%s\n", words[i]);
    }
    time_t t;
    srand((unsigned) time(&t));

    int ix;
    for(int j = 0; j < len; j++){
        char* randomstring = words[rand() % NUM_WORDS - 1];
        strcat(sample, randomstring);
        if(j != len - 1)
            strcat(sample, " ");
    }
}