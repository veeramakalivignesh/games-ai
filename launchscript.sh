mkdir -p bin

echo "compiling bot0 lib"
g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot0.o src/cpp/bot.cpp 
g++ -shared -Wl,-install_name,./bin/bot0lib.so -o ./bin/bot0lib.so ./bin/bot0.o

echo "compiling bot lib"
g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot.o src/cpp/bot/src/Bot.cpp 
g++ -c -fPIC -std=c++11 -O3 -o ./bin/abstract_bot.o src/cpp/bot/src/AbstractBot.cpp
g++ -c -fPIC -std=c++11 -O3 -o ./bin/cannon_bot.o src/cpp/bot/src/cannon/CannonBot.cpp
g++ -shared -Wl,-install_name,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o ./bin/abstract_bot.o ./bin/cannon_bot.o

echo "starting server"
uvicorn src.python.app:app