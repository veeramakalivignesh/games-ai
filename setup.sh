#!/bin/sh

echo "Creating bin directory"
mkdir -p bin

echo "Compiling bot lib"
g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot.o src/cpp/bot/src/Bot.cpp 
g++ -c -fPIC -std=c++11 -O3 -o ./bin/abstract_bot.o src/cpp/bot/src/AbstractBot.cpp
g++ -c -fPIC -std=c++11 -O3 -o ./bin/cannon_bot.o src/cpp/bot/src/cannon/CannonBot.cpp

if [[ $OSTYPE == 'darwin'* ]]; then
    g++ -shared -Wl,-install_name,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o ./bin/abstract_bot.o ./bin/cannon_bot.o
else
    g++ -shared -Wl,-soname,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o ./bin/abstract_bot.o ./bin/cannon_bot.o
fi

echo "Setup complete"