#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Player.hpp"
#include "Moves.hpp"
#include "Undo.hpp"
#include "portal.hpp"
#include "ConfigReader.hpp"

class Game {
private:
    int moveCount = 0;
    bool promotion = false;
    Board board;
    ConfigReader config;
    Portal portal; 
    Moves moves;
    Undo undo;
    bool whiteCheckmate = false;
    bool blackCheckmate = false;
    
public:
    Player player1;
    Player player2;

    Game();

    bool playerMove(Player& player, const Position& from, const Position& to);
    void displayBoard() const { board.display(); }
    bool UndoMove();
    void isChangedKingOrRook(const Position& from);
    void makeCastling(Player& player,const Position& from, const Position& to);
    void displayPortals() const { portal.displayPortals(); }
    void promotoPawn( const Position& from , std::string Piece);
    bool getCheckmate(const Player& player);
    std::vector<Position> getPossibleMoves(const Position& pos, bool isWhite);
};


#endif
