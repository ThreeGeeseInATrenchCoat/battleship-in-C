#include "header.h"

int main(void) {

	srand(time(NULL));

	char p1board[10][10] = { { '~' } };
	char p2board[10][10] = { { '~' } };
	int whats_left[5] = { 1, 1, 1, 1, 1 };
	char is_random = 'n';
	int choice = 0;
	int player_number = 1;
	int has_won = 0;

	struct Player player1 = { 0, 0, 0 };
	struct Player player2 = player1;

	struct Ship carrier = { "carrier", 5, 'C' };
	struct Ship patrol_boat = { "patrol boat", 2, 'P' };
	struct Ship battleship = { "battleship", 4, 'B' };
	struct Ship cruiser = { "cruiser", 3, 'R' };
	struct Ship submarine = { "submarine", 3, 'S' };

	do {
		choice = welcome_screen();
		switch (choice) {
		case 1:
			print_rules();
			break;
		case 2:
			system("cls");
			break;
		case 3:
			return 0;
		}
	} while (choice == 1);

	FILE * logfile = fopen("log.txt", "w");

	do {
		printf("randomly place ships?(y/n): ");
		scanf(" %c", &is_random);
	} while (is_random != 'y' && is_random != 'n');

	init_board(p1board, 10, 10, '~');
	init_board(p2board, 10, 10, '~');

	//place bot ships
	fprintf(logfile, "CPU ship placement:\n");
	place_ship(carrier, p2board, 'y', logfile);
	place_ship(battleship, p2board, 'y', logfile);
	place_ship(cruiser, p2board, 'y', logfile);
	place_ship(submarine, p2board, 'y', logfile);
	place_ship(patrol_boat, p2board, 'y', logfile);

	fprintf(logfile, "\nPlayer ship placement:\n");
	place_ship(carrier, p1board, is_random, logfile);
	place_ship(battleship, p1board, is_random, logfile);
	place_ship(cruiser, p1board, is_random, logfile);
	place_ship(submarine, p1board, is_random, logfile);
	place_ship(patrol_boat, p1board, is_random, logfile);
	fprintf(logfile, "\n");

	print_board(p2board, 10, 10, 2);
	print_board(p1board, 10, 10, 1);

	while (has_won == 0) {

		has_won = win_condition(whats_left);

		if (!has_won) {
			if (player_number == 1) {
				printf("Player turn\n");
				if (take_turn(p2board, 0, logfile) == 0) {
					player1.misses++;
				}
				else {
					player1.hits++;
				}
				check_destroyed(whats_left, p2board, 1, logfile);
				system("pause");
			}
			else {
				printf("CPU turn\n");
				if (take_turn(p1board, 1, logfile) == 0) {
					player2.misses++;
				}
				else {
					player2.hits++;
				}
				check_destroyed(whats_left, p1board, 2, logfile);
				system("pause");
				system("cls");
				print_board(p2board, 10, 10, 2);
				printf("\n");
				print_board(p1board, 10, 10, 1);
			}
		}

		if (player_number == 1) {
			player_number = 2;
		}
		else {
			player_number = 1;
		}

		if (has_won) {
			if (player_number == 1) {
				printf("Player wins!\n");
				fprintf(logfile, "Player wins!\n");
			}
			else {
				printf("CPU wins!\n");
				fprintf(logfile, "CPU wins!\n");
			}
		}
	}

	player1.hmr = (double)player1.hits / (double)player1.misses;
	player2.hmr = (double)player2.hits / (double)player2.misses;

	//check if misses is 0 to avoid division by 0
	if (player1.misses == 0) {
		player1.hmr = 1;
	}
	if (player2.misses == 0) {
		player2.hmr = 1;
	}

	fprintf(logfile, "\nPlayer hits: %d misses: %d hit/miss ratio: %.2f\n", player1.hits, player1.misses, player1.hmr);
	fprintf(logfile, "CPU hits: %d misses: %d hit/miss ratio: %.2f\n", player2.hits, player2.misses, player2.hmr);

	printf("Player hits: %d misses: %d hit/miss ratio: %.2f\n", player1.hits, player1.misses, player1.hmr);
	printf("CPU hits: %d misses: %d hit/miss ratio: %.2f\n", player2.hits, player2.misses, player2.hmr);
}
