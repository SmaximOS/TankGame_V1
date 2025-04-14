#pragma once
#include "point.h"
#include "gameConfig.h"
#include "general.h"
#include "iostream"
using namespace std;


class Wall {
private:
    Point pos;
    int hitCount = 0;

public:
    Wall(int x, int y) : pos(x, y) {}
    Wall(Point p) : pos(p) {}

    Point getPosition() const { return pos; }

    void draw() const {
        gotoxy(GameConfig::MINX + pos.Point::getX(), GameConfig::MINY + pos.Point::getY());
        cout << (hitCount == 0 ? '#' : '%');
    }

    void erase() const {
        gotoxy(GameConfig::MINX + pos.Point::getX(), GameConfig::MINY + pos.Point::getY());
        cout << ' ';
    }

    bool hit() {
        hitCount++;
        return hitCount >= 2;
    }

    bool isAt(const Point& p) const {
        return pos.isEqual(p);

    }
};


