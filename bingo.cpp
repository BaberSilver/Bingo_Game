#include <iostream>
#include<fstream>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cstdlib>
#include<windows.h>
using namespace std;

const int MaxLimit = 100;
int  sizeTotal, SizeRow, SizeColumn, Topscore[11] = {}, score1, score2;
char Name[11][100] = {};
fstream gameFileForPlayer1, gameFileForPlayer2, HighScore,gameHistory;

void main_menu(); //Displaying Main Menu
void PlayGame();  //Displaying menu for New Game and Previous Game
void NewGame(); //Function for the Game
void generateGameID(int&);    //Generate Game ID
void GenerateFirstCard(int[MaxLimit][MaxLimit]);   //Generate first  Board
void GenerateSecondCard(int[MaxLimit][MaxLimit]);    //Generate second Board
void DisplayCard1(int[MaxLimit][MaxLimit], string, int, int);  //Displaying the first Board
void DisplayCard2(int[MaxLimit][MaxLimit], string, int, int);  //Displaying the second Board
void RemoveNumber(int r[], int& RemoveNumber, int& num);  //Taking number that is to be removed from board by the user
bool Search(int[MaxLimit][MaxLimit], int[MaxLimit][MaxLimit], int found);  //To search from the player's board and removing it
bool Sum(int[MaxLimit][MaxLimit]);   //For checking the winning condition, (sum of complete rows,column or diagonal is zero.)
bool checkEscape();  //For checking if escape was pressed
void clearInputBuffer();   //clearing input buffer for later use
void clearScreen();
void PreviousGame();    //The Previous Game Function
void Game_History();     //The Function for displaying Game History
void High_Score(string playerName1, char Name[][100], int Topscore[], int score1); //The Function for displaying High Score
void DisplayHighScore(int Topscore[], char Name[][100]);
void GameGuide();   //The How To Play Function
void Exit_Game(char& Confirm);  //The Exit Function

int main() {
	main_menu();
	system("pause");
	return 0;
}

void main_menu() {                                               //Displaying Main Menu
	char Option, Confirm;
	do {
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 11);
		cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl;
		cout << "\t\t\t" << "MENU" << endl << endl;
		cout << setw(27) << "1. PLAY GAME" << endl;
		cout << setw(30) << "2. GAME HISTORY" << endl;
		cout << setw(28) << "3. HIGH SCORE" << endl;
		cout << setw(30) << "4. HOW TO PLAY?" << endl;
		cout << setw(22) << "5. EXIT" << endl;
		cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
		cout << "What do you want to select(1 to 5) : ";
		cin >> Option;
		while (Option < '1' || Option>'5') {
			cout << "The option you chose is incorrect! Enter again : ";
			cin >> Option;
		}
		if (Option == '1') {
			clearScreen();
			PlayGame();
			cout << endl;
		}
		else if (Option == '2') {
			clearScreen();
			Game_History();
			cout << endl;
		}
		else if (Option == '3') {
			clearScreen();
			DisplayHighScore(Topscore, Name);
			cout << endl;
		}
		else if (Option == '4') {
			clearScreen();
			GameGuide();
			cout << endl;
		}
		else {
			clearScreen();
			Exit_Game(Confirm);
		}
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 7);
		cout << "Enter 1 if you want to go to Continue or 0 to exit: ";
		cin >> Confirm;
		if (Confirm == '0')
			exit(0);
		cout << endl;
		while (Confirm != '0' && Confirm != '1') {
			cout << "Invaid input.Only press 1 to go to Main Menu or 0 to exit: ";
			cin >> Confirm;
		}

	} while (Confirm == '1');
}

void PlayGame() {                                                      //Displaying menu for New Game and Previous Game
	char Option;
	while (true) {
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 11);
		cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
		cout << setw(34) << "1) Start New Game\n";
		cout << setw(42) << "2) Continue Previous Game\n";
		cout << setw(27) << "3) Go back\n";
		cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
		cout << "What do you want to select(1 to 3) : ";
		cin >> Option;
		Option = tolower(Option);
		if (Option == '1') {
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			clearScreen();
			NewGame();
		}
		else if (Option == '2') {
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			clearScreen();
			PreviousGame();
		}
		else if (Option == '3') {
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			clearScreen();
			main_menu();
		}
		else
			cout << "Invalid option. Please choose a valid option." << endl;
	}
}

void NewGame() {                              //Function for the Game
	do {
		cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
		cout << "Enter the number of rows or columns for the board: ";
		score1 = 0; score2 = 0;
		if (!(cin >> SizeRow)) {
			cout << "Invalid input. Please enter a valid number." << endl;
			cin.clear();
			clearInputBuffer();
		}
		else if (SizeRow <= 0) {
			cout << "Number of rows or columns should be greater than zero. Please enter again." << endl;
		}
	} while (SizeRow <= 0);

	SizeColumn = SizeRow;
	sizeTotal = SizeRow * SizeColumn;
	int ArrayForFirstBoard[MaxLimit][MaxLimit], ArrayForSecondBoard[MaxLimit][MaxLimit], RemovedNumberList[MaxLimit];
	int RemoveNumberForPlayer1, RemoveNumberForPlayer2 = 0, sizeOfList1 = 0, sizeOfList2 = 0, GameID;
	char Option;
	int gameTurn = 1;
	int Player[2] = { 0, 1 };
	srand(time(0));
	int index = rand() % 2;
	int playerPosition = Player[index];
	cin.ignore();
	char playerName1[MaxLimit], playerName2[MaxLimit];
	
	cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
	cout << "Player 1 \n Enter your name :\n";
	cin.getline(playerName1, 100);
	cout << "\n";
	cout << "Player 2 \n Enter your name :\n";
	cin.getline(playerName2, 100);
	cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
	cout << "\n";
	generateGameID(GameID);
	GenerateFirstCard(ArrayForFirstBoard);
	GenerateSecondCard(ArrayForSecondBoard);

	if (playerPosition == 1) {
		swap(playerName1, playerName2);
		cout << endl;
		cout << "\tPlayer " << playerPosition + 1 << " Wins the toss!! " << endl;
	}
	else
		cout << "\tPlayer " << playerPosition + 1 << " Wins the toss!! " << endl;
	cout << endl;
	cout << setw(30) << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+" << endl << endl;
	bool firstIteration = false;
	while (true) {
		clearInputBuffer();
		if (!firstIteration) {
            cout << "Press Q to go back (anything else to continue)\n";
                if (checkEscape()) {
                    gameHistory.open("GameHistory.txt", ios::app);
                    if (gameHistory.is_open()) {
                        gameHistory << playerName1 << "\t" << playerName2 << "\t" << "N/A" << "\t" << gameTurn << "\n";
                    }
                    gameHistory.close();
                    cout << "Q pressed. Exiting number removal section." << endl;
                    break;
                }
            } 
    	else {
            firstIteration = false; 
        }
		if (gameTurn % 2 != 0) {
			clearScreen();
			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();

			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();
			do {
				DisplayCard1(ArrayForFirstBoard, playerName1, GameID, gameTurn);
				RemoveNumber(RemovedNumberList, RemoveNumberForPlayer1, sizeOfList1);
			} while (!(Search(ArrayForFirstBoard, ArrayForSecondBoard, RemoveNumberForPlayer1)));
			gameTurn++;

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();

			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();
	    	if (Sum(ArrayForFirstBoard)) {
    			HANDLE console_color;
    			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(console_color, 9);
    			cout << playerName1 << " Wins!" << endl;
		    	gameHistory.open("GameHistory.txt", ios::app);
                if (!gameHistory.is_open()) {
               cout << "Error opening GameHistory.txt" << endl;
               return;
               }
               gameHistory << playerName1 << "\t" << playerName2 << "\t" << playerName1 << "\t" << gameTurn << "\n";   
               gameHistory.close();
				score1=1;
    			score2 = 0;
    			High_Score(playerName1, Name, Topscore, score1);
		    	main_menu();
	    	}			
		}
		else {
			clearScreen();
			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << "  \t  " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();
			do {
				DisplayCard2(ArrayForSecondBoard, playerName2, GameID, gameTurn);
				RemoveNumber(RemovedNumberList, RemoveNumberForPlayer2, sizeOfList2);
			} while (!(Search(ArrayForSecondBoard, ArrayForFirstBoard, RemoveNumberForPlayer2)));
			gameTurn++;
			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();
            if (Sum(ArrayForSecondBoard)) {
	    		HANDLE console_color;
		    	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(console_color, 9);
	    		cout << playerName2 << " Wins!" << endl;
		    	gameHistory.open("GameHistory.txt", ios::app);
                if (!gameHistory.is_open()) {
               cout << "Error opening GameHistory.txt" << endl;
               return;
               }
               gameHistory << playerName1 << "\t" << playerName2 << "\t" << playerName2 << "\t" << gameTurn << "\n";   
               gameHistory.close();
				score2=1;
    			score1 = 0;
	    		High_Score( playerName2, Name, Topscore,score2);
    			main_menu();		
    		}
    	}
    }
}

void generateGameID(int& gameID) {                                  //Generate Game ID
	srand(time(0));
	gameID = rand() % (9999 - 1000) + 1000;
}

void GenerateFirstCard(int a[MaxLimit][MaxLimit]) {                //Generate first Board
	int n = 1;
	srand(time(0));

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {                         //put every number from 1 to
			a[i][j] = n++;                                              //N (square of number provided by user) in an array
		}
	}

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			int randRow = rand() % SizeRow;                        //randomly take a row
			int randCol = rand() % SizeColumn;                        //randomly take a column
			swap(a[i][j], a[randRow][randCol]);        //put values from the array in those rows and columns
		}
	}
}

void GenerateSecondCard(int a[MaxLimit][MaxLimit]) {                //Generate second Board
	int n = sizeTotal;
	srand(time(0));

	for (int j = 0; j < SizeColumn; ++j) {                         //put every number from N (square of number provided by user)
		for (int i = 0; i < SizeRow; ++i) {                        //to 1 in an array
			a[j][i] = n--;
		}
	}

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			int randRow = rand() % SizeRow;                        //randomly take a row
			int randCol = rand() % SizeColumn;                        //randomly take a column
			swap(a[i][j], a[randRow][randCol]);                        //put values in those rows and columns
		}
	}
}


void DisplayCard1(int a[MaxLimit][MaxLimit], string playerName, int gameID, int gameTurn) {             //Displaying the first Board
	cout << "Game ID : " << gameID << setw(10) << "TURN: " << gameTurn << endl;
	for (int i = 0; i < SizeRow; ++i) {
		cout << setw(10) << " |";
		for (int j = 0; j < SizeColumn; ++j) {
			cout << setw(4) << a[i][j] << " | ";
		}
		cout << "\n";
	}

	cout << playerName << " select any number from board :";
}

void DisplayCard2(int a[MaxLimit][MaxLimit], string playerName, int gameID, int gameTurn) {             //Displaying the second Board
	cout << "Game ID : " << gameID << setw(10) << "TURN: " << gameTurn << endl;
	for (int i = 0; i < SizeRow; ++i) {
		cout << setw(10) << " |";
		for (int j = 0; j < SizeColumn; ++j) {
			cout << setw(4) << a[i][j] << " | ";
		}
		cout << "\n";
	}

	cout << playerName << " select any number from board :";
}

void RemoveNumber(int r[], int& RemoveNumber, int& num) {             //Taking number that is to be removed from board by the user
	bool numberPresent = false;
	do {

		if (!(cin >> RemoveNumber)) {                                  //checking if a number was given as input
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 12);
			cout << "Invalid input. Please enter a number." << endl;
			cin.clear();
			clearInputBuffer();
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			continue;
		}

		if (RemoveNumber < 0 || RemoveNumber > sizeTotal) {             //check if number is actually on the board
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 12);
			cout << "Number out of range. Please choose a number between 0 and " << sizeTotal << "." << endl;
			cin.clear();
			clearInputBuffer();
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			continue;
		}

		for (int i = 0; i < num; ++i) {                                   //check if number has already been removed
			if (r[i] == RemoveNumber) {
				numberPresent = true;
				break;
			}
		}

		if (numberPresent) {                                                  //take input again
			HANDLE console_color;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 12);
			cout << "Number already chosen. Please select a different number." << endl;
			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console_color, 11);
			continue;
		}
		r[num] = RemoveNumber;                                               //adding the number in remove number list
		++num;
		break;
	} while (num < sizeTotal && !numberPresent);
}

bool Search(int a[MaxLimit][MaxLimit], int b[MaxLimit][MaxLimit], int found) {
	bool confirmRemoved = false;
	bool foundOnBoard = false;

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			if (found == a[i][j] || found == b[i][j]) {
				foundOnBoard = true;
				break;
			}
		}
	}

	if (!foundOnBoard) {
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 12);
		cout << "Number not found on any board. Please choose a different number." << endl;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 11);
		return false;
	}

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			if (found == a[i][j]) {
				a[i][j] = 0;
				confirmRemoved = true;
			}
			if (found == b[i][j]) {
				b[i][j] = 0;
				confirmRemoved = true;
			}
		}
	}

	if (confirmRemoved) {
		return true;
	}
	return false;
}

bool Sum(int a[MaxLimit][MaxLimit]) {                           //For checking the winning condition, (sum of complete rows,column or diagonal is zero.)
	int SumRow[MaxLimit] = { 0 };
	int SumColumn[MaxLimit] = { 0 };
	int SumDiagonal[2] = { 0 };
	int AllSums=0;

	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			SumRow[i] += a[i][j];                             //sum of rows
			SumColumn[j] += a[i][j];                             //sum of columns
			if (i == j) {
				SumDiagonal[0] += a[i][j];                             //sum of diagonal
			}
			if (i + j == SizeRow - 1) {
				SumDiagonal[1] += a[i][j];                             //sum of anti diagonal
			}
		}
	}

	for (int i = 0; i < SizeRow; ++i) {
		if (SumRow[i] == 0 )
			AllSums++;
		if(SumColumn[i] == 0)
	    	AllSums++;	
	}
	if (SumDiagonal[0] == 0  ) 
		AllSums++;
	if(SumDiagonal[1] == 0)
	    AllSums++;
	if(AllSums >= SizeRow)
    	return true;
	else
    	return false;
}

bool checkEscape() {
    char input;
    bool hasQuit = false;

    cin >> input;
    if (input == 'q' || input == 'Q') {
        hasQuit = true;
    } else {
        cout << "Press Q to go back (anything else to continue)\n";
    }

    clearInputBuffer();
    return hasQuit;
}

void clearInputBuffer() {                                         //clearing input buffer for later use
	while (cin.get() != '\n') {
		cin.ignore();
		continue;
	}
}

void clearScreen() {                                           //Function for Clearing screen
	system("cls");
}

void PreviousGame() {                                            //The Previous Game Function
	int ArrayForFirstBoard[MaxLimit][MaxLimit], ArrayForSecondBoard[MaxLimit][MaxLimit], GameID, RemovedNumberList[MaxLimit];
	int RemoveNumberForPlayer1, RemoveNumberForPlayer2 = 0, sizeOfList1 = 0, sizeOfList2 = 0;
	string playerName1, playerName2, line;
	string gameTurnStr, gameIDStr, SizeRowStr, ArrayStr;
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 11);

	gameFileForPlayer1.open("inputFileForBoard1.csv", ios::in);
	getline(gameFileForPlayer1, line);
	getline(gameFileForPlayer1, SizeRowStr, '\n');
	SizeRow = stoi(SizeRowStr);
	SizeColumn = SizeRow;
	sizeTotal = SizeRow*SizeColumn;
	getline(gameFileForPlayer1, line);
	getline(gameFileForPlayer1, gameIDStr, '\t');
	GameID = stoi(gameIDStr);
	getline(gameFileForPlayer1, gameTurnStr, '\n');
	int gameTurn = stoi(gameTurnStr);
	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			gameFileForPlayer1 >> ArrayForFirstBoard[i][j];
			gameFileForPlayer1 >> line;
		}
		getline(gameFileForPlayer1, line, '\n');
	}
	getline(gameFileForPlayer1, playerName1, '\t');
	gameFileForPlayer1.close();

	gameFileForPlayer2.open("inputFileForBoard2.csv", ios::in);
	getline(gameFileForPlayer2, line);
	getline(gameFileForPlayer2, line);
	getline(gameFileForPlayer2, line);
	getline(gameFileForPlayer2, gameIDStr, '\t');
	GameID = stoi(gameIDStr);
	getline(gameFileForPlayer2, gameTurnStr, '\n');
	gameTurn = stoi(gameTurnStr);
	for (int i = 0; i < SizeRow; ++i) {
		for (int j = 0; j < SizeColumn; ++j) {
			gameFileForPlayer2 >> ArrayForSecondBoard[i][j];
			gameFileForPlayer2 >> line;
		}
		getline(gameFileForPlayer2, line, '\n');
	}
	getline(gameFileForPlayer2, playerName2, '\t');
	gameFileForPlayer2.close();

	if (Sum(ArrayForFirstBoard)) {
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 9);
		cout << playerName1 << " Wins!" << endl;
		main_menu();
	}
	else if (Sum(ArrayForSecondBoard)) {
		HANDLE console_color;
		console_color = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console_color, 9);
		cout << playerName2 << " Wins!" << endl;
		main_menu();
	}
	bool firstIteration = false;
	while (true) {
		if (!firstIteration) {
            cout << "Press Q to go back (anything else to continue)\n";
                if (checkEscape()) {
                    gameHistory.open("GameHistory.txt", ios::app);
                    if (gameHistory.is_open()) {
                        gameHistory << playerName1 << "\t" << playerName2 << "\t" << "N/A" << "\t" << gameTurn << "\n";
                    }
                    gameHistory.close();
                    cout << "Q pressed. Exiting number removal section." << endl;
                    break;
                }
            } 
    	else {
            firstIteration = false; 
        }
		if (gameTurn % 2 != 0) {
			clearScreen();
			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();

			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();
			do {
				DisplayCard1(ArrayForFirstBoard, playerName1, GameID, gameTurn);
				RemoveNumber(RemovedNumberList, RemoveNumberForPlayer1, sizeOfList1);
			} while (!(Search(ArrayForFirstBoard, ArrayForSecondBoard, RemoveNumberForPlayer1)));
			gameTurn++;

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();

			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();
	    	if (Sum(ArrayForFirstBoard)) {
    			HANDLE console_color;
    			console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(console_color, 9);
    			cout << playerName1 << " Wins!" << endl;
		    	gameHistory.open("GameHistory.txt", ios::app);
                if (!gameHistory.is_open()) {
               cout << "Error opening GameHistory.txt" << endl;
               return;
               }
               gameHistory << playerName1 << "\t" << playerName2 << "\t" << playerName1 << "\t" << gameTurn << "\n";   
               gameHistory.close();
    			score1=1;
    			score2 = 0;
    		    gameHistory.close();
    			High_Score(playerName1, Name, Topscore, score1);
		    	main_menu();
	    	}			
		}
		else {
			clearScreen();
			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << "  \t  " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();
			do {
				DisplayCard2(ArrayForSecondBoard, playerName2, GameID, gameTurn);
				RemoveNumber(RemovedNumberList, RemoveNumberForPlayer2, sizeOfList2);
			} while (!(Search(ArrayForSecondBoard, ArrayForFirstBoard, RemoveNumberForPlayer2)));
			gameTurn++;
			gameFileForPlayer2.open("inputFileForBoard2.csv", ios::out);
			if (gameFileForPlayer2.is_open()) {
				gameFileForPlayer2 << "Row or column \n";
				gameFileForPlayer2 << SizeRow << "\n";
				gameFileForPlayer2 << " GameID \t gameTurn\n";
				gameFileForPlayer2 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer2 << ArrayForSecondBoard[i][j] << " | ";
					}
					gameFileForPlayer2 << "\n";
				}
				gameFileForPlayer2 << playerName2;
			}
			gameFileForPlayer2.close();

			gameFileForPlayer1.open("inputFileForBoard1.csv", ios::out);
			if (gameFileForPlayer1.is_open()) {
				gameFileForPlayer1 << "Row or column \n";
				gameFileForPlayer1 << SizeRow << "\n";
				gameFileForPlayer1 << " GameID \t gameTurn \n";
				gameFileForPlayer1 << GameID << " \t " << gameTurn << "\n";
				for (int i = 0; i < SizeRow; ++i) {
					for (int j = 0; j < SizeColumn; ++j) {
						gameFileForPlayer1 << ArrayForFirstBoard[i][j] << " | ";
					}
					gameFileForPlayer1 << "\n";
				}
				gameFileForPlayer1 << playerName1;
			}
			gameFileForPlayer1.close();
            if (Sum(ArrayForSecondBoard)) {
	    		HANDLE console_color;
		    	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    			SetConsoleTextAttribute(console_color, 9);
	    		cout << playerName2 << " Wins!" << endl;
	    		gameHistory.open("GameHistory.txt",ios::out);
                if (!gameHistory.is_open()) {
               cout << "Error opening GameHistory.txt" << endl;
               return;
               }
               gameHistory << playerName1 << "\t" << playerName2 << "\t" << playerName2 << "\t" << gameTurn << "\n";   
               gameHistory.close();
		    	score2=1;
    			score1 = 0;
	    		High_Score( playerName2, Name, Topscore,score2);
    			main_menu();		
    		}
    	}
    }
}

void Game_History() {
    string playerName1[10], playerName2[10], winnerName[10];
    int totalTurns[10];

    gameHistory.open("GameHistory.txt",ios::in);
    if (!gameHistory.is_open()) {
        cout << "Unable to open GameHistory.txt" << endl;
        return;
    }

    string line;
    getline(gameHistory, line); 

    int count = 0;
    while (count < 10 && getline(gameHistory, playerName1[count], '\t') && getline(gameHistory, playerName2[count], '\t') && getline(gameHistory, winnerName[count], '\t') && gameHistory >> totalTurns[count]) {
        count++;
    }
    gameHistory.close();

    gameHistory.open("GameHistory.txt", ios::out);
    if (!gameHistory.is_open()) {
        cout << "Unable to open GameHistory.txt for writing" << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        gameHistory << playerName1[i] << "\t" << playerName2[i] << "\t" << winnerName[i] << "\t" << totalTurns[i] << "\n";
    }
    gameHistory.close();

    cout << "Game History:\n";
    for (int i = 0; i < count; i++) {
        cout << setw(15) << "versus " << setw(10) <<  " winner" << setw(30)<<" Turns \n";
        cout << playerName1[i] << setw(10) << playerName2[i]  << setw(7) << winnerName[i] << setw(27) << totalTurns[i] << "\n";
    }
}

void High_Score(string playerName1, char Name[][100], int Topscore[], int score1)      //The Function for making High Score file
{
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 4);
	cout << endl << endl;
	HighScore.open("Highscore.txt", ios::in);
	for (int i = 0; i < 11; i++)
	{
		HighScore >> Name[i] >> Topscore[i];
	}
	HighScore.close();

	HighScore.open("HighScore.txt", ios::out);
	int temporary = 0;
	for (int i = 0; i < 11; i++)
	{
		if (Name[i] == playerName1)
		{
			Topscore[i]++;
			temporary = 1;
			for (int i = 0; i < 11; i++)
			{
				for (int j = i + 1; j < 11; j++)
				{

					if (Topscore[i] < Topscore[j])
					{
						swap(Topscore[i], Topscore[j]);
						swap(Name[i], Name[j]);
					}
				}
			}
		}
	}
	if(temporary == 0)
	{
		Topscore[10] = score1;
		for (int j = 0; j < playerName1.length(); j++)
		{
			Name[10][j] = playerName1[j];
		}
		for (int i = 0; i < 11; i++)
		{
			for (int j = i + 1; j < 11; j++)
			{

				if (Topscore[i] < Topscore[j])
				{
					swap(Topscore[i], Topscore[j]);
					swap(Name[i], Name[j]);
				}
			}
		}

	}
		for (int i = 0; i < 11; i++)
		{
			HighScore << Name[i] << " ";
			HighScore << Topscore[i] << endl;
		}
	HighScore.close();
}

void DisplayHighScore(int Topscore[], char Name[][100])    //The functions to display highscore.
{
	HighScore.open("HighScore.txt", ios::in);
	string line;
	for (int i = 0; i < 11; i++)
		{
			HighScore >> Name[i] ;
			HighScore >> Topscore[i];
			getline(HighScore,line);
		}
	HighScore.close();
	cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-HIGHSCORE-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=";
	cout << endl;
	cout << setw(20) << "NAME" << setw(20) << "Score" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << setw(20) << Name[i] << setw(20) << Topscore[i] << endl;
	}
}

void GameGuide() {                                            //The How To Play Function
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 9);
	cout << endl << endl;
	cout << "\t\t -+-+-HOW TO PLAY?-+-+- \n\n";
	cout << "1. Each player will choose a number between 1 to N (N being Size of Row / Column provided by the player). \n\n";
	cout << "2. The selected number will be replaced by zero on both players' boards.\n\n";
	cout << "\t\t -+-+-WINNING CONDITIONS?-+-+- \n\n";
	cout << "--->Five(depends on rows and columns) complete row,column or diagonal should be zero.The one who completes it first wins.<---" << endl;
	cout << endl;
}

void Exit_Game(char& Confirm) {                                            //The Exit Function
	HANDLE console_color;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 9);
	cout << "Are you sure you want to Exit the game? " << endl;
	console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 7);
	cout << "Enter 1 if you want to go to Main Menu or 0 to exit: ";
	cin >> Confirm;
	if (Confirm == '0')
		exit(0);
	cout << endl;
	while (Confirm != '0' && Confirm != '1') {
		cout << "Invaid input.Only press 1 to go to Main Menu or 0 to exit: ";
		cin >> Confirm;
	}
}