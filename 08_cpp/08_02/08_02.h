#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>

enum Opcode_t  {op_UNDEFINED    = 0, 
                op_ADD          = 1, 
                op_MULTIPLY     = 2, 
                op_INPUT_VAL     = 3, 
                op_OUTPUT_VAL   = 4,
                op_JUMP_TRUE    = 5,
                op_JUMP_FALSE   = 6,
                op_LESS         = 7,
                op_EQUAL        = 8,
                op_HALT         = 99};

enum ParMode_t {POSITION_MODE   = 0, 
                IMMEDIATE_MODE  = 1};

//const int64_t g_diag_program_original[] = {3,8,1001,8,10,8,105,1,0,0,21,42,55,64,77,94,175,256,337,418,99999,3,9,102,4,9,9,1001,9,5,9,102,2,9,9,101,3,9,9,4,9,99,3,9,102,2,9,9,101,5,9,9,4,9,99,3,9,1002,9,4,9,4,9,99,3,9,102,4,9,9,101,5,9,9,4,9,99,3,9,102,5,9,9,1001,9,3,9,1002,9,5,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,1001,9,2,9,4,9,3,9,1001,9,1,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,1001,9,1,9,4,9,3,9,1001,9,1,9,4,9,99,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,101,2,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,1001,9,2,9,4,9,3,9,101,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,99,3,9,1001,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,102,2,9,9,4,9,3,9,102,2,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,101,1,9,9,4,9,3,9,101,1,9,9,4,9,3,9,1002,9,2,9,4,9,3,9,1002,9,2,9,4,9,3,9,1001,9,1,9,4,9,99};

//Test 1 (Phase settings 9,8,7,6,5 lead to result 139_629_729)
const int64_t g_diag_program_original[] = {3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5};

//Test 1 (Phase settings 9,7,8,5,6 lead to result 18_216)
//const int64_t g_diag_program_original[] = {3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};

const int16_t size_prog = sizeof(g_diag_program_original)/sizeof(int64_t);

int64_t run_with_permutations(int* phase_combination, int no_phases, int first_element, int64_t maximum);
int64_t run_all_amplifiers(int* phase_combination);



class CAmplifier
{
private:
    int64_t ampl_program[size_prog] = {0};
    Opcode_t get_opcode(int16_t instruction);
    int8_t get_increase_size(int16_t instruction);
    ParMode_t get_parameter_mode_first(int16_t instruction);
    ParMode_t get_parameter_mode_second(int16_t instruction);
    ParMode_t get_parameter_mode_third(int16_t instruction);
    int64_t get_value_based_on_mode(ParMode_t paramode, int32_t idx_instruction, int8_t idx_offset);
    int64_t get_parameter_value(int32_t instruction_idx, int8_t parameter_number);
    CAmplifier* child_amp;
    std::deque<int64_t> input_queue;
    int64_t input_waiting_index;

public:
    CAmplifier(int8_t phase_setting, int64_t input);
    ~CAmplifier();
    int64_t run_program();
    int64_t run_program(int64_t input, int8_t phase);
    void set_child_amp(CAmplifier* child);
    void add_input(int new_input);
    int64_t get_last_input();
};