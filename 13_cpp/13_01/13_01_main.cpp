#include "intcode_computer.h"

CGame_map g_game_map;

int main()
{
    CIntcode_computer intcoder;
    intcoder.run_programm(&g_game_map);
    
    cout << "\n------------------\n";
    cout << "program ends\n";

    int16_t element_counter = g_game_map.count_element_types(BLOCK);
    cout << endl << "Total blocks in game " << element_counter << endl;
}