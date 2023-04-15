from ctypes import *

bot_lib = cdll.LoadLibrary('./bin/botlib.so')

class CannonBotService():
  
    def __init__(self):
        bot_lib.new_bot.restype = c_void_p
        self.c_bot_ptr = c_void_p(bot_lib.new_bot())

    def convert_game_state_to_int(game_state):
        convertion_dict = {'E':0, 'B':-1, 'W':1, 'Tb':-2, 'Tw':2}
        int_game_state = []
        for row in game_state:
            int_row = []
            for str in row:
                int_row.append(convertion_dict[str])
            int_game_state.append(int_row)
        return int_game_state
  
    # check 2d list
    def convert_list_to_cint_array (list_2d):
        c_array = (POINTER(c_int) * len(list_2d))()
        i = 0
        for int_row in list_2d:
            c_row = (c_int * len(int_row))(*int_row)
            c_array[i] = c_row
            i += 1
        return (c_array, c_int(len(list_2d)), c_int(len(list_2d[0])))


    def find_best_move(self, game_state, is_black_turn):
        # convert game_state into 2d c array
        int_game_state = CannonBotService.convert_game_state_to_int(game_state)
        (c_game_state, c_num_rows, c_num_columns) = CannonBotService.convert_list_to_cint_array(int_game_state)
        print(c_num_rows, c_num_columns)

        # buffer on which the response move will be written
        c_response_move_buffer = create_string_buffer(11)
       
        bot_lib.find_best_move(self.c_bot_ptr, c_game_state, c_num_rows, c_num_columns, is_black_turn, c_response_move_buffer)
        response_move = c_response_move_buffer.value.decode()
        return response_move