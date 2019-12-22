#include "07_02.h"

using namespace std;


CAmplifier::CAmplifier(int8_t phase_setting, int64_t input=-1)
{
    copy(begin(g_diag_program_original), end(g_diag_program_original), begin(ampl_program) );
    add_input(phase_setting);
    if (input >= 0)
    {
        add_input(input);
    }
    input_waiting_index = 0;
}

CAmplifier::~CAmplifier()
{
}

int64_t CAmplifier::get_last_input()
{
    return input_queue[-1];
}

void CAmplifier::set_child_amp(CAmplifier* child)
{
    child_amp = child;
}

void CAmplifier::add_input(int new_input)
{
    input_queue.push_back(new_input);
}

Opcode_t CAmplifier::get_opcode(int16_t instruction){
    Opcode_t operation = static_cast<Opcode_t>(instruction % 100);
    return(operation);
}

int8_t CAmplifier::get_increase_size(int16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_first(int16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_second(int16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_third(int16_t instruction){
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

int64_t CAmplifier::get_value_based_on_mode(ParMode_t paramode, int32_t idx_instruction, int8_t idx_offset){
    int64_t return_val = -1;
    if (paramode == IMMEDIATE_MODE)
    {
        return_val = ampl_program[idx_instruction + idx_offset];
    } else if (paramode == POSITION_MODE)
    {
        return_val = ampl_program[ampl_program[idx_instruction + idx_offset]];
    } else
    {
        return_val = -1;
    }
    return(return_val);
}

int64_t CAmplifier::get_parameter_value(int32_t instruction_idx, int8_t parameter_number){
    int16_t instruction        = ampl_program[instruction_idx];
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
    } else if ( current_operation == op_INPUT_VAL )
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

int64_t CAmplifier::run_program(){
    int32_t idx                    = input_waiting_index;
    int32_t new_idx_offset         = 0;
    int64_t current_instruction    = ampl_program[idx];
    Opcode_t current_op_code        = get_opcode(current_instruction);
    bool idx_update                 = true;

    int64_t output;

    while (current_op_code != op_HALT)
    {
        if (current_op_code == op_ADD)
        {
            int64_t add_1 = get_parameter_value(idx, 1);
            int64_t add_2 = get_parameter_value(idx, 2);
            ampl_program[ampl_program[idx + 3]] = add_1 + add_2;
            idx_update = true;
        } else if (current_op_code == op_MULTIPLY)
        {
            int64_t multiplicator_1 = get_parameter_value(idx, 1);
            int64_t multiplicator_2 = get_parameter_value(idx, 2);
            ampl_program[ampl_program[idx + 3]] = multiplicator_1 * multiplicator_2;
            idx_update = true;
        }
        else if (current_op_code == op_INPUT_VAL)
        {
            if( input_queue.size() > 0 )
            {
                ampl_program[ampl_program[idx + 1]] = input_queue[0];
                input_queue.pop_front();
                idx_update = true;
            }
            else
            {
                input_waiting_index = idx;
                return(0);
            }
            
        }
        else if (current_op_code == op_OUTPUT_VAL)
        {
            output = get_parameter_value(idx, 1);

            child_amp->add_input(output);
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
            int64_t compare_1 = get_parameter_value(idx, 1);
            int64_t compare_2 = get_parameter_value(idx, 2);
            if (compare_1 < compare_2)
            {
                ampl_program[ampl_program[idx + 3]] = 1;
            }
            else
            {
                ampl_program[ampl_program[idx + 3]] = 0;
            }
            idx_update = true;
        }    
        else if (current_op_code == op_EQUAL)
        {
            int64_t compare_1 = get_parameter_value(idx, 1);
            int64_t compare_2 = get_parameter_value(idx, 2);
            if (compare_1 == compare_2)
            {
                ampl_program[ampl_program[idx + 3]] = 1;
            }
            else
            {
                ampl_program[ampl_program[idx + 3]] = 0;
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
        

        current_instruction = ampl_program[idx];
        current_op_code = get_opcode(current_instruction);
    }

    input_waiting_index = idx;
    return(output);
}

int64_t run_with_permutations(int* phase_combination, int no_phases, int first_element=0, int64_t maximum=0)
{
    int32_t temp_result = 0;

    if(first_element == no_phases)
    {
        temp_result = run_all_amplifiers(phase_combination);
        if (temp_result > maximum)
        {
            maximum = temp_result;
            cout << "new max with combination " << phase_combination[0] << ", " << phase_combination[1] << ", " << phase_combination[2] << ", " << phase_combination[3] << ", " << phase_combination[4] << endl;
        }
    }

    else
    {
        for (int i = 0; i < no_phases; i++)
        {
            swap(phase_combination[0], phase_combination[i]);
            maximum = run_with_permutations(phase_combination, no_phases, first_element+1, maximum);
            swap(phase_combination[0], phase_combination[i]);
        }
    }

    return maximum;
}

int64_t run_all_amplifiers(int* phase_combination)
{
    int64_t result = 0;

    int64_t last_output = 0;
    int64_t input_a = 0;

    int8_t phase_a = *phase_combination;
    int8_t phase_b = *(phase_combination+1);
    int8_t phase_c = *(phase_combination+2);
    int8_t phase_d = *(phase_combination+3);
    int8_t phase_e = *(phase_combination+4);

    CAmplifier amplifier_a(phase_a, input_a);
    CAmplifier amplifier_b(phase_b);
    CAmplifier amplifier_c(phase_c);
    CAmplifier amplifier_d(phase_d);
    CAmplifier amplifier_e(phase_e);

    amplifier_a.set_child_amp(&amplifier_b);
    amplifier_b.set_child_amp(&amplifier_c);
    amplifier_c.set_child_amp(&amplifier_d);
    amplifier_d.set_child_amp(&amplifier_e);
    amplifier_e.set_child_amp(&amplifier_a);

    while (last_output == 0)
    {
        amplifier_a.run_program();
        amplifier_b.run_program();
        amplifier_c.run_program();
        amplifier_d.run_program();
        last_output = amplifier_e.run_program();
    }
    
    result = amplifier_a.get_last_input();

    return result;
}

int main()
{
    int64_t result = 0;

    const int no_phases = 5;
    int phase_combinations[no_phases] = {5,6,7,8,9};

    result = run_with_permutations(phase_combinations, no_phases);

    cout << "Result: " << result << "\n";

    cout << "------------------\n";
    cout << "program ends";

}