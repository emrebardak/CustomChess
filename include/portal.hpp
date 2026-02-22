#pragma once

#ifndef PORTAL_HPP
#define PORTAL_HPP

#include "ConfigReader.hpp"
#include "Board.hpp"
#include <queue>
#include <vector>

struct portalData
{
    int id;
    Position entry;
    Position exit;
    int cooldown;
    std::queue<int> WhiteCooldown;
    std::queue<int> BlackCooldown;
    portalData() : id(0), cooldown(0) {}
};


class Portal{
public:
    Portal(Board& board, const ConfigReader& config);
    bool checkPortal(bool isWhite, int portalId);
    int cooldownTime(bool isWhite, int portalId);
    void decreaseCooldown(bool isWhite);
    bool isPortalPosition(const Position& pos);
    Position getExitPosition(const Position& entry);
    int getPortalId(const Position& pos) const;
    void displayPortals() const;
    
private:
    Board& board;
    std::vector<portalData> portals;
    void setPortals(const ConfigReader& config);
    void setCooldown();
};


#endif