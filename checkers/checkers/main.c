#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int onePieces = 12;
int twoPieces = 12;

void draw(char board[][8]);
void clrscr();
int validate(char str1[2], char str2[2], int player, char board[][8], int mustEat);
void convertString(char str[2], int arr[2]);
void move(char board[][8], int from[2], int to[2]);
int checkBoard(char board[][8], int player);
int kingValidate(int from[2], int to[2], int player, char board[][8], int mustEat);

void main() {

	char board[][8] =
	{{' ', 'x',' ', 'x', ' ', 'x', ' ', 'x'},
	{'x', ' ','x', ' ', 'x', ' ', 'x', ' '},
	{' ', 'x',' ', 'x', ' ', 'x', ' ', 'x'},
	{' ', ' ',' ', ' ', ' ', ' ', ' ', ' '},
	{' ', ' ',' ', ' ', ' ', ' ', ' ', ' '},
	{'o', ' ','o', ' ', 'o', ' ', 'o', ' '},
	{' ', 'o',' ', 'o', ' ', 'o', ' ', 'o'},
	{'o', ' ','o', ' ', 'o', ' ', 'o', ' '}};

	int running = 1;
	int player = 1;
	int error = 0;
	int mustEat = 0;
	int cheats = 0;
	//Main GameLoop
	while (running==1) {
		//Draw UI
		draw(board);
		printf("==========\nPlayer: %d", player);

		//Print if Error
		if (error == 1) {
			printf("\n!!!!!That Move Isn't Allowed Try Again!!!!!");
			error = 0;
		}
		//Check board state to see if player must eat a piece;
		mustEat = checkBoard(board, player);
		if (mustEat == 1) printf("\nYou Must Eat A Piece This Turn!");

		//Get User Input
		char target[3];
		printf("\nSelect Target Peice:\n");
		scanf("%2s", &target);

		//Check For CheatCode
		if (target[0] == 'C' && target[1] == 'C') {
			if (cheats == 0) cheats = 1;
			else cheats = 0;
			continue;
		}

		char pos[3];
		printf("Select Target Location:\n");
		scanf("%2s", &pos);

		//Validate + Move
		int isValid = 1;
		if(cheats == 0) isValid = validate(target, pos, player, board, mustEat);
		if (isValid == 1) {
			//Convert String input to 2D Array Coords
			int from[2];
			convertString(target, from);
			int to[2];
			convertString(pos, to);

			move(board, from, to);

			mustEat = checkBoard(board, player);
			printf("\nChanging Players: ");
			printf("%d", player);
			printf("<= Player MustEat=> ");
			printf("%d", mustEat);
			printf(" Trying Bool: ");
			//printf(player == 1 && mustEat == 0);
			if (player == 1 && mustEat == 0) player = 2;
			else if(mustEat == 0) player = 1;
		}
		else error = 1;

		if (onePieces == 0 || twoPieces == 0) running = 0;
	}
	clrscr();
	if (onePieces == 0) printf("===> Congratulations Player 2 Has Won! <===\n");
	else printf("===> Congratulations Player 1 Has Won! <===\n");
}
//Draw Game
void draw(char board[][8]) {
	clrscr();
	printf("===========> C-Checkers By: Jahson<===========\n");
	printf("To Play Use The Following Input:\nA4\nWhere A4= Position On Table\n\n");
	printf("Player 1= x Player 2=o\n");
	printf("==========\n");
	printf("  A B C D E F G H\n");
	for (int i = 0; i < 8; i++) {
		printf("%d", i + 1);
		for (int j = 0; j < 8; j++) {
			printf("%c", '|');
			printf("%c", board[i][j]);
		}
		printf("|");
		printf("%d", i + 1);
		printf("\n");
	}
	printf("  A B C D E F G H\n");

}

//Validate Move
int validate(char target[2], char pos[2], int player, char board[][8], int mustEat) {
	//Check If Coords Is On Board
	if (target[0] < 'A' || pos[0] < 'A' || target[0]>'H' || pos[0]>'H') return 0;
	if (target[1] - '0' < 1 || pos[1] - '0' < 1 || target[1] - '0' > 8 || pos[1] - '0' > 8) return 0;

	int from[2];
	int to[2];
	convertString(target, from);
	convertString(pos, to);

	//Check if Target Location is Empty
	if (board[to[0]][to[1]] != ' ') return 0;

	//Check If Kings
	if (board[from[0]][from[1]] == '*' || board[from[0]][from[1]] == '!') return kingValidate(from, to, player, board, mustEat);

	//Check if Target Piece is Of Player
	if (player == 1 && board[from[0]][from[1]] != 'x') return 0;
	if (player == 2 && board[from[0]][from[1]] != 'o') return 0;


	//Check if Target Location is +1 than normal a piece is inbetween and remove the piece;
	if (to[0] - from[0] == 2  && abs(to[1] - from[1]) == 2) {
		if (player == 1) {
			if (board[to[0] - 1][(to[1] + from[1]) / 2] != 'o') return 0;
			else {
				board[to[0] - 1][(to[1] + from[1]) / 2] = ' ';
				mustEat = 0;
				twoPieces--;
				return 1;
			}
		}
		else {
			if (board[to[0] - 1][(to[1] + from[1]) / 2] != 'x') return 0;
			else {
				board[to[0] - 1][(to[1] + from[1]) / 2] = ' ';
				mustEat = 0;
				onePieces--;
				return 1;
			}
		}

	}
	if (from[0] - to[0] == 2 && abs(to[1] - from[1]) == 2) {
		if (player == 1) {
			if (board[to[0] + 1][(to[1] + from[1]) / 2] != 'o') return 0;
			else {
				board[to[0] + 1][(to[1] + from[1]) / 2] = ' ';
				mustEat = 0;
				twoPieces--;
				return 1;
			}
		}
		else {
			if (board[to[0] + 1][(to[1] + from[1]) / 2] != 'x') return 0;
			else {
				board[to[0] + 1][(to[1] + from[1]) / 2] = ' ';
				mustEat = 0;
				onePieces--;
				return 1;
			}
		}
	}

	//Check If Target Location is Valid;
	if (player == 1 && (to[0] != from[0] + 1 || (to[1] != from[1] + 1 && to[1] != from[1] - 1))) return 0;
	if (player == 2 && (to[0] != from[0] - 1 || (to[1] != from[1] + 1 && to[1] != from[1] - 1))) return 0;
	 
	return 1 + mustEat;
}

//Handle Move Validation For Kings
int kingValidate(int from[2], int to[2], int player, char board[][8], int mustEat) {
	//Check If Players King
	if (player == 1 && board[from[0]][from[1]] == '!') return 0;
	if (player == 2 && board[from[0]][from[1]] == '*') return 0;

	int eaten = 0;
	int eatCoord[2] = { 0,0 };
	//If Going SouthEast
	for (int i = from[0]+1, j=from[1]+1; i < to[0] && j<to[1]; i++, j++) {
		if (board[i][j] != ' ') {
			if (player == 1) {
				//Check If path Blocked and If is, can eat.
				if (board[i][j] == 'x') return 0;
				if (board[i][j] == 'o' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
			else {
				if (board[i][j] == 'o') return 0;
				if (board[i][j] == 'x' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
		}
	}

	//If NorthWest
	for (int i = from[0] - 1, j = from[1] - 1; i > to[0] && j > to[1]; i--, j--) {
		if (board[i][j] != ' ') {
			if (player == 1) {
				//Check If path Blocked and If is, can eat.
				if (board[i][j] == 'x') return 0;
				if (board[i][j] == 'o' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
			else {
				if (board[i][j] == 'o') return 0;
				if (board[i][j] == 'x' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
		}
	}

	//If SouthWest
	for (int i = from[0] + 1, j = from[1] - 1; i < to[0] && j > to[1]; i++, j--) {
		if (board[i][j] != ' ') {
			if (player == 1) {
				//Check If path Blocked and If is, can eat.
				if (board[i][j] == 'x') return 0;
				if (board[i][j] == 'o' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
			else {
				if (board[i][j] == 'o') return 0;
				if (board[i][j] == 'x' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
		}
	}

	//If NorthEast
	for (int i = (from[0] - 1), j = (from[1] + 1); i > to[0] && j < to[1]; i--, j++) {
		if (board[i][j] != ' ') {
			if (player == 1) {
				//Check If path Blocked and If is, can eat.
				if (board[i][j] == 'x' || board[i][j] == '*') { return 0; }
				if ((board[i][j] == 'o' || board[i][j] == '!') && eaten == 1) { return 0; }
				else if(eaten == 0){
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
			else {
				if (board[i][j] == 'o') return 0;
				if (board[i][j] == 'x' && eaten == 1) return 0;
				else if (eaten == 0) {
					eaten = 1;
					eatCoord[0] = i;
					eatCoord[1] = j;
				}
			}
		}
	}

	if (eaten == 1) {
		if (player == 1) twoPieces--;
		else onePieces--;
		mustEat = 0;
		board[eatCoord[0]][eatCoord[1]] = ' ';

	}
	return 1;
}

//Checks board to see if the player must eat.
int checkBoard(char board[][8], int player) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//If Player 1
			if (board[i][j] == 'x' && player == 1) {
				if (i + 2 < 8 && j + 2 < 8 && board[i + 1][j + 1] == 'o' && board[i + 2][j + 2] == ' ') { return 1; }
				if (i + 2 < 8 && j - 2 >= 0 && board[i + 1][j - 1] == 'o' && board[i + 2][j - 2] == ' ') { return 1; }
				if (i - 2 >= 0 && j + 2 < 8 && board[i - 1][j + 1] == 'o' && board[i - 2][j + 2] == ' ') { return 1; }
				if (i - 2 >= 0 && j - 2 >= 0 && board[i - 1][j - 1] == 'o' && board[i - 2][j - 2] == ' ') { return 1; }
			}
			//If Player 2
			if (board[i][j] == 'o' && player == 2) {
				if (i + 2 < 8 && j + 2 < 8 && board[i + 1][j + 1] == 'x' && board[i + 2][j + 2] == ' ') { return 1; }
				if (i + 2 < 8 && j - 2 >= 0 && board[i + 1][j - 1] == 'x' && board[i + 2][j - 2] == ' ') { return 1; }
				if (i - 2 >= 0 && j + 2 < 8 && board[i - 1][j + 1] == 'x' && board[i - 2][j + 2] == ' ') { return 1; }
				if (i - 2 >= 0 && j - 2 >= 0 && board[i - 1][j - 1] == 'x' && board[i - 2][j - 2] == ' ') { return 1; }
			}
		}
	}
	return 0;
}

void move(char board[][8], int from[2], int to[2]) {
	char piece = board[from[0]][from[1]];
	//Check if Should promote to King
	if (to[0] == 0 && piece == 'o') piece = '!';
	if (to[0] == 7 && piece == 'x') piece = '*';

	board[from[0]][from[1]] = ' ';
	board[to[0]][to[1]] = piece;
}

//Fill integer array with converted coords
void convertString(char str[2], int arr[2]) {
	//Set X
	switch (str[0]) {
		case 'A': 
			arr[1] = 0;
			break;
		case 'B':
			arr[1] = 1;
			break;
		case 'C':
			arr[1] = 2;
			break;
		case 'D':
			arr[1] = 3;
			break;
		case 'E':
			arr[1] = 4;
			break;
		case 'F':
			arr[1] = 5;
			break;
		case 'G':
			arr[1] = 6;
			break;
		case 'H':
			arr[1] = 7;
			break;
	}
	arr[0] = str[1] - '0'-1;
}
//Clear terminal
void clrscr()
{
	system("@cls||clear");
}