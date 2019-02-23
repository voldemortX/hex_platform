#include "hexboard.h"


hexBoard::hexBoard()
{
    this->clearBoard();
}

hexBoard::~hexBoard()
{

}

bool hexBoard::makeMove(short x, short y, short player)
{
    // a player makes a move, players: 1-red, 2-blue
    if(x<1 || x>11||y<1 || y>11)
    {
        QString wrongString = "Wrong Move("+QString::number(x)+","+QString::number(y)+","+
                QString::number(player)+")\n";
        moves.append(wrongString);
        return false;
    }
    //if(y<1 || y>11) return false;
    if (board[x][y])
    {
        QString wrongString = "Wrong Move("+QString::number(x)+","+QString::number(y)+","+
                QString::number(player)+")\n";
        moves.append(wrongString);
        return false;
    }

    board[x][y] = player;
    // record format: (x,y,player)[\n]
    QString tempString = "(" + QString::number(x) + "," + QString::number(y)
                      + "," + QString::number(player) + ")\n";
    moves.append(tempString);
    return true;
}

bool hexBoard::clearBoard()
{
    for (int i = 0; i <= 12; ++i)
        for (int j = 0; j <= 12; ++j)
            board[i][j] = 0;

    turn = 0;
    moves = "";
    status = -1;
    who = 0;
    return true;
}

void hexBoard::setTurn(short turnIn)
{
    turn = turnIn;
}

short hexBoard::getTurn()
{
    return turn;
}

QString hexBoard::getMoves()
{
    return moves;
}

short hexBoard::getWho()
{
    return who;
}

bool hexBoard::setWho(short whoIn)
{
    if (whoIn != 0 && whoIn != 1)
        return false;
    who = whoIn;
    return true;
}

bool hexBoard::setStatus(short statusIn)
{
    if (statusIn != 0 && statusIn != -1 && statusIn != 2 && statusIn != 3)
        return false;
    status = statusIn;
    return true;
}

short hexBoard::getStatus()
{
    return status;
}

short hexBoard::checkStatus()
{
    // 0: ongoing, 1: red won, 2: blue won
    short flag[13][13] = {0};
    status = 0;
    for(int i = 1;i <= 11 ;i++)
    {
        if(searchBoard(RED,1,i,flag)) {status = 1;qDebug()<<i<<"Red";return 1;}
        if(searchBoard(BLUE,i,1,flag)) {status = 2;qDebug()<<i<<"Blue";return 2;}
    }
    return status;
}
bool hexBoard::searchBoard(short type, short posx, short posy, short flag[][13])
{
    // francis's dfs
//    if(!board[posx][posy]) return false;
    if(board[posx][posy]!=type) return false;
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
