#include "../include/bot/Bot.h"
#include "./cannon/CannonBot.h"

Bot::Bot(Game game) {
    if (game == Cannon) {
        this->botImplementation = new CannonBot();
    }
}

string Bot::findBestMove(bool isBlackTurn) {
    // giving a time limit of 2 seconds
    MiniMaxResult idsResult = this->botImplementation->iterativeDeepeningSearch(isBlackTurn, 2);
    if (!idsResult.strategy.empty()) {
        cout << idsResult.strategy.size() << endl;
        return idsResult.strategy[0];
    } else {
        return "GAME OVER";
    }
}