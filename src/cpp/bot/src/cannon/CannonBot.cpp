#include "CannonBot.h"

int n = 8;
int m = 8;

struct Position {
    int i;
    int j;
};

struct Cannon {
    Position rearEnd;
    Position frontEnd;
    bool isBlack;
};

Position getPositionSum(Position positionA, Position positionB) {
    return {positionA.i + positionB.i, positionA.j + positionB.j};
}

Position getPositionDifference(Position positionA, Position positionB) {
    return {positionA.i - positionB.i, positionA.j - positionB.j};
}

Position getPositionProductWithScalar(float scalar, Position position) {
    return {(int)(scalar * position.i), (int)(scalar * position.j)};
}

bool isPositionValid(Position &position) {
    return position.i >= 0 && position.i < n && position.j >= 0 &&
           position.j < m;
}

int getPiece(vector<vector<int>> &gameState, Position &position) {
    return gameState[position.i][position.j];
}

bool isCannonExists(vector<vector<int>> &gameState, Position &rearEnd,
                    Position &middlePosition, Position &frontEnd) {
    int rearPiece = getPiece(gameState, rearEnd);
    int middlePiece = getPiece(gameState, middlePosition);
    int frontPiece = getPiece(gameState, frontEnd);

    return ((rearPiece == 1 || rearPiece == -1) && frontPiece == rearPiece &&
            middlePiece == rearPiece);
}

vector<Cannon> getCannonsWithMiddlePosition(vector<vector<int>> &gameState,
                                            Position &position) {
    bool isBlack = (getPiece(gameState, position) == -1) ? true : false;
    vector<Cannon> cannons;

    // check horizontal
    Position frontEnd = getPositionSum(position, {0, 1});
    Position rearEnd = getPositionSum(position, {0, -1});
    if (isPositionValid(frontEnd) && isPositionValid(rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check vertical
    frontEnd = getPositionSum(position, {-1, 0});
    rearEnd = getPositionSum(position, {1, 0});
    if (isPositionValid(frontEnd) && isPositionValid(rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check positive diagonal
    frontEnd = getPositionSum(position, {-1, 1});
    rearEnd = getPositionSum(position, {1, -1});
    if (isPositionValid(frontEnd) && isPositionValid(rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check negative diagonal
    frontEnd = getPositionSum(position, {1, 1});
    rearEnd = getPositionSum(position, {-1, -1});
    if (isPositionValid(frontEnd) && isPositionValid(rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    return cannons;
}

string getMove(bool isBomb, Position selectedPosition,
               Position targetPosition) {
    string move = "S " + to_string(selectedPosition.i) + " " +
                  to_string(selectedPosition.j) + " ";
    isBomb ? move += "B " : move += "M ";
    move += to_string(targetPosition.i) + " " + to_string(targetPosition.j);
    return move;
}

string getCannonBombMoves(vector<vector<int>> &gameState, Cannon &cannon) {
    Position offset = getPositionProductWithScalar(
        0.5, getPositionDifference(cannon.frontEnd, cannon.rearEnd));
    Position immediateFront = getPositionSum(cannon.frontEnd, offset);
    bool isFrontBlocked = isPositionValid(immediateFront) &&
                          getPiece(gameState, immediateFront) != 0;

    offset = getPositionProductWithScalar(
        0.5, getPositionDifference(cannon.rearEnd, cannon.frontEnd));
    Position immediateBack = getPositionSum(cannon.rearEnd, offset);
    bool isBackBlocked = isPositionValid(immediateBack) &&
                          getPiece(gameState, immediateBack) != 0;

    vector<string> cannonBombMoves;
    Position bombTarget;
    if(!isFrontBlocked) {
        offset = getPositionDifference(cannon.frontEnd, cannon.rearEnd);
        bombTarget = getPositionSum(cannon.frontEnd, offset); 
    }
}

//************//

void CannonBot::setGameState(vector<vector<int>> gameState) {
    this->gameState = gameState;
}

vector<string> CannonBot::getValidMoves(bool isBlackTurn) {

}

void CannonBot::executeMove(string move){

}

bool CannonBot::isGameOver() {

}

float CannonBot::getUtility() {

}