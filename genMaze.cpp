/********************************************************
 *
 * Random maze generate algorithm using backtracking
 *
 * Function getIdx(), createMaze(), and part of generateMaze()
 * is wrote by @Akshat Malviya (kshat157)
 *
 * https://github.com/akshat157/randomMazeGenerator
 *
 *********************************************************/

#include <fstream>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "genMaze.h"

using namespace std;

// store maze to default (and user defined location (optimal))
void storeMaze(int M, int N, char** maze);

// print out the maze, 'S' stand for start, 'F' stand for finish, '.' stand for
// legal path, '#' stand for wall
void displayMaze(int M, int N, char** maze);

int getIdx(int x, int y, vector<pair<int, pair<int, int> > > cell_list);
void createMaze(int M, int N, char** maze);
char** generateMaze(int* rows, int* cols);

const char* invalid_char_message = "Error, invalid character";
const char* invalid_maze_message = "Invalid maze";
const char* no_path_message = "No path could be found!";
const char* invalid_input_message = "\nInvalid input, please re-enter";
const char* main_program_quit_message =
    "********** Main program quit **********";
const char* main_program_restart_message =
    "********** Main program restart **********";
const char* user_input_message = "User input: ";
char maze_raw_addresss[] = "output/mazeRaw.bmp";
char maze_solved_addresss[] = "output/mazeSolved.bmp";

// Default values
int m = 4, n = 4;

void storeMaze(int M, int N, char** maze) {
    ofstream ofile("output/mazeRaw.in");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == ' ') {
                maze[i][j] = '.';
            }
        }
    }

    ofile << M << " " << N << endl;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            ofile << maze[i][j];
        }
        ofile << endl;
    }
    ofile.close();
    cout << "Successfully saves the random maze to 'output/mazeRaw.in'" << endl;
    cout << endl;
    cout << "Save the maze to user defined location?" << endl;
    char storeMaze;
    cout << "Please enter Y or N" << endl;
    cout << user_input_message;
    cin >> storeMaze;
    char storeMazeDoubleCheck = '\0';
    while (!(storeMaze == 'Y' || storeMaze == 'y' || storeMaze == 'N' ||
             storeMaze == 'n')) {
        cout << invalid_input_message << endl;
        cout << "Please enter Y or N" << endl;
        cout << user_input_message;
        cin >> storeMaze;
    }

    if (storeMaze == 'Y' || storeMaze == 'y') {
        do {
            cout << "Please enter the location (name) of maze (enter q to "
                    "quit): ";
            char buffer[40];
            cin >> buffer;
            char* outputAddress = new char[strlen(buffer) + 1];
            strcpy(outputAddress, buffer);
            if (!strcmp(outputAddress, "q")) {
                cout << "********** Maze saver quit **********" << endl;
                break;
            }
            cout << "Confirm save the maze to '" << outputAddress << "'"
                 << endl;
            cout << "Please enter Y or N" << endl;
            cout << user_input_message;
            cin >> storeMazeDoubleCheck;
            while (
                !(storeMazeDoubleCheck == 'Y' || storeMazeDoubleCheck == 'y' ||
                  storeMazeDoubleCheck == 'N' || storeMazeDoubleCheck == 'n')) {
                cout << invalid_input_message << endl;
                cout << "Please enter Y or N" << endl;
                cout << user_input_message;
                cin >> storeMazeDoubleCheck;
            }

            if (storeMazeDoubleCheck == 'N' || storeMazeDoubleCheck == 'n') {
                continue;
                ;
            }

            ofstream ofile(outputAddress);
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (maze[i][j] == ' ') {
                        maze[i][j] = '.';
                    }
                }
            }

            ofile << M << " " << N << endl;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    ofile << maze[i][j];
                }
                ofile << endl;
            }
            ofile.close();

            if (storeMaze != 'q') {
                cout << "Successfully save the random maze to '"
                     << outputAddress << "'" << endl;
            }
        } while (!(storeMazeDoubleCheck == 'Y' || storeMazeDoubleCheck == 'y'));
    }
}

void displayMaze(int M, int N, char** maze) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (maze[i][j] == ' ') {
                maze[i][j] = '.';
            }
        }
    }

    cout << M << " " << N << endl;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
}

// A utility function to get the index of cell with indices x, y;
int getIdx(int x, int y, vector<pair<int, pair<int, int> > > cell_list) {
    for (int i = 0; i < cell_list.size(); i++) {
        if (cell_list[i].second.first == x && cell_list[i].second.second == y)
            return cell_list[i].first;
    }
    cout << "getIdx() couldn't find the index!" << endl;
    return -1;
}

void createMaze(int M, int N, char** maze) {
    vector<pair<int, pair<int, int> > > cell_list;
    vector<bool> visited(m * n, false);
    stack<pair<int, pair<int, int> > > m_stack;
    random_device rdev;
    mt19937 rng(rdev());
    uniform_int_distribution<mt19937::result_type> dist100(1, 100);

    int nVisited = 0;
    int k = 0;

    for (int i = 1; i < M; i += 2) {
        for (int j = 1; j < N; j += 2) {
            cell_list.push_back(make_pair(k, make_pair(i, j)));
            k++;
        }
    }

    int randIdx = dist100(rng) % m * n;
    m_stack.push(cell_list[randIdx]);
    visited[randIdx] = true;
    nVisited++;

    // Algo
    while (nVisited < m * n) {
        vector<int> neighbours;
        // North
        if (m_stack.top().second.first > 1) {
            if (maze[m_stack.top().second.first - 2]
                    [m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first - 2,
                                m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(0);
            }
        }
        // East
        if (m_stack.top().second.second < N - 2) {
            if (maze[m_stack.top().second.first + 0]
                    [m_stack.top().second.second + 2] &&
                !visited[getIdx(m_stack.top().second.first + 0,
                                m_stack.top().second.second + 2, cell_list)]) {
                neighbours.push_back(1);
            }
        }
        // South
        if (m_stack.top().second.first < M - 2) {
            if (maze[m_stack.top().second.first + 2]
                    [m_stack.top().second.second + 0] &&
                !visited[getIdx(m_stack.top().second.first + 2,
                                m_stack.top().second.second + 0, cell_list)]) {
                neighbours.push_back(2);
            }
        }
        // West
        if (m_stack.top().second.second > 1) {
            if (maze[m_stack.top().second.first + 0]
                    [m_stack.top().second.second - 2] &&
                !visited[getIdx(m_stack.top().second.first + 0,
                                m_stack.top().second.second - 2, cell_list)]) {
                neighbours.push_back(3);
            }
        }
        // Neighbours available?
        if (!neighbours.empty()) {
            // Choose a random direction
            int next_cell_dir = neighbours[dist100(rng) % neighbours.size()];
            // Create a path between this cell and neighbour
            switch (next_cell_dir) {
                case 0:  // North
                    maze[m_stack.top().second.first - 1]
                        [m_stack.top().second.second + 0] = '.';
                    m_stack.push(cell_list[getIdx(
                        m_stack.top().second.first - 2,
                        m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 1:  // East
                    maze[m_stack.top().second.first + 0]
                        [m_stack.top().second.second + 1] = '.';
                    m_stack.push(cell_list[getIdx(
                        m_stack.top().second.first + 0,
                        m_stack.top().second.second + 2, cell_list)]);
                    break;
                case 2:  // South
                    maze[m_stack.top().second.first + 1]
                        [m_stack.top().second.second + 0] = '.';
                    m_stack.push(cell_list[getIdx(
                        m_stack.top().second.first + 2,
                        m_stack.top().second.second + 0, cell_list)]);
                    break;
                case 3:  // West
                    maze[m_stack.top().second.first + 0]
                        [m_stack.top().second.second - 1] = '.';
                    m_stack.push(cell_list[getIdx(
                        m_stack.top().second.first + 0,
                        m_stack.top().second.second - 2, cell_list)]);
                    break;
            }

            visited[m_stack.top().first] = true;
            nVisited++;
        } else {
            m_stack.pop();
        }
    }
}

char** generateMaze(int* rows, int* cols) {
    cout << "Random maze generator initiated" << endl;
    cout << "Please enter the size of maze you want (5 <= integer number of "
            "rows <= 250) x (5 <= integer number of columns <= 250), enter 0 "
            "to quit"
         << endl;
    cout << "Number of rows = ";
    // m, n;
    cin >> m;

    if (m == 0) {
        cout << "********** Random maze generator quit **********" << endl;
        return NULL;
    }

    while (cin.fail() || m > 250 || m < 5) {
        cout << "\nInvalid input of number of rows" << endl;
        cout << "Please re-enter the number of rows = ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> m;
        if (m == 0) {
            cout << "********** Random maze generator quit **********" << endl;
            return NULL;
        }
    }

    cout << "Number of columns = ";
    cin >> n;

    if (n == 0) {
        cout << "********** Random maze generator quit **********" << endl;
        return NULL;
    }

    while (cin.fail() || n > 250 || n < 5) {
        cout << "\nInvalid input of number of columns" << endl;
        cout << "Please re-enter the number of columns = ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> n;
        if (m == 0) {
            cout << "********** Random maze generator quit **********" << endl;
            return NULL;
        }
    }

    cout << "\nGenerating..." << endl;
    m /= 2;
    n /= 2;

    /*
    while (m < 1 || n < 1) {
        cout << "Desired dimensions impossible. Re-enter pls." << endl;
        cin >> m >> n;
    }
    */

    int M = 2 * m + 1;
    int N = 2 * n + 1;

    *rows = M;
    *cols = N;

    char** maze;
    maze = new char*[M];

    for (int i = 0; i < M; i++) {
        maze[i] = new char[N];
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (!(i & 1) || !(j & 1))
                maze[i][j] = '#';
            else
                maze[i][j] = '.';
        }
    }

    for (int i = 1; i < M; i += 2) {
        for (int j = 1; j < N; j += 2) {
            maze[i][j] = '.';
        }
    }
    createMaze(M, N, maze);
    maze[0][1] = 'S';
    maze[2 * m][2 * n - 1] = 'F';
    // displayMaze(M, N, maze);
    cout << "Successfully generates the random maze" << endl;
    storeMaze(M, N, maze);
    return maze;
}
