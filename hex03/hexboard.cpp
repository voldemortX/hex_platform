#include "hexboard.h"
#include <QPainter>
#include <QMessageBox>
#include <QString>
#include <string>

hexBoard::hexBoard(MainWindow* window)
{
    QPixmap tempPix;
    tempPix.load("img/board.jpg");
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
    bool flag = tempPix.load("img/blue.png");
    if (player == 1)
        flag = tempPix.load("img/red.png");
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
    for (int i = 0; i <= 12; ++i)
        for (int j = 0; j <= 12; ++j)
            board[i][j] = 0;
    moves = "";
    status = 0;
    return window->clearPieces();
}
std::string hexBoard::getMoves()
{
    return moves;
}
short hexBoard::checkStatus()
{
    // 0: ongoing, 1: red won, 2: blue won
    short flag[13][13];
    for(int i = 1;i <= 11 ;i++)
    {
        if(searchBoard(RED,1,i,flag)) status = 1;
        if(searchBoard(BLUE,i,1,flag)) status = 2;
    }
    status = 0;
    return status;
}
bool hexBoard::searchBoard(short type, short posx, short posy, short flag[][13])
{
    if(((type == RED) && (posx == 11))||((type == BLUE) && (posy == 11))) return 1;
    flag[posx][posy] = 1;
    short newx,newy;
    short diff[6][2]={
        {0,-1},//up
        {0,1},//down
        {-1,0},//left
        {1,0},//right
        {-1,1},//left-down
        {1,-1},//right-up
    };
    for (int i = 0; i < 6; i++)
    {
        newx = posx + diff[i][0];
        newy = posy + diff[i][1];
        if((board[newx][newy] == type) && (!flag[newx][newy]))
        {
            if(searchBoard(type,newx,newy,flag)) return 1;
        }
    }
    flag[posx][posy] = 0;
    return 0;
}
