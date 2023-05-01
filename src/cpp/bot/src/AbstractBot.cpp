#include "AbstractBot.h"

Result AbstractBot::miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta,
                                  vector<string> prevDepthStrategy) {
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

    if (!prevDepthStrategy.empty()) {
        for (int i = 0; i < validMoves.size(); i++) {
            if (validMoves[i] == prevDepthStrategy[0]) {
                validMoves.erase(validMoves.begin() + i);
            }
        }
        validMoves.insert(validMoves.begin(), prevDepthStrategy[0]);
    }

    miniMaxResult.payoff = isBlackTurn ? beta : alpha;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->clone();
        currentBotCopy->executeMove(move);

        float childAlpha = isBlackTurn ? alpha : miniMaxResult.payoff;
        float childBeta = isBlackTurn ? miniMaxResult.payoff : beta;

        vector<string> childPrevDepthStrategy;
        if (!prevDepthStrategy.empty() && move == prevDepthStrategy[0]) {
            childPrevDepthStrategy = prevDepthStrategy;
            childPrevDepthStrategy.erase(childPrevDepthStrategy.begin());
        }

        Result resultForMove =
            currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1, childAlpha, childBeta, childPrevDepthStrategy);
        delete currentBotCopy;

        if ((isBlackTurn && (resultForMove.payoff < miniMaxResult.payoff)) ||
            (!isBlackTurn && (resultForMove.payoff > miniMaxResult.payoff))) {
            miniMaxResult.payoff = resultForMove.payoff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
            if ((isBlackTurn && miniMaxResult.payoff <= alpha) || (!isBlackTurn && miniMaxResult.payoff >= beta)) {
                break;
            }
        }
    }

    return miniMaxResult;
}

Result AbstractBot::iterativeDeepeningSearch(bool isBlackTurn) {
    Result result;
    for (int i = 1; i <= 5; i++) {
        result = miniMaxSearch(isBlackTurn, i, -0.12, 10.12, result.strategy);
    }
    return result;
}