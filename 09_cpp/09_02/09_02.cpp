#include "09_02.h"

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
    case op_INPUT_VAL:
    case op_OUTPUT_VAL:
    case op_REL_BASE:
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
    int64_t calc_value = instruction % 1'000;
    if (calc_value > 99 && calc_value <= 199)
    {
        return(IMMEDIATE_MODE);
    }
    else if (calc_value > 199)
    {
        return(RELATIVE_MODE);
    }
    else
    {
        return(POSITION_MODE);
    } 
}

ParMode_t get_parameter_mode_second(uint16_t instruction){
    int64_t calc_value = instruction % 10'000;
    if (calc_value > 999 && calc_value <= 1'999)
    {
        return(IMMEDIATE_MODE);
    }
    else if (calc_value > 1'999)
    {
        return(RELATIVE_MODE);
    }
    else
    {
        return(POSITION_MODE);
    } 
}

ParMode_t get_parameter_mode_third(uint16_t instruction){
    int64_t calc_value = instruction % 100'000;
    if (calc_value > 9'999 && calc_value <= 19'999)
    {
        return(IMMEDIATE_MODE);
    }
    else if (calc_value > 19'999)
    {
        return(RELATIVE_MODE);
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
    } else if (paramode == RELATIVE_MODE)
    {
        return_val = diag_program[diag_program[idx_instruction + idx_offset] + g_base_adress];
    } else
    {
        return_val = -1;
    }
    return(return_val);
}

void write_value_to_position(int64_t current_instruction, int64_t value_to_write, uint64_t idx)
{
    ParMode_t parameter_mode = get_parameter_mode_third(current_instruction);
    if (parameter_mode == POSITION_MODE)
    {
        diag_program[diag_program[idx + 3]] = value_to_write;
    }
    else if (parameter_mode == RELATIVE_MODE)
    {
        diag_program[diag_program[idx + 3] + g_base_adress] = value_to_write;
    }
}

int64_t get_parameter_value(uint32_t instruction_idx, ParameterNumber_t parameter_number){
    uint16_t instruction        = diag_program[instruction_idx];
    Opcode_t current_operation  = get_opcode(instruction);
    ParMode_t paramode_first    = get_parameter_mode_first(instruction);
    ParMode_t paramode_second   = get_parameter_mode_second(instruction);
    int64_t return_val          = -1;

    if( current_operation == op_ADD || current_operation == op_MULTIPLY || current_operation == op_LESS || current_operation == op_EQUAL || current_operation == op_JUMP_TRUE || current_operation == op_JUMP_FALSE || current_operation == op_REL_BASE )
    {
        switch (parameter_number)
        {
        case FIRST_PARAMETER:
            return_val = get_value_based_on_mode(paramode_first, instruction_idx, FIRST_PARAMETER);
            break;
        case SECOND_PARAMETER:
            return_val = get_value_based_on_mode(paramode_second, instruction_idx, SECOND_PARAMETER);
            break;
        case THIRD_PARAMETER:
            return_val = get_value_based_on_mode(POSITION_MODE, instruction_idx, THIRD_PARAMETER); // for add and multiply always a write operation, which is always position mode
            break;                        
        default:
            return_val = -1;
            break;
        }   
    } else if ( current_operation == op_INPUT_VAL )
    {
        if(parameter_number == FIRST_PARAMETER)
        {
            return_val = get_value_based_on_mode(paramode_first, instruction_idx, FIRST_PARAMETER);
            return(return_val); // for add and multiply always a write operation, which is always position mode
        } else
        {
            return_val = -1;
        }
    } else if ( current_operation == op_OUTPUT_VAL )
    {
        if(parameter_number == FIRST_PARAMETER)
        {
            return_val = get_value_based_on_mode(paramode_first, instruction_idx, FIRST_PARAMETER);
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

int64_t run_programm(uint8_t input_program)
{
    copy(begin(g_diag_program_original), end(g_diag_program_original), begin(diag_program));

    uint32_t idx                    = 0;
    uint32_t new_idx_offset         = 0;
    uint64_t current_instruction    = diag_program[idx];
    Opcode_t current_op_code        = get_opcode(current_instruction);
    bool idx_update                 = true;

    int64_t output;

    while (current_op_code != op_HALT)
    {
        if (current_op_code == op_ADD)
        {
            uint64_t add_1 = get_parameter_value(idx, FIRST_PARAMETER);
            uint64_t add_2 = get_parameter_value(idx, SECOND_PARAMETER);
            int64_t value_to_write = add_1 + add_2;
            write_value_to_position(current_instruction, value_to_write, idx);
            idx_update = true;
        } else if (current_op_code == op_MULTIPLY)
        {
            uint64_t multiplicator_1 = get_parameter_value(idx, FIRST_PARAMETER);
            uint64_t multiplicator_2 = get_parameter_value(idx, SECOND_PARAMETER);
            int64_t value_to_write = multiplicator_1 * multiplicator_2;
            write_value_to_position(current_instruction, value_to_write, idx);
            idx_update = true;
        }
        else if (current_op_code == op_INPUT_VAL)
        {
            write_value_to_position(current_instruction, input_program, idx);
            idx_update = true;
        }
        else if (current_op_code == op_OUTPUT_VAL)
        {
            output = get_parameter_value(idx, FIRST_PARAMETER);
            cout << "------------------\n";
            cout << "Output: " << output << "\n";

            idx_update = true;
        }
        else if (current_op_code == op_JUMP_TRUE)
        {
            if( get_parameter_value(idx, FIRST_PARAMETER) == 0)
            {
                idx_update = true;
            }
            else
            {
                idx = get_parameter_value(idx, SECOND_PARAMETER);
                idx_update = false;
            }
        }
        else if (current_op_code == op_JUMP_FALSE)
        {
            if( get_parameter_value(idx, FIRST_PARAMETER) != 0)
            {
                idx_update = true;
            }
            else
            {
                idx = get_parameter_value(idx, SECOND_PARAMETER);
                idx_update = false;
            }
        }
        else if (current_op_code == op_LESS)
        {
            uint64_t compare_1 = get_parameter_value(idx, FIRST_PARAMETER);
            uint64_t compare_2 = get_parameter_value(idx, SECOND_PARAMETER);
            if (compare_1 < compare_2)
            {
                write_value_to_position(current_instruction, 1, idx);
            }
            else
            {
                write_value_to_position(current_instruction, 0, idx);
            }
            idx_update = true;
        }    
        else if (current_op_code == op_EQUAL)
        {
            uint64_t compare_1 = get_parameter_value(idx, FIRST_PARAMETER);
            uint64_t compare_2 = get_parameter_value(idx, SECOND_PARAMETER);
            if (compare_1 == compare_2)
            {
                write_value_to_position(current_instruction, 1, idx);
            }
            else
            {
                write_value_to_position(current_instruction, 0, idx);
            }
            idx_update = true;
        }
        else if (current_op_code == op_REL_BASE)
        {
            int64_t new_base_adress = get_parameter_value(idx, FIRST_PARAMETER);
            g_base_adress = g_base_adress + new_base_adress;
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
    uint8_t input = 2;
    run_programm(input);
    cout << "------------------\n";
    cout << "program ends";

}