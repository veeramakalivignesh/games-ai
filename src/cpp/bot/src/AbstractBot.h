#ifndef ABSTRACTBOT_H
#define ABSTRACTBOT_H

#include <iostream>
#include <vector>
using namespace std;

/**
 * This struct definies the result of minimax search
 * @param strategy optimal path found in the search tree
 * @param payOff utility function value of that path
 */
struct MiniMaxResult {
    vector<string> strategy;
    float payOff;
};

/**
 * This is an abstract class that defines the function templates for the
 * game-specific bot implementation. Game independent logics are implemented here
 *
 * @author veeramakali vignesh
 */
class AbstractBot {
  private:
    // These values are game dependent and needs to be set in the respective constructor
    float minUtility;
    float maxUtility;
    int initialDepthForIDS;

  protected:
    void setMinUtility(float minUtility);
    void setMaxUtility(float maxUtility);
    void setInitialDepthForIDS(int initialDepthForIDS);

  public:
    virtual ~AbstractBot() = default;

    /**
     * Creates a new object instance with the same gameState
     * @return Pointer to the instance
     */
    virtual AbstractBot* cloneGameState() = 0;

    /**
     * Calculates the list of valid moves for the current gameState and player turn
     * @param isBlackTurn
     */
    virtual vector<string> getValidMoves(bool isBlackTurn) = 0;

    /**
     * Executes the move and modifies the gameState accordingly
     * @param move
     */
    virtual void executeMove(string move) = 0;

    /**
     * Checks if the game is over for the current gameState
     */
    virtual bool isGameOver() = 0;

    /**
     * Calculates the utility funtion value for the current gameState
     */
    virtual float getUtility() = 0;

    /**
     * Checks if the parameter bot gameState is forbidden to avoid stagnant game conditions
     */
    virtual bool isBotInForbiddenState(AbstractBot* abstractBot) = 0;

    /**
     * Performs the minimax search from the current gameState
     * @param isBlackTurn current player
     * @param depth depth of the search tree
     * @param alpha lower limit of the utility value in alpha-beta pruning
     * @param beta upper limit of the utility value in alpha-beta pruning
     * @param prevDepthStrategy strategy calculated in the previous depth search in IDS
     * @param timeLimit value to limit the execution time of the search
     * @param applyTimeLimit says if time limit should be applied
     */
    MiniMaxResult miniMaxSearch(bool isBlackTurn, int depth, float alpha, float beta, vector<string> prevDepthStrategy,
                                float timeLimit, bool applyTimeLimit);

    /**
     * Performs the above minimax search with incremental depth untill the given time limit is exceeded
     */
    MiniMaxResult iterativeDeepeningSearch(bool isBlackTurn, float timeLimit);
};

#endif