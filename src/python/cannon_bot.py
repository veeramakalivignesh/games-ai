from ctypes import *

lib = cdll.LoadLibrary('./bin/botlib.so')

class CannonBot():
  
    def __init__(self):
        Class_ctor_wrapper = lib.new_bot
        Class_ctor_wrapper.restype = c_void_p
        self.obj = c_void_p(Class_ctor_wrapper())
  
    def get_move(self, move):
        c_move = c_char_p(move.encode())
        c_buffer = create_string_buffer(11)
        lib.get_move(self.obj, c_move, c_buffer)
        return c_buffer.value.decode()