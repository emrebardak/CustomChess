#pragma once

#include "ConfigReader.hpp"
#include "Board.hpp"
#include "portal.hpp"
#include <vector>
#include <unordered_map>

class Moves{
public:
    Moves(Board& boardRef, Portal& portalRef);

    struct Node {
        Position pos;
        std::string pieceType;
        std::vector<Position> neighbors;
    };

    std::unordered_map<Position, Node> boardGraph;
    
    bool whiteCheck = false;
    bool blackCheck = false;
    //for castling
    bool kingMovedWhite = false;
    bool kingMovedBlack = false;
    bool shortRookMovedWhite = false;
    bool shortRookMovedBlack = false;
    bool longRookMovedWhite = false;
    bool longRookMovedBlack = false;

    std::vector<Position> getPossibleMoves(const Position& from ,  bool isWhite);
    void createBoardNode();
    void initializeGraph();
    bool isValidMove(const Position& from, const Position& to,bool isWhite);
    bool isCheckMate(bool isWhite);
    bool isCheck(bool isWhite);
    bool isValidPromotion(const Position& pos);
    bool willBeCheck(const Position& from, const Position& to, bool isWhite);
    bool isValidShortCastling(const Position& from, const Position& to, bool isWhite);
    bool isValidLongCastling(const Position& from, const Position& to, bool isWhite);

private:

    Board& board;
    Portal& portal;

    bool isWithinBoard(const Position& pos);
    bool isEmpty(const Position& pos);

    void initializePawnMoves(Node& node, const Position& pos);
    void initializeKnightMoves(Node& node, const Position& pos);
    void initializeBishopMoves(Node& node, const Position& pos);
    void initializeRookMoves(Node& node, const Position& pos);
    void initializeQueenMoves(Node& node, const Position& pos);
    void initializeKingMoves(Node& node, const Position& pos);
    void initializeLongManMoves(Node& node, const Position& pos);
};