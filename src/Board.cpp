#include "Board.hpp"
#include "ConfigReader.hpp"
#include <iostream>
#include <iomanip>

Board::Board() {

    std::cout << "Initializing board..." << std::endl;
    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            chess_board[i][j] = "·";  
        }
    }
    
    std::string configPath = "data/chess_pieces.json";
  
    if (!configReader.loadFromFile(configPath)) {
        std::cerr << "Failed to load configuration." << std::endl;
        return;
    }

    const GameConfig &config = configReader.getConfig();

    for (const auto &piece : config.pieces) {
        std::cout << "Placing piece: " << piece.type << std::endl;
        place_pieces(piece);
    }
    for(const auto &piece : config.custom_pieces) {
        std::cout << "Placing piece: " << piece.type << std::endl;
        place_pieces(piece);
    }
    
    std::cout << "Board initialization complete. Current board state:" << std::endl;
    display();

}

void Board::place_pieces(const PieceConfig &piece) {
    if (piece.positions.count("white") > 0) {
        for (const auto &pos : piece.positions.at("white")) {
            chess_board[pos.y][pos.x] = "w"+ piece.type;
        }
    }
    if (piece.positions.count("black") > 0) {
        for (const auto &pos : piece.positions.at("black")) {
            chess_board[pos.y][pos.x] = "b"+ piece.type;
        }
    }
}

const std::string Board::getSymbolWhite(const PieceConfig &piece) const{
    return piece.symbol.at("white");
}

const std::string Board::getSymbolBlack(const PieceConfig &piece) const{
    return piece.symbol.at("black");
}

void Board::display() const {
    const GameConfig &config = configReader.getConfig();
    std::cout << "\n  ";
    for(int x = 0; x < 8; x++) {
        std::cout << static_cast<char>('a' + x) << " ";
    }
    std::cout << "\n\n";

    for(int y = 0; y < 8; y++) {
        std::cout << y+1 << " ";

        for(int x = 0; x < 8; x++) {
            bool pieceFound = false;
            for(const auto& piece : config.pieces) {
                if(chess_board[y][x] == "w" + piece.type) {
                    std::cout << getSymbolWhite(piece) << " ";
                    pieceFound = true;
                    break;
                } else if(chess_board[y][x] == "b" + piece.type) {
                    std::cout << getSymbolBlack(piece) << " ";
                    pieceFound = true;
                    break;
                }
            }
            for(const auto& piece : config.custom_pieces){
                if(chess_board[y][x] == "w" + piece.type) {
                    std::cout << piece.symbol.at("white") << " ";
                    pieceFound = true;
                    break;
                } else if(chess_board[y][x] == "b" + piece.type) {
                    std::cout << piece.symbol.at("black") << " ";
                    pieceFound = true;
                    break;
                }
            }
            if (!pieceFound) {
                std::cout << chess_board[y][x] << " ";
            }
        }
        std::cout << std::endl;
    }
}
