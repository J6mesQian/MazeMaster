#ifndef GENMAZE_H
#define GENMAZE_H

#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <fstream>

void storeMaze(int M, int N, char** maze);
void displayMaze(int M, int N, char** maze);
void createMaze(int M, int N, char** maze);
char** generateMaze(int *rows, int *cols);

#endif