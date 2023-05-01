#include "AbstractBot.h"

Result AbstractBot::miniMaxSearch(bool isBlackTurn, int depth) {
    Result miniMaxResult;

    if (this->isGameOver() || depth == 0) {
        miniMaxResult.payoff = this->getUtility();
        return miniMaxResult;
    }

    vector<string> validMoves = this->getValidMoves(isBlackTurn);
    if (validMoves.size() == 0) {
        miniMaxResult.payoff = this->getUtility();
        return miniMaxResult;
    }

    miniMaxResult.payoff = isBlackTurn ? 10.12 : -0.12;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->clone();
        currentBotCopy->executeMove(move);
        Result resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1);
        if (((isBlackTurn) && (resultForMove.payoff < miniMaxResult.payoff)) ||
            ((!isBlackTurn) && (resultForMove.payoff > miniMaxResult.payoff))) {
            miniMaxResult.payoff = resultForMove.payoff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
        }
        delete currentBotCopy;
    }

    return miniMaxResult;
}

Result AbstractBot::iterativeDeepeningSearch(bool isBlackTurn) {
    Result result = miniMaxSearch(isBlackTurn, 4);
    return result;
}