from fastapi import Request, FastAPI
from fastapi.middleware.cors import CORSMiddleware
from src.python.cannon_bot_service import CannonBotService

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:3000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

bot_service = CannonBotService()

@app.post("/move")
async def find_best_move(request: Request):
    body = await request.json()
    response_move = bot_service.find_best_move(body["gameState"], body["isBlackTurn"])
    # body["move"] = response_move
    return body
