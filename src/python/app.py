from fastapi import Request, FastAPI
from fastapi.middleware.cors import CORSMiddleware
from src.python.cannon_bot_service import CannonBotService
from src.python.abalone_bot_service import AbaloneBotService

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=[ "https://games.veera-ai.com",
                    "http://games.veera-ai.com",
                    "https://games-ai-ui-abfe0e7a7e95.herokuapp.com",
                    "http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

cannon_primary   = CannonBotService('./bin/botlib.so')
cannon_secondary = CannonBotService('./bin/botlib.so')

abalone_primary   = AbaloneBotService('./bin/botlib.so')
abalone_secondary = AbaloneBotService('./bin/botlib.so')

@app.get("/health")
async def health():
    return {"status": "up"}


@app.post("/primary/move")
async def cannon_primary_move(request: Request):
    body = await request.json()
    move = cannon_primary.find_best_move(body["gameState"], body["isBlackTurn"], body["forbiddenStates"])
    return {"move": move}

@app.post("/secondary/move")
async def cannon_secondary_move(request: Request):
    body = await request.json()
    move = cannon_secondary.find_best_move(body["gameState"], body["isBlackTurn"], body["forbiddenStates"])
    return {"move": move}


@app.post("/abalone/primary/move")
async def abalone_primary_move(request: Request):
    body = await request.json()
    move = abalone_primary.find_best_move(body["gameState"], body["isBlackTurn"], body["forbiddenStates"])
    return {"move": move}

@app.post("/abalone/secondary/move")
async def abalone_secondary_move(request: Request):
    body = await request.json()
    move = abalone_secondary.find_best_move(body["gameState"], body["isBlackTurn"], body["forbiddenStates"])
    return {"move": move}
