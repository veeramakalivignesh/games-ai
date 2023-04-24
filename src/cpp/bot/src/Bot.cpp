#include "../include/bot/Bot.h"
#include "./cannon/CannonBot.h"

Bot::Bot(Game game) {
    this->botImplementation = new CannonBot();
}

string Bot::findBestMove(bool isBlackTurn) {
    string move = "S 4 4 M 4 1";
    CannonBot* cannonBot = dynamic_cast<CannonBot*> (this->botImplementation);
    cannonBot->printGame();
    cout << endl << move << endl << endl;
    cannonBot->executeMove(move);
    cannonBot->printGame();
    return "Heyya";
}