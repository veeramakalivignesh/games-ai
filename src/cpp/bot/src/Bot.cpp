#include "../include/bot/Bot.h"
#include "./cannon/CannonBot.h"

Bot::Bot(Game game) {
    this->botImplementation = new CannonBot();
}

// handle gameover condition
string Bot::findBestMove(bool isBlackTurn) {
    Result idsResult = this->botImplementation->iterativeDeepeningSearch(isBlackTurn, 2);
    string bestMove = idsResult.strategy[0];
    return bestMove;
} 