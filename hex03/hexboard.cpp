#include "hexboard.h"
#include <QPainter>
#include <QMessageBox>
#include <QString>
#include <string>

hexBoard::hexBoard(MainWindow* window)
{
    QPixmap tempPix;
    tempPix.load("../hex03/img/board.jpg");
    bool flag = window->setPic(0, 0, tempPix);
    if (!flag)
    {
        QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    this->clearBoard(window);
}
hexBoard::~hexBoard()
{

}
//checkStatus()
bool hexBoard::makeMove(std::string move, int player, MainWindow* window)
{
    // a player makes a move, players: 1-red, 2-blue
    short x = move[0] - 'A' + 1;
    short y = move[1] - 'A' + 1;
    if (board[x][y])
        return false;

    board[x][y] = player;
    QPixmap tempPix;
    bool flag = tempPix.load("../hex03/img/blue.png");
    if (player == 1)
        flag = tempPix.load("../hex03/img/red.png");
    else if (player != 2)
        return false;

    if (!flag)
    {
        QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return false;
    }
    window->setPic(x, y, tempPix);
    // record format: (x,y,player)[\n]
    std::string tempString = "(" + std::to_string(x) + "," + std::to_string(y)
                      + "," + std::to_string(player) + ")\n";
    moves += tempString;
    return true;
}
bool hexBoard::clearBoard(MainWindow* window)
{
    for (int i = 0; i <= 11; ++i)
        for (int j = 0; j <= 11; ++j)
            board[i][j] = 0;
    moves = "";
    status = 0;
    return window->clearPieces();
}
std::string hexBoard::getMoves()
{
    return moves;
}

