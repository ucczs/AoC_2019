#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Opcode_t  {op_UNDEFINED    = 0, 
                op_ADD          = 1, 
                op_MULTIPLY     = 2, 
                op_SAVE_VAL     = 3, 
                op_OUTPUT_VAL   = 4,
                op_JUMP_TRUE    = 5,
                op_JUMP_FALSE   = 6,
                op_LESS         = 7,
                op_EQUAL        = 8,
                op_HALT         = 99};

enum ParMode_t {POSITION_MODE   = 0, 
                IMMEDIATE_MODE  = 1};