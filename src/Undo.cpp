#include <Undo.hpp>

bool Undo::UndoMove(){
    if(LastMoves.empty()) {
        return false;
    }

    MoveHistory lastMove = LastMoves.top();

    // Restore the pieces to their original positions
    board.chess_board[lastMove.beforePos.y][lastMove.beforePos.x] = lastMove.beforeType;  
    board.chess_board[lastMove.afterPos.y][lastMove.afterPos.x] = lastMove.afterType;     

    LastMoves.pop();
    moves.createBoardNode();
    moves.initializeGraph();

    return true;
}

Undo::MoveHistory Undo::getLastMove(){
    if(LastMoves.empty()) {
        throw std::runtime_error("No moves available");
    }
    return LastMoves.top(); 
}

bool Undo::addMove(const MoveHistory& move) {
    LastMoves.push(move);
    return true;
}

