#include "Game.hpp"
#include <iostream>

Game::Game() : moveCount(0),
               board(),
               config(),
               portal(board, config),
               moves(board ,portal),
               undo(board , portal),
               player1("White Player", true),
               player2("Black Player", false) {
                
    if (!config.loadFromFile("data/chess_pieces.json")) {
        std::cerr << "Failed to load configuration file!" << std::endl;
        return;
    }           
    moves.createBoardNode();
    moves.initializeGraph();
    
    portal.displayPortals();
}

std::vector<Position> Game::getPossibleMoves(const Position& pos, bool isWhite) {
    return moves.getPossibleMoves(pos, isWhite);
}

bool Game::playerMove(Player& player, const Position& from, const Position& to) {
    if(player.getMovesCount() > 100){
        std::cout << "Game Finished" << std::endl;
        return false;
    }
    std::cout << "Move Count : " << player.getMovesCount() << std::endl;
    if(!moves.isValidMove(from, to, player.isWhitePlayer())) {
        std::cout << "Invalid Move" << std::endl;
        return false;
    }
    
    // Save the original pieces
    std::string movingPiece = board.chess_board[from.y][from.x];
    std::string capturedPiece = board.chess_board[to.y][to.x];

    Undo::MoveHistory move(from, to, movingPiece, capturedPiece, player.isWhitePlayer());
    
    // Save to history
    if(!undo.addMove(move)){
        std::cout << "Failed to add Move" << std::endl;
        return false;
    }

    // Make the move
    board.chess_board[from.y][from.x] = "·";
    board.chess_board[to.y][to.x] = movingPiece;

    if (movingPiece.find("King") != std::string::npos) {
        makeCastling(player,from,to);
    }

    //Promotion
    if(movingPiece == "wPwan" || movingPiece == "bPawn"){
        promotion = moves.isValidPromotion(to);
    }

    // Update the graph with the new board state
    moves.createBoardNode();
    moves.initializeGraph();

    // Check if the move puts the opponent in check
    bool putsInCheck = moves.isCheck(!player.isWhitePlayer());
    if(putsInCheck) {
        std::cout << (player.isWhitePlayer() ? "Black" : "White") << " is in check!" << std::endl;
        
        // Check if it's checkmate
        if(moves.isCheckMate(!player.isWhitePlayer())) {
            std::cout << "Checkmate! " << (player.isWhitePlayer() ? "White" : "Black") << " wins!" << std::endl;
            if(player.isWhitePlayer()) {
                blackCheckmate = true;
            } else {
                whiteCheckmate = true;
            }
            return true;
        }
    }

    player.increaseMoveCount();
    isChangedKingOrRook(from);
    portal.decreaseCooldown(player.isWhitePlayer());
    return true;
}


bool Game::UndoMove() {
    if(undo.UndoMove()) {
        Undo::MoveHistory lastMove = undo.getLastMove();
        std::cout << (lastMove.isWhiteMove ? "White's" : "Black's") << " move from " 
                  << char(lastMove.beforePos.x + 'a') << (8 - lastMove.beforePos.y)
                  << " to "
                  << char(lastMove.afterPos.x + 'a') << (8 - lastMove.afterPos.y)
                  << " has been undone." << std::endl;
        moves.createBoardNode();
        moves.initializeGraph();
        moveCount--;
        return true;
    } else {
        std::cout << "Cannot undo: No moves available." << std::endl;
    }   
    return false;
}

void Game::promotoPawn( const Position& from , std::string Piece){
    board.chess_board[from.y][from.x] = Piece;
}

void Game::isChangedKingOrRook(const Position& from){
    Moves::Node node = moves.boardGraph[from];
    
    if(node.pieceType == "wRook"){
        if(from.x == 0 ){
            moves.longRookMovedWhite = true;
        }else if (from.x == 7 ){
            moves.shortRookMovedWhite = true;
        }
    }else if(node.pieceType == "bRook"){
        if(from.x == 0 ){
            moves.longRookMovedBlack = true;
        }else if (from.x == 7 ){
            moves.shortRookMovedBlack = true;
        }
    }else if(node.pieceType == "wKing"){
        moves.kingMovedWhite = true;
    }else if(node.pieceType == "bKing"){
        moves.kingMovedBlack = true;
    }
}

void Game::makeCastling(Player& player , const Position& from, const Position& to){
        // Short 
        if (from.x == 4 && to.x == 6) {
            if (player.isWhitePlayer()) {
                board.chess_board[0][7] = "·";
                board.chess_board[0][5] = "wRook";
            } else {
                board.chess_board[7][7] = "·";
                board.chess_board[7][5] = "bRook";
            }
        }
        // Long 
        else if (from.x == 4 && to.x == 2) {
            if (player.isWhitePlayer()) {
                board.chess_board[0][0] = "·";
                board.chess_board[0][3] = "wRook";
            } else {
                board.chess_board[7][0] = "·";
                board.chess_board[7][3] = "bRook";
            }
        }
}

bool Game::getCheckmate(const Player& player){
    return player.isWhitePlayer() ? blackCheckmate : whiteCheckmate ;
}