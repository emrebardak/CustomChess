#ifndef UNDO_HPP
#define UNDO_HPP

#include <Moves.hpp>
#include <Board.hpp>

#include <stack>


class Undo{
public:

    Undo(Board& boardRef, Portal& portalRef) : board(boardRef), moves(boardRef, portalRef) {}

    struct MoveHistory
    {
        Position beforePos;
        Position afterPos;
        std::string beforeType;
        std::string afterType;
        bool isWhiteMove;

        MoveHistory(const Position& before, const Position& after, const std::string& beforeType, const std::string& afterType, bool isWhite)
            : beforePos(before), afterPos(after), beforeType(beforeType), afterType(afterType), isWhiteMove(isWhite) {}
    };
    
    MoveHistory getLastMove();
    bool UndoMove();
    bool addMove(const MoveHistory& move);

    
private:
    Board& board;
    Moves moves;
    std::stack<MoveHistory> LastMoves;

};

#endif