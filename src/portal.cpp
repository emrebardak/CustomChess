#include <portal.hpp>
#include <iostream>

Portal::Portal(Board& board, const ConfigReader& config) : board(board) {
    setPortals(config);
}

void Portal::setPortals(const ConfigReader& config) {
    const auto& portalConfigs = config.getConfig().portals;
    portals.clear();
    
    Position entry;
    Position exit;

    portalData portal1;
    entry.x = 2;
    entry.y = 3;
    exit.x = 5;
    exit.y = 4;
    portal1.id = 1;
    portal1.cooldown = 1;
    portal1.entry = entry;
    portal1.exit = exit;
    
    portalData portal2;
    entry.x = 6;
    entry.y = 2;
    exit.x = 1;
    exit.y = 5;
    portal2.id = 2;
    portal2.cooldown = 2;
    portal2.entry = entry;
    portal2.exit = exit;

    portals.push_back(portal1);
    portals.push_back(portal2);

    /*
    // Load each portal from the config
    for (const auto& portalConfig : portalConfigs) {
        try {
            portalData newPortal;
            
            // Extract portal ID (remove "portal" prefix)
            std::string idStr = portalConfig.id;
            if (idStr.substr(0, 6) != "portal") {
                std::cerr << "Invalid portal ID format: " << idStr << std::endl;
                continue;
            }
            newPortal.id = std::stoi(idStr.substr(6));
            
            // Set positions
            newPortal.entry = portalConfig.positions.entry;
            newPortal.exit = portalConfig.positions.exit;
            
            // Set cooldown
            newPortal.cooldown = portalConfig.properties.cooldown;
            
            std::cout << "Added portal " << newPortal.id 
                      << " at entry (" << newPortal.entry.x << "," << newPortal.entry.y << ")"
                      << " to exit (" << newPortal.exit.x << "," << newPortal.exit.y << ")"
                      << " with cooldown " << newPortal.cooldown
                      << std::endl;

            portals.push_back(newPortal);
        } catch (const std::exception& e) {
            std::cerr << "Error setting up portal: " << e.what() << std::endl;
        }
    
    }
        IT DIDNT WORK
    */ 
    setCooldown();
}

void Portal::setCooldown(){
    for(auto& prt : portals){
        for(int i = 0 ; i < prt.cooldown + 1 ; i ++){
            prt.BlackCooldown.push(i);
            prt.WhiteCooldown.push(i);
        }
    }
}

bool Portal::checkPortal(bool isWhite, int portalId){
    if((portals[portalId - 1].WhiteCooldown.front() == 0 && isWhite) ||
       (portals[portalId - 1].BlackCooldown.front() == 0 && !isWhite)){
        return true;
    }
    return false;
}

int Portal::cooldownTime(bool isWhite, int portalId){
    int time = 0;
    if(isWhite){
        time = portals[portalId - 1].WhiteCooldown.front();
    }else{
        time = portals[portalId - 1].BlackCooldown.front();
    }
    return time;
}
void Portal::decreaseCooldown(bool isWhite){
    int temp;
    if(isWhite){
        for(auto& prt : portals){
            int portalId = prt.id;
            temp = portals[portalId - 1].WhiteCooldown.front();
            portals[portalId - 1].WhiteCooldown.pop();
            if(temp != 0){
                portals[portalId - 1].WhiteCooldown.push(temp);
            }
        }
        
    }else{
        for(auto& prt : portals){
            int portalId = prt.id;
            temp = portals[portalId - 1].BlackCooldown.front();
            portals[portalId - 1].BlackCooldown.pop();
            if(temp != 0){
                portals[portalId - 1].BlackCooldown.push(temp);
            }         
        };
    }
}

int Portal::getPortalId(const Position& pos) const {
    for(const auto& prt : portals){
        if(prt.entry.x == pos.x && prt.entry.y == pos.y) return prt.id;
    }
    return -1;
}

bool Portal::isPortalPosition(const Position& pos){
    for(const auto& prt : portals){
        if(prt.entry.x == pos.x && prt.entry.y == pos.y) return true;
    }
    return false;
}

Position Portal::getExitPosition(const Position& entry){
    int id = getPortalId(entry);
    return portals[ id - 1 ].exit;  
}

void Portal::displayPortals() const {
    std::cout << "\nPortal Information:" << std::endl;
    std::cout << "------------------" << std::endl;
    
    for(const auto& prt : portals) {
        std::cout << "Portal " << prt.id << ":" << std::endl;
        std::cout << "  Entry: " << static_cast<char>('a' + prt.entry.x) << (8 - prt.entry.y) << std::endl;
        std::cout << "  Exit: " << static_cast<char>('a' + prt.exit.x) << (8 - prt.exit.y) << std::endl;
        std::cout << "  Cooldown: " << prt.cooldown << " turns" << std::endl;
        std::cout << "  White Cooldown: " << prt.WhiteCooldown.front() << " turns remaining" << std::endl;
        std::cout << "  Black Cooldown: " << prt.BlackCooldown.front() << " turns remaining" << std::endl;
        std::cout << "------------------" << std::endl;
    }
}