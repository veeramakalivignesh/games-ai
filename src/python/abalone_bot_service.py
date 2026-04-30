from ctypes import *

class AbaloneBotService():

    def __init__(self, lib_name):
        self.bot_lib = cdll.LoadLibrary(lib_name)
        self.bot_lib.new_bot_abalone.restype = c_void_p
        self.c_bot_ptr = c_void_p(self.bot_lib.new_bot_abalone())

    def convert_game_state_to_int(game_state):
        conv = {'E': 0, 'B': -1, 'W': 1, 'F': 2}
        return [[conv[cell] for cell in row] for row in game_state]

    def convert_list_to_cint_array_2d(list_2d):
        c_array = (POINTER(c_int) * len(list_2d))()
        for i, row in enumerate(list_2d):
            c_row = (c_int * len(row))(*row)
            c_array[i] = c_row
        return c_array, c_int(len(list_2d)), c_int(len(list_2d[0]))

    def convert_list_to_cint_array_3d(list_3d):
        c_array = (POINTER(POINTER(c_int)) * len(list_3d))()
        for i, state in enumerate(list_3d):
            c_2d, _, _ = AbaloneBotService.convert_list_to_cint_array_2d(state)
            c_array[i] = c_2d
        return c_array, c_int(len(list_3d))

    def find_best_move(self, game_state, is_black_turn, forbidden_states):
        int_gs = AbaloneBotService.convert_game_state_to_int(game_state)
        (c_gs, c_rows, c_cols) = AbaloneBotService.convert_list_to_cint_array_2d(int_gs)

        int_fs = [AbaloneBotService.convert_game_state_to_int(fs) for fs in forbidden_states]
        (c_fs, c_num_fs) = AbaloneBotService.convert_list_to_cint_array_3d(int_fs)

        c_response = create_string_buffer(32)

        self.bot_lib.find_best_move_abalone(
            self.c_bot_ptr, c_gs, c_rows, c_cols,
            c_fs, c_num_fs, is_black_turn, c_response
        )
        return c_response.value.decode()
