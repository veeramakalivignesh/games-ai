#include "../include/bot/Bot.h"
#include "./cannon/CannonBot.h"
#include "./abalone/AbaloneBot.h"

Bot::Bot(Game game) {
    if (game == Cannon) {
        this->botImplementation = new CannonBot();
    } else if (game == Abalone) {
        this->botImplementation = new AbaloneBot();
    }
}

string Bot::findBestMove(bool isBlackTurn) {
    MiniMaxResult idsResult = this->botImplementation->iterativeDeepeningSearch(isBlackTurn, 2);
    if (!idsResult.strategy.empty()) {
        cout << idsResult.strategy.size() << endl;
        return idsResult.strategy[0];
    } else {
        return "GAME OVER";
    }
}
