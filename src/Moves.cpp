#include "Moves.hpp"
#include "iostream"
Moves::Moves(Board& boardRef, Portal& portalRef) 
: board(boardRef), portal(portalRef){
    createBoardNode();
}

void Moves::createBoardNode(){

    for (auto& pair : boardGraph) {
        pair.second.neighbors.clear();
    }

    boardGraph.clear();

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Position pos{x, y};
            Node node;
            node.pos = pos;
            node.pieceType = board.chess_board[y][x];
            boardGraph[pos] = node;
            
        }
    }
}

void Moves::initializeGraph() {
    Position pos;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            pos.x = j;
            pos.y = i;

            Node& node = boardGraph[pos];
            node.neighbors.clear();

            if(node.pieceType.empty() || node.pieceType == "·" || node.pieceType.length() < 2) {
                continue;
            }
            
            if (node.pieceType.length() > 1) {
                std::string type = node.pieceType.substr(1); // for erase w and b
                if (type == "Pawn") {
                    initializePawnMoves(node, pos);
                } else if (type == "Knight") {
                    initializeKnightMoves(node, pos);
                } else if (type == "Bishop") {
                    initializeBishopMoves(node, pos);
                } else if (type == "Rook") {
                    initializeRookMoves(node, pos);
                } else if (type == "Queen") {
                    initializeQueenMoves(node, pos);
                } else if (type == "King") {
                    initializeKingMoves(node, pos);
                } else if(type == "LongMan"){
                    initializeLongManMoves(node,pos);
                }
            }        
        }
    }  
}

std::vector<Position> Moves::getPossibleMoves(const Position& from, bool isWhite) {
    
    if(isEmpty(from)) {

        return {};
    }

    if (boardGraph.find(from) == boardGraph.end()) {
        createBoardNode();
        initializeGraph();
    }

    //it should be same color
    if(boardGraph.at(from).pieceType[0] == 'b' && isWhite) {
        return {};
    }
    if(boardGraph.at(from).pieceType[0] == 'w' && !isWhite) {
        return {};
    }
    
    std::vector<Position> possibleMoves = boardGraph.at(from).neighbors;

    if(portal.isPortalPosition(from)){
        int id = portal.getPortalId(from);
        if(portal.checkPortal(isWhite, id)){
            possibleMoves.push_back(portal.getExitPosition(from));
        }
    }

    return possibleMoves;
}

void Moves::initializePawnMoves(Node& node, const Position& pos) {
    
    bool isWhite;
    int drc; // white positive  |  black negative 
    if(node.pieceType[0] == 'w'){
        drc = 1;
        isWhite = true;
    }else{
        drc = -1;
        isWhite = false;
    }
    
    // move
    Position forward{pos.x, pos.y + (1 * drc)};
    
    if(isWithinBoard(forward) && isEmpty(forward)){
        if(!willBeCheck(pos, forward , !isWhite)) node.neighbors.push_back(forward);
        
    }
    
    if((isWhite && pos.y == 1) || (!isWhite && pos.y == 6)){  // Starting positions for pawns
        Position doubleForward{pos.x, pos.y + (2 * drc)};
        
        if(isWithinBoard(doubleForward) && isEmpty(forward) && isEmpty(doubleForward)){
            if(!willBeCheck(pos, doubleForward , !isWhite)) node.neighbors.push_back(doubleForward);
        }
    }
    
    // capture
    Position leftDiagonal{pos.x - 1, pos.y + (1 * drc)};
    if(isWithinBoard(leftDiagonal)){
        std::string targetPiece = board.chess_board[leftDiagonal.y][leftDiagonal.x];
        
        if(!isEmpty(leftDiagonal) && 
           ((isWhite && targetPiece[0] == 'b') || (!isWhite && targetPiece[0] == 'w'))) {
            if(!willBeCheck(pos, leftDiagonal , isWhite)) node.neighbors.push_back(leftDiagonal);        
        }
    }
    
    Position rightDiagonal{pos.x + 1, pos.y + (1 * drc)};
    if(isWithinBoard(rightDiagonal)){
        std::string targetPiece = board.chess_board[rightDiagonal.y][rightDiagonal.x];
        
        if(!isEmpty(rightDiagonal) && 
           ((isWhite && targetPiece[0] == 'b') || (!isWhite && targetPiece[0] == 'w'))) {
            if(!willBeCheck(pos, rightDiagonal , isWhite)) node.neighbors.push_back(rightDiagonal);
        }
    }

}

void Moves::initializeKnightMoves(Node& node, const Position& pos) {
    std::vector<std::pair<int, int>> moves = {
        {-2, -1},  // 2 left, 1 up
        {-2, 1},   // 2 left, 1 down
        {2, -1},   // 2 right, 1 up
        {2, 1},    // 2 right, 1 down
        {-1, -2},  // 1 left, 2 up
        {-1, 2},   // 1 left, 2 down
        {1, -2},   // 1 right, 2 up
        {1, 2}     // 1 right, 2 down
    };

    bool isWhite = node.pieceType[0] == 'w';

    for(const auto& move : moves) {
        Position newPos = {
            pos.x + move.first,
            pos.y + move.second
        };
        
        if(!isWithinBoard(newPos)) {
            continue;
        }

        if(isEmpty(newPos)) {
            if(!willBeCheck(pos, newPos, isWhite)) node.neighbors.push_back(newPos);
        } else {
            std::string targetPiece = board.chess_board[newPos.y][newPos.x];
            
            // capture
            if((targetPiece[0] == 'b' && node.pieceType[0] == 'w') || 
               (targetPiece[0] == 'w' && node.pieceType[0] == 'b')) {
                if(!willBeCheck(pos, newPos , isWhite)) node.neighbors.push_back(newPos);
            }
        }
    }
}

void Moves::initializeBishopMoves(Node& node, const Position& pos) {
    std::vector<std::pair<int, int>> directions = {
        {-1, -1},  // left-up
        {-1, 1},   // left-down
        {1, -1},   // right-up
        {1, 1}     // right-down
    };

    bool isWhite = node.pieceType[0] == 'w';

    for (const auto& dir : directions) {
        Position current = pos;
        while (true) {
            current.x += dir.first;
            current.y += dir.second;

            if (!isWithinBoard(current)) {
                break;
            }

            if (isEmpty(current)) {
                if(!willBeCheck(pos, current , isWhite)) node.neighbors.push_back(current);
            } else {
                // capture
                std::string targetPiece = board.chess_board[current.y][current.x];
                if ((targetPiece[0] == 'b' && node.pieceType[0] == 'w') || 
                    (targetPiece[0] == 'w' && node.pieceType[0] == 'b')) {
                    if(!willBeCheck(pos, current , isWhite)) node.neighbors.push_back(current);
                }
                break;
            }
        }
    }
}

void Moves::initializeRookMoves(Node& node, const Position& pos) {
    std::vector<std::pair<int, int>> directions = {
        {-1, 0},   // left
        {1, 0},    // right
        {0, -1},   // up
        {0, 1}     // down
    };

    bool isWhite = node.pieceType[0] == 'w';

    for(const auto& dir : directions){
        Position current = pos;
        while(true){
            current.x += dir.first;
            current.y += dir.second;

            if(!isWithinBoard(current)) break;

            if (isEmpty(current)) {
                if(!willBeCheck(pos, current , isWhite)) node.neighbors.push_back(current);
            } else {
                std::string targetPiece = board.chess_board[current.y][current.x];
                // capture
                if ((targetPiece[0] == 'b' && node.pieceType[0] == 'w') || 
                    (targetPiece[0] == 'w' && node.pieceType[0] == 'b')) {
                    if(!willBeCheck(pos, current , isWhite)) node.neighbors.push_back(current);
                }
                break;
            }
        }
    }
}

void Moves::initializeQueenMoves(Node& node, const Position& pos) {
    initializeBishopMoves(node, pos);
    initializeRookMoves(node, pos);
}

void Moves::initializeKingMoves(Node& node, const Position& pos) {
    std::vector<std::pair<int, int>> moves = {
        {-1, -1},  // left-up
        {-1, 0},   // left
        {-1, 1},   // left-down
        {0, -1},   // up
        {0, 1},    // down
        {1, -1},   // right-up
        {1, 0},    // right
        {1, 1}     // right-down
    };

    bool isWhite = node.pieceType[0] == 'w';

    for(const auto& move : moves) {
        Position newPos = {
            pos.x + move.first,
            pos.y + move.second
        };
        
        if(!isWithinBoard(newPos)) {
            continue;
        }

        if(isEmpty(newPos) || 
           (board.chess_board[newPos.y][newPos.x][0] != (isWhite ? 'w' : 'b'))) {
            if (!willBeCheck(pos,newPos,isWhite)) {
                node.neighbors.push_back(newPos);
            }
        }
    }

    Position shortCastle{6, pos.y};
    if(isValidShortCastling(pos, shortCastle, isWhite)) {
        node.neighbors.push_back(shortCastle);
    }

    Position longCastle{2, pos.y};
    if(isValidLongCastling(pos, longCastle, isWhite)) {
        node.neighbors.push_back(longCastle);
    }

}
//custom piece
void Moves::initializeLongManMoves(Node& node, const Position& pos){
    std::vector<std::pair<int, int>> moves = {
        {-2, -2},  // left-up
        {-2, 0},   // left
        {-2, 2},   // left-down
        {0, -2},   // up
        {0, 2},    // down
        {2, -2},   // right-up
        {2, 0},    // right
        {2, 2}     // right-down
    };

    bool isWhite = node.pieceType[0] == 'w';
    
    for(const auto& move : moves) {
        Position newPos = {
            pos.x + move.first,
            pos.y + move.second
        };
        
        if(!isWithinBoard(newPos)) {
            continue;
        }

        if(isEmpty(newPos)){
            if(!willBeCheck(pos,newPos,isWhite)) node.neighbors.push_back(newPos);          
        }else{
            std::string targetPiece = board.chess_board[newPos.y][newPos.x];
            // capture
            if ((targetPiece[0] == 'b' && node.pieceType[0] == 'w') || 
                (targetPiece[0] == 'w' && node.pieceType[0] == 'b')) {
                if(!willBeCheck(pos, newPos , isWhite)) node.neighbors.push_back(newPos);
            }
        }
    }
}

bool Moves::isWithinBoard(const Position& pos){
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

bool Moves::isEmpty(const Position& pos)  {
    if (!isWithinBoard(pos)) {
        return false;
    }
    if(board.chess_board[pos.y][pos.x] == "·") return true;
    return false;
}

bool Moves::isValidMove(const Position& from, const Position& to, bool isWhite) {
    
    if (!isWithinBoard(from) || !isWithinBoard(to)) {
        return false;
    }


    if (boardGraph.find(from) == boardGraph.end()) {
        createBoardNode();
        initializeGraph();
    }
    //Check valid Portal
    if(portal.isPortalPosition(from)){
        int id = portal.getPortalId(from);
        if(portal.checkPortal(isWhite, id)){
            Position exit = portal.getExitPosition(from);
            if(exit.x == to.x && exit.y == to.y){
                return true;
            }
        }
        
    }
    const Node& fromNode = boardGraph[from];
    const Node& toNode = boardGraph[to];
    
    // Check if there's a piece at the source position
    if (fromNode.pieceType.empty() || fromNode.pieceType == "·") {
        return false;
    }

    // Check if the piece belongs to the correct player
    bool pieceIsWhite = fromNode.pieceType[0] == 'w';
    if (pieceIsWhite != isWhite) {
        return false;
    }
    // Can't capture your own piece
    if (!toNode.pieceType.empty() && toNode.pieceType != "·") {
        bool targetIsWhite = toNode.pieceType[0] == 'w';
        if (targetIsWhite == isWhite) {
            return false;  
        }
    }

    std::vector<Position> validMoves = getPossibleMoves(from, isWhite);
    if (std::find(validMoves.begin(), validMoves.end(), to) == validMoves.end()) {
        return false;
    }

    // Then check if the move would result in check
    return !willBeCheck(from, to, isWhite);

}

bool Moves::isCheck(bool isWhite) {

    Position kingPos;
    whiteCheck = false;
    blackCheck = false;
    bool kingFound = false;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            std::string piece = board.chess_board[y][x];
            if (piece[0] == (isWhite ? 'w' : 'b') && piece.find("King") != std::string::npos) {
                kingPos = Position{x, y};
                kingFound = true;
                break;
            }
        }
    }

    if (!kingFound) {
        return false;
    }

    std::string King = board.chess_board[kingPos.y][kingPos.x];

    board.chess_board[kingPos.y][kingPos.x] = "·"; // for looking back of it

    // Check if any opponent's piece can capture the king
    bool inCheck = false;
    for (const auto& entry : boardGraph) {
        const Node& node = entry.second;
        if (node.pieceType == "·" || node.pieceType[0] == (isWhite ? 'w' : 'b'))
            continue;
        for (const Position& move : node.neighbors) {
            if (move.x == kingPos.x && move.y == kingPos.y) {
                if(isWhite){
                    whiteCheck = true;                
                }else{
                    blackCheck = true;
                }
                inCheck = true;
            }
        }
        if (inCheck) break;
    }

    board.chess_board[kingPos.y][kingPos.x] = King;

    return inCheck;
}

bool Moves::isCheckMate(bool isWhite) {

    if (!isCheck(isWhite)) {
        return false;
    }

    // Try to find any move that gets out of check
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            std::string piece = board.chess_board[y][x];

            if (piece == "·" || piece[0] != (isWhite ? 'w' : 'b')) {
                continue;
            }

            Position piecePos{x, y};
            std::vector<Position> moves = getPossibleMoves(piecePos, isWhite);

            // Try each move
            for (const auto& move : moves) {
                // Save original positions
                std::string originalFromPiece = board.chess_board[y][x];
                std::string originalToPiece = board.chess_board[move.y][move.x];

                // Make the move
                board.chess_board[y][x] = "·";
                board.chess_board[move.y][move.x] = originalFromPiece;

                // Check if still in check
                bool stillInCheck = isCheck(isWhite);

                // Undo the move
                board.chess_board[y][x] = originalFromPiece;
                board.chess_board[move.y][move.x] = originalToPiece;

                if (!stillInCheck) {
                    return false; // Found a move that gets out of check
                }
            }
        }
    }

    return true; // No moves get out of check
}

bool Moves::isValidPromotion(const Position& pos){
    if(pos.y  == 7 && boardGraph[pos].pieceType == "wPawn"){
        return true;
    }else if(pos.y  == 0 && boardGraph[pos].pieceType == "bPawn"){
        return true;
    }
    return false;
}

bool Moves::willBeCheck(const Position& from, const Position& to, bool isWhite) {
    // Save the pieces 
    std::string movingPiece = board.chess_board[from.y][from.x];
    std::string capturedPiece = board.chess_board[to.y][to.x];

    // Make the move
    board.chess_board[from.y][from.x] = "·";
    board.chess_board[to.y][to.x] = movingPiece;

    bool check = isCheck(isWhite);

    // Undo the move
    board.chess_board[from.y][from.x] = movingPiece;
    board.chess_board[to.y][to.x] = capturedPiece;

    return check;
}

bool Moves::isValidShortCastling(const Position& from, const Position& to, bool isWhite){
    if(isCheck(isWhite)) return false;
    //they shouldnt have move
    if(isWhite){
        if(kingMovedWhite || shortRookMovedWhite) return false;
    }else{
        if(kingMovedBlack ||shortRookMovedBlack) return false;
    }
    // path clear
    for(int x = 5; x < 7 ; x++){
        Position pos{x,from.y};
        if(!isEmpty(pos)) return false;
    }
    //path doesnt in attack
    for(int x = 5 ; x < 8 ; x++){
        Position pos {x,from.y};
        if(willBeCheck(from,pos,isWhite)) return false;
    }
    return from.x == 4 && to.x == 6;
}
bool Moves::isValidLongCastling(const Position& from, const Position& to, bool isWhite){
    if(isCheck(isWhite)) return false;
    //they shouldnt have move
    if(isWhite){
        if(kingMovedWhite || longRookMovedWhite) return false;
    }else{
        if(kingMovedBlack ||longRookMovedBlack) return false;
    }
    //path clear
    for(int x = 1 ; x < 4 ; x++){
        Position pos{x,from.y};
        if(!isEmpty(pos)) return false;
    }
    //path doesnt in attack
    for(int x = 1 ; x < 5 ; x++){
        Position pos {x,from.y};
        if(willBeCheck(from,pos,isWhite)) return false;
    }
    return from.x == 4 && to.x == 2;
}