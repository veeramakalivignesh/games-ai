#include "AbstractBot.h"

MiniMaxResult AbstractBot::miniMaxSearch(bool isBlackTurn, int depth) {
    MiniMaxResult miniMaxResult;

    if (this->isGameOver() || depth == 0) {
        miniMaxResult.payOff = this->getUtility();
        return miniMaxResult;
    }

    vector<string> validMoves = this->getValidMoves(isBlackTurn);
    if (validMoves.size() == 0) {
        miniMaxResult.payOff = this->getUtility();
        return miniMaxResult;
    }

    miniMaxResult.payOff = isBlackTurn ? 10.12 : -0.12;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->clone();
        currentBotCopy->executeMove(move);
        MiniMaxResult resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1);
        if (((isBlackTurn) && (resultForMove.payOff < miniMaxResult.payOff)) ||
            ((!isBlackTurn) && (resultForMove.payOff > miniMaxResult.payOff))) {
            miniMaxResult.payOff = resultForMove.payOff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
        }
        delete currentBotCopy;
    }

    return miniMaxResult;
}

MiniMaxResult AbstractBot::iterativeDeepeningSearch(bool isBlackTurn) {
    MiniMaxResult result = miniMaxSearch(isBlackTurn, 4);
    return result;
}