#include "AbstractBot.h"

Result AbstractBot::miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta,
                                  vector<string> prevDepthStrategy, float timeLimit, bool applyTimeLimit) {
    clock_t beginTime = clock();
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

        float childTimeLimit = timeLimit - (float)(clock() - beginTime) / CLOCKS_PER_SEC;
        if (applyTimeLimit && childTimeLimit < 0) {
            break;
        }

        Result resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1, childAlpha, childBeta,
                                                             childPrevDepthStrategy, childTimeLimit, false);
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

Result AbstractBot::iterativeDeepeningSearch(bool isBlackTurn, float timeLimit) {
    clock_t beginTime = clock();
    Result result;
    for (int depth = 5;; depth++) {
        float timeLeft = timeLimit - (float)(clock() - beginTime) / CLOCKS_PER_SEC;
        if (timeLeft < 0) {
            break;
        }
        Result resultForDepth = miniMaxSearch(isBlackTurn, depth, -0.12, 10.12, result.strategy, timeLeft, depth > 5);
        if (!resultForDepth.strategy.empty()) {
            result = resultForDepth;
        }
    }
    cout << result.strategy.size() << endl;
    return result;
}