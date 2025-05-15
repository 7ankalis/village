#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x, y;
    Position(int x = 0, int y = 0);
    bool operator==(const Position& other) const;
    
    // Adding getters and setters for compatibility
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
};

#endif
