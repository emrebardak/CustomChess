#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
private:
    std::string name;
    bool isWhite;
    int movesCount;

public:
    Player(const std::string& playerName, bool white);
    
    std::string getName() const;
    bool isWhitePlayer() const;
    int getMovesCount() const;
    int increaseMoveCount();

};

#endif 