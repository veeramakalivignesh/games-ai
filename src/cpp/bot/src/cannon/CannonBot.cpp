#include "CannonBot.h"

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

bool isPositionValid(vector<vector<int>> &gameState, Position &position) {
    int numRows = gameState.size();
    int numColumns = numRows>0 ? gameState[0].size() : 0;
    return position.i >= 0 && position.i < numRows && position.j >= 0 && position.j < numColumns;
}

int getPiece(vector<vector<int>> &gameState, Position &position) { return gameState[position.i][position.j]; }

void setPiece(vector<vector<int>> &gameState, Position &position, int piece) {
    gameState[position.i][position.j] = piece;
}

bool isEmpty(vector<vector<int>> &gameState, Position &position) { return getPiece(gameState, position) == 0; }

bool isBlack(vector<vector<int>> &gameState, Position &position) { return getPiece(gameState, position) < 0; }

bool isSoldier(vector<vector<int>> &gameState, Position &position) {
    return getPiece(gameState, position) == 1 || getPiece(gameState, position) == -1;
}

bool areOpponents(vector<vector<int>> &gameState, Position &positionA, Position positionB) {
    return getPiece(gameState, positionA) * getPiece(gameState, positionB) < 0;
}

bool isCannonExists(vector<vector<int>> &gameState, Position &rearEnd, Position &middlePosition, Position &frontEnd) {
    int rearPiece = getPiece(gameState, rearEnd);
    int middlePiece = getPiece(gameState, middlePosition);
    int frontPiece = getPiece(gameState, frontEnd);

    return ((rearPiece == 1 || rearPiece == -1) && frontPiece == rearPiece && middlePiece == rearPiece);
}

vector<Cannon> getCannonsWithMiddlePosition(vector<vector<int>> &gameState, Position position) {
    bool isBlack = (getPiece(gameState, position) == -1) ? true : false;
    vector<Cannon> cannons;

    // check horizontal
    Position frontEnd = getPositionSum(position, {0, 1});
    Position rearEnd = getPositionSum(position, {0, -1});
    if (isPositionValid(gameState, frontEnd) && isPositionValid(gameState, rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check vertical
    frontEnd = getPositionSum(position, {-1, 0});
    rearEnd = getPositionSum(position, {1, 0});
    if (isPositionValid(gameState, frontEnd) && isPositionValid(gameState, rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check positive diagonal
    frontEnd = getPositionSum(position, {-1, 1});
    rearEnd = getPositionSum(position, {1, -1});
    if (isPositionValid(gameState, frontEnd) && isPositionValid(gameState, rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    // check negative diagonal
    frontEnd = getPositionSum(position, {1, 1});
    rearEnd = getPositionSum(position, {-1, -1});
    if (isPositionValid(gameState, frontEnd) && isPositionValid(gameState, rearEnd) &&
        isCannonExists(gameState, rearEnd, position, frontEnd)) {
        cannons.push_back({rearEnd, frontEnd, isBlack});
    }

    return cannons;
}

string getMove(bool isBomb, Position selectedPosition, Position targetPosition) {
    string move = "S " + to_string(selectedPosition.i) + " " + to_string(selectedPosition.j) + " ";
    isBomb ? move += "B " : move += "M ";
    move += to_string(targetPosition.i) + " " + to_string(targetPosition.j);
    return move;
}

vector<string> getCannonBombMoves(vector<vector<int>> &gameState, Cannon &cannon) {
    Position offset = getPositionProductWithScalar(0.5, getPositionDifference(cannon.frontEnd, cannon.rearEnd));
    Position immediateFront = getPositionSum(cannon.frontEnd, offset);
    bool isFrontBlocked = isPositionValid(gameState, immediateFront) && !isEmpty(gameState, immediateFront);

    offset = getPositionProductWithScalar(0.5, getPositionDifference(cannon.rearEnd, cannon.frontEnd));
    Position immediateBack = getPositionSum(cannon.rearEnd, offset);
    bool isBackBlocked = isPositionValid(gameState, immediateBack) && !isEmpty(gameState, immediateBack);

    vector<string> cannonBombMoves;
    Position bombTarget;
    if (!isFrontBlocked) {
        // shoot 2 steps front
        offset = getPositionDifference(cannon.frontEnd, cannon.rearEnd);
        bombTarget = getPositionSum(cannon.frontEnd, offset);
        if (isPositionValid(gameState, bombTarget) &&
            (areOpponents(gameState, cannon.rearEnd, bombTarget) || isEmpty(gameState, bombTarget))) {
            cannonBombMoves.push_back(getMove(true, cannon.rearEnd, bombTarget));
        }

        // shoot 3 steps front
        offset = getPositionProductWithScalar(1.5, getPositionDifference(cannon.frontEnd, cannon.rearEnd));
        bombTarget = getPositionSum(cannon.frontEnd, offset);
        if (isPositionValid(gameState, bombTarget) &&
            (areOpponents(gameState, cannon.rearEnd, bombTarget) || isEmpty(gameState, bombTarget))) {
            cannonBombMoves.push_back(getMove(true, cannon.rearEnd, bombTarget));
        }
    }
    if (!isBackBlocked) {
        // shoot 2 steps back
        offset = getPositionDifference(cannon.rearEnd, cannon.frontEnd);
        bombTarget = getPositionSum(cannon.rearEnd, offset);
        if (isPositionValid(gameState, bombTarget) &&
            (areOpponents(gameState, cannon.rearEnd, bombTarget) || isEmpty(gameState, bombTarget))) {
            cannonBombMoves.push_back(getMove(true, cannon.frontEnd, bombTarget));
        }

        // shoot 3 steps back
        offset = getPositionProductWithScalar(1.5, getPositionDifference(cannon.rearEnd, cannon.frontEnd));
        bombTarget = getPositionSum(cannon.rearEnd, offset);
        if (isPositionValid(gameState, bombTarget) &&
            (areOpponents(gameState, cannon.rearEnd, bombTarget) || isEmpty(gameState, bombTarget))) {
            cannonBombMoves.push_back(getMove(true, cannon.frontEnd, bombTarget));
        }
    }

    return cannonBombMoves;
}

vector<string> getCannonMoves(vector<vector<int>> &gameState, Cannon &cannon) {
    vector<string> cannonMoves;

    // move one step front
    Position offset = getPositionProductWithScalar(0.5, getPositionDifference(cannon.frontEnd, cannon.rearEnd));
    Position moveTarget = getPositionSum(cannon.frontEnd, offset);
    if (isPositionValid(gameState, moveTarget) && isEmpty(gameState, moveTarget)) {
        cannonMoves.push_back(getMove(false, cannon.rearEnd, moveTarget));
    }

    // move one step back
    offset = getPositionProductWithScalar(0.5, getPositionDifference(cannon.rearEnd, cannon.frontEnd));
    moveTarget = getPositionSum(cannon.rearEnd, offset);
    if (isPositionValid(gameState, moveTarget) && isEmpty(gameState, moveTarget)) {
        cannonMoves.push_back(getMove(false, cannon.frontEnd, moveTarget));
    }

    return cannonMoves;
}

vector<string> getSoldierMoves(vector<vector<int>> &gameState, Position soldierPosition) {
    vector<string> soldierMoves;
    int forwardOffset = isBlack(gameState, soldierPosition) ? -1 : 1;
    bool isUnderAttack = false;

    // move 1 step front
    Position moveTarget = getPositionSum(soldierPosition, {forwardOffset, 0});
    if (isPositionValid(gameState, moveTarget) &&
        (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
        if (isSoldier(gameState, moveTarget)) {
            isUnderAttack = true;
        }
        soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
    }

    // move one step along diagonals
    moveTarget = getPositionSum(soldierPosition, {forwardOffset, 1});
    if (isPositionValid(gameState, moveTarget) &&
        (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
        if (isSoldier(gameState, moveTarget)) {
            isUnderAttack = true;
        }
        soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
    }

    moveTarget = getPositionSum(soldierPosition, {forwardOffset, -1});
    if (isPositionValid(gameState, moveTarget) &&
        (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
        if (isSoldier(gameState, moveTarget)) {
            isUnderAttack = true;
        }
        soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
    }

    // capture one step horizontally
    moveTarget = getPositionSum(soldierPosition, {0, 1});
    if (isPositionValid(gameState, moveTarget) && areOpponents(gameState, soldierPosition, moveTarget)) {
        if (isSoldier(gameState, moveTarget)) {
            isUnderAttack = true;
        }
        soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
    }

    moveTarget = getPositionSum(soldierPosition, {0, -1});
    if (isPositionValid(gameState, moveTarget) && areOpponents(gameState, soldierPosition, moveTarget)) {
        if (isSoldier(gameState, moveTarget)) {
            isUnderAttack = true;
        }
        soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
    }

    if (isUnderAttack) {
        // retreat 2 steps backward
        moveTarget = getPositionSum(soldierPosition, {-2 * forwardOffset, 0});
        if (isPositionValid(gameState, moveTarget) &&
            (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
            soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
        }

        // retreat 2 steps along diagonals
        moveTarget = getPositionSum(soldierPosition, {-2 * forwardOffset, -2});
        if (isPositionValid(gameState, moveTarget) &&
            (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
            soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
        }

        moveTarget = getPositionSum(soldierPosition, {-2 * forwardOffset, 2});
        if (isPositionValid(gameState, moveTarget) &&
            (areOpponents(gameState, soldierPosition, moveTarget) || isEmpty(gameState, moveTarget))) {
            soldierMoves.push_back(getMove(false, soldierPosition, moveTarget));
        }
    }

    return soldierMoves;
}

vector<Cannon> getAllCannons(vector<vector<int>> &gameState, bool isBlack) {
    int soldier = isBlack ? -1 : 1;
    vector<Cannon> cannons;

    for (int i = 0; i < gameState.size(); i++) {
        for (int j = 0; j < gameState[i].size(); j++) {
            if (gameState[i][j] == soldier) {
                vector<Cannon> cannonsForSoldier = getCannonsWithMiddlePosition(gameState, {i, j});
                cannons.insert(cannons.end(), cannonsForSoldier.begin(), cannonsForSoldier.end());
            }
        }
    }

    return cannons;
}

bool isCannonDiagonal(Cannon &cannon) {
    return (cannon.rearEnd.i != cannon.frontEnd.i) && (cannon.rearEnd.j != cannon.frontEnd.j);
}

//************//

void CannonBot::printGame() {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (this->gameState[i][j] == 2) cout << "T ";
            if (this->gameState[i][j] == 1) cout << "W ";
            if (this->gameState[i][j] == 0) cout << "0 ";
            if (this->gameState[i][j] == -1) cout << "B ";
            if (this->gameState[i][j] == -2) cout << "U ";
        }
        cout << endl;
    }
}

void CannonBot::setGameState(vector<vector<int>> gameState) { this->gameState = gameState; }

vector<string> CannonBot::getValidMoves(bool isBlackTurn) {
    vector<string> validMoves;

    vector<Cannon> cannons = getAllCannons(this->gameState, isBlackTurn);
    for (Cannon cannon : cannons) {
        vector<string> cannonBombMoves = getCannonBombMoves(this->gameState, cannon);
        vector<string> cannonMoves = getCannonMoves(this->gameState, cannon);
        validMoves.insert(validMoves.end(), cannonBombMoves.begin(), cannonBombMoves.end());
        validMoves.insert(validMoves.end(), cannonMoves.begin(), cannonMoves.end());
    }

    int soldier = isBlackTurn ? -1 : 1;
    for (int i = 0; i < this->gameState.size(); i++) {
        for (int j = 0; j < this->gameState[i].size(); j++) {
            if (this->gameState[i][j] == soldier) {
                vector<string> soldierMoves = getSoldierMoves(this->gameState, {i, j});
                validMoves.insert(validMoves.end(), soldierMoves.begin(), soldierMoves.end());
            }
        }
    }

    return validMoves;
}

void CannonBot::executeMove(string move) {
    Position selectedPosition = {move[2] - '0', move[4] - '0'};
    Position targetPosition = {move[8] - '0', move[10] - '0'};
    bool isBomb = move[6] == 'B' ? true : false;

    if (isBomb) {
        setPiece(this->gameState, targetPosition, 0);
    } else {
        setPiece(this->gameState, targetPosition, getPiece(this->gameState, selectedPosition));
        setPiece(this->gameState, selectedPosition, 0);
    }
}

bool CannonBot::isGameOver() {
    int minTownhalls = this->gameState[0].size() / 2 - 2;
    int numWhiteTownHalls = 0;
    int numBlackTownHalls = 0;

    for (int piece : this->gameState[0]) {
        if (piece == 2) {
            numWhiteTownHalls++;
        }
    }

    for (int piece : this->gameState[this->gameState.size() - 1]) {
        if (piece == -2) {
            numBlackTownHalls++;
        }
    }

    return (numWhiteTownHalls <= minTownhalls) || (numBlackTownHalls <= minTownhalls);
}

float CannonBot::getUtility() {
    int minTownhalls = this->gameState[0].size() / 2 - 2;
    float midRowDisplacement = (float)(this->gameState.size() - 1) / 2;
    vector<Cannon> blackCannons = getAllCannons(this->gameState, true);
    vector<Cannon> whiteCannons = getAllCannons(this->gameState, false);

    int numWhiteTownHalls = 0;
    for (int piece : this->gameState[0]) {
        if (piece == 2) {
            numWhiteTownHalls++;
        }
    }

    int numBlackTownHalls = 0;
    for (int piece : this->gameState[this->gameState.size() - 1]) {
        if (piece == -2) {
            numBlackTownHalls++;
        }
    }

    float numWhiteSoldiers = 0;
    float numBlackSoldiers = 0;
    float averageSoldierDisplacementFromCenter = 0;
    for (int i = 0; i < this->gameState.size(); i++) {
        for (int piece : this->gameState[i]) {
            if (piece == 1) {
                numWhiteSoldiers++;
                averageSoldierDisplacementFromCenter += i - midRowDisplacement;
            } else if (piece == -1) {
                numBlackSoldiers++;
                averageSoldierDisplacementFromCenter += i - midRowDisplacement;
            }
        }
    }
    averageSoldierDisplacementFromCenter /= numWhiteSoldiers + numBlackSoldiers;

    float numWhiteCannons = whiteCannons.size();
    float numWhiteDiagonalCannons = 0;
    for (Cannon cannon : whiteCannons) {
        if (isCannonDiagonal(cannon)) {
            numWhiteDiagonalCannons++;
        }
    }

    float numBlackCannons = blackCannons.size();
    float numBlackDiagonalCannons = 0;
    for (Cannon cannon : blackCannons) {
        if (isCannonDiagonal(cannon)) {
            numBlackDiagonalCannons++;
        }
    }

    float utility = 0;
    if (numWhiteTownHalls == minTownhalls + 2 && numBlackTownHalls == minTownhalls + 2) {
        utility += 5;
    } else if (numWhiteTownHalls == minTownhalls + 1 && numBlackTownHalls == minTownhalls + 1) {
        utility += 5;
    } else if (numWhiteTownHalls == minTownhalls + 2 && numBlackTownHalls == minTownhalls + 1) {
        utility += 7;
    } else if (numWhiteTownHalls == minTownhalls + 1 && numBlackTownHalls == minTownhalls + 2) {
        utility += 3;
    } else if (numWhiteTownHalls == minTownhalls + 1 && numBlackTownHalls == minTownhalls) {
        utility += 8;
    } else if (numWhiteTownHalls == minTownhalls && numBlackTownHalls == minTownhalls + 1) {
        utility += 2;
    } else if (numWhiteTownHalls == minTownhalls + 2 && numBlackTownHalls == minTownhalls) {
        utility += 10;
    } else if (numWhiteTownHalls == minTownhalls && numBlackTownHalls == minTownhalls + 2) {
        utility += 0;
    }

    utility += (numWhiteSoldiers - numBlackSoldiers) * 0.01;
    utility += (numWhiteCannons - numBlackCannons) * 0.001;
    utility += (numWhiteDiagonalCannons - numBlackDiagonalCannons) * 0.001;
    utility += averageSoldierDisplacementFromCenter * 0.01;

    return utility;
}