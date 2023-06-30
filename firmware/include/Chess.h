#pragma once

#include "Configuration.h"

typedef struct {
    int16_t x;
    int16_t y;
} SquarePosition;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} Position;

class Chess {
    public:
    Chess(RoboticArm &arm, SquarePosition squarePositions[8][8], SquarePosition discardPiecePosition, SquarePosition idlePosition);    
    void handle();
    bool isIdle();
      
    void moveToSquare(int x, int y);
    void reset();
    void discard();
    void grab();
    void release();    
    void wait(uint32_t time);  

    private:
    enum MainState {
        MAIN_IDLE,
        MOVEMENT,
        CONTROL,
        WAITING
    };

    enum ControlState {
        CONTROL_IDLE,
        GRABBING,
        RELEASING
    };

    enum GrabReleaseState {
        GRAB_RELEASE_IDLE,
        MOVING_DOWN,
        MOVING_UP,
        GRABBING_RELEASING
    };

    void move();
    void handleWait();
    void handleControl();
    void handleMovement();
    void handleGrabbing();
    void handleReleasing();

    bool finishedMovement();

    RoboticArm &arm;
    SquarePosition squarePositions[8][8];
    SquarePosition discardPiecePosition;
    SquarePosition idlePosition;

    MainState mainState = MAIN_IDLE;
    ControlState controlState = CONTROL_IDLE;
    GrabReleaseState grabReleaseState = GRAB_RELEASE_IDLE;

    Position currentPosition;
    Position targetPosition;
    uint32_t waitTime;
};