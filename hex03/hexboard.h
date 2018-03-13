#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <string>
#include "mainwindow.h"


class hexBoard
{
public:
    hexBoard(MainWindow* window);
    ~hexBoard();
    checkStatus();
    makeMove(std::string move, int player, MainWindow* window);
    clearBoard();
    getMoves();

private:
    short status;  // 0: Ongoing, 1: Blue won, 2: Red won
    short board[12][12];  // indices start from 1
    std::string moves;  // history
};

#endif // HEXBOARD_H

