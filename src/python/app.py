from fastapi import Request, FastAPI
from fastapi.middleware.cors import CORSMiddleware
from src.python.cannon_bot import CannonBot
from ctypes import c_wchar_p

origins = [
    "http://localhost:3000",
]

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

bot = CannonBot()

@app.post("/move")
async def find_best_move(request: Request):
    body = await request.json()
    response_move = bot.get_move(body["move"])
    body["move"] = response_move
    return body
