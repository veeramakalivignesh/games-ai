#include "AbstractBot.h"

void AbstractBot::setMinUtility(float minUtility) {
    this->minUtility = minUtility;
}

void AbstractBot::setMaxUtility(float maxUtility) {
    this->maxUtility = maxUtility;
}

void AbstractBot::setInitialDepthForIDS(int initialDepthForIDS) {
    this->initialDepthForIDS = initialDepthForIDS;
}

MiniMaxResult AbstractBot::miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta,
                                         vector<string> prevDepthStrategy, float timeLimit, bool applyTimeLimit) {
    clock_t beginTime = clock();
    MiniMaxResult miniMaxResult;

    if (this->isGameOver() || depth == 0) {
        miniMaxResult.payOff = this->getUtility();
        return miniMaxResult;
    }

    vector<string> validMoves = this->getValidMoves(isBlackTurn);
    // stalemate condition
    if (validMoves.size() == 0) {
        miniMaxResult.payOff = this->getUtility();
        return miniMaxResult;
    }

    /**
     * shifting the optimal move calculated for the previous depth
     * to the begining of the list for efficient alpha-beta pruning
     */
    if (!prevDepthStrategy.empty()) {
        for (int i = 0; i < validMoves.size(); i++) {
            if (validMoves[i] == prevDepthStrategy[0]) {
                validMoves.erase(validMoves.begin() + i);
            }
        }
        validMoves.insert(validMoves.begin(), prevDepthStrategy[0]);
    }

    miniMaxResult.payOff = isBlackTurn ? beta : alpha;
    for (string move : validMoves) {
        AbstractBot* currentBotCopy = this->cloneGameState();
        currentBotCopy->executeMove(move);
        
        // skip forbidden moves to avoid stagnant game conditions
        if(this->isBotInForbiddenState(currentBotCopy)) {
            continue;
        }

        float childAlpha = isBlackTurn ? alpha : miniMaxResult.payOff;
        float childBeta = isBlackTurn ? miniMaxResult.payOff : beta;

        vector<string> childPrevDepthStrategy;
        if (!prevDepthStrategy.empty() && move == prevDepthStrategy[0]) {
            childPrevDepthStrategy = prevDepthStrategy;
            childPrevDepthStrategy.erase(childPrevDepthStrategy.begin());
        }

        float childTimeLimit = timeLimit - (float)(clock() - beginTime) / CLOCKS_PER_SEC;
        if (applyTimeLimit && childTimeLimit < 0) {
            break;
        }

        /**
         * Not applying timelimit for further levels since incomplete search can lead to bad moves.
         *
         * Incomplete search wont affect the initial level as long as the initial branch explored is
         * the optimal move calculated in the previous depth miminax search. Here, incomplete search
         * may not give the best move, but it will output a move that is atleast as good as the
         * previous depth optimal move.
         *
         * This means that the time cutoff is not strict and the execution time might exceed the timeLimit
         */
        MiniMaxResult resultForMove = currentBotCopy->miniMaxSearch(!isBlackTurn, depth - 1, childAlpha, childBeta,
                                                                    childPrevDepthStrategy, childTimeLimit, false);
        delete currentBotCopy;

        if ((isBlackTurn && (resultForMove.payOff < miniMaxResult.payOff)) ||
            (!isBlackTurn && (resultForMove.payOff > miniMaxResult.payOff))) {
            miniMaxResult.payOff = resultForMove.payOff;
            miniMaxResult.strategy = resultForMove.strategy;
            miniMaxResult.strategy.insert(miniMaxResult.strategy.begin(), move);
            // alpha-beta pruning
            if ((isBlackTurn && miniMaxResult.payOff <= alpha) || (!isBlackTurn && miniMaxResult.payOff >= beta)) {
                break;
            }
        }
    }

    return miniMaxResult;
}

MiniMaxResult AbstractBot::iterativeDeepeningSearch(bool isBlackTurn, float timeLimit) {
    clock_t beginTime = clock();
    MiniMaxResult idsResult;

    for (int depth = this->initialDepthForIDS;; depth++) {
        float timeLeft = timeLimit - (float)(clock() - beginTime) / CLOCKS_PER_SEC;
        if (timeLeft < 0) {
            break;
        }
        // not applying time limit for the initial depth to avoid bad moves due to incomplete search
        bool applyTimeLimit = depth > this->initialDepthForIDS;
        MiniMaxResult resultForDepth = miniMaxSearch(isBlackTurn, depth, this->minUtility, this->maxUtility,
                                                     idsResult.strategy, timeLeft, applyTimeLimit);

        /**
         * Checking for result validity. It will be empty when the timelimit
         * is too small that even the first branch is not explored
         */
        if (!resultForDepth.strategy.empty()) {
            idsResult = resultForDepth;
        }
    }

    return idsResult;
}