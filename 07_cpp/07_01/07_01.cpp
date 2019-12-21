#include "07_01.h"

using namespace std;

class CAmplifier
{
private:
    int64_t ampl_program[size_prog] = {0};
    uint8_t phase_setting;
    uint32_t input;
    Opcode_t get_opcode(uint16_t instruction);
    uint8_t get_increase_size(uint16_t instruction);
    ParMode_t get_parameter_mode_first(uint16_t instruction);
    ParMode_t get_parameter_mode_second(uint16_t instruction);
    ParMode_t get_parameter_mode_third(uint16_t instruction);
    int64_t get_value_based_on_mode(ParMode_t paramode, uint32_t idx_instruction, uint8_t idx_offset);
    int64_t get_parameter_value(uint32_t instruction_idx, uint8_t parameter_number);
    uint8_t no_input;


public:
    CAmplifier(uint8_t phase_setting, uint32_t input);
    ~CAmplifier();
    void setInput(uint32_t input);
    void setPhase(uint8_t phase_setting);
    int64_t run_program();
    int32_t run_program(int32_t input, uint8_t phase);


};

CAmplifier::CAmplifier(uint8_t phase_setting, uint32_t input)
{
    copy(begin(g_diag_program_original), end(g_diag_program_original), begin(ampl_program) );
    this->phase_setting = phase_setting;
    this->input = input;
    no_input = 0;
}

CAmplifier::~CAmplifier()
{
}


Opcode_t CAmplifier::get_opcode(uint16_t instruction){
    Opcode_t operation = static_cast<Opcode_t>(instruction % 100);
    return(operation);
}

uint8_t CAmplifier::get_increase_size(uint16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_first(uint16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_second(uint16_t instruction){
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

ParMode_t CAmplifier::get_parameter_mode_third(uint16_t instruction){
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

int64_t CAmplifier::get_value_based_on_mode(ParMode_t paramode, uint32_t idx_instruction, uint8_t idx_offset){
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

int64_t CAmplifier::get_parameter_value(uint32_t instruction_idx, uint8_t parameter_number){
    uint16_t instruction        = ampl_program[instruction_idx];
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

int64_t CAmplifier::run_program(){
    uint32_t idx                    = 0;
    uint32_t new_idx_offset         = 0;
    uint64_t current_instruction    = ampl_program[0];
    Opcode_t current_op_code        = get_opcode(current_instruction);
    bool idx_update                 = true;

    int64_t output;

    while (current_op_code != op_HALT)
    {
        if (current_op_code == op_ADD)
        {
            uint64_t add_1 = get_parameter_value(idx, 1);
            uint64_t add_2 = get_parameter_value(idx, 2);
            ampl_program[ampl_program[idx + 3]] = add_1 + add_2;
            idx_update = true;
        } else if (current_op_code == op_MULTIPLY)
        {
            uint64_t multiplicator_1 = get_parameter_value(idx, 1);
            uint64_t multiplicator_2 = get_parameter_value(idx, 2);
            ampl_program[ampl_program[idx + 3]] = multiplicator_1 * multiplicator_2;
            idx_update = true;
        }
        else if (current_op_code == op_SAVE_VAL)
        {
            if (no_input == 0)
            {
                ampl_program[ampl_program[idx + 1]] = phase_setting;
                no_input++;
            }
            else
            {
                ampl_program[ampl_program[idx + 1]] = input;
                no_input++;
            }
            idx_update = true;
        }
        else if (current_op_code == op_OUTPUT_VAL)
        {
            output = get_parameter_value(idx, 1);
            //cout << "------------------\n";
            //cout << "Index: " << idx << "\n";
            //cout << "Output: " << output << "\n\n";

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
            uint64_t compare_1 = get_parameter_value(idx, 1);
            uint64_t compare_2 = get_parameter_value(idx, 2);
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

    return(output);
}

uint32_t run_with_permutations(int* phase_combination, int no_phases, int first_element=0, uint32_t maximum=0)
{
    uint32_t temp_result = 0;

    if(first_element == no_phases)
    {
        temp_result = run_all_amplifiers(phase_combination);
        if (temp_result > maximum)
        {
            maximum = temp_result;
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

uint32_t run_all_amplifiers(int* phase_combination)
{
    uint32_t input_a = 0;

    uint8_t phase_a = *phase_combination;
    uint8_t phase_b = *(phase_combination+1);
    uint8_t phase_c = *(phase_combination+2);
    uint8_t phase_d = *(phase_combination+3);
    uint8_t phase_e = *(phase_combination+4);

    CAmplifier amplifier_a(phase_a, input_a);
    uint32_t input_b = amplifier_a.run_program();

    CAmplifier amplifier_b(phase_b, input_b);
    uint32_t input_c = amplifier_b.run_program();

    CAmplifier amplifier_c(phase_c, input_c);
    uint32_t input_d = amplifier_c.run_program();

    CAmplifier amplifier_d(phase_d, input_d);
    uint32_t input_e = amplifier_d.run_program();

    CAmplifier amplifier_e(phase_e, input_e);
    uint32_t result = amplifier_e.run_program();

    return result;
}

int main()
{
    uint32_t result = 0;

    const int no_phases = 5;
    int phase_combinations[no_phases] = {0,1,2,3,4};

    result = run_with_permutations(phase_combinations, no_phases);

    cout << "Result: " << result << "\n";

    cout << "------------------\n";
    cout << "program ends";

}