#include "Player.hpp"

Player::Player(const std::string& playerName, bool white)
    : name(playerName), isWhite(white), movesCount(0) {}


std::string Player::getName() const {
    return name;
}

bool Player::isWhitePlayer() const {
    return isWhite;
}

int Player::getMovesCount() const {
    return movesCount;
}

int Player::increaseMoveCount(){
    return ++movesCount;
}