#include "05_02.h"

int64_t g_diag_program_original[] = {3,225,1,225,6,6,1100,1,238,225,104,0,1101,72,36,225,1101,87,26,225,2,144,13,224,101,-1872,224,224,4,224,102,8,223,223,1001,224,2,224,1,223,224,223,1102,66,61,225,1102,25,49,224,101,-1225,224,224,4,224,1002,223,8,223,1001,224,5,224,1,223,224,223,1101,35,77,224,101,-112,224,224,4,224,102,8,223,223,1001,224,2,224,1,223,224,223,1002,195,30,224,1001,224,-2550,224,4,224,1002,223,8,223,1001,224,1,224,1,224,223,223,1102,30,44,225,1102,24,21,225,1,170,117,224,101,-46,224,224,4,224,1002,223,8,223,101,5,224,224,1,224,223,223,1102,63,26,225,102,74,114,224,1001,224,-3256,224,4,224,102,8,223,223,1001,224,3,224,1,224,223,223,1101,58,22,225,101,13,17,224,101,-100,224,224,4,224,1002,223,8,223,101,6,224,224,1,224,223,223,1101,85,18,225,1001,44,7,224,101,-68,224,224,4,224,102,8,223,223,1001,224,5,224,1,223,224,223,4,223,99,0,0,0,677,0,0,0,0,0,0,0,0,0,0,0,1105,0,99999,1105,227,247,1105,1,99999,1005,227,99999,1005,0,256,1105,1,99999,1106,227,99999,1106,0,265,1105,1,99999,1006,0,99999,1006,227,274,1105,1,99999,1105,1,280,1105,1,99999,1,225,225,225,1101,294,0,0,105,1,0,1105,1,99999,1106,0,300,1105,1,99999,1,225,225,225,1101,314,0,0,106,0,0,1105,1,99999,7,677,226,224,102,2,223,223,1005,224,329,101,1,223,223,8,677,226,224,1002,223,2,223,1005,224,344,1001,223,1,223,1107,677,677,224,102,2,223,223,1005,224,359,1001,223,1,223,1107,226,677,224,102,2,223,223,1005,224,374,101,1,223,223,7,226,677,224,102,2,223,223,1005,224,389,101,1,223,223,8,226,677,224,1002,223,2,223,1005,224,404,101,1,223,223,1008,226,677,224,1002,223,2,223,1005,224,419,1001,223,1,223,107,677,677,224,102,2,223,223,1005,224,434,101,1,223,223,1108,677,226,224,1002,223,2,223,1006,224,449,101,1,223,223,1108,677,677,224,102,2,223,223,1006,224,464,101,1,223,223,1007,677,226,224,102,2,223,223,1006,224,479,101,1,223,223,1008,226,226,224,102,2,223,223,1006,224,494,101,1,223,223,108,226,226,224,1002,223,2,223,1006,224,509,101,1,223,223,107,226,226,224,102,2,223,223,1006,224,524,101,1,223,223,1107,677,226,224,102,2,223,223,1005,224,539,1001,223,1,223,108,226,677,224,1002,223,2,223,1005,224,554,101,1,223,223,1007,226,226,224,102,2,223,223,1005,224,569,101,1,223,223,8,226,226,224,102,2,223,223,1006,224,584,101,1,223,223,1008,677,677,224,1002,223,2,223,1005,224,599,1001,223,1,223,107,226,677,224,1002,223,2,223,1005,224,614,1001,223,1,223,1108,226,677,224,102,2,223,223,1006,224,629,101,1,223,223,7,677,677,224,1002,223,2,223,1005,224,644,1001,223,1,223,108,677,677,224,102,2,223,223,1005,224,659,101,1,223,223,1007,677,677,224,102,2,223,223,1006,224,674,101,1,223,223,4,223,99,226};
const uint16_t size_prog = sizeof(g_diag_program_original)/sizeof(int64_t);
int64_t diag_program[size_prog];


Opcode_t get_opcode(uint16_t instruction){
    Opcode_t operation = static_cast<Opcode_t>(instruction % 100);
    return(operation);
}

uint8_t get_increase_size(uint16_t instruction){
    Opcode_t opcode = get_opcode(instruction);
    switch (opcode)
    {
    case op_ADD:
    case op_MULTIPLY:
    case op_LESS:
    case op_EQUAL:
        return(4);
        break;
    case op_SAVE_VAL:
    case op_OUTPUT_VAL:
        return(2);
        break;
    case op_JUMP_TRUE:
    case op_JUMP_FALSE:
        return(3);
        break;
    default:
        return(0);
    }
}

ParMode_t get_parameter_mode_first(uint16_t instruction){
    int64_t calc_value = instruction % 1000;
    if (calc_value > 99)
    {
        return(IMMEDIATE_MODE);
    }
    else
    {
        return(POSITION_MODE);
    } 
}

ParMode_t get_parameter_mode_second(uint16_t instruction){
    int64_t calc_value = instruction % 10000;
    if (calc_value > 999)
    {
        return(IMMEDIATE_MODE);
    }
    else
    {
        return(POSITION_MODE);
    } 
}

ParMode_t get_parameter_mode_third(uint16_t instruction){
    int64_t calc_value = instruction % 100000;
    if (calc_value > 9999)
    {
        return(IMMEDIATE_MODE);
    }
    else
    {
        return(POSITION_MODE);
    } 
}

int64_t get_value_based_on_mode(ParMode_t paramode, uint32_t idx_instruction, uint8_t idx_offset){
    int64_t return_val = -1;
    if (paramode == IMMEDIATE_MODE)
    {
        return_val = diag_program[idx_instruction + idx_offset];
    } else if (paramode == POSITION_MODE)
    {
        return_val = diag_program[diag_program[idx_instruction + idx_offset]];
    } else
    {
        return_val = -1;
    }
    return(return_val);
}

int64_t get_parameter_value(uint32_t instruction_idx, uint8_t parameter_number){
    uint16_t instruction        = diag_program[instruction_idx];
    Opcode_t current_operation  = get_opcode(instruction);
    ParMode_t paramode_first    = get_parameter_mode_first(instruction);
    ParMode_t paramode_second   = get_parameter_mode_second(instruction);
    int64_t return_val          = -1;

    if( current_operation == op_ADD || current_operation == op_MULTIPLY || current_operation == op_LESS || current_operation == op_EQUAL || current_operation == op_JUMP_TRUE || current_operation == op_JUMP_FALSE )
    {
        switch (parameter_number)
        {
        case 1:
            return_val = get_value_based_on_mode(paramode_first, instruction_idx, 1);
            break;
        case 2:
            return_val = get_value_based_on_mode(paramode_second, instruction_idx, 2);
            break;
        case 3:
            return_val = get_value_based_on_mode(POSITION_MODE, instruction_idx, 3); // for add and multiply always a write operation, which is always position mode
            break;                        
        default:
            return_val = -1;
            break;
        }   
    } else if ( current_operation == op_SAVE_VAL )
    {
        if(parameter_number == 1)
        {
            return_val = get_value_based_on_mode(POSITION_MODE, instruction_idx, 1);
            return(return_val); // for add and multiply always a write operation, which is always position mode
        } else
        {
            return_val = -1;
        }
    } else if ( current_operation == op_OUTPUT_VAL )
    {
        if(parameter_number == 1)
        {
            return_val = get_value_based_on_mode(paramode_first, instruction_idx, 1);
            return(return_val);
        } else
        {
            return_val = -1;
        }        
    } else
    {
        return_val = -1;
    }
    
    return(return_val);    
}

int64_t run_programm(uint8_t input_program){
    copy(begin(g_diag_program_original), end(g_diag_program_original), begin(diag_program));

    uint32_t idx                    = 0;
    uint32_t new_idx_offset         = 0;
    uint64_t current_instruction    = diag_program[0];
    Opcode_t current_op_code        = get_opcode(current_instruction);
    bool idx_update                 = true;

    int64_t output;

    while (current_op_code != op_HALT)
    {
        if (current_op_code == op_ADD)
        {
            uint64_t add_1 = get_parameter_value(idx, 1);
            uint64_t add_2 = get_parameter_value(idx, 2);
            diag_program[diag_program[idx + 3]] = add_1 + add_2;
            idx_update = true;
        } else if (current_op_code == op_MULTIPLY)
        {
            uint64_t multiplicator_1 = get_parameter_value(idx, 1);
            uint64_t multiplicator_2 = get_parameter_value(idx, 2);
            diag_program[diag_program[idx + 3]] = multiplicator_1 * multiplicator_2;
            idx_update = true;
        }
        else if (current_op_code == op_SAVE_VAL)
        {
            diag_program[diag_program[idx + 1]] = input_program;
            idx_update = true;
        }
        else if (current_op_code == op_OUTPUT_VAL)
        {
            output = get_parameter_value(idx, 1);
            cout << "------------------\n";
            cout << "Index: " << idx << "\n";
            cout << "Output: " << output << "\n\n";

            idx_update = true;
        }
        else if (current_op_code == op_JUMP_TRUE)
        {
            if( get_parameter_value(idx, 1) == 0)
            {
                idx_update = true;
            }
            else
            {
                idx = get_parameter_value(idx, 2);
                idx_update = false;
            }
        }
        else if (current_op_code == op_JUMP_FALSE)
        {
            if( get_parameter_value(idx, 1) != 0)
            {
                idx_update = true;
            }
            else
            {
                idx = get_parameter_value(idx, 2);
                idx_update = false;
            }
        }
        else if (current_op_code == op_LESS)
        {
            uint64_t compare_1 = get_parameter_value(idx, 1);
            uint64_t compare_2 = get_parameter_value(idx, 2);
            if (compare_1 < compare_2)
            {
                diag_program[diag_program[idx + 3]] = 1;
            }
            else
            {
                diag_program[diag_program[idx + 3]] = 0;
            }
            idx_update = true;
        }    
        else if (current_op_code == op_EQUAL)
        {
            uint64_t compare_1 = get_parameter_value(idx, 1);
            uint64_t compare_2 = get_parameter_value(idx, 2);
            if (compare_1 == compare_2)
            {
                diag_program[diag_program[idx + 3]] = 1;
            }
            else
            {
                diag_program[diag_program[idx + 3]] = 0;
            }
            idx_update = true;
        }
        else
        {
            return(-1);
        }


        if (idx_update)
        {
            new_idx_offset = get_increase_size(current_instruction);
            idx = idx + new_idx_offset;
        }
        

        current_instruction = diag_program[idx];
        current_op_code = get_opcode(current_instruction);
    }

    return(diag_program[0]);
}

int main()
{
    uint8_t input = 5;
    run_programm(input);
    cout << "------------------\n";
    cout << "program ends";

}