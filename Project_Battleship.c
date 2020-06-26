#include<stdio.h>   //Call the library Standard Input and Output (for the use of basic functions such as printf & scanf).
#include<stdlib.h>  //We call on the Standard General Utilities Library (for use malloc, free, rand, srand).
#include<time.h>    //We need this library only to use time function.
#include<ctype.h>   //This library is for toupper function.


void hFields(int N, int K);     //This function is for memory allocation of the 2 hidden grids were all ships are placed.
void PrintField(int N, int K, char** field);    //This function is used to print each field we want as an argument.
void GetCoords(int N, int K,char** field,char** hfield,int ConditionsArray[5][4]);  //This function is to get coordinates from "human-user".
void TextArt(void);     //Welcome messages at start.
int RandomNumbers(int min, int max);    //Function that use to generate random numbers between min and max.
void ManuallyPlace(int N, int K, int size, char shipCh, char shipN[], char** field);    //This function is used from "human-users" to place ships manually.
void RandomPlace(int N, int K, int size, char shipCh, char** field);    //This function is used to place the ships randomly (meaning random direction and random position [x,y]).
int GameMode(int N,int K);      //This function sets the game mode (PvP,PvAI,AIvAI) and sets game mode between a game with predefined rounds and a game that ends.
void PlayerTurn(int N, int K,char** hfield,char** field,char player[],int ConditionsArray[5][4]);   //This function is used at a "human-user"'s turn.
void CPUturn(int N, int K,char** hfield,char** field,char player[],int ConditionsArray[5][4]);  //This function is used at computer turn.
int SmartTarget(int x, int y, int N, int K,char** field ,char** hfield, char player[],int ConditionsArray[5][4]);   //This function is used when the computer is playing for second time in a row.
void Refresh(int N,int K,char** hfield, char** field,int ConditionsArray[5][4]);    //This function scans the grids and refreshes them if ships have been struck or sunk and save the round that a ship sunk.
void RoundMessage(int N,int K,char** hfield, char** field,char player[],int Sflag,int ConditionsArray[5][4]);   //This function prints some messages right after the grid that shows this rounds situation.
void ScoreCounter(int N, int K);    //This function counts the score of each player.
void Help(int N, int K,char** field,char** hfield,int ConditionsArray[5][4]);   //This function is used for HELP chose when user types the character 'h'.
void End(int N, int K);     //This function is a set of messages that shows the situation at the end(who won etc).
void FreeMemory(int N,int K);   //After last use of the arrays that we have allocate in memory we use this function to free this memory.


/*Global variables declaration!!*/
char** hfieldP;     // pointers declaration that use for the grids
char** hfieldP2;

char** fieldP;
char** fieldP2;

int x,y,x2,y2,lastRound,points1,points2,Cround=0;   //some integer variables that used many times in program
char Player1[20],Player2[20];   //2 strings that are used to save players' names

int ConditionsArray1[5][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};/*rows: 1st:Aircraft Carrier 2nd Battleship 3rd Submarine 4th Destroyer 5th Cruiser.
                                                                                columns:1st column ship sunk condition, 2nd (help)column 1 part destroyed condition , 3rd column round of sunk ship condition, 4th print check condition */
int ConditionsArray2[5][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};/*rows: 1st:Aircraft Carrier 2nd Battleship 3rd Submarine 4th Destroyer 5th Cruiser.
                                                                                columns:1st column ship sunk condition, 2nd (help)column 1 part destroyed condition , 3rd column round of sunk ship condition,4th print check condition*/
//for more info about ConditionsArray check the end of code

/*--------------------Start of main--------------------------------------- */
int main(void){

    int N, K, i, j;     //variables declaration
    int mode,start;

    srand(time(NULL));  //This command seeds the rand command
    TextArt();      //function call of TextArt
        do{
        printf("\nGive the size of grid NxK!!!");   //The user give the size of the grid.
        printf("\nGive length, N[5,26]:");
        scanf("%d",&N);
        printf("Give width, K[5,30]:");
        scanf("%d",&K);
    }while((N<5 || N>26) || (K<5 || K>30));

    hFields(N,K);   // function call of hField
    fieldP=(char**)malloc(N*sizeof(char*));     //Memory allocation for fieldP grid.
    for(i=0;i<N;i++){
        fieldP[i]=(char*)malloc(K*sizeof(char));
        if (fieldP[i] == NULL)	{
			printf("Not enough memory available to store the data!\nThe program will stop running...");
			exit(0);
		}
    }
    for(i=0;i<N;i++){       //Fill fieldP with the character '.' (dot).
        for(j=0;j<K;j++){
            fieldP[i][j]='.';
        }
    }
    fieldP2=(char**)malloc(N*sizeof(char*));    //Memory allocation for fieldP2.
    for(i=0;i<N;i++){
        fieldP2[i]=(char*)malloc(K*sizeof(char));
        if (fieldP2[i] == NULL)	{
			printf("Not enough memory available to store the data!\nThe program will stop running...");
			exit(0);
		}
    }
    for(i=0;i<N;i++){       //Fill the fieldP2 with the character '.' (dot).
        for(j=0;j<K;j++){
            fieldP2[i][j]='.';
        }
    }

    mode=GameMode (N, K);   //function call of GameMode that return an integer for the mode.

    start=RandomNumbers(0,1);   // Here determine tje player who start first head(0) for p1 and tails(1) for p2
    if (start==0)
        printf("\n%s start first",Player1);
    else
        printf("\n%s start first",Player2);

    if (mode==1){      //This mode is for Player vs CPU
        if (start==0){ //player 1 starts
            for(i=1;i<=lastRound;i++){
                if (i%2==1) //When the round is an odd number Player is playing and at an even round CPU is playing.
                    PlayerTurn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
                else
                    CPUturn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
            }
        }
        else{
            for(i=1;i<=lastRound;i++){
                if (i%2==1) // Here happens the opposite from before because CPU start first.
                    CPUturn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
                else
                    PlayerTurn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
            }
        }
    }
    if (mode==2){   //This mode is for Player vs Player
        if(start==0){
            for (i=1;i<=lastRound;i++){
                if (i%2==1)
                    PlayerTurn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
                else
                    PlayerTurn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
            }
        }
        else{
            for (i=1;i<=lastRound;i++){
                if (i%2==1)
                    PlayerTurn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
                else
                    PlayerTurn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
            }
        }
    }
    if (mode==3){   //This mode is for CPU vs CPU
        if(start==0){
            for (i=1;i<=lastRound;i++){
                if (i%2==1)
                    CPUturn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
                else
                    CPUturn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
            }
        }
        else{
            for (i=1;i<=lastRound;i++){
                if (i%2==1)
                    CPUturn(N,K,hfieldP,fieldP,Player2,ConditionsArray1);
                else
                    CPUturn(N,K,hfieldP2,fieldP2,Player1,ConditionsArray2);
            }
        }
    }
    /*There is only 1 loop for each mode running from the above and if the loop ends while currentRound=lastRound(last round is predefined from the user, else the program end in a different way) the game ends and the function End is called*/
    End(N,K);

return 0;
}

/*-------------------Functions-----------------*/

void TextArt(void){ //welcome message and an awesome text art!!!
    char ch;
    printf("\n\t\t\tWelcome to the game 'Battleship' !!!\n\n");
    printf("\n                                     # #  ( )");
    printf("\n                                  ___#_#___|__");
    printf("\n                               _  |____________|  _");
    printf("\n                        _=====| | |            | | |==== _");
    printf("\n                  =====| |.---------------------------. | |====");
    printf("\n    <--------------------'   .  .  .  .  .  .  .  .   '--------------/");
    printf("\n     \\                                                              /");
    printf("\n      \\________________________________________BATTLESHIP__________/");
    printf("\n   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
    printf("\n wwwwwwwwwwwwww CST11068 wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
    printf("\n    wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");
		do{
			printf("\nDo you want to continue, y/n? ");
			scanf("%s", &ch);
		}while ((ch != 'y') && (ch != 'n'));       //ask the use if he want to start the game or not!
		if (ch=='n')
            exit(0);
}

int  RandomNumbers (int min, int max){      // This function has 2 arguments a min number and a max, this function generate and return a random number at [min,max].
     return (rand()%(max-min+1)+min); // (rand()%(MAX-MIN+1)+MIN)
}

void hFields(int N, int K){     // This function allocates memory for hfields and filling them with dot ( . ) character
    int i,j;

    hfieldP=(char**)malloc(N*sizeof(char*));
    for(i=0;i<N;i++){
        hfieldP[i]=(char*)malloc(K*sizeof(char));
        if (hfieldP[i] == NULL)	{   //check if memory is available to store the data
			printf("Not enough memory available to store the data!\nThe program will stop running...");
			exit(0);
		}
    }
    for(i=0;i<N;i++){
        for(j=0;j<K;j++){
            hfieldP[i][j]='.';
        }
    }

    hfieldP2=(char**)malloc(N*sizeof(char*));
    for(i=0;i<N;i++){
        hfieldP2[i]=(char*)malloc(K*sizeof(char));
        if (hfieldP2[i] == NULL)	{
			printf("Not enough memory available to store the data!\nThe program will stop running...");
			exit(0);
        }
    }
    for(i=0;i<N;i++){
        for(j=0;j<K;j++){
            hfieldP2[i][j]='.';
        }
    }

    return;
}

void GetCoords( int N, int K,char** field,char** hfield,int ConditionsArray[5][4]){ //read coords and convert them or users choice
    char target[4];
    int temp1,temp2;
    char temp;
    temp=(N-1)+'A';     //conver the max size from an integer to char(just to print),ex x=5 temp=F

    do{
        printf("\nInput Allowed:[A:%c][1:%d]   i.e B12\n",temp,K);
        scanf("%s",&target);    //read user's input
        if (target[1]=='\0'){ //if user give only letter, program check if it is h or x
            if (target[0]=='h' && Cround!=0){    //if its h program call the function help
                Help(N,K,field,hfield,ConditionsArray);
            }
            if (target[0]=='x'){    //if its x program call the function end
                End(N,K);
            }
        }   //if user give normal coordinates the program convert the letter and the number to coordinates on an array
        x=toupper(target[0])-'A';  // x coordinate = (ASCII of the letter that user gave) - (ASCII of A character)

        if(target[2]==' ' || target[2]=='\0' )  //checks if user gave a 2 digit number or with 1 digit
            y=target[1]-'0'-1;      //y coordinate = 1st digit - '0'
        else {
            temp1=target[1]-'0';
            temp2=target[2]-'0';
            y=temp1*10+(temp2-1);   // y coordinate = (1digit -'0')*10 + (2nd digit -1 - '0')
            }
    }while(x<0 || x>N-1 || y<0 || y>K-1);   //if user give wrong numbers the program checks it and asks user to give his choice again

}

void PrintField(int N, int K, char** field){    //print any grid the program needs
    int i,j;

     printf("\n");
     printf("\n");
     printf("  ");

     for(j=0;j<K;j++){  //print the numbering of the grid's width
        printf("%3d",j+1);
     }

    printf("\n");
     for(j=0;j<(3.2*K);j++){ //3.2 ideal ratio to multiplied with the width of the grid at this size K[5,32] and print the perfect number of times the character hyphen(-) is need
        printf("-");
     }

    for(i=0;i<N;i++) {  //print the numbering with letters of the grid's length
        printf("\n");
        printf("%c|",'A'+i);
        for(j=0;j<K;j++){
            printf("%3c",field[i][j]);
        }
    }

     printf("\n");
     printf("\n");
     return;
}

void ManuallyPlace(int N, int K, int size, char shipCh, char shipN[], char** field){    //this function let user place his ships as he wants

	int i,j;
    int counter;
    char kat;
    do{

        printf("Give the coords of %s 's bow.\n",shipN);
        GetCoords(N, K, fieldP,hfieldP,ConditionsArray1 );

        do{
            printf("\n\nPlease, give direction of the ship.\nFor North(up%c) press 'n',  for South(down%c) press 's',  for East(right%c) press 'e' and  for West(left%c) press 'w':",24,25,26,27);
            scanf("%s",&kat);
            counter=0;
        }while((kat!='n') && (kat!='s') && (kat!='e') && (kat!='w'));

        switch (kat){   //this switch command checks if the ships is within the game grid and not on the top of other ships depending on the direction that user gives
            //North
            case 'n' :
                    if(x+1>=size){
                        for (i=0;i<size;i++){
                            if(field[x-i][y]=='.'){ //checks if the positions of the ship were the user want to place are filled with dot and re not out of the grid then program places the ship
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(i=0;i<size;i++)
                                field[x-i][y]=shipCh;
                        }
                        else{
                           printf("Do not place ships on top of each other\n\n");
                        }
                    }
                    else{
                        printf("Place your ships within the game grid!!!\a\n\n");
                    }
            break;

            //South
            case 's' :
                    if(N-x>=size){
                        for (i=0;i<size;i++){
                            if(field[x+i][y]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(i=0;i<size;i++){
                                field[x+i][y]=shipCh;
                            }
                        }
                        else{
                           printf("Do not place ships on top of each other\n\n");
                        }
                    }
                    else{
                        printf("Place your ships within the game grid!!!\a\n\n");
                    }
            break;

            //East
            case 'e' :
                    if(K-y>=size){
                        for (j=0;j<size;j++){
                            if(field[x][y+j]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(j=0;j<size;j++){
                                field[x][y+j]=shipCh;
                            }
                        }
                        else{
                           printf("Do not place ships on top of each other\n\n");
                        }
                    }
                    else{
                        printf("Place your ships within the game grid!!!\a\n\n");
                    }
            break;

            //West
            case 'w' :
                    if(y+1>=size){
                        for (j=0;j<size;j++){
                            if(field[x][y-j]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(j=0;j<size;j++){
                                field[x][y-j]=shipCh;
                            }
                        }
                        else{
                           printf("Do not place ships on top of each other\n\n");
                        }
                    }
                    else{
                        printf("Place your ships within the game grid!!!\a\n\n");
                    }
            break;
        }
    }while(counter!=size);
return ;
}

void RandomPlace(int N, int K, int size, char shipCh, char** field){    //this function randomly places  his ships

	int x, y,i,j;
    int counter,temp;
    char kat;
    do{


        x= RandomNumbers(0,N-1);    //generate random numbers for x and y coordinates were the ship's bow
		y= RandomNumbers(0,K-1);

        do{
            temp=RandomNumbers(0,3);    //generate random number between 0-3 for the ships direction
			if(temp==0)
				kat='n';
			if(temp==1)
				kat='s';
			if(temp==2)
				kat='e';
			if(temp==3)
				kat='w';


            counter=0;
        }while((kat!='n') && (kat!='s') && (kat!='e') && (kat!='w'));

        switch (kat){
            //North
            case 'n' :
                    if(x+1>=size){
                        for (i=0;i<size;i++){
                            if(field[x-i][y]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(i=0;i<size;i++)
                                field[x-i][y]=shipCh;
                        }
                    }
            break;

            //South
            case 's' :
                    if(N-x>=size){
                        for (i=0;i<size;i++){
                            if(field[x+i][y]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(i=0;i<size;i++){
                                field[x+i][y]=shipCh;
                            }
                        }
                    }
            break;

            //East
            case 'e' :
                    if(K-y>=size){
                        for (j=0;j<size;j++){
                            if(field[x][y+j]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(j=0;j<size;j++){
                                field[x][y+j]=shipCh;
                            }
                        }
                    }
            break;

            //West
            case 'w' :
                    if(y+1>=size){
                        for (j=0;j<size;j++){
                            if(field[x][y-j]=='.'){
                                counter+=1;
                            }
                        }
                        if(counter==size){
                            for(j=0;j<size;j++){
                                field[x][y-j]=shipCh;
                            }
                        }
                    }
            break;
        }
    }while(counter!=size);  // this function ends when the ships is correctly placed on the grid, while checking if the counter=size of the ship
return ;
}

int GameMode(int N, int K){     //This function sets the game mode (PvP,PvAI,AIvAI) and sets game mode between a game with predefined rounds and a game that ends.
    int gmode, mode, place;
 do{
        printf("\n1. First Mode: Player vs CPU.");
        printf("\n2. Second Mode: Player vs Player.");
        printf("\n3. Third Mode: CPU vs CPU.");
        printf("\nSelect Mode:");
        scanf("%d",&mode);
    }while(mode!=1 && mode !=2 && mode!=3);

    if (mode==1){
        printf("\nPlease Enter Your Name: ");   //if player is being played by computer the name of this player is CPU
        scanf("%s",&Player1);
        Player2[0]='C';
        Player2[1]='P';
        Player2[2]='U';
        Player2[3]='\0';
    }
    if (mode==2){
        printf("\nPlease Enter First Player Name: ");
        scanf("%s",&Player1);
        printf("\nPlease Enter  Second Player Name: ");
        scanf("%s",&Player2);
    }
    if (mode==3){
        Player1[0]='C';
        Player1[1]='P';
        Player1[2]='U';
        Player1[3]='_';
        Player1[4]='1';
        Player1[5]='\0';

        Player2[0]='C';
        Player2[1]='P';
        Player2[2]='U';
        Player2[3]='_';
        Player2[4]='2';
        Player2[5]='\0';
    }

    do{
        printf("\n1. First Mode: Game ends when all ships of one player are sunk.");
        printf("\n2. Second Mode: Game ends after some predefined rounds.(that you gave)");
        printf("\nSelect Game Mode:");
        scanf("%d",&gmode);
    }while(gmode!=1 && gmode !=2);  //if gmode=1 the game end if all ships of a player are sunk and if gmode=2 the game end after the round that user gave.
    if (gmode==1)
        lastRound=10*(N*K);
    else{
        printf("\nGive the number of rounds you want to play:");
        scanf("%d",&lastRound);
    }

    if(mode==1){
        do{
            printf("\n%s do you want to place your ships 1. Manually or 2.Randomly?:",Player1);
            scanf("%d",&place);
        }while(place!=1 && place!=2);

        printf("\nThis is the grid of %s:\n",Player1);
        PrintField(N,K,hfieldP);
        if (place==1){  //call the functions ManuallyPlace or RandomPlace for each ship for each player who want to place his ships on the grid, this depends on user's choice(for computer is only RandomPlace).
            ManuallyPlace(N,K, 5, 'a' ,"Aircraft Carrier",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 4, 'b' ,"Battleship",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 3, 'd' ,"Destroyer",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 3, 's' ,"Submarine",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 2, 'c' ,"Cruiser",hfieldP);
        }
        else{
            RandomPlace(N,K,5,'a',hfieldP);
            RandomPlace(N,K,4,'b',hfieldP);
            RandomPlace(N,K,3,'d',hfieldP);
            RandomPlace(N,K,3,'s',hfieldP);
            RandomPlace(N,K,2,'c',hfieldP);
        }
        PrintField(N,K,hfieldP);

        RandomPlace(N,K,5,'a',hfieldP2);
        RandomPlace(N,K,4,'b',hfieldP2);
        RandomPlace(N,K,3,'d',hfieldP2);
        RandomPlace(N,K,3,'s',hfieldP2);
        RandomPlace(N,K,2,'c',hfieldP2);

    }
    if(mode==2){
        do{
            printf("\n%s do you want to place your ships 1. Manually or 2.Randomly?:",Player1);
            scanf("%d",&place);
        }while(place!=1 && place!=2);

        printf("\nThis is the grid of %s:\n",Player1);
        PrintField(N,K,hfieldP);
        if (place==1){
            ManuallyPlace(N,K, 5, 'a' ,"Aircraft Carrier",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 4, 'b' ,"Battleship",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 3, 'd' ,"Destroyer",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 3, 's' ,"Submarine",hfieldP);
            PrintField(N,K,hfieldP);
            ManuallyPlace(N,K, 2, 'c' ,"Cruiser",hfieldP);
        }
        else{
            RandomPlace(N,K,5,'a',hfieldP);
            RandomPlace(N,K,4,'b',hfieldP);
            RandomPlace(N,K,3,'d',hfieldP);
            RandomPlace(N,K,3,'s',hfieldP);
            RandomPlace(N,K,2,'c',hfieldP);
        }
        PrintField(N,K,hfieldP);

        do{
            printf("\n%s do you want to place your ships 1. Manually or 2.Randomly?:",Player2);
            scanf("%d",&place);
        }while(place!=1 && place!=2);

        printf("\nThis is the grid of %s:\n",Player2);
        PrintField(N,K,hfieldP2);
        if (place==1){
            ManuallyPlace(N,K, 5, 'a' ,"Aircraft Carrier",hfieldP2);
            PrintField(N,K,hfieldP2);
            ManuallyPlace(N,K, 4, 'b' ,"Battleship",hfieldP2);
            PrintField(N,K,hfieldP2);
            ManuallyPlace(N,K, 3, 'd' ,"Destroyer",hfieldP2);
            PrintField(N,K,hfieldP2);
            ManuallyPlace(N,K, 3, 's' ,"Submarine",hfieldP2);
            PrintField(N,K,hfieldP2);
            ManuallyPlace(N,K, 2, 'c' ,"Cruiser",hfieldP2);
        }
        else{
            RandomPlace(N,K,5,'a',hfieldP2);
            RandomPlace(N,K,4,'b',hfieldP2);
            RandomPlace(N,K,3,'d',hfieldP2);
            RandomPlace(N,K,3,'s',hfieldP2);
            RandomPlace(N,K,2,'c',hfieldP2);
        }
        PrintField(N,K,hfieldP2);
    }
    if (mode==3){

        RandomPlace(N,K,5,'a',hfieldP);
        RandomPlace(N,K,4,'b',hfieldP);
        RandomPlace(N,K,3,'d',hfieldP);
        RandomPlace(N,K,3,'s',hfieldP);
        RandomPlace(N,K,2,'c',hfieldP);

        RandomPlace(N,K,5,'a',hfieldP2);
        RandomPlace(N,K,4,'b',hfieldP2);
        RandomPlace(N,K,3,'d',hfieldP2);
        RandomPlace(N,K,3,'s',hfieldP2);
        RandomPlace(N,K,2,'c',hfieldP2);

    }
    return mode;

}

void PlayerTurn(int N, int K,char** hfield,char** field,char player[],int ConditionsArray[5][4]){   //This function is used at a "human-user"'s turn.
    int Sflag;
    Cround+=1;  //each time at start of someone turn the round increased by 1
    do{
        Sflag=0;    //each time at start of the loop flag take the number 0
        printf("\n\nGive coords were you want to shoot:");
        GetCoords(N,K,field,hfield,ConditionsArray);    //read coordinate that user want to shoot

        if (hfield[x][y]=='o' || hfield[x][y]=='A' || hfield[x][y]=='B' || hfield[x][y]=='S' || hfield[x][y]=='C' || hfield[x][y]=='D'){
            Sflag=2;    //if player shoot a place again flag takes number 2
        }
        if (hfield[x][y]!='o' && hfield[x][y]!='.'&& hfield[x][y]!='A' && hfield[x][y]!='B' && hfield[x][y]!='S' && hfield[x][y]!='C' && hfield[x][y]!='D'){
            hfield[x][y]=toupper(hfield[x][y]);
            field[x][y]='x';
            Sflag=1;    //if player hit something he play again
        }
        if(hfield[x][y]=='.'){
            hfield[x][y]='o';
            field[x][y]='o';
        }
        Refresh(N,K,hfield,field,ConditionsArray);  //after player has shoot program call those 3 functions.
        PrintField(N,K,field);
        RoundMessage(N,K,hfield,field,player,Sflag,ConditionsArray);
    }while(Sflag==1);
}

void CPUturn(int N, int K,char** hfield,char** field,char player[],int ConditionsArray[5][4]){  //This function is used at computer turn.
    int Sflag;
    Cround+=1;  //each time at start of someone turn the round increased by 1
    do{
        Sflag=0;
        x=RandomNumbers(0,N-1);     //random coordinates to shoot
        y=RandomNumbers(0,K-1);

        if(hfield[x][y]!='.' && hfield[x][y]!='o'&& hfield[x][y]!='A' && hfield[x][y]!='B' && hfield[x][y]!='S' && hfield[x][y]!='C' && hfield[x][y]!='D'){ //if cpu hit something then flag=1
            hfield[x][y]=toupper(hfield[x][y]);
            field[x][y]='x';
            Sflag=1;
        }
        else if(hfield[x][y]=='o' || hfield[x][y]=='A' || hfield[x][y]=='B' || hfield[x][y]=='S' || hfield[x][y]=='C' || hfield[x][y]=='D'){
            Sflag=2;
        }
        else{
            hfield[x][y]='o';
            field[x][y]='o';
        }
        Refresh(N,K,hfield,field,ConditionsArray);
        PrintField(N,K,field);
        RoundMessage(N,K,hfield,field,player,Sflag,ConditionsArray);

        if (hfield[x][y]!='.' && hfield[x][y]!='o')
            Sflag=SmartTarget( x, y, N, K, field, hfield,player,ConditionsArray);   //if cpu hit something then play again and hit close of his last shot by using this function
    }while(Sflag==1);
}

int SmartTarget(int x, int y, int N, int K,char** field ,char** hfield,char player[],int ConditionsArray[5][4]){    //This function is used when the computer is playing for second time in a row.
    int temp,Sflag=0;
    if((x!=0 && x!=N-1) && (y!=0 && y!=K-1)){   // some cases were  previous shot of cpu are at the corners of the grid ([0,0][0,K] etc and near the edge of the grid
        temp=RandomNumbers(0,3);
        if (temp==0){//north
            x2=x-1;
            y2=y;
        }
        if (temp==1){//south
            x2=x+1;
            y2=y;
        }
        if (temp==2){//east
            x2=x;
            y2=y+1;
        }
        if (temp==3){//west
            x2=x;
            y2=y-1;
        }
    }
    if (x==0){
        if(y==0){   //(0,0)
            temp=RandomNumbers(0,1);
            if(temp==0){//south
                x2=x+1;
                y2=y;
            }
            if(temp==1){//east
                x2=x;
                y2=y+1;
            }
        }
        else if (y==K-1){   //(0,K)
            temp=RandomNumbers(0,1);
            if (temp==0){//south
                x2=x+1;
                y2=y;
            }
            if (temp==1){//west
                x2=x;
                y2=y-1;
            }
        }
        else{   // (0, 1....K-1)
            temp=RandomNumbers(0,2);
            if (temp==0){//south
                x2=x+1;
                y2=y;
            }
            if (temp==1){//west
                x2=x;
                y2=y-1;
            }
            if (temp==2){//east
                x2=x;
                y2=y+1;
            }
        }
    }
    else if (y==0){
        if (x==N-1){    //(N,0)
            if (temp==0){//north
                x2=x-1;
                y2=y;
            }
            if (temp==1){//east
                x2=x;
                y2=y+1;
            }
        }
        else{   //(1...N-1, 0)
            temp=RandomNumbers(0,2);
            if (temp==0){//north
                x2=x-1;
                y2=y;
            }
            if (temp==1){//south
                x2=x+1;
                y2=y;
            }
            if (temp==2){//east
                x2=x;
                y2=y+1;
            }
        }
    }
    else if (x==N-1){
        if(y==K-1){      //(N,K)
            temp=RandomNumbers(0,1);
            if (temp==0){//north
                x2=x-1;
                y2=y;
            }
            if (temp==1){//west
                x2=x;
                y2=y-1;
            }
        }
        else{   // (N, 1....K-1)
        temp=RandomNumbers(0,2);
        if (temp==0){//north
            x2=x-1;
            y2=y;
        }
        if (temp==1){//west
            x2=x;
            y2=y-1;
        }
        if (temp==2){//east
            x2=x;
            y2=y+1;
        }
        }
    }
    else if (y==K-1){   // ( 1.....N-1, K)
        temp=RandomNumbers(0,2);
        if (temp==0){//north
            x2=x-1;
            y2=y;
        }
        if (temp==1){//south
            x2=x+1;
            y2=y;
        }
        if (temp==2){//west
            x2=x;
            y2=y-1;
        }
    }
    if (hfield[x2][y2]=='o' || hfield[x2][y2]=='A' || hfield[x2][y2]=='B' || hfield[x2][y2]=='S' || hfield[x2][y2]=='C' || hfield[x2][y2]=='D')
        Sflag=2;        ////if cpu shoot a place again flag takes number 2
    if (hfield[x2][y2]!='o' && hfield[x2][y2]!='.'&& hfield[x][y]!='A' && hfield[x][y]!='B' && hfield[x][y]!='S' && hfield[x][y]!='C' && hfield[x][y]!='D'){
        hfield[x2][y2]=toupper(hfield[x2][y2]);
        field[x2][y2]='x';
        Sflag=1;        //after cpu has shoot program call those 3 functions.
    }
    if(hfield[x2][y2]=='.'){
        hfield[x2][y2]='o';
        field[x2][y2]='o';
    }
    //////////////
    x=x2;
    y=y2;
    Refresh(N,K,hfield,field,ConditionsArray);
    PrintField(N,K,field);
    RoundMessage(N,K,hfield,field,player,Sflag,ConditionsArray);
    return Sflag;
}

void Refresh(int N,int K,char** hfield, char** field,int ConditionsArray[5][4]){    //This function scans the grids and refreshes them if ships have been struck or sunk and save the round that a ship sunk. this happens with the use of ConditionsArray
    int i,j,A=0,B=0,S=0,D=0,C=0;
    for (i=0;i<N;i++){
        for(j=0;j<K;j++){       //check if at least 1 part of each ship i destroyed, this is usefull for help function
            if(hfield[i][j]=='A'){
                A+=1;
                ConditionsArray[0][1]=1;
            }
            if(hfield[i][j]=='B'){
                B+=1;
                ConditionsArray[1][1]=1;
            }
            if(hfield[i][j]=='S'){
                S+=1;
                ConditionsArray[2][1]=1;
            }
            if(hfield[i][j]=='D'){
                D+=1;
                ConditionsArray[3][1]=1;
            }
            if(hfield[i][j]=='C'){
                C+=1;
                ConditionsArray[4][1]=1;
            }
        }
    }
    if (A==5){  //check if each ship is sunk
        ConditionsArray[0][0]=1;
        if(ConditionsArray[0][2]==0){   //save ONLY 1st time the round that the ship is sunk
            ConditionsArray[0][2]=Cround;
            if (ConditionsArray[0][3]==0)   //check if it has printed that this ship is destroyed ( it has to print only once at the time that sunk)
                ConditionsArray[0][3]=1;
        }
        for (i=0;i<N;i++){
            for(j=0;j<K;j++){
                if (hfield[i][j]=='A')
                    field[i][j]='A';
            }
        }
    }
    if (B==4){
        ConditionsArray[1][0]=1;
        if(ConditionsArray[1][2]==0){
            ConditionsArray[1][2]=Cround;
            if (ConditionsArray[1][3]==0)
                ConditionsArray[1][3]=1;
        }
        for (i=0;i<N;i++){
            for(j=0;j<K;j++){
                if (hfield[i][j]=='B')
                    field[i][j]='B';
            }
        }
    }
    if (S==3){
        ConditionsArray[2][0]=1;
        if(ConditionsArray[2][2]==0){
            ConditionsArray[2][2]=Cround;
            if (ConditionsArray[2][3]==0)
                ConditionsArray[2][3]=1;
        }
        for (i=0;i<N;i++){
            for(j=0;j<K;j++){
                if (hfield[i][j]=='S')
                    field[i][j]='S';
            }
        }
    }
    if (D==3){
        ConditionsArray[3][0]=1;
        if(ConditionsArray[3][2]==0){
            ConditionsArray[3][2]=Cround;
            if (ConditionsArray[3][3]==0)
                ConditionsArray[3][3]=1;
        }
        for (i=0;i<N;i++){
            for(j=0;j<K;j++){
                if (hfield[i][j]=='D')
                    field[i][j]='D';
            }
        }
    }
    if (C==2){
        ConditionsArray[4][0]=1;
        if(ConditionsArray[4][2]==0){
            ConditionsArray[4][2]=Cround;
            if (ConditionsArray[4][3]==0)
                ConditionsArray[4][3]=1;
        }
        for (i=0;i<N;i++){
            for(j=0;j<K;j++){
                if (hfield[i][j]=='C')
                    field[i][j]='C';
            }
        }
    }

}

void RoundMessage(int N,int K,char** hfield, char** field,char player[],int Sflag,int ConditionsArray[5][4]){   //This function prints some messages right after the grid that shows this rounds situation.
    char target[4];

    target[0]='A'+x;    //convert from the programing array's coordinates to normal coordinates for print the right message
    if(y<10){
        target[1]=y+1+'0';
        target[2]='\0';
    }
    else{
        target[1]=(y/10)+'0'+1;
        target[2]=(y%10)+'0'+1;
        target[3]='\0';

    }

    ScoreCounter(N,K);
    printf("\nRound %d, %s is playing!",Cround,player);
    printf("\n%s: %d points,%s: %d points",Player1,points1,Player2,points2);
    if (Sflag==0){
        printf("\nMissed!");
    }
    else if(Sflag==1){  //if flag=1 then player hit something and then checks if there is any ship sinking at the current round
            if(Cround==ConditionsArray[0][2] && ConditionsArray[0][3]==1){
                    printf("\nA Aircraft Carrier is sinking!!!");
                    ConditionsArray[0][3]=2;
            }
            if(Cround==ConditionsArray[1][2] && ConditionsArray[1][3]==1){
                    printf("\nA Battlefield is sinking!!!");
                    ConditionsArray[1][3]=2;
            }
            if(Cround==ConditionsArray[2][2] && ConditionsArray[2][3]==1){
                    printf("\nA Submarine is sinking!!!");
                    ConditionsArray[2][3]=2;
            }
            if(Cround==ConditionsArray[3][2] && ConditionsArray[3][3]==1){
                    printf("\nA Destroyer is sinking!!!");
                    ConditionsArray[3][3]=2;
            }
            if(Cround==ConditionsArray[4][2] && ConditionsArray[4][3]==1){
                    printf("\nA Cruiser is sinking!!!");
                    ConditionsArray[4][3]=2;
            }

            printf("\nShip attack at %s!",target);  //prints the coordinates were player hit something
    }
    else if(Sflag==2){
        printf("\nAlready shoot here!");
    }
    printf("\n");
}

void ScoreCounter(int N, int K){    //This function counts the score of each player and ends the program if all the ships of 1 player are sunk
    points1=0;
    points2=0;
    if (ConditionsArray1[0][0]==1)
        points2+=1;
    if (ConditionsArray1[1][0]==1)
        points2+=2;
    if (ConditionsArray1[2][0]==1)
        points2+=4;
    if (ConditionsArray1[3][0]==1)
        points2+=4;
    if (ConditionsArray1[4][0]==1)
        points2+=5;

    if (ConditionsArray2[0][0]==1)
        points1+=1;
    if (ConditionsArray2[1][0]==1)
        points1+=2;
    if (ConditionsArray2[2][0]==1)
        points1+=4;
    if (ConditionsArray2[3][0]==1)
        points1+=4;
    if (ConditionsArray2[4][0]==1)
        points1+=5;

    if(points1==16 || points2==16)
        End(N,K);

}

void Help(int N, int K,char** field,char** hfield,int ConditionsArray[5][4]){   //This function is used for HELP chose when user types the character 'h'.
    int temp=0,rn,i,j,ship=0;
    Refresh(N,K,hfield,field,ConditionsArray);
    for (i=0;i<5;i++){
        temp+=ConditionsArray[i][1];
    }
    if (temp==5)        //if all ships have been struck then player cant use help
        printf("All ships have been struck! You can't use help.");
    else{
        do{     //if there is and ship that is hasn't been struck then randomly the low character of the ship (ex. 'a' for aircraft carrier) is replaced at all the positions
            rn=RandomNumbers(0,4);
                if(rn==0){
                    if(ConditionsArray[0][1]==0)
                        ship=1;
                }
                if(rn==1){
                    if(ConditionsArray[1][1]==0)
                        ship=2;
                }
                if(rn==2){
                    if(ConditionsArray[2][1]==0)
                        ship=3;
                }
                if(rn==3){
                    if(ConditionsArray[3][1]==0)
                        ship=4;
                }
                if(rn==4){
                    if(ConditionsArray[4][1]==0)
                        ship=5;
                }
        }while(ship==0);

        if(ship==1){
            for(i=0;i<N;i++){
                for(j=0;j<K;j++){
                    if(hfield[i][j]=='a')
                        field[i][j]='a';
                }
            }
        }
        if(ship==2){
            for(i=0;i<N;i++){
                for(j=0;j<K;j++){
                    if(hfield[i][j]=='b')
                        field[i][j]='b';
                }
            }
        }
        if(ship==3){
            for(i=0;i<N;i++){
                for(j=0;j<K;j++){
                    if(hfield[i][j]=='s')
                        field[i][j]='s';
                }
            }
        }
        if(ship==4){
            for(i=0;i<N;i++){
                for(j=0;j<K;j++){
                    if(hfield[i][j]=='d')
                        field[i][j]='d';
                }
            }
        }
        if(ship==5){
            for(i=0;i<N;i++){
                for(j=0;j<K;j++){
                    if(hfield[i][j]=='c')
                        field[i][j]='c';
                }
            }
        }
        PrintField(N,K,field);      //after the use of help the field recovers to the previous form
        for(i=0;i<N;i++){
            for(j=0;j<K;j++){
                if(hfield[i][j]=='a' || hfield[i][j]=='b' || hfield[i][j]=='c' || hfield[i][j]=='d' || hfield[i][j]=='s')
                    field[i][j]='.';
                }
            }
    }
}

void End(int N, int K){     //This function is a set of messages that shows the situation at the end(who won etc).
    int j;
    printf("\n\n");
    for(j=0;j<(3.2*K);j++){     //3.2 ideal ratio to multiplied with the width of the grid at this size K[5,32] and print the perfect number of times the character equal(=) is need
        printf("=");
     }
    printf("\nThe grid of %s:",Player1);
    PrintField(N,K,hfieldP2);
    printf("\nThe grid of %s:",Player2);
    PrintField(N,K,hfieldP);    //last use of arrays
    FreeMemory(N,K);
    if(points1==points2){
        printf("\nThere is no winner. :(");
        printf("\n%s: %d points, %s: %d points",Player1,points1,Player2,points2);
        printf("\nDRAW!!!!");
    }
    if(points1>points2){
        printf("\n");
        printf("\n  __        _____ _   _ _   _ _____ ____  ");
        printf("\n  \\ \\      / /_ _| \\ | | \\ | | ____|  _ \\ ");
        printf("\n   \\ \\ /\\ / / | ||  \\| |  \\| |  _| | |_) |");
        printf("\n    \\ V  V /  | || |\\  | |\\  | |___|  _ <  ");
        printf("\n     \\_/\\_/  |___|_| \\_|_| \\_|_____|_| \\_\\ ");
        printf("\n\n\tThe winner is %s!!!",Player1);
        printf("\n\t%s: %d points, %s: %d points",Player1,points1,Player2,points2);
    }
    if(points1<points2){
        printf("\n");
        printf("\n  __        _____ _   _ _   _ _____ ____  ");
        printf("\n  \\ \\      / /_ _| \\ | | \\ | | ____|  _ \\ ");
        printf("\n   \\ \\ /\\ / / | ||  \\| |  \\| |  _| | |_) |");
        printf("\n    \\ V  V /  | || |\\  | |\\  | |___|  _ <  ");
        printf("\n     \\_/\\_/  |___|_| \\_|_| \\_|_____|_| \\_\\ ");
        printf("\n\n\tThe winner is %s!!!",Player2);
        printf("\n\t%s: %d points, %s: %d points",Player1,points1,Player2,points2);
    }

    printf("\n\n\n\nThank you for playing!!!!!!!!!");
    printf("\n\n\t\tCREDITS");
    printf("\n\nCREATED BY:");
    printf("\n\nPlessias Alexandros (cst11068@uop.gr)");
    printf("\n");
    exit(0);
}

void FreeMemory(int N,int K){   //After last use of the arrays that we have allocate in memory we use this function to free this memory.
    int i;

    for(i=0;i<N;i++){
        free(fieldP[i]);
        free(fieldP2[i]);
        free(hfieldP[i]);
        free(hfieldP2[i]);
    }
    free(fieldP);
    free(fieldP2);
    free(hfieldP);
    free(hfieldP2);
}

/*

  --------------ConditionsArray-------------
  (0,1) for sunk, help, print
  (0,....) round
                   |sunk|help|round|print|
------------------------------------------
 Aircraft Carrier  |    |    |     |     |
 Battleship        |    |    |     |     |
 Submarine         |    |    |     |     |
 Destroyer         |    |    |     |     |
 Cruiser           |    |    |     |     |
 -----------------------------------------

*/
