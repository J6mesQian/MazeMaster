/********************************************************
 * Welcome to Maze Maste
 * Author: Yuxi Qian
 * Date: 10-20-2022
 * Version: 2.3
 *********************************************************/

#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "bmplib.h"
#include "genMaze.h"
#include "mazeio.h"
#include "queue.h"

using namespace std;

// maze solver
int maze_search(char**, int, int);

// output the maze as bmp file
void bmpOut(char** maze, int rows, int cols, char* outputAddress);

// clear mazeRaw.bmp and mazeSolved.bmp
void clearBMP(unsigned char blank[256][256][3], char* mazeRawBMP,
              char* mazeSolvedBMP);

// main function
int main(int argc, char* argv[]) {
    const char* invalid_char_message = "Error, invalid character";
    const char* invalid_maze_message = "Invalid maze";
    const char* no_path_message = "No path could be found!";
    const char* invalid_input_message = "\nInvalid input";
    const char* main_program_quit_message =
        "********** Main program quit **********";
    const char* main_program_restart_message =
        "********** Main program restart **********";
    const char* user_input_message = "User input: ";
    char maze_raw_addresss[] = "output/mazeRaw.bmp";
    char maze_solved_addresss[] = "output/mazeSolved.bmp";
    int rows, cols, result;
    char** mymaze = NULL;

    // construct a all white bmp file
    unsigned char blank[256][256][3];
    for (int r = 0; r < 256; r++) {
        for (int c = 0; c < 256; c++) {
            blank[r][c][0] = 255;
            blank[r][c][1] = 255;
            blank[r][c][2] = 255;
        }
    }

    cout << endl;
    cout << endl;
    cout << "**************************" << endl;
    cout << "* Welcome to Maze Master *          " << endl;
    cout << "* Author: Yuxi Qian      *" << endl;
    cout << "* Date: 10-20-2022       *" << endl;
    cout << "* Version: 2.3           *" << endl;
    cout << "**************************" << endl;
    cout << endl;
    cout << "********** Maze Master Initiated **********" << endl;
    cout << endl;
    ///*

    char mode = '\0';

    while (mode != 'q') {
        // cout << endl;
        cout << " ------------------------------------- " << endl;
        cout << "| 0: Read a maze file and solve       |" << endl;
        cout << "| 1: Generate a random maze and solve |" << endl;
        cout << "| c: Clear all BMP files              |" << endl;
        cout << "| q: Quit                             |" << endl;
        cout << " -------------------------------------" << endl;
        cout << "Please enter 0 or 1 or c or q" << endl;
        cout << user_input_message;
        cin >> mode;
        cout << endl;

        while (!(mode == '0' || mode == '1' || mode == 'c' || mode == 'q')) {
            // cout << endl;
            cout << invalid_input_message << endl;
            cout << " ------------------------------------- " << endl;
            cout << "| 0: Read a maze file and solve       |" << endl;
            cout << "| 1: Generate a random maze and solve |" << endl;
            cout << "| c: Clear all BMP files              |" << endl;
            cout << "| q: Quit                             |" << endl;
            cout << " -------------------------------------" << endl;
            cout << "Please enter 0 or 1 or c or q" << endl;
            cout << user_input_message;
            cin >> mode;
            cout << endl;
        }

        char* inputAddress;

        // mode 0 read a maze file and solve
        if (mode == '0') {
            bool breakMode0 = false;
            cout << "Please enter the address of maze (enter q to quit): ";
            char buffer[40];
            cin >> buffer;
            inputAddress = new char[strlen(buffer) + 1];
            strcpy(inputAddress, buffer);
            // if user enters q, then quit the maze reader
            if (!strcmp(inputAddress, "q")) {
                cout << "********** Maze reader quit **********" << endl;
                cout << main_program_restart_message << endl;
                cout << endl;
                continue;
            }
            mymaze = read_maze(inputAddress, &rows, &cols);
            // if successfully reads
            if (mymaze != NULL) {
                // if rows or cols of maze > 250 then quit the maze reader
                if (rows > 250 || cols > 250) {
                    cout << "Input maze size is too large" << endl;
                    cout << "********** Maze reader quit **********" << endl;
                    cout << main_program_restart_message << endl;
                    cout << endl;
                    continue;
                }
                cout << "Successfully reads "
                     << "'" << inputAddress << "'" << endl;
                bmpOut(mymaze, rows, cols, maze_raw_addresss);
                cout << "You can access the mazeRaw.bmp at the output folder"
                     << endl;
                cout << endl;
            }
            // if doesn't successfully reads, then ask user to re-enter the
            // location of file
            while (mymaze == NULL) {
                cout << "Couldn't locate file "
                     << "'" << inputAddress << "'" << endl;
                cout << "Please re-enter the address of maze (enter q to "
                        "quit): ";
                char buffer[40];
                cin >> buffer;
                inputAddress = new char[strlen(buffer) + 1];
                strcpy(inputAddress, buffer);
                // if user enters q, then quit the maze reader
                if (!strcmp(inputAddress, "q")) {
                    cout << "********** Maze reader quit **********" << endl;
                    cout << main_program_restart_message << endl;
                    cout << endl;
                    breakMode0 = true;
                    break;
                }
                // if rows or cols of maze > 250 then quit the maze reader
                mymaze = read_maze(inputAddress, &rows, &cols);
                if (mymaze != NULL) {
                    if (rows > 250 || cols > 250) {
                        cout << "Input maze size is too large" << endl;
                        cout << "********** Maze reader quit **********"
                             << endl;
                        cout << main_program_restart_message << endl;
                        cout << endl;
                        breakMode0 = true;
                        break;
                    }
                    cout << "Successfully reads "
                         << "'" << inputAddress << "'" << endl;
                    bmpOut(mymaze, rows, cols, maze_raw_addresss);
                    cout << "You can access the "
                         << "'" << maze_raw_addresss << "'"
                         << " at the output folder" << endl;
                    cout << endl;
                }
            }
            // if breakMode0 = ture, then restart the main program
            if (breakMode0) {
                continue;
            }
        }

        // generate a random maze and solve
        else if (mode == '1') {
            // genrate the random raze
            mymaze = generateMaze(&rows, &cols);
            if (mymaze == NULL) {
                cout << "********** Maze generation quit or failed **********"
                     << endl;
                cout << main_program_restart_message << endl;
                cout << endl;
                continue;
            }
            bmpOut(mymaze, rows, cols, maze_raw_addresss);
            cout << "You can access the "
                 << "'" << maze_raw_addresss << "'"
                 << " at the output folder" << endl;
            cout << endl;
        }

        // clear all bmp file
        else if (mode == 'c') {
            clearBMP(blank, maze_raw_addresss, maze_solved_addresss);
            cout << "********** BMP files are cleared **********" << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            continue;
        }

        // quit the main program
        else if (mode == 'q') {
            cout << main_program_quit_message << endl;
            cout << endl;
            return 0;
        }
        //*/

        // check the validity of the maze
        bool invalidChar = false;
        bool invalidMaze = false;
        int foundS = 0;
        int foundF = 0;

        // check for invalid_char and invalid_maze situation
        for (int r = 0; r < rows; r++) {
            if (invalidChar) {
                break;
            }
            for (int c = 0; c < cols; c++) {
                if (!(mymaze[r][c] == 'S' || mymaze[r][c] == 'F' ||
                      mymaze[r][c] == '.' || mymaze[r][c] == '#')) {
                    invalidChar = true;
                    cout << "r =" << r << " "
                         << "c = " << c << " - '" << mymaze[r][c] << "'"
                         << endl;
                    break;
                }
                if (mymaze[r][c] == 'S') {
                    foundS++;
                }
                if (mymaze[r][c] == 'F') {
                    foundF++;
                }
            }
        }

        invalidMaze = !(foundF == 1 && foundS == 1);

        if (invalidChar) {
            cout << invalid_char_message << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            for (int r = 0; r < rows; r++) {
                delete[] mymaze[r];
            }
            delete[] mymaze;
            continue;
        }

        if (invalidMaze) {
            cout << invalid_maze_message << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            for (int r = 0; r < rows; r++) {
                delete[] mymaze[r];
            }
            delete[] mymaze;
            continue;
        }

        // call maze_search and output the appropriate message or, if
        // successful, print the maze.
        char initiateSolver;
        cout << "Execute maze solver?" << endl;
        cout << "Please enter Y or N" << endl;
        cout << user_input_message;
        cin >> initiateSolver;
        while (!(initiateSolver == 'Y' || initiateSolver == 'y' ||
                 initiateSolver == 'N' || initiateSolver == 'n')) {
            cout << invalid_input_message << endl;
            cout << "Please enter Y or N" << endl;
            cout << user_input_message;
            cin >> initiateSolver;
        }

        if (initiateSolver == 'Y' || initiateSolver == 'y') {
            result = maze_search(mymaze, rows, cols);
        } else if (initiateSolver == 'N' || initiateSolver == 'n') {
            cout << "********** Maze solver quit **********" << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            continue;
            // return 0;
        }

        // -1 -> invalid_maze, 0 -> no_path, 1 -> success and print out the
        // modified maze
        if (result == -1) {
            cout << invalid_maze_message << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            for (int r = 0; r < rows; r++) {
                delete[] mymaze[r];
            }
            delete[] mymaze;
            continue;
        } else if (result == 0) {
            cout << no_path_message << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            for (int r = 0; r < rows; r++) {
                delete[] mymaze[r];
            }
            delete[] mymaze;
            continue;
        } else if (result == 1) {
            cout << "The maze is successfully solved" << endl;
            bmpOut(mymaze, rows, cols, maze_solved_addresss);
            cout << "You can access the "
                 << "'" << maze_solved_addresss << "'"
                 << " at the output folder" << endl;
            cout << main_program_restart_message << endl;
            cout << endl;
            for (int r = 0; r < rows; r++) {
                delete[] mymaze[r];
            }
            delete[] mymaze;
            continue;
        }

        // delete all memory that read_maze allocated: CHECKPOINT 2

        for (int r = 0; r < rows; r++) {
            delete[] mymaze[r];
        }
        delete[] mymaze;
        delete inputAddress;
    }
    return 0;
}

// BFS,
// attempt to find shortest path and return: 1 if successful, 0 if no path
// exists if path is found fill it in with '*' characters but don't overwrite the
// 'S' and 'F' cells
int maze_search(char** maze, int rows, int cols) {
    // *** You complete **** CHECKPOINT 4
    Location locS;
    // Location locF;
    bool foundS = false;
    bool foundF = false;
    // Locate the S and F and check for invalid_maze
    for (int r = 0; r < rows; r++) {
        if (foundS && foundF) {
            break;
        }
        // inverse S and F
        for (int c = 0; c < cols; c++) {
            if (maze[r][c] == 'S') {
                foundS = true;
            }
            if (maze[r][c] == 'F') {
                foundF = true;
                locS.row = r;
                locS.col = c;
            }
            if (foundS && foundF) {
                break;
            }
        }
    }

    if (!(foundS && foundF)) {
        return -1;
    }

    // construct explored 2-D array
    bool** explored = new bool*[rows];
    for (int r = 0; r < rows; r++) {
        explored[r] = new bool[cols];
        for (int c = 0; c < cols; c++) {
            explored[r][c] = false;
        }
    }

    // set S as explored
    explored[locS.row][locS.col] = true;

    // construct predecessor 2-D array, set nullLoc as the default value
    Location nullLoc;
    nullLoc.row = -1;
    nullLoc.col = -1;
    Location** predecessor = new Location*[rows];
    for (int r = 0; r < rows; r++) {
        predecessor[r] = new Location[cols];
        for (int c = 0; c < cols; c++) {
            predecessor[r][c] = nullLoc;
        }
    }

    // construct the queue
    Queue queue(rows * cols);
    queue.add_to_back(locS);
    Location temp;
    // int trail = 0;
    bool found = false;

    // executed the BFS pseudocode
    while (!queue.is_empty()) {
        // cout << trail++ << endl;
        // get head of queue and store it in temp
        temp = queue.remove_from_front();
        int tempRow = temp.row;
        int tempCol = temp.col;
        // cout << "tempRow =" << temp.row << " " << "tempCol =" << temp.col
        // <<endl;

        // Check below
        if (tempRow - 1 >= 0 && tempRow - 1 < rows) {
            if (maze[tempRow - 1][tempCol] == 'S') {
                found = true;
                break;
            }

            if (maze[tempRow - 1][tempCol] == '.' &&
                !explored[tempRow - 1][tempCol]) {
                explored[tempRow - 1][tempCol] = true;
                predecessor[tempRow - 1][tempCol] = temp;
                Location add;
                add.row = tempRow - 1;
                add.col = tempCol;
                queue.add_to_back(add);
            }
        }

        // Check above
        if (tempRow + 1 >= 0 && tempRow + 1 < rows) {
            if (maze[tempRow + 1][tempCol] == 'S') {
                found = true;
                break;
            }
            if (maze[tempRow + 1][tempCol] == '.' &&
                !explored[tempRow + 1][tempCol]) {
                explored[tempRow + 1][tempCol] = true;
                predecessor[tempRow + 1][tempCol] = temp;
                Location add;
                add.row = tempRow + 1;
                add.col = tempCol;
                queue.add_to_back(add);
            }
        }

        // Check left
        if (tempCol - 1 >= 0 && tempCol - 1 < cols) {
            if (maze[tempRow][tempCol - 1] == 'S') {
                found = true;
                break;
            }
            if (maze[tempRow][tempCol - 1] == '.' &&
                !explored[tempRow][tempCol - 1]) {
                explored[tempRow][tempCol - 1] = true;
                predecessor[tempRow][tempCol - 1] = temp;
                Location add;
                add.row = tempRow;
                add.col = tempCol - 1;
                queue.add_to_back(add);
            }
        }

        // Check right
        if (tempCol + 1 >= 0 && tempCol + 1 < cols) {
            // cout << "successful" << endl;
            if (maze[tempRow][tempCol + 1] == 'S') {
                found = true;
                break;
            }
            if (maze[tempRow][tempCol + 1] == '.' &&
                !explored[tempRow][tempCol + 1]) {
                explored[tempRow][tempCol + 1] = true;
                predecessor[tempRow][tempCol + 1] = temp;
                Location add;
                add.row = tempRow;
                add.col = tempCol + 1;
                queue.add_to_back(add);
            }
        }
    }
    const char* invalid_input_message = "\nInvalid input";
    const char* user_input_message = "User input: ";

    // if reach the F then return 1, otherwise return 0
    if (!found) {
        for (int r = 0; r < rows; r++) {
            delete[] explored[r];
        }
        delete[] explored;
        for (int r = 0; r < rows; r++) {
            delete[] predecessor[r];
        }
        delete[] predecessor;
        return 0;
    } else {
        char outputAddress[] = "output/mazeSolved.bmp";
        char** mazePlayBack = new char*[rows];
        for (int r = 0; r < rows; r++) {
            mazePlayBack[r] = new char[cols];
            for (int c = 0; c < cols; c++) {
                mazePlayBack[r][c] = maze[r][c];
            }
        }

        Location playBackTemp = temp;
        char mazePlayBackBool;
        cout << "\nExecuted maze solver with animation?" << endl;
        cout << "Please enter Y or N" << endl;
        cout << user_input_message;
        cin >> mazePlayBackBool;
        while (!(mazePlayBackBool == 'Y' || mazePlayBackBool == 'y' ||
                 mazePlayBackBool == 'N' || mazePlayBackBool == 'n')) {
            cout << invalid_input_message << endl;
            cout << "Please enter Y or N" << endl;
            cout << user_input_message;
            cin >> mazePlayBackBool;
        }

        if (mazePlayBackBool == 'Y' || mazePlayBackBool == 'y') {
            cout << "Playing maze solving animation..." << endl;
        }

        while (!(temp.row == locS.row && temp.col == locS.col)) {
            maze[temp.row][temp.col] = '*';
            temp = predecessor[temp.row][temp.col];
            if (mazePlayBackBool == 'Y' || mazePlayBackBool == 'y') {
                bmpOut(maze, rows, cols, outputAddress);
                usleep(100000 / ((rows + cols) / 2));
            }
        }

        while (mazePlayBackBool == 'Y' || mazePlayBackBool == 'y') {
            cout << "\nPlay back the maze solver animation?" << endl;
            cout << "Please enter Y or N" << endl;
            cout << user_input_message;
            cin >> mazePlayBackBool;
            while (!(mazePlayBackBool == 'Y' || mazePlayBackBool == 'y' ||
                     mazePlayBackBool == 'N' || mazePlayBackBool == 'n')) {
                cout << invalid_input_message << endl;
                cout << "Please enter Y or N" << endl;
                cout << user_input_message;
                cin >> mazePlayBackBool;
            }

            if (mazePlayBackBool == 'Y' || mazePlayBackBool == 'y') {
                for (int r = 0; r < rows; r++) {
                    for (int c = 0; c < cols; c++) {
                        maze[r][c] = mazePlayBack[r][c];
                    }
                }
                temp = playBackTemp;

                double playBackSpeed;
                cout << "Please enter animation play back speed (1.0 as "
                        "defalt, 0.1 <= play back speed <= 5)"
                     << endl;
                cout << user_input_message;
                cin >> playBackSpeed;
                while (cin.fail() || playBackSpeed < 0.1 || playBackSpeed > 5) {
                    cout << invalid_input_message << endl;
                    cout << "Please enter animation play back speed (1.0 as "
                            "defalt, 0.1 <= play back speed <= 5)"
                         << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cin >> playBackSpeed;
                }

                cout << "Playing maze solving animation..." << endl;
                while (!(temp.row == locS.row && temp.col == locS.col)) {
                    maze[temp.row][temp.col] = '*';
                    temp = predecessor[temp.row][temp.col];
                    bmpOut(maze, rows, cols, outputAddress);
                    usleep(100000 /
                           ((rows + cols) / 2 * pow(playBackSpeed, 1.5)));
                }

            } else if (mazePlayBackBool == 'N' || mazePlayBackBool == 'n') {
                cout << "********** Maze solver animation play back quit "
                        "**********"
                     << endl;
                cout << endl;
                break;
                // return 0;
            }
        }

        for (int r = 0; r < rows; r++) {
            delete[] explored[r];
        }
        delete[] explored;

        for (int r = 0; r < rows; r++) {
            delete[] predecessor[r];
        }
        delete[] predecessor;

        for (int r = 0; r < rows; r++) {
            delete[] mazePlayBack[r];
        }
        delete[] mazePlayBack;
        return 1;
    }
}

void bmpOut(char** maze, int rows, int cols, char* outputAddress) {
    unsigned char image[256][256][3];
    unsigned char imageZoom[256][256][3];
    // initiazte the image to all white with a 1px black borader
    for (int r = 0; r < 256; r++) {
        for (int c = 0; c < 256; c++) {
            image[r][c][0] = 255;
            image[r][c][1] = 255;
            image[r][c][2] = 255;
        }
    }

    for (int r = 0; r < rows + 2; r++) {
        for (int c = 0; c < cols + 2; c++) {
            image[r][c][0] = 0;
            image[r][c][1] = 0;
            image[r][c][2] = 0;
        }
    }

    // print out maze to bmp format
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (maze[r][c] == '.') {
                image[r + 1][c + 1][0] = 255;
                image[r + 1][c + 1][1] = 255;
                image[r + 1][c + 1][2] = 255;
            }
            if (maze[r][c] == '#') {
                image[r + 1][c + 1][0] = 0;
                image[r + 1][c + 1][1] = 0;
                image[r + 1][c + 1][2] = 0;
            }
            if (maze[r][c] == '*') {
                image[r + 1][c + 1][0] = 253;
                image[r + 1][c + 1][1] = 218;
                image[r + 1][c + 1][2] = 13;
            }
            if (maze[r][c] == 'S') {
                image[r + 1][c + 1][0] = 0;
                image[r + 1][c + 1][1] = 255;
                image[r + 1][c + 1][2] = 0;
            }
            if (maze[r][c] == 'F') {
                image[r + 1][c + 1][0] = 255;
                image[r + 1][c + 1][1] = 0;
                image[r + 1][c + 1][2] = 0;
            }
        }
    }

    for (int r = 0; r < 256; r++) {
        for (int c = 0; c < 256; c++) {
            imageZoom[r][c][0] = 255;
            imageZoom[r][c][1] = 255;
            imageZoom[r][c][2] = 255;
        }
    }

    // zoom the image
    int zoom = 256 / max(rows + 4, cols + 4);
    int blankRow = (256 - (rows + 2) * zoom) / 2;
    int blankCol = (256 - (cols + 2) * zoom) / 2;

    for (int r = 0; r < 256; r++) {
        for (int c = 0; c < 256; c++) {
            imageZoom[r + blankRow][c + blankCol][0] =
                image[r / zoom][c / zoom][0];
            imageZoom[r + blankRow][c + blankCol][1] =
                image[r / zoom][c / zoom][1];
            imageZoom[r + blankRow][c + blankCol][2] =
                image[r / zoom][c / zoom][2];
        }
    }

    writeRGBBMP(outputAddress, imageZoom);
}

// clear all BMP file
void clearBMP(unsigned char blank[256][256][3], char* mazeRawBMP,
              char* mazeSolvedBMP) {
    writeRGBBMP(mazeRawBMP, blank);
    writeRGBBMP(mazeSolvedBMP, blank);
}