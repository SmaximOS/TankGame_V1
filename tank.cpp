// Tank.cpp
#include "Tank.h"
#include "GameConfig.h"
#include <iostream>
#include <windows.h>
#include "general.h"
using namespace std;

Point Tank::getPosition() const {
    return pos;
}

void Tank::draw() const {
    gotoxy(GameConfig::MINX + pos.getX(), GameConfig::MINY + pos.getY());
    cout << 'O';
    Point p=getPointFromDir( cannonDir);
    if (hasCannon) {
        Point cannonPos = pos.add(p);
        gotoxy(GameConfig::MINX + cannonPos.getX(), GameConfig::MINY + cannonPos.getY());
        cout << getCannonChar(cannonDir);
    }
}

void Tank::erase() const {
    gotoxy(GameConfig::MINX + pos.getX(), GameConfig::MINY + pos.getY());
    cout << ' ';
    Point p = getPointFromDir(cannonDir);
    if (hasCannon) {
        Point cannonPos = pos.add(p);
        gotoxy(GameConfig::MINX + cannonPos.getX(), GameConfig::MINY + cannonPos.getY());
        cout << ' ';
    }
}

char Tank::getCannonChar(GameConfig::cannonDir dir) const {
    if (dir== GameConfig::cannonDir::NORTH || dir == GameConfig::cannonDir::SOUTH) return '|';
    if (dir == GameConfig::cannonDir::EAST || dir == GameConfig::cannonDir::WEST) return '-';
    if (dir == GameConfig::cannonDir::NORTH_EAST || dir == GameConfig::cannonDir::SOUTH_WEST) return '/';
    if (dir == GameConfig::cannonDir::NORTH_WEST || dir == GameConfig::cannonDir::SOUTH_EAST) return '\\';
    return '?';
}
Point Tank::getPointFromDir(GameConfig::cannonDir dir) const {
    Point p;

    switch(dir)
    {
    case GameConfig::cannonDir::NORTH:
        p.setx(0);
        p.sety(-1);
        break;

    case GameConfig::cannonDir::NORTH_EAST:
        p.setx(1);
        p.sety(-1);
        break;
        
    case GameConfig::cannonDir::EAST:
        p.setx(1);
        p.sety(0);
        break;
    case GameConfig::cannonDir::SOUTH_EAST:
        p.setx(1);
        p.sety(1);
        break;

    case GameConfig::cannonDir::SOUTH:
        p.setx(0);
        p.sety(1);
        break;

    case GameConfig::cannonDir::SOUTH_WEST:
        p.setx(-1);
        p.sety(1);
        break;

    case GameConfig::cannonDir::WEST:
        p.setx(-1);
        p.sety(0);
        break;

    case GameConfig::cannonDir::NORTH_WEST:
        p.setx(-1);
        p.sety(-1);
        break;
    }

    return p;
}



void Tank::move(int x, int y) {
    Point p(x, y);
    pos = pos.add(p);
}

void Tank::setDirection(GameConfig::cannonDir newDir) {
    cannonDir = newDir;
}

//char Tank::getCannonChar() const {
//    if (cannonDir.getX() == 0 && cannonDir.getY() == -1) return '|';
//    if (cannonDir.getX() == 0 && cannonDir.getY() == 1) return '|';
//    if (cannonDir.getX() == -1 && cannonDir.getY() == 0) return '-';
//    if (cannonDir.getX() == 1 && cannonDir.getY() == 0) return '-';
//    if (cannonDir.getX() == 1 && cannonDir.getY() == -1) return '/';
//    if (cannonDir.getX() == -1 && cannonDir.getY() == 1) return '/';
//    if (cannonDir.getX() == -1 && cannonDir.getY() == -1) return '\\';
//    if (cannonDir.getX() == 1 && cannonDir.getY() == 1) return '\\';
//    return '?';
//}

//void Tank::shoot(std::vector<Shell>& shells) {
//    if (hasCannon && shootCooldown == 0) {
//        shells.emplace_back(pos.add(cannonDir), cannonDir, playerID);
//        shootCooldown = 5;
//    }
//}

void Tank::tick() {
    if (shootCooldown > 0)
        --shootCooldown;
}

void Tank::hitCannon() {
    hasCannon = false;
}

bool Tank::isAt(const Point& p) const {
    return pos.isEqual(p);
}

int Tank::getPlayerID() const {
    return playerID;
}

void Tank::checkKeysP1() {
    // 1) Read raw key states
    bool lf = GetAsyncKeyState('Q') & 0x8000;  // left?track forward
    bool rf = GetAsyncKeyState('E') & 0x8000;  // right?track forward
    bool lb = GetAsyncKeyState('A') & 0x8000;  // left?track backward
    bool rb = GetAsyncKeyState('D') & 0x8000;  // right?track backward
    bool stay = GetAsyncKeyState('S') & 0x8000;  // stay

    // 2) STOP immediately on stay
    if (stay) {
        isMoving = false;
        velocity = Point(0, 0);
        return;
    }

    // 3) ROTATION IN PLACE (no movement this frame)
    // 3a) Diagonal (8?step) rotations:
    if (rf && !lf && !lb && !rb) {
        // RIGHT track forward ONLY ? CCW 1 step
        int idx = (static_cast<int>(cannonDir) + 7) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }
    else if (lf && !rf && !lb && !rb) {
        // LEFT track forward ONLY ? CW 1 step
        int idx = (static_cast<int>(cannonDir) + 1) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }

    // 3b) Cardinal (4?step) rotations:
    if (rf && lb && !lf && !rb) {
        // RIGHT forward + LEFT backward ? CCW 2 steps
        int idx = (static_cast<int>(cannonDir) + 6) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }
    else if (lf && rb && !rf && !lb) {
        // LEFT forward + RIGHT backward ? CW 2 steps
        int idx = (static_cast<int>(cannonDir) + 2) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }

    // 4) START COASTING if both tracks are driven
    if (lf && rf) {
        velocity = getPointFromDir(cannonDir);
        isMoving = true;
    }
    else if (lb && rb) {
        Point d = getPointFromDir(cannonDir);
        velocity = Point(-d.getX(), -d.getY());
        isMoving = true;
    }

    // 5) COAST if flagged
    if (isMoving) {
        move(velocity.getX(), velocity.getY());
    }
}




void Tank:: checkKeysP2() {
    bool lf = GetAsyncKeyState('U') & 0x8000;
    bool rf = GetAsyncKeyState('O') & 0x8000;
    bool lb = GetAsyncKeyState('J') & 0x8000;
    bool rb = GetAsyncKeyState('L') & 0x8000;
    bool stay = GetAsyncKeyState('K') & 0x8000;

    
    // 2) STOP immediately on stay
    if (stay) {
        isMoving = false;
        velocity = Point(0, 0);
        return;
    }

    // 3) ROTATION IN PLACE (no movement this frame)
    // 3a) Diagonal (8?step) rotations:
    if (rf && !lf && !lb && !rb) {
        // RIGHT track forward ONLY ? CCW 1 step
        int idx = (static_cast<int>(cannonDir) + 7) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }
    else if (lf && !rf && !lb && !rb) {
        // LEFT track forward ONLY ? CW 1 step
        int idx = (static_cast<int>(cannonDir) + 1) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }

    // 3b) Cardinal (4?step) rotations:
    if (rf && lb && !lf && !rb) {
        // RIGHT forward + LEFT backward ? CCW 2 steps
        int idx = (static_cast<int>(cannonDir) + 6) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }
    else if (lf && rb && !rf && !lb) {
        // LEFT forward + RIGHT backward ? CW 2 steps
        int idx = (static_cast<int>(cannonDir) + 2) % 8;
        cannonDir = static_cast<GameConfig::cannonDir>(idx);
        return;
    }

    // 4) START COASTING if both tracks are driven
    if (lf && rf) {
        velocity = getPointFromDir(cannonDir);
        isMoving = true;
    }
    else if (lb && rb) {
        Point d = getPointFromDir(cannonDir);
        velocity = Point(-d.getX(), -d.getY());
        isMoving = true;
    }

    // 5) COAST if flagged
    if (isMoving) {
        move(velocity.getX(), velocity.getY());
    }
}
