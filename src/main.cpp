#include "Game.hpp"
#include <iostream>
#include <string>

Position notationToNum(std::string& notation){
    if (notation.length() != 2) {
        throw std::invalid_argument("Chess notation must be 2 characters");
    }
    
    char notationX = notation[0];
    char notationY = notation[1];
    
    if (notationX < 'a' || notationX > 'h') {
        throw std::invalid_argument("File must be a letter between a and h");
    }
    
    if (notationY < '1' || notationY > '8') {
        throw std::invalid_argument("Rank must be a number between 1 and 8");
    }
    
    Position pos;
    pos.x = notationX - 'a';
    pos.y = notationY - '1';
    
    return pos;
}

void displayHelp(){
    std::cout<< "move <AA> <BB>   ==> move AA to BB " << std::endl;
    std::cout<< "show <AA>   ==> show possible moves from AA " << std::endl;
    std::cout<< "undo   ==> undo last move" << std::endl;
    std::cout<< "exit  ==> close the program " << std::endl;
}

int main() {
    Game game;
    std::string input;
    displayHelp();
    int turn = 0; // turn % 2 = 0 White    turn % 2 = 1 black
    while(true) {
        Player& player = (turn % 2 == 0) ? game.player1 : game.player2;
        std::cout << (player.isWhitePlayer() ? "White's" : "Black's") << " Turn" << std::endl;
        std::cout << "\nEnter command: ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
      
        size_t spacePos = input.find(' ');
        std::string command = input.substr(0, spacePos);

        if (command == "move") {
            if (spacePos == std::string::npos || input.length() < 8) {  
                std::cout << "Invalid move format. Use: move e2 e4\n";
                continue;
            }
            
            std::string from = input.substr(spacePos + 1, 2);  
            std::string to = input.substr(spacePos + 4, 2);    
            
            try {
                Position fromPos = notationToNum(from);
                Position toPos = notationToNum(to);
                bool moveSuccess;
                if(turn % 2 == 0 ){
                    moveSuccess = game.playerMove(player, fromPos, toPos);
                }else{
                    moveSuccess = game.playerMove(player, fromPos, toPos);
                }
                                
                if (moveSuccess) {
                    std::cout << "Move successful!\n";
                    game.displayBoard();
                    turn++;
                    
                } else {
                    std::cout << "Invalid move!\n";
                }
                if(game.getCheckmate(player)){
                    break;
                }
                
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid notation: " << e.what() << std::endl;
            }
        }
        else if (command == "show") {
            if (spacePos == std::string::npos || input.length() < 6) {  
                std::cout << "Invalid show format. Use: show e2\n";
                continue;
            }
            
            std::string from = input.substr(spacePos + 1, 2);
            
            try {
                Position position = notationToNum(from);
                std::vector<Position> possibleMoves;
                if(turn % 2 == 0 ){
                    possibleMoves = game.getPossibleMoves(position, true);
                }else{
                    possibleMoves = game.getPossibleMoves(position, false);
                }
                
                std::cout << "\nPossible moves for position " << from << ":\n";
                if (possibleMoves.empty()) {
                    std::cout << "No possible moves found.\n";
                } else {
                    for (const auto& move : possibleMoves) {
                        char file = 'a' + move.x;
                        char rank = '1' + move.y;
                        std::cout << file << rank << " ";
                    }
                    std::cout << "\n";
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid notation: " << e.what() << std::endl;
            }
        }
        else if (command == "undo") {
             
            if (game.UndoMove()) {
                turn--;
                game.displayBoard();
            }
        }
        else if (command == "exit") {
            std::cout << "Thanks for playing!\n";
            break;
        }
        else if (command == "help") {
            displayHelp();
            game.displayPortals();
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    return 0;
}