mkdir -p bin

# echo "compiling bot_old lib"
# g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot_old.o src/cpp/bot_old.cpp 
# g++ -shared -Wl,-install_name,./bin/botlib_old.so -o ./bin/botlib_old.so ./bin/bot_old.o

echo "compiling bot lib"
g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot.o src/cpp/bot/src/Bot.cpp 
g++ -c -fPIC -std=c++11 -O3 -o ./bin/abstract_bot.o src/cpp/bot/src/AbstractBot.cpp
g++ -c -fPIC -std=c++11 -O3 -o ./bin/cannon_bot.o src/cpp/bot/src/cannon/CannonBot.cpp

if [[ $OSTYPE == 'darwin'* ]]; then
    g++ -shared -Wl,-install_name,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o ./bin/abstract_bot.o ./bin/cannon_bot.o
else
    g++ -shared -Wl,-soname,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o ./bin/abstract_bot.o ./bin/cannon_bot.o
fi

echo "starting server"
uvicorn src.python.app:app --host 0.0.0.0