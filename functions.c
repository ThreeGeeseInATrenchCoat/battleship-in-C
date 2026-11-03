#include "header.h"

int welcome_screen() {
	int choice;
	do {
		printf("Welcome to Battleship!\n"
			"Input 1 to view rules\n"
			"Input 2 to play game\n"
			"input 3 to exit\n");
		scanf("%d", &choice);
	} while (choice != 1 && choice != 2 && choice != 3);
	return choice;
}

void print_rules() {
	system("cls");
	printf("RULES\n"
		"1. Each player has a 10x10 board\n"
		"2. Each player has 5 ships of varying sizes\n"
		"3. Players take turns guessing coordinates to strike\n"
		"4. If a ship is hit, it will be marked with a #\n"
		"5. If a strike misses, it will be marked with a *\n"
		"6. The first player to destroy all of the opponent's ships wins\n");
	system("pause");
	system("cls");
}

void init_board(char board[][10], int rows, int cols, char symbol) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			board[i][j] = symbol;
		}
	}
}

void print_board(char board[][10], int rows, int cols, int player) {
	printf("  0 1 2 3 4 5 6 7 8 9\n");
	for (int i = 0; i < rows; i++) {
		printf("%d ", i);
		for (int j = 0; j < cols; j++) {

			//dont give away cpu positions
			if (player == 2) {
				if (board[i][j] != '~' && board[i][j] != '*' && board[i][j] != '#') {
					printf("~ ");
				}
				else {
					printf("%c ", board[i][j]);
				}
			}
			else {
				printf("%c ", board[i][j]);
			}
			
		}
		printf("\n");
	}
}

void place_ship(struct Ship ship, char board[][10], char israndom, FILE* logfile) {

	int flag = 0;
	int direction, x, y;

	do {
		if (flag == 1 && israndom != 'y') {
			system("cls");
			printf("INVALID SPACE\n");
		}

		flag = 0;
		if (israndom != 'y') {
			
			print_board(board, 10, 10, 1);

			do {
				printf("input direction of %s (0/1) (horizontal/vertical): ", ship.name);
				scanf("%d", &direction);
			} while(direction != 0 && direction != 1);

			do {
				printf("input coordinates of ship (x y): ");
				scanf("%d", &x);
				scanf("%d", &y);
			} while (x < 0 || x >= 10 || y < 0 || y >= 10);
		}
		else {
			direction = rand() % 2;
			x = rand() % 10;
			y = rand() % 10;
		}
		if (direction == 0) {	//left/right

			if (x < 0 || x + ship.size > 10 ||
				y < 0 || y >= 10) {
				flag = 1;
			}

			//check if it can be placed there
			for (int i = x; i < ship.size + x; i++) {
				if (board[y][i] != '~') {
					flag = 1;
					break;
				}
			}

			if (flag == 0) {
				for (int i = x; i < ship.size + x; i++) {
					board[y][i] = ship.character;
				}
			}
		}
		if (direction == 1) {	//up/down

			if (y < 0 || y + ship.size > 10 ||
				x < 0 || x >= 10) {
				flag = 1;
			}

			//check for valid placement
			for (int i = y; i < ship.size + y; i++) {
				if (board[i][x] != '~') {
					flag = 1;
					break;
				}
			}

			if (flag == 0) {
				for (int i = y; i < ship.size + y; i++) {
					board[i][x] = ship.character;
				}
			}
		}
	} while (flag == 1);
	fprintf(logfile, "Ship %s placed at (%d, %d) direction: %d\n", ship.name, x, y, direction);
}

int take_turn(char board[][10],int random, FILE* logfile) {
	int x, y, flag;
	do {
		flag = 0;
		if (random == 0) {
			printf("input coordinates to strike: ");
			scanf("%d", &x);
			scanf("%d", &y);
		}
		if (random == 1) {
			x = rand() % 10;
			y = rand() % 10;
		}

		if (board[y][x] == '*' || board[y][x] == '#') {
			if (random == 0) {
				printf("cannot strike same place twice.\n");
			}		
			flag = 1;
		}
		else if (board[y][x] == '~') {

			if (random == 0) {
				fprintf(logfile, "Player MISS at (%d, %d)\n", random + 1, x, y);
			}
			else {
				fprintf(logfile, "CPU MISS at (%d, %d)\n", random + 1, x, y);
			}

			printf("player %d ", random + 1);
			printf("MISS\n");
			board[y][x] = '*';
			return 0;
		}
		else {
			
			if (random == 0) {
				fprintf(logfile, "Player HIT at (%d, %d)\n", random + 1, x, y);
			}
			else {
				fprintf(logfile, "CPU HIT at (%d, %d)\n", random + 1, x, y);
			}

			printf("player %d ", random + 1);
			printf("HIT\n");
			board[y][x] = '#';
			return 1;
		}

	} while (flag == 1);
}

void check_destroyed(int whats_left[], char board[][10], int player, FILE* logfile) {
	int carrier = 0;
	int patrol_boat = 0;
	int battleship = 0;
	int cruiser = 0;
	int submarine = 0;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] == 'C') {
				carrier = 1;
			}
			if (board[i][j] == 'P') {
				patrol_boat = 1;
			}
			if (board[i][j] == 'B') {
				battleship = 1;
			}
			if (board[i][j] == 'R') {
				cruiser = 1;
			}
			if (board[i][j] == 'S') {
				submarine = 1;
			}
		}
	}

	if (carrier == 0) {
		//check if its been destroyed already
		if (whats_left[0] == 1) {

			if (player == 1) {
				fprintf(logfile, "\nPlayer destroyed CPU carrier\n\n");
				printf("Player destroyed CPU carrier\n");
			}
			else {
				fprintf(logfile, "\nCPU destroyed Player carrier\n\n");
				printf("CPU destroyed Player carrier\n");
			}

			//set it to have been destroyed
			whats_left[0] = 0;
		}
	}
	if (patrol_boat == 0) {
		if (whats_left[1] == 1) {
			if (player == 1) {
				fprintf(logfile, "\nPlayer destroyed CPU patrol boat\n\n");
				printf("Player destroyed CPU patrol boat\n");
			}
			else {
				fprintf(logfile, "\nCPU destroyed Player patrol boat\n\n");
				printf("CPU destroyed Player patrol boat\n");
			}
			whats_left[1] = 0;
		}
	}
	if (battleship == 0) {
		if (whats_left[2] == 1) {
			if (player == 1) {
				fprintf(logfile, "\nPlayer destroyed CPU battleship\n\n");
				printf("Player destroyed CPU battleship\n");
			}
			else {
				fprintf(logfile, "\nCPU destroyed Player battleship\n\n");
				printf("CPU destroyed Player battleship\n");
			}
			whats_left[2] = 0;
		}
	}
	if (cruiser == 0) {
		if (whats_left[3] == 1) {
			if (player == 1) {
				fprintf(logfile, "\nPlayer destroyed CPU cruiser\n\n");
				printf("Player destroyed CPU cruiser\n");
			}
			else {
				fprintf(logfile, "\nCPU destroyed Player cruiser\n\n");
				printf("CPU destroyed Player cruiser\n");
			}
			whats_left[3] = 0;
		}
	}
	if (submarine == 0) {
		if (whats_left[4] == 1) {
			if (player == 1) {
				fprintf(logfile, "\nPlayer destroyed CPU submarine\n\n");
				printf("Player destroyed CPU submarine\n");
			}
			else {
				fprintf(logfile, "\nCPU destroyed Player submarine\n\n");
				printf("CPU destroyed Player submarine\n");
			}
			whats_left[4] = 0;
		}
	}
}

int win_condition(int whats_left[]) {
	if (whats_left[0] == 0 && whats_left[1] == 0 && whats_left[2] == 0 && whats_left[3] == 0 && whats_left[4] == 0) {
		return 1;
	}
	else {
		return 0;
	}
}
