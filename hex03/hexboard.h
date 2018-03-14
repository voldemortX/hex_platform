#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <string>
#include "mainwindow.h"


class hexBoard
{
public:
    hexBoard(MainWindow* window);
    ~hexBoard();
    short checkStatus();
    bool makeMove(std::string move, int player, MainWindow* window);
    bool clearBoard(MainWindow* window);
    std::string getMoves();

private:
    short status;  // 0: Ongoing, 1: Red won, 2: Blue won
    short turn;  // 1: red's turn, 2: blue's turn
    short board[12][12];  // indices start from 1, 0: nothing, 1: red, 2:blue
    std::string moves;  // history
};

#endif // HEXBOARD_H

