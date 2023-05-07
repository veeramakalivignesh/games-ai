from ctypes import *

class CannonBotService():
  
    def __init__(self, lib_name):
        self.bot_lib = cdll.LoadLibrary(lib_name)
        self.bot_lib.new_bot_cannon.restype = c_void_p
        self.c_bot_ptr = c_void_p(self.bot_lib.new_bot_cannon())

    def convert_game_state_to_int(game_state):
        convertion_dict = {'E':0, 'B':-1, 'W':1, 'Tb':-2, 'Tw':2}
        int_game_state = []
        for row in game_state:
            int_row = []
            for str in row:
                int_row.append(convertion_dict[str])
            int_game_state.append(int_row)
        return int_game_state
  
    def convert_list_to_cint_array_2d (list_2d):
        c_array = (POINTER(c_int) * len(list_2d))()
        i = 0
        for int_row in list_2d:
            c_row = (c_int * len(int_row))(*int_row)
            c_array[i] = c_row
            i += 1
        return (c_array, c_int(len(list_2d)), c_int(len(list_2d[0])))
    
    def convert_list_to_cint_array_3d (list_3d):
        c_array = (POINTER(POINTER(c_int)) * len(list_3d))()
        i = 0
        for list_2d in list_3d:
            (c_row, _, _) = CannonBotService.convert_list_to_cint_array_2d(list_2d)
            c_array[i] = c_row
            i += 1
        return (c_array, c_int(len(list_3d)))


    def find_best_move(self, game_state, is_black_turn, forbidden_states):
        # convert game_state into 2d c array
        int_game_state = CannonBotService.convert_game_state_to_int(game_state)
        (c_game_state, c_num_rows, c_num_columns) = CannonBotService.convert_list_to_cint_array_2d(int_game_state)

        # convert forbidden_states into 3d c array
        int_forbidden_states = []
        for forbidden_state in forbidden_states:
            int_forbidden_states.append(CannonBotService.convert_game_state_to_int(forbidden_state))
        (c_forbidden_states, c_num_forbidden_states) = CannonBotService.convert_list_to_cint_array_3d(int_forbidden_states)

        # buffer on which the response move will be written
        c_response_move_buffer = create_string_buffer(11)
       
        self.bot_lib.find_best_move_cannon(self.c_bot_ptr, c_game_state, c_num_rows, c_num_columns, 
                                           c_forbidden_states, c_num_forbidden_states, is_black_turn, c_response_move_buffer)
        response_move = c_response_move_buffer.value.decode()
        return response_move