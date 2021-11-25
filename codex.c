//	Gabriel Racz
//	11-24-2021
//	v1.1
//
// char sample[] = "And as he rose from the black of night his cold smile pierced like light";
#include<ncurses.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<unistd.h>

#define MAX_WORD_LEN 12
#define NUM_WORDS 1000

//Scoring
//	read numwords from file

int GenerateSample(char* sample, int len);
int FormattedPrint(WINDOW* win, char ch, int width);

int main(int argc, char* argv[]){


    int wrds;
    printf("Word count:  ");
    scanf("%d", &wrds);

	//wrds = 45;
    
	WINDOW* win = initscr();
    start_color();
    noecho();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_BLACK);
    // wbkgd(win, COLOR_PAIR(5));
    
	//resizeterm(60, 60);
	clear();
	//usleep(500000);
   	refresh(); 
	int w, h;
    getmaxyx(win, h, w);


	char* sample = malloc(sizeof(char) * wrds * MAX_WORD_LEN);
    GenerateSample(sample, wrds);
    int len = strlen(sample);
	
		
	//Maximum lines
	int ends[10];
	int n = 0;
	char rc;
   	for(int i = 0; i < len; i++){
		rc = FormattedPrint(win, sample[i], w - 11);
		if(rc > 0){
			ends[n] = rc;
			n++;
		}
	}
    refresh();

    int input_y = len / w + 1; 
    int index = 0;
    int wordcount = 0;
    char ch;

    struct timespec start, finish; 
    char* userinput = malloc(sizeof(char) * len);

    short first = 1;
	int ix = 0;
	move(0, 0);
    while(index < len){
        ch = getch();

		int x, y;
		getyx(win, y, x);
        
        if(first){	//Start clock
            clock_gettime(CLOCK_MONOTONIC, &start);
            first = 0;
        }
	
        if(ch == 127){    //Backspace
            if(wordcount > 0){
                wordcount--;
                index--;
            }
            attron(COLOR_PAIR(4));
			int srows;
			int scols;
			if(x == 0){
				ix--;
				srows = ix;
				scols = ends[ix];
				move(n, ends[ix]);
            	FormattedPrint(win, sample[index], 1000);
				move(srows, scols);
			}else{
				move(y, x-1);
            	FormattedPrint(win, sample[index], 1000);
				move(y, x-1);
			}
            continue;
        }
		else if(ch == sample[index]){	//Match
            attron(COLOR_PAIR(1));
            //mvaddch(index/w, index%w, ch);
        }
		else{								//Error
            attron(COLOR_PAIR(2));
			if(sample[index] == ' ')
                ch = '_';
            else
                ch = sample[index];

           // mvaddch(index/w, index%w, ch);
        }
		
		if(FormattedPrint(win, ch, w - 11)){
			ix++;
		}	

        wordcount++;
        index++;
        refresh();
    }

    endwin();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    double delta = (finish.tv_sec - start.tv_sec);
    delta += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Delta:       %.2fs\n", delta);
    printf("WPM:         %d\n", (int) round(wrds/delta*60));
    return 0;
}

int FormattedPrint(WINDOW* win, char ch, int width){
	int cursorx;
	int cursory;
	int savex;
	getyx(win, cursory, cursorx);
	//printw("%d ", ch);
	addch(ch);
	if(ch == ' ' && cursorx > width - 11){
		savex = cursorx;
		addch('\n');
		return savex;
	}
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
