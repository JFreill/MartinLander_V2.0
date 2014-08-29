/***********************************************************************************
•       Inital Filename:		ass3.cpp
*       Current Filename:       martinlanderMain_V2.0.cpp
•       Version:				100.02
•       Author:					James Freill
•       Student Number:			040 590 657
•       Course Name/Number:		13W_CST8219_400 C++ Programming
•       Lab Section:			010
•       Assignment #:			3
•       Assignment Name:		Ass3_Freill1
•       Due Date:				19th April 2013
•       Submission Date:		19th April 2013
*       Editted Date:           28th August 2014
•       Professor:				Andrew Tyler
•       Purpose:				Runs a small command prompt program and saves records to a file
************************************************************************************/
#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
#include <cassert>
#include <ctime>
#include <istream>
#include <conio.h>
#include <cmath>
#include <cstdlib>
using namespace std;
/***********************************************************************************
******************************* FUNCTION PROTOTYPES*********************************
************************************************************************************/
void printFinalTime(int, int, int);
void displayRecords(int, int, int, double);
void printgame(int height, bool thrust, bool crash);
/********************************************************************
Function name:		main()
Purpose:			runs the main program and allows for the choices of displaying and saving records
In parameters:		none
Out parameters:		none
Version:			2.0
Author:				James Freill
**********************************************************************/
int main(void)
{
	bool running = true, thrust_on = false, crash = false;
	char opt;
	char i;

	int min, sec, milli;
	double v, v2, height, dV, dT, thrust, fuel, tE;
	const double g = 3.5, c = 12.5, m = 65, a = 8.0, vMax = 50;
	clock_t t1;
	clock_t t2;

	while(running)
	{
		cout<<"\n********* MARTIAN LANDER **********\n"
			"1 = Run Simulation\n"
			"2 = Display Results\n"
			"3 = Exit\n"
			"***********************************"<<endl;
			cin>>opt;

		switch(opt) {
			case '1':

                /* initilizes game variables  to start */
                min = 0, sec = 0, milli = 0;
                v = 0, v2 = 0, height = 1000.00, dV = 0, dT = 0, thrust = 0, fuel = 200,tE = 0;
                t1 = t2 = 0;
				t1 = clock();

				while(1)	{

                    /* calculates variables as game runs */
					t2 = clock();
					dT = (double)(t2 - t1)/CLOCKS_PER_SEC;
					tE += dT;
					v = v2;
					v2 = v + dV*dT;
					height -= ((v2 + v)/2)*dT;
					dV = g - c/m*(v+a*pow((v2/vMax), 2)) - thrust;

                    /* Monitors buttons hit during game for thrust, and qutting */
					if(kbhit()) {
						i = getch();
						if(i == 'w' || i == 'W'){
                            thrust_on = true;
                            ++thrust;
						}
						if(i == 'e' || i == 'E'){
                            if(thrust > 0){
                                thrust_on = false;
                                --thrust;
                            }
						}
						if(i == 'q' || i == 'Q')
							break;
					}

                    /* time calculations */
					min = (int)(tE/60);
					sec = (int)tE % 60;
					if(min != 0)
						milli = (int)(tE*1000)-(sec*1000)-((min*1000)*60);
					else
						milli = (int)(tE*1000)-(sec*1000);

					system("cls");

					cout<<" ************* CONTROLS ************* \n"
                          " Q = quit \n"
                          " W = Increase Thrust \n"
                          " E = Decrease Thrust \n"
                          " ************************************ \n"
                          " ************* READ OUTS ************ \n"
                          " THRUST: "<< thrust <<"\n"
						  " FUEL: "<< fuel <<"\n"
						  " TIME: "<< min <<":"<< sec <<":"<< milli <<"\n"
						  " SPEED: "<< v2 <<"\n"
						  " HEIGHT: "<< height <<" metres\n"
                          " ************************************ \n"<<endl;

                    /*end of game, win or die */
					if(v2 <= 2 && height <= 1) {
						cout<<"You landed!"<<endl;
						printFinalTime(min, sec, milli);
						break;
					}else if(v2 > 2 && height <= 0) {
					    crash = true;
						cout<<"You crashed!"<<endl;
						printFinalTime(min, sec, milli);
						break;
					}

                    /* function to rint graphics */
					printgame((int)height, thrust_on, crash);

					if(fuel > 0) fuel -= thrust*dT;
					else if(fuel <= 1){ thrust = 0; fuel = 0; }
					t1 = t2;
				}
			break;
		case '2':
			displayRecords(min, sec, milli, v);
			break;
		case '3':
			cout<<"\nThanks for playing!"<<endl;
			system("pause");
			return false;
			break;
		default:
			cout<<"You didnt enter the right choice..."<<endl;
			break;
		}
	}
	return 0;
}
/********************************************************************
Function name:		printFinalTime()
Purpose:			prints the time when called in a certain format
In parameters:		int min, int sec, int milli
Out parameters:		none
Version:			1.0
Author:				James Freill
**********************************************************************/
void printFinalTime(int min, int sec, int milli)
{
	cout<<"Time Taken = "<< min <<":"<< sec <<":"<< milli <<endl;
	system("pause");
}
/********************************************************************
Function name:		displayRecords()
Purpose:			displays saved records to file and allows for the choice of saving your record
In parameters:		int min, int sec, int milli, double newImpact
Out parameters:		none
Version:			2.0
Author:				James Freill
NOTES:              NOT WORKINGGGG
**********************************************************************/
void displayRecords(int min, int sec, int milli, double newImpact)
{
	int tempPos, tempMin, tempSec, tempMilli, size = 0, placement;
	char tempName[32], newName[32], option;
	double tempImpact;

	struct Record {
		char* r_name;
		int r_min;
		int r_sec;
		int r_milli;
		double r_impact;
	};

	Record* ptr_Records;

	ifstream ifs("Martian_Lander_Scores.txt");
	while(ifs>>tempPos>>tempName>>tempImpact>>tempMin>>tempSec>>tempMilli) {
		size++;
	}
	ifs.close();

	ptr_Records = new Record[size];

	if(size == 0) {
		cout<<"No records"<<endl;
	} else if (size != 0) {

		int i = 0;

		cout<<"*** RECORDS FOR MARTIAN LANDER! ***"<<endl;

		ifs.open("Martian_Lander_Scores.txt");
		/* Read from file, & store records & display records */
		while(ifs>>tempPos>>tempName>>tempImpact>>tempMin>>tempSec>>tempMilli) {

			cout<<tempPos<<"NAME:"<<tempName<<"SPD@IMPACT:"<<tempImpact<<"TIME:"<<tempMin<<":"<<tempSec<<":"<<tempMilli<<endl;

			ptr_Records[i].r_name = new char[strlen(tempName)+1];
			strcpy(ptr_Records[i].r_name, tempName);
			ptr_Records[i].r_impact = tempImpact;
			ptr_Records[i].r_min = tempMin;
			ptr_Records[i].r_sec = tempSec;
			ptr_Records[i].r_milli = tempMilli;

			/* check to see if your record is lower than the existing ones */
			if(newImpact < ptr_Records[i].r_impact) {
				if ( min < ptr_Records[i].r_min ) {
					placement = i;
				} else if ( min == ptr_Records[i].r_min ) {
					if (sec < ptr_Records[i].r_sec ) {
						placement = i;
					} else if ( sec == ptr_Records[i].r_sec ) {
						if (milli < ptr_Records[i].r_milli ) {
							placement = i;
						}
					}
				}
			}

			i++;
		}
		ifs.close();
	}

	cout<<"\n***********************************\n"
	"Save your Record?\n"
	"y = yes\n"
	"n = no\n"
	"***********************************"<<endl;
	cout<<"Enter choice: ";
		cin>>option;

	ofstream ofs;

	int rank = 1;
	bool recorded = false;

	switch(option) {
		case 'y':

			cout<<"Please enter your name: ";
				cin>>newName;
			/* Writes to file & adds your new record into existing records */
			ofs.open("Martian_Lander_Scores.txt");
			if(ofs.is_open()) {

				for(int i = 0; i <= size; i++, rank++ ) {
					if( placement == i ) {
						ofs<<rank<<" "<<newName<<" "<<newImpact<<" "<<min<<" "<<sec<<" "<<milli<<endl;
						recorded = true;
						if ( i != 0 ) {
							rank++;
						}
					}
					if ( i != 0 && recorded == false ) {
						ofs<<rank<<" "<<ptr_Records[i].r_name<<" "<<ptr_Records[i].r_impact<<" "<<ptr_Records[i].r_min<<" "<<ptr_Records[i].r_sec<<" "<<ptr_Records[i].r_milli<<endl;
					} else if ( i != 0 && recorded == true ) {
						ofs<<rank<<" "<<ptr_Records[i-1].r_name<<" "<<ptr_Records[i-1].r_impact<<" "<<ptr_Records[i-1].r_min<<" "<<ptr_Records[i-1].r_sec<<" "<<ptr_Records[i-1].r_milli<<endl;
					}
				}
			}
			ofs.close();

			cout<<"Record Saved"<<endl;

			for(int i = 0; i < size; i++) {
				delete[] ptr_Records[i].r_name;
			}
			delete[] ptr_Records;

			break;
		case 'n':
			cout<<"Record was not saved.."<<endl;
			break;
		default: cout<<"\nPlease enter right choice to proceed..."<<endl;
	}
}
/********************************************************************
Function name:		printgame()
Purpose:			displays spaceship as it tries to land
In parameters:		int height, bool thrust, bool crash
Out parameters:		none
Version:			2.0
Author:				James Freill
**********************************************************************/
void printgame(int height, bool thrust, bool crash) {
    for(int i = 10; i >=0 ; i--) {
        if(i == 10) {
            cout<<" *   *      *          *      *   *"<<endl;
            cout<<"  *       *     *            *     "<<endl;
            cout<<" *            *      *          *  "<<endl;
        }
        if((height/100) == i){
            cout<<"                   ___    "<<endl;
            cout<<"                  ( * )   "<<endl;
            cout<<"                  [___]   "<<endl;
            if(thrust == true) {
                cout<<"                  ^ ^ ^  \n"<<endl;
            }
        }
        if(i == 0)cout<<"\n______________________________________\n"<<endl;
        else cout<<"\n"<<endl;
    }
}

