#include "AbstractBot.h"

MiniMaxResult AbstractBot::miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta) {
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

    miniMaxResult.payOff = isBlackTurn ? beta : alpha;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->clone();
        currentBotCopy->executeMove(move);

        float childAlpha = isBlackTurn ? alpha : miniMaxResult.payOff;
        float childBeta = isBlackTurn ? miniMaxResult.payOff : beta;
        MiniMaxResult resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1, childAlpha, childBeta);
        delete currentBotCopy;

        if ((isBlackTurn && (resultForMove.payOff < miniMaxResult.payOff)) ||
            (!isBlackTurn && (resultForMove.payOff > miniMaxResult.payOff))) {
            miniMaxResult.payOff = resultForMove.payOff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
            if ((isBlackTurn && miniMaxResult.payOff <= alpha) || (!isBlackTurn && miniMaxResult.payOff >= beta)) {
                break;
            }
        }
    }

    return miniMaxResult;
}

MiniMaxResult AbstractBot::iterativeDeepeningSearch(bool isBlackTurn) {
    MiniMaxResult result = miniMaxSearch(isBlackTurn, 5, -0.12, 10.12);
    return result;
}