#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct Ship {
	char name[12];
	int size;
	char character;
};

struct Player {
	int hits;
	int misses;
	double hmr;
};

int welcome_screen();
void print_rules();
void init_board(char board[][10], int rows, int cols, char symbol);
void print_board(char board[][10], int rows, int cols, int player);
void place_ship(struct Ship ship, char board[][10], char israndom, FILE* logfile);
int take_turn(char board[][10], int random, FILE* logfile);
void check_destroyed(int whats_left[], char board[][10], int player, FILE* logfile);
int win_condition(int whats_left[]);