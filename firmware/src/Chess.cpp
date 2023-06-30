#include "Chess.h"

Chess::Chess(RoboticArm &arm, SquarePosition squarePositions[8][8],
             SquarePosition discardPiecePosition, SquarePosition idlePosition) : arm(arm),
                                                                                 discardPiecePosition(discardPiecePosition), idlePosition(idlePosition) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->squarePositions[i][j] = squarePositions[i][j];
        }
    }

    currentPosition.x = idlePosition.x;
    currentPosition.y = idlePosition.y;
    currentPosition.z = FREE_Z;
    targetPosition = currentPosition;
    reset();
}

void Chess::move() {
    mainState = MOVEMENT;
}

bool Chess::isIdle() {
    return mainState == MAIN_IDLE;
}

void Chess::wait(uint32_t time) {
    mainState = WAITING;
    waitTime = millis() + time;
}

void Chess::handle() {
    switch (mainState) {
        case MAIN_IDLE:
            return;
        case MOVEMENT:
            handleMovement();
            return;
        case CONTROL:
            handleControl();
            return;
        case WAITING:
            handleWait();
            return;
    }

}

bool Chess::finishedMovement() {
    return currentPosition.z == targetPosition.z && currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y;
}

void Chess::handleWait() {
    if (millis() < waitTime) {
        return;
    }

    if (finishedMovement()) {
        mainState = CONTROL;
    }
    else {
        mainState = MOVEMENT;
    }
}

void Chess::handleControl() {
    switch (controlState) {
        case CONTROL_IDLE:
            mainState = MAIN_IDLE;
            Serial.println("OK");
            return;
        case GRABBING:
            handleGrabbing();
            return;
        case RELEASING:
            handleReleasing();
            return;
    }
}

void Chess::handleMovement() {
    int16_t zDir = currentPosition.z < targetPosition.z ? 1 : -1;
    int16_t xDir = currentPosition.x < targetPosition.x ? 1 : -1;
    int16_t yDir = currentPosition.y < targetPosition.y ? 1 : -1;

    currentPosition.z += zDir * UP_DOWN_STEP_SIZE;
    currentPosition.x += xDir * LEFT_RIGHT_STEP_SIZE;
    currentPosition.y += yDir * LEFT_RIGHT_STEP_SIZE;

    if (currentPosition.z * zDir >= targetPosition.z * zDir) {
        currentPosition.z = targetPosition.z;
    }
    if (currentPosition.x * xDir >= targetPosition.x * xDir) {
        currentPosition.x = targetPosition.x;
    }
    if (currentPosition.y * yDir >= targetPosition.y * yDir) {
        currentPosition.y = targetPosition.y;
    }

    if (currentPosition.z == targetPosition.z && currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y) {
        mainState = WAITING;
    }

    arm.moveTo(currentPosition.x, currentPosition.y, currentPosition.z);
    wait(MOVE_WAIT);
}

void Chess::handleGrabbing(){
    switch (grabReleaseState) {
        case GRAB_RELEASE_IDLE:
            targetPosition.z = BOARD_Z;
            grabReleaseState = MOVING_DOWN;
            move();
            break;
        case MOVING_DOWN:
            arm.grab();
            wait(GRAB_RELEASE_WAIT);
            grabReleaseState = GRABBING_RELEASING;
            break;
        case GRABBING_RELEASING:
            targetPosition.z = FREE_Z;
            grabReleaseState = MOVING_UP;
            move();
            break;
        case MOVING_UP:
            grabReleaseState = GRAB_RELEASE_IDLE;
            controlState = CONTROL_IDLE;
            break;            
    }
}

void Chess::handleReleasing() {
    switch (grabReleaseState) {
        case GRAB_RELEASE_IDLE:
            targetPosition.z = BOARD_Z;
            grabReleaseState = MOVING_DOWN;
            move();
            break;
        case MOVING_DOWN:
            arm.release();
            wait(GRAB_RELEASE_WAIT);
            grabReleaseState = GRABBING_RELEASING;
            break;
        case GRABBING_RELEASING:
            targetPosition.z = FREE_Z;
            grabReleaseState = MOVING_UP;
            move();
            break;
        case MOVING_UP:
            grabReleaseState = GRAB_RELEASE_IDLE;
            controlState = CONTROL_IDLE;
            break;
    }
}

void Chess::grab() {
    mainState = CONTROL;
    controlState = GRABBING;
}

void Chess::release() {
    mainState = CONTROL;
    controlState = RELEASING;
}

void Chess::moveToSquare(int x, int y) {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        Serial.println("ERROR");
        return;
    }

    SquarePosition targetSquare = squarePositions[x][y];
    
    if (targetSquare.x == 0 && targetSquare.y == 0) {
        Serial.println("ERROR");
        return;
    }

    targetPosition.x = targetSquare.x;
    targetPosition.y = targetSquare.y;
    targetPosition.z = FREE_Z;
    move();
}

void Chess::reset() {
    targetPosition.x = idlePosition.x;
    targetPosition.y = idlePosition.y;
    targetPosition.z = FREE_Z;
    move();
}

void Chess::discard() {
    targetPosition.x = discardPiecePosition.x;
    targetPosition.y = discardPiecePosition.y;
    targetPosition.z = FREE_Z;
    move();
}