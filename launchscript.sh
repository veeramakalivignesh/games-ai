mkdir -p bin

echo "compiling bot lib"
g++ -c -fPIC -std=c++11 -O3 -o ./bin/bot.o src/cpp/bot.cpp 
g++ -shared -Wl,-install_name,./bin/botlib.so -o ./bin/botlib.so ./bin/bot.o

echo "starting server"
uvicorn src.python.app:app