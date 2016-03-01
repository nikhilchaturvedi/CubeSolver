/**
A 2x2x2 Cube Solver
@Author Nikhil Chaturvedi, San Jose State University
*/

#include <iostream>
#include <stdio.h>
#include "cube_solver.h"
#include<math.h>
#include<stdlib.h>


/**
These arrays hold the colours of the finished cube
used to check the finish condition
*/
char yellow_side[4] = {'y', 'y',
                      'y', 'y'};

char white_side[4] = {'w', 'w',
                      'w', 'w'};

char blue_side[4] = {'b', 'b',
                     'b', 'b'};

char red_side[4] = {'r', 'r',
                    'r', 'r'};

char green_side[4] = {'g', 'g',
                      'g', 'g'};

char orange_side[4] = {'o', 'o',
                       'o', 'o'};

/**
These arrays hold the
colours of the faces of the shuffled cube
*/
char read_up_side[CUBE_ARRAY_SIZE] = {'\0'};
char read_down_side[CUBE_ARRAY_SIZE] = {'\0'};
char read_left_side[CUBE_ARRAY_SIZE] = {'\0'};
char read_right_side[CUBE_ARRAY_SIZE] = {'\0'};
char read_front_side[CUBE_ARRAY_SIZE] = {'\0'};
char read_back_side[CUBE_ARRAY_SIZE] = {'\0'};
using namespace std;

std::string initial_cube_string = "";

/**
Function to read a cube and parse its faces and
fill the arrays that hold the shuffled cube faces
*/
void read_and_parse_cube()
{

}

void cube_top_face_decide()
{
    if(yellow_side[0] == 'y' && yellow_side[1] == 'y' &&
       yellow_side[2] == 'y' && yellow_side[3] == 'y')
    {
        //the top face is always chosen to be yellow
        //we have solved the top face. update the solve stage and return
        stage = 2; //goto the next stage
    }
    else if(stage == 1)
    {
        //check if the top is done along with the alignment with the corner edges.

    }
}

void parse_initial_cube()
{
    int index = 0;

    printf("Getting the yellow side\n");
    for(index = 0; index < 4; index++)
    {
        yellow_side[index] = initial_cube_string[index];
    }

    printf("Getting the white side\n");
    index = 4;
    for(index = 4; index < 8; index++)
    {
        white_side[index - 4] = initial_cube_string[index];
    }

    printf("Getting the red side\n");
    index = 8;
    for(index = 8; index < 12; index++)
    {
        red_side[index - 8] = initial_cube_string[index];
    }

    printf("Getting the blue side\n");
    index = 12;
    for(index = 12; index < 16; index++)
    {
        blue_side[index - 12] = initial_cube_string[index];
    }

    printf("Getting the green side\n");
    index = 16;
    for(index = 16; index < 20; index++)
    {
        green_side[index - 16] = initial_cube_string[index];
    }

    printf("Getting the orange side\n");
    index = 20;
    for(index = 20; index < 24; index++)
    {
        orange_side[index - 20] = initial_cube_string[index];
    }
}
int adj[6][6] = {0};
/*int posit[24] = {2,2,2,2,
				1,1,1,1,
				0,0,0,0,
				3,3,3,3,
				4,4,4,4,
				5,5,5,5};*/
/*Yellow - 1
  Orange - 2
  Green - 0
  Red - 3
  White - 4
  Blue - 5*/
int posit[24] = {2,3,1,3,  //D
				1,5,2,0,   //L
				4,0,4,5,   //B
				2,3,5,0,   //U
				3,4,1,4,   //R
				2,1,0,5};  //F

int piece[] = {15,16,16,21,21,15,  13,9,9,17,17,13,  14,20,20,4,4,14,  12,5,5,8,8,12,
               3,23,23,18,18,3,   1,19,19,11,11,1,  2,6,6,22,22,2,    0,10,10,7,7,0};

void calcadj();
void mix();
void calcperm();
void do_solve();

int main()
{
    //now, hard code the colours every time we need to solve
    //until we figure out a way to send the colours to the SJOne board.
    //calcadj();

    /**read the file here and populate posit array*/

    FILE *fp;

    fp = fopen("G:\\CMPE244CubeFile\\cube_faces.txt","r");

    if(fp == 0)
    {
        printf("Error reading file\n");
        exit(1);
    }

    int i = 0;
    int arr_size = sizeof(posit)/sizeof(posit[0]);
    printf("Size of the array %d\n",arr_size);

    char ch;

    for(i = 0; i < arr_size; i++)
    {
        ch = fgetc(fp);

        if(ch == 'y')
        {
            posit[i] = 1;
        }

        if(ch == 'w')
        {
            posit[i] = 4;
        }

        if(ch == 'b')
        {
            posit[i] = 5;
        }

        if(ch == 'r')
        {
            posit[i] = 3;
        }

        if(ch == 'g')
        {
            posit[i] = 0;
        }

        if(ch == 'o')
        {
            posit[i] = 2;
        }
    }

    for(i = 0; i < arr_size; i++)
    {
        printf("posit %d ",posit[i]);
    }

    fclose(fp);

    printf("calcperm\n");
    calcperm();
    printf("end calcperm\n");
    mix();
    do_solve();
    return 0;
}

void calcadj(){
	//count all adjacent pairs (clockwise around corners)
	int a,b;
	for(a=0;a<6;a++)
        for(b=0;b<6;b++)
            adj[a][b]=0;

	for(a=0;a<48;a+=2)
    {
		if(posit[piece[a]]<=5 && posit[piece[a+1]]<=5)
        {
            adj[posit[piece[a]]][posit[piece[a+1]]]++;
            //printf(" %d - %d; ",posit[piece[a]], piece[a]);
        }
	}

}

bool search_for_solution(int d,int q, int t, int l, int lm);
int sol[20] = {0};

void do_solve()
{
    //find adjacent/opposite colour pairs
	calcadj();
	int opp[6];//=new Array();
	int a=0;
	int b=0;
	for(a=0;a<6;a++){
		for(b=0;b<6;b++){
			if(a!=b && adj[a][b]+adj[b][a]==0) { opp[a]=b; opp[b]=a; }
		}
	}
	//Each piece is determined by which of each pair of opposite colours it uses.
	int ps[8];//=new Array();
	int tws[8];//=new Array();
	a=0;
	int d = 0;
	for(d=0; d<7; d++){
		int p=0;
		for(b=a;b<a+6;b+=2){
			if(posit[piece[b]]==posit[piece[42]]) p+=4;
			if(posit[piece[b]]==posit[piece[44]]) p+=1;
			if(posit[piece[b]]==posit[piece[46]]) p+=2;
		}
		ps[d]=p;
		if(posit[piece[a]]==posit[piece[42]] || posit[piece[a]]==opp[posit[piece[42]]]) tws[d]=0;
		else if(posit[piece[a+2]]==posit[piece[42]] || posit[piece[a+2]]==opp[posit[piece[42]]]) tws[d]=1;
		else tws[d]=2;
		a+=6;
	}
	//convert position to numbers
	int q=0;
	for(a=0;a<7;a++){
		b=0;
		int c = 0;
		for(c=0;c<7;c++){
			if(ps[c]==a)
                break;

			if(ps[c]>a)
                b++;
		}
		q=q*(7-a)+b;
	}
	//var t=0;
	//string t = "";
	int t = 0;
	for(a=5;a>=0;a--){
		t+=a+tws[a]-3*floor(tws[a]/3);
		printf("  t tws[a] %d %d",t, tws[a]);
	}
	printf("\n");

	printf("q and t values %d %d \n",q,t);
	char sol_arr1[] = {'U', 'R', 'F'};
	char sol_arr2[] = {' ', '2','\''};// 2\'

	if(q!=0 || t!=0){
		//sol.length=0;
		printf("q and t not zero\n");
		int l = 0;
		for(l=0;l<12;l++){	//max length of solution is 11
			if(search_for_solution(0,q,t,l,-1)) break;
		}
		//t="Solution:";
		for(q=0;q<20;q++){
			//t+=" "+"URF".charAt(sol[q]/10)+" 2\'".charAt(sol[q]%10);
			//printf("%d ** %d ** %d \n",sol[q], sol[q]/10, sol[q]%10);
			printf("%c%c ",sol_arr1[sol[q]/10], sol_arr2[sol[q]%10]);
		}
	}

}

int twst[729] = {0};
int perm[5040] = {0};
int **permmv;//[5050][5050];
int **twstmv;
int getprmmv(int, int);
int gettwsmv(int p, int m);


void calcperm(){
	//calculate solving arrays
	//first permutation
	permmv = (int**)malloc(5040 * sizeof(int*));

    int p = 0;
    //printf("first loop\n");
	for(p=0;p<5040;p++){
        //printf("first loop, loop start\n");
		perm[p]=-1;
        //printf("first loop, assigned\n");
		permmv[p]=(int*)malloc(3 * sizeof(int));//(int*)malloc(3 * sizeof(int));//{0};
		//printf("first loop, size set\n");
		int m = 0;
		for(m=0;m<3;m++){
			permmv[p][m]=getprmmv(p,m);
		}
	}
    //printf("first loop end\n");
	perm[0]=0;
	int l = 0;
	for(l=0;l<=6;l++){
		int n=0;
		p = 0;
		for(p=0;p<5040;p++){
			if(perm[p]==l){
                int m = 0;
				for(m=0;m<3;m++){
					int q=p;
                    int c = 0;
					for(c=0;c<3;c++){
						int q=permmv[q][m];
						if(perm[q]==-1) { perm[q]=l+1; n++; }
					}
				}
			}
		}
	}

	//then t</sol
	twstmv = (int**)malloc(710 * sizeof(int*));
	p = 0;
	for(p=0;p<729;p++){
		twst[p]=-1;
		twstmv[p]=(int*)malloc(3 * sizeof(int));//new Array();
		int m = 0;
		for(m=0;m<3;m++){
			twstmv[p][m]=gettwsmv(p,m);
		}
	}
    //printf("twstmv end\n");
	twst[0]=0;
	for(l=0;l<=5;l++){
        //printf("after %d\n ",l);
		int n=0;
		for(p=0;p<729;p++){
			if(twst[p]==l){
			    //printf("after second\n");
                int m = 0;
				for(m=0;m<3;m++){
				    //printf("after third\n");
					int q=p;
                    int c = 0;
					for(c=0;c<3;c++){
					    //printf("after fourth q and m %d %d \n ",q,m);
						int i=twstmv[q][m];
                        //printf("after after fourth q and m and i %d %d %d\n ",q,m,i);
                        if(i < 729)
                        {
                            if(twst[i]==-1)
                            {
                                twst[i]=l+1; n++;
                            }
                        }

					}
				}
			}
		}
	}
	//remove wait sign
	//signoff();
}

int gettwsmv(int p, int m){
	//given orientation p<729 and move m<3, return new orientation number
	int a,b,c,d,q;
	//convert number into array;
	int ps[7];//=new Array()
	q=p;
    d=0;
	for(a=0;a<=5;a++){
		c=floor(q/3);
		b=q-3*c;
		q=c;
		ps[a]=b;
		d-=b;if(d<0)d+=3;
	}
	ps[6]=d;
	//perform move on array
	if(m==0){
		//U
		c=ps[0];ps[0]=ps[1];ps[1]=ps[3];ps[3]=ps[2];ps[2]=c;
	}else if(m==1){
		//R
		c=ps[0];ps[0]=ps[4];ps[4]=ps[5];ps[5]=ps[1];ps[1]=c;
		ps[0]+=2; ps[1]++; ps[5]+=2; ps[4]++;
	}else if(m==2){
		//F
		c=ps[0];ps[0]=ps[2];ps[2]=ps[6];ps[6]=ps[4];ps[4]=c;
		ps[2]+=2; ps[0]++; ps[4]+=2; ps[6]++;
	}
	//convert array back to number
	q=0;
	for(a=5;a>=0;a--){
		q=q*3+(ps[a]%3);
	}
	return(q);
}

int getprmmv(int p, int m){
	//given position p<5040 and move m<3, return new position number
	int a,b,c,q;
	//convert number into array;
	int ps[8];//=new Array()
	q = p;
    for(a=1;a<=7;a++){
		b=q%a;
		q=(q-b)/a;
		for(c=a-1;c>=b;c--) ps[c+1]=ps[c];
		ps[b]=7-a;
	}
	//perform move on array
	if(m==0){
		//U
		c=ps[0];ps[0]=ps[1];ps[1]=ps[3];ps[3]=ps[2];ps[2]=c;
	}else if(m==1){
		//R
		c=ps[0];ps[0]=ps[4];ps[4]=ps[5];ps[5]=ps[1];ps[1]=c;
	}else if(m==2){
		//F
		c=ps[0];ps[0]=ps[2];ps[2]=ps[6];ps[6]=ps[4];ps[4]=c;
	}
	//convert array back to number
	q=0;
	for(a=0;a<7;a++){
		b=0;
		for(c=0;c<7;c++){
			if(ps[c]==a)break;
			if(ps[c]>a)b++;
		}
		q=q*(7-a)+b;
	}
	return(q);
}

bool search_for_solution(int d,int q, int t, int l, int lm){
	//searches for solution, from position q|t, in l moves exactly. last move was lm, current depth=d
	//printf("searching for solution\n");
	if(l == 0)
    {
		if(q == 0 && t == 0)
		{
			return(true);
		}
	}
	else
    {
		if(perm[q] > l || twst[t] > l)
		if(perm[q] > l || twst[t] > l)
            return(false);

		int p,s,a,m;

		for(m = 0; m < 3; m++)
		{
			if(m != lm)
			{
				p=q; s=t;
				for(a = 0; a < 3; a++)
				{
					p = permmv[p][m];
					s = twstmv[s][m];
					sol[d] = 10 * m + a;

					if(search_for_solution(d+1, p, s, l-1, m))
                        return(true);
				}
			}
		}
	}
	return(false);
}

void initbrd(){

}
void mix()
{
    int i = 0;
    for(i=0;i<50;i++){
		int y = floor(rand() * 6) + 16 * floor(rand()*3);
		//domove(f);
		//printf(" %d ", f);
		int f = y & 15;
		//printf(" %d ",f);
	}
}



//-------------New Implementation------------------//




