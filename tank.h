#pragma once
#pragma once
#include "Point.h"
#include <vector>
#include "Shell.h"
#include "gameConfig.h"

class Tank {
private:
    Point pos;
    GameConfig::cannonDir cannonDir;
    bool isMoving = false;
    bool hasCannon = true;
    int shootCooldown = 0;
    int playerID;
    Point velocity{ 0,0 };

public:
    Tank(Point position, int id, GameConfig::cannonDir dir) : pos(position), playerID(id), cannonDir(dir) {}


    Point getPosition() const;
    void draw() const;
    void erase() const;
    void move(int x, int y);
    void setDirection(GameConfig::cannonDir newDir);
    char getCannonChar(GameConfig::cannonDir dir)  const;
    void shoot(std::vector<Shell>& shells);
    void tick();
    void hitCannon();
    bool isAt(const Point& p) const;
    int getPlayerID() const;
    void checkKeysP1();
    void checkKeysP2();
    Point getPointFromDir(GameConfig::cannonDir dir) const;

};


