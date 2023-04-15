from ctypes import *

bot_lib = cdll.LoadLibrary('./bin/botlib.so')

class CannonBotService():
  
    def __init__(self):
        bot_lib.new_bot.restype = c_void_p
        self.c_bot_ptr = c_void_p(bot_lib.new_bot())
  
    def find_best_move(self, move):
        c_move = c_char_p(move.encode())
        c_buffer = create_string_buffer(11)
        bot_lib.find_best_move(self.c_bot_ptr, c_move, c_buffer)
        return c_buffer.value.decode()