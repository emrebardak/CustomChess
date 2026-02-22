#ifndef BOARD_HPP
#define BOARD_HPP

#include <string>
#include "ConfigReader.hpp"

class Board {
private:
    ConfigReader configReader;

public:
    std::string chess_board[8][8];
    Board();
    void display() const;
    void place_pieces(const PieceConfig &piece);
    const std::string& getPiece(int x, int y) const { return chess_board[y][x]; }
    const std::string getSymbolWhite(const PieceConfig &piece) const;
    const std::string getSymbolBlack(const PieceConfig &piece) const;
};

#endif
