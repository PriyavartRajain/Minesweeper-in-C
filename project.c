/*Project part 1
Submitted by - Priyavart Rajain
The University of Calgary 
Course - CPSC 355 Spring 2020*/

#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<time.h>
#include<ctype.h>

/* Below are the function prototypes: */

/*This function takes the array gameBoard as a pointer, and
the dimension N as parameteres and initializes the game board with random numbers between -15 and 15
Return type: void*/
void initializeGame(float *gameBoard,int N);

/*This function takes the lower and upper bound of random numbers as min and max
and a boolean neg (to decide whether the result is going to be positive or negative) as parameteres*/
/*Returns a random floating point number between -15 and 15*/
float randomNum(float min, float max, bool neg);

/*This function calculates the score and alters teh cell values for identification
, it takes score, the pointer to the
gameboard array, user inputs i and j, array dimension N, a pointer to surprise count
and total number of surprises as parameters */
/*Returns score as float*/
float calculateScore(float score,float *gameBoard,int i, int j, int N,float *surpCount, float total_Surprises);

/*This function displays the gameboard, score and time and
takes the pointer to game board array , dimension N, score, and remaining time as parameters*/
/*Return type Void*/
void displayGame(float *gameBoard,int N,float score, float time_rem);

/*This function calculates the remaining time and takes
clock variables start_t, end_t and time remaining as parameters*/
/*Returns the remaining time as double*/
double timeRem(clock_t start_t, clock_t end_t, double time_rem);

/*This function checks if all the cells have been visisted, if yes then it returns true*/
bool checkBoard(float *gameBoard, int N);

/*This function logs the score and time taken to the log file
and takes pointer to the file Game_record.log , score, and tiem taken as parameters*/
void logScore(FILE *fptr, double score, double time_taken);

/*This function displays the scores along with playername and timetaken*/
void displayTopScores(FILE *fptrRead);

/*This function decides whether or not to give a time bonus, if yes
then it adds 5 seconds to remaining time and returns it
Takes in pointer to gameboard, input i coordinate, j coordinate, N,
remining time, time_rem, and a pointert to surprise count and total surprises as parameters*/
double timeBonus(float *gameBoard, int i_coord, int j_coord, int N, double time_rem, float *surpCount,float total_Surprises);

/*This fucntion is called at the end and it writes everything to the logfile and gives the user an option to display the top scores*/
void exitGame(FILE *fptr, float score, double total_time, double time_rem);


void initializeGame(float *gameBoard,int N){                                                  // pointer to array gameBoard, and dimension N as parameters
    float totalCells = N*N;                                                                   // total cells in the board will be N*N
    float totalNegCells = 0.0;                                                                // initially there are 0 negative cells
    bool neg;                                                                                 // boolean neg, if true, a negative number is generated, otherwise a positive no. is genetrated
    srand(time(0));                                                                           // passing 0 to time function which is passed to srand
    for(int i=0;i<N;i++){                                                                     // looping through the 2D array
        for(int j=0; j<N;j++){
            int randBit = rand()%2;                                                           // randBit either is 0 or 1
            if(randBit==0){                                                                   // if it is 0 then neg is set to true
                neg = true;
                totalNegCells+=1;                                                             // if neg is true we check if 20% of cells are negative
                if(totalNegCells/totalCells > 0.2 ){
                    neg = false;                                                              // if they are, we set neg to false and  a positive numebr is generated
                    }
            }
            else if(randBit == 1){                                                            // otherwise  if randBit is 1, we generate a positive number
                neg = false;
            }
            *(gameBoard+i*N+j) = (float)randomNum(0.00, 14.00, neg);                          // calling randNum function to assign a random float to the i.j th cell.
        }
    }
}

float randomNum(float min, float max, bool neg){

    float randfloat;                                                                          // declaring a float called randfloat
   randfloat = (rand()&(int)max);                                                             // randfloat = bitwise and of the number generate by  rand and max i.e 14
    float scale = (rand()/(float)RAND_MAX);                                                   // scale is assigned a value between 0 and 1
    scale = roundf(scale*100)/100;                                                            // rounding scale
    randfloat = scale + randfloat;                                                            // finally randfloat = scale added to the random number generated in step 1

    if(neg == true){                                                                          // if neg is true, we convert this random number to negative by multiplying with -1
        randfloat = -1*randfloat;
    }
    return randfloat;                                                                         // random number is returned
    }

                                                                                              // calculateScore function alters the cell value after they have been explored
void displayGame(float *gameBoard,int N,float score, float time_rem){
    for(int i=0; i<N;i++){                                                                    // loop through the game board,
        for(int j=0; j<N;j++){
            if(*(gameBoard+i*N+j) == -20.00)                                                  // If cell value is -20 , that represents a bomb
                printf(" - ");
            else if(*(gameBoard+i*N+j) == 20.00)                                              // A value of 20 represents a nice cell
                printf(" + ");
            else if(*(gameBoard+i*N+j) == -21.00)                                             // A ! represents a bad surprise
                printf(" ! ");
            else if(*(gameBoard+i*N+j) == 21.00)                                              // A $ represenst a good surprise
                printf(" $ ");
            else if(*(gameBoard+i*N+j) == 23.00)                                              // A @ represents time pack
                printf(" @ ");
            else
                printf(" X ");                                                                // otherwise, the cell has not been explored and represented by X
        }
        printf("\n");
    }
    printf("\n");

    for(int i=0;i<N;i++){
	for(int j=0;j<N;j++){
	printf(" %.2f ",*(gameBoard+i*N+j));
}	
printf("\n");
}
printf("\n\n");

    printf("Score:\t%.2f\n\n",score);                                                        // prints the score

    printf("Time: %.2f\n\n",(time_rem));                                                     // prints the time remaining

}

float calculateScore(float score,float *gameBoard,int i, int j, int N,float *surpCount, float total_Surprises){                     // calculaets the score and alters the cell values for identification
    float cellValue = *(gameBoard+i*N+j);                                                                                           // cell value holds the value at a particular cell
    srand(time(0));
    int isSurprise = rand()&11;                                                                                                     // isSurprise is a value between 0 and 10

    if((cellValue == -20.00) || (cellValue == 20.00) || (cellValue == -21.00) || (cellValue == 21.00) || (cellValue == 23.00) ){    // if cellValue is among any of these values, it means the cell has already been dicovered
        printf("\nCell already discovered make another choice\n\n");
    }
    else if(cellValue == 22.00){                                                                                                    // else if the value is 22, player discovered a time pack, change teh value to 23.00
        printf("\nYou discovered a time pack. 5 seconds will be added to the timer\n\n");                                           // Note thet the value 22 is assigned in timeBonus function, it is NOT genertaed by randNum
        *(gameBoard+i*N+j) = 23.00;                                                                                                 // All values generated by randNum are still between -15 and 15
        return score;                                                                                                               // if its a time pack dont change score and return it as it is
    }
    else if(cellValue>0.00){                                                                                                        // else if cell value is > 0.00, and isSurprise is 1
        if(isSurprise == 1){                                                                                                        // and if surpCount is less than total permitted surprise count
            if(*(surpCount)<total_Surprises){
            	int temp= 0;                                                                                                            // display the following message and double the score using bitwise left sift operator
            	printf("\nYou discovered a nice surprise, score will be doubled!\n\n");
            	score = score*100;
            	temp = (int)score;                                                                                                      // cast score as int and store in temp
            	temp = temp<<1;                                                                                                         // left shift temp by 1
            	score = (float)temp/100;                                                                                                // temp is divided by 100, casted as float and stored in score
            	*(gameBoard+i*N+j)= 21.00;                                                                                              // change the value of cell to 21.00
            	*(surpCount)+=1.00;
		}                                                                                           // increase the surprise count by 1
        }

	else{        
	
             printf("\nUncovered a reward of %.2f points\n\n",cellValue);                                                            // if isSurprise not equal to 1, just add the cell value to score
             score+=cellValue;
             *(gameBoard+i*N+j)= 20.00;                                                                                              // change the score value to 20.00
            }
    }
    else if(cellValue<0.00){                                                                                                        // if cell value is negative and isSurprise is 1
        if(isSurprise == 1){
            if(*(surpCount)<total_Surprises){                                                                                       // and surprise count is less than total permissible surprises,
            	int temp=0;
            	printf("\nYou discovered a bad surprise, score will be halved!\n\n");                                                   // thsi means the user has encountered a bad surprise
            	score = score*100;
            	temp = (int)score;                                                                                                      // in this case right shift temp by 1 to divide the score by 2
            	temp = temp>>1;
            	score = (float)temp/100;
            	*(gameBoard+i*N+j)= -21.00;                                                                                             // chanege cell value to -21.00
            	*(surpCount)+=1;
		}
			                                                                                                       // increment the surprise counter by 1
        }

       
       else { 
	     printf("\nBang!! You lost %.2f points\n\n",cellValue);                                                                  // otherwise if isSurprise !=0 or 2, it is a regular bomb
            score+=cellValue;                                                                                                       // add teh negative float to score, hence decreasing it
            *(gameBoard+i*N+j) = -20.00;                                                                                            // set teh cell value to -20.00 for identification
       }
    }
    return score;                                                                                                                   // retrun updated score
}

double timeRem(time_t start_t, time_t end_t, double time_rem){                                                                    // calculaets remaining time
    double time_taken;                                                                                                             // time taken is simply the time taken by user to respond
    time_taken = difftime(end_t, start_t);                                                                          // time between being prompted to entering the coordinates and actually receiving them using scanf

    time_rem -= time_taken;                                                                                               // remaining time =  total time available - time taken
    return time_rem;                                                                                                                // return the remaing time
}

bool checkBoard(float *gameBoard, int N){                                                                                           // check if all cells have been discovered
    bool allDiscovered = false;                                                                                                     // initially the boolean is false
    int count = 0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if((*(gameBoard+i*N+j) == 20.00) || (*(gameBoard+i*N+j) == -20.00) || (*(gameBoard+i*N+j) == 21.00) || (*(gameBoard+i*N+j) == -21.00) || (*(gameBoard+i*N+j) == 23.00)){
                count++;                                                                                                            // if any of these values are found in the array, increment count by 1
            }
        }
    }
    if (count==N*N)                                                                                                                 // when count becomes equal to total cells i.e N*N, this implies all cells have been visited
        allDiscovered = true;                                                                                                       // so set the boolean to true and return true
    return allDiscovered;                                                                                                           // otherwise return the initial value i.e false
}

void logScore(FILE *fptr, double score, double time_taken){                                               // logscore function logs score, and time taken by player into the Game_Record.log file
    fprintf(fptr,"SCORE: ");
    fprintf(fptr,"%.2f",score);                                                                           // storing the value of score in log file
    fprintf(fptr,"\n");
    fprintf(fptr,"TIME TAKEN: ");
    fprintf(fptr,"%.2f",time_taken);                                                                      // storing the time taken by player in log file
    fprintf(fptr,"\n\n");
}

void displayTopScores(FILE *fptrRead){                                                                   // displays scores stored in log file upon request by user
    char line[50];
    while(!feof(fptrRead)){                                                                              // while end of file is not reached print the contents , each line consists of 50 characters
        fgets(line,50,fptrRead);
        puts(line);                                                                                      // display contents on console
    }
}

double timeBonus(float *gameBoard, int i_coord, int j_coord, int N, double time_rem, float *surpCount,float total_Surprises){           // Assigns time bonus randomly
    int isBonus = rand()&11;                                                                                                            // isBonus is a value between 0 and 10
    float cellValue = *(gameBoard+i_coord*N+j_coord);                                                                                   // cellValue has the value of i,j th cell
    if ((cellValue == -20.00) || (cellValue == 20.00) || (cellValue == -21.00) || (cellValue == 21.00) || (cellValue == 23.00)){        // if any of adjacent values are found, then cell has been discovered already
        printf(" ");
        return time_rem;                                                                                                                // make no changes to remaining time and return it
    }
    else if(isBonus == 2){                                                                                                              // else if isBonus is 2
        if(*(surpCount)<total_Surprises){                                                                                               // and total surprises created are less than allowable surprises
            time_rem+=5.00;                                                                                                             // add 5 seconds to remianing time
            *(gameBoard+i_coord*N+j_coord) = 22.00;                                                                                     // change cell value to 22 for identification
            *(surpCount)+=1.00;                                                                                                         // increment the surprise counter by 1
        }
    }
    return time_rem;                                                                                                                    // return updated time
}

void exitGame(FILE *fptr, float score, double total_time, double time_rem){                                                             // exitGame saves the score and time taken by calling logScore function
    char view;                                                                                                                          // also gives the option to print the contents of the log file
    logScore(fptr,score,(total_time-time_rem));
    fclose(fptr);
    printf("Would you like to view the top scores? [y] or [n]\n");                                                                       // if the user chooses y, the contents of log file are displayed
    getchar();
    scanf("%c",&view);
    FILE *fptrRead;
    fptrRead = fopen("Game_Record.log","r");
    if(view == 'y'){
        displayTopScores(fptrRead);                                                                                                      // calling displayTopScores to print the scores
                                                                                                                                         // This game only displays the contents in an unsorted manner
    }
    fclose(fptrRead);
    printf("\n\nThanks for playing\n\n");

}


int main(int argc, char *argv[]) {                                                                                         //Entry point to the program
	if (argc < 3) {                                                                                                        // Input validation
		printf("\nPlease enter the player name followed by the dimension of the game board\n");
		return 0;
	}
	else if (argc > 3) {
		printf("\nPlease enter the player name followed by the dimension of the game board\n");
		return 0;
	}

	char *playerName = *(argv + 1);                                                                                      // storing player name in playerName
	int N =  atoi(*(argv + 2));                                                                                          // storing board dimensions in N by first converting the cmd input to integer as it is a string
	if (N < 5) {
		printf("Board dimensions too small, enter a value between 5 and 20\n");                                          // Input validation
		return 0;
	}
	else if (N > 20) {
		printf("Board dimensions too big, enter a value between 5 and 20\n");
		return 0;
	}

    float arr[N][N];                                                                                         // declaring NxN array arr
	float (*gameBoard)[N] = arr;                                                                             // declaring pointer to arr, this will be used for the rest of the program

	FILE    *fptr;                                                                                           // declaring pointer to file Game_Record.log
	fptr =  fopen("Game_Record.log","a");
	if(fptr == NULL){                                                                                        // if NULL, the file coundnt open
        printf("Error opening file: Game_Record.log");
        exit(1);                                                                                             // exit if fptr is NULL
	}
    fprintf(fptr,playerName);                                                                                // store player name into the log file
    fprintf(fptr,"\n");
	float score = 0.00;                                                                                      // initializing score with 0.00

    initializeGame((float*)gameBoard,N);                                                                     // calling initializeGame to initialize the array with random floats between -15 and 15

    int i_coord;                                                                                             // declaring i and j coordinates, input by user
    int j_coord;

    char response;                                                                                           // declaring response, inputted by user, if q , then quit otehrwise continue

    int firstMove = 0;                                                                                       // if firstMove is 0 then it is the initial move of user

    double time_rem = N*12.00;                                                                               // time_rem is initially N*12.00 , i.e if N is 5 , time alloted is 60 seconds
    double total_time = time_rem;                                                                            // set total time to be eqiual to time_rem

    bool allDiscovered = false;                                                                              // initially no cells are discovered so it is false

    displayGame((float*)gameBoard,N,score,time_rem);                                                         // calling displayGame to display board and initial score and time

    float total_Surprises =  0.12*N*N;                                                                       // permissiblle number of surprises, i.e if N is 5 , only 3 surprises are allowed
    float surpriseCount = 0.00;                                                                              // initially surprise count is 0 since no cells have been discovered

    float *surpCount = &surpriseCount;                                                                       // pointer to surpriseCount for easier computation among different functions

    printf("Hello %s \n", playerName);

    while(response!='q'){                                                                                    // until user types q, keep prompting for i and j coordinates and keep progressing the game

        time_t start_t,end_t;                                                                                // declaring clock variables

        printf("Enter your move ( x , y ):\t");

        time(&start_t);                                                                                    // start the timer when user is prompted

        scanf("%d %d",&i_coord,&j_coord);
        getchar();
        while(i_coord<0 || i_coord>(N-1) || j_coord<0 || j_coord>(N-1)){                                      // input validation
            printf("\nPlease enter valid coordinates:\t");
            scanf("%d %d",&i_coord,&j_coord);
            getchar();
        }

        time(&end_t);                                                                                      // end the timer when user has entered teh values of i_coord and j_coord

        time_rem = timeRem(start_t,end_t,time_rem);                                                           // calculate remining time by calling timeRem fuction

        if(time_rem <= 0.00){                                                                                 // if remining time reaches 0 or less, game is over and break out the loop
            printf("\nTime Up. Game Over");
            break;
        }

        time_rem = timeBonus((float*)gameBoard,i_coord,j_coord,N,time_rem,surpCount,total_Surprises);         // otherwise, call timeBonus function to look for time packs and update time_rem accordingly
        score = calculateScore(score,(float*)gameBoard,i_coord,j_coord,N,surpCount,total_Surprises);          // calculate score by calling calculateScore
        displayGame((float*)gameBoard,N,score,time_rem);                                                      // display gameboard , teh time and score

        if(firstMove!=0){
            if(score<=0){                                                                                       // if score reaches 0 or below, game over and break
                printf("Score went below 0. Game Over\n");
                break;}
        }
        allDiscovered = checkBoard((float*)gameBoard,N);                                                        // if all cells dicovered game over and break
            if(allDiscovered==true){
            printf("\nAll cells have been discovered. Game Over\n");
            break;
        }
        printf("\nPress q to quit or any other key to continue:   ");                                           // user may quit voluntarily by pressing q

        scanf("%c",&response);
        printf("\n");
        displayGame((float*)gameBoard,N,score,time_rem);                                                        // display the board at the end of each iteration
        firstMove = 1;                                                                                          // upadate firstMove i.e teh next move will not be first Move

    }
    exitGame(fptr,score,total_time,time_rem);                                                                   // call the exit game function to log the name, score and time and give option to display the scores

	return 0;                                                                                                   // return 0
}
