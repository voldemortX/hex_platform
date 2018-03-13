#include "hexboard.h"
#include <QPainter>
#include <QMessageBox>
#include <QString>

hexBoard::hexBoard(MainWindow* window)
{
    //QPainter boardPainter(window);
    status = 0;
    for (int i = 0; i <= 11; ++i)
        for (int j = 0; j <= 11; ++j)
            board[i][j] = 0;
    moves = "";

    QPixmap tempPix;
    tempPix.load("../hex03/img/board.jpg");
    bool flag = window->setPic(0, 0, tempPix);
    if (!flag)
    {
        QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    //boardPainter.drawPixmap(0, 0, 900, 600, tempPix);
}
hexBoard::~hexBoard()
{

}
//checkStatus()
hexBoard::makeMove(std::string move, int player, MainWindow* window)
{
    // a player makes a move, players: 1-red, 2-blue
    short x = move[0] - 'A' + 1;
    short y = move[1] - 'A' + 1;
    board[x][y] = player;
    QPixmap tempPix;
    bool flag = tempPix.load("../hex03/img/blue.png");
    if (player == 1)
        flag = tempPix.load("../hex03/img/red.png");

    if (!flag)
    {
        QMessageBox::information(NULL, "Title", QString::number(flag), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    window->setPic(x, y, tempPix);
}
//clearBoard()
//getMoves()

