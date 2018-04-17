#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <string>
#include <QPainter>
#include <QMessageBox>
#include <QString>
#include <QDebug>

class hexBoard
{
public:
    hexBoard();
    ~hexBoard();
    short checkStatus();
    bool setStatus(short statusIn);
    short getStatus();
    bool setWho(short whoIn);
    short getWho();
    bool makeMove(short x, short y, short player);
    bool clearBoard();
    void setTurn(short turnIn);
    short getTurn();
    QString getMoves();

private:
    short status;  // 0: Ongoing, 1: Red won, 2: Blue won, -1: Pending(Nada)
    short who;  // 0: red is thinking, 1: blue is thinking
    short turn;  // 0: red's turn, 1: blue's turn
    short board[13][13];  // indices start from 1, 0: nothing, 1: red, 2:blue
    QString moves;  // history
    bool searchBoard(short type,short posx,short posy,short flag[][13]);
    const short RED=1;
    const short BLUE=2;
};

#endif // HEXBOARD_H

