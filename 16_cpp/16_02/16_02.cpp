#include "16_02.h"

void create_input_list_repeated(uint16_t repetition)
{
    input.clear();
    for (uint16_t i = 0; i < repetition; i++)
    {
        for (uint16_t i = 0; i < SIZE_INPUT_SINGLE; i++)
        {
            input.push_back(input_single[i]);
        }
    }
}

void creat_current_pattern(uint16_t calc_number)
{
    g_current_pattern.clear();
    for (uint8_t base_pattern_idx = 0; base_pattern_idx < LENGTH_BASE_PATTERN; base_pattern_idx++)
    {
        for (uint16_t i = 0; i < calc_number; i++)
        {
            g_current_pattern.push_back(base_pattern[base_pattern_idx]);
        }
    }
    g_length_pattern = g_current_pattern.size();
}

uint64_t get_idx_output()
{
    uint64_t idx = 0;
    idx += input_single[0] * 1'000'000;
    idx += input_single[1] * 100'000;
    idx += input_single[2] * 10'000;
    idx += input_single[3] * 1'000;
    idx += input_single[4] * 100;
    idx += input_single[5] * 10;
    idx += input_single[6] * 1;

    return idx;
}

uint64_t find_repetition_number()
{
    for (uint32_t i = 1; i <= INPUT_REPETITION; i++)
    {
        if ( (i*SIZE_INPUT_SINGLE) % g_length_pattern == 0 && INPUT_REPETITION % i == 0)
        {
            return i;
        }
    }
    return INPUT_REPETITION;
}

int main()
{
    create_input_list_repeated(INPUT_REPETITION);
    g_output_before = input;

    bool all_input_encode = false;

    uint64_t input_iterator = 0;
    uint64_t base_pattern_iterator = 0;

    int32_t sum = 0;
    uint8_t output_number = 0;

    for (uint16_t cnt_phase = 0; cnt_phase < MAX_PHASES; cnt_phase++)
    {
        g_output_one_phase.clear();
        for (uint64_t calc_no_in_phase = 1; calc_no_in_phase <= SIZE_INPUT; calc_no_in_phase++)
        {
            creat_current_pattern(calc_no_in_phase);
            g_calc_repetition = find_repetition_number();
            base_pattern_iterator = calc_no_in_phase;
            
            while (input_iterator < g_calc_repetition * SIZE_INPUT_SINGLE)
            {
                int8_t pattern_val = g_current_pattern[base_pattern_iterator];
                int8_t value = g_output_before[input_iterator];

                sum += g_current_pattern[base_pattern_iterator] * g_output_before[input_iterator];

                base_pattern_iterator++;
                if (base_pattern_iterator == 4 * calc_no_in_phase)
                    base_pattern_iterator = 0;
                
                input_iterator++;
            }
            sum = sum * (INPUT_REPETITION / g_calc_repetition);
            output_number = abs(sum)%10;
            sum = 0;
            input_iterator = 0;
            g_output_one_phase.push_back(output_number);

            if (calc_no_in_phase % 100 == 0)
            {
                std::cout << "Calculation " << calc_no_in_phase << " with currently " << g_calc_repetition << " number of calcs and  in Phase no " << cnt_phase << std::endl;
            }
            
        }
        g_output_before = g_output_one_phase;

        std::cout << "Phase no: " << cnt_phase << std::endl;
    }

    uint64_t idx_output = get_idx_output();

    std::cout << "Result digits are: " << std::endl;
    std::cout << g_output_before[idx_output + 1] << ", ";
    std::cout << g_output_before[idx_output + 2] << ", ";
    std::cout << g_output_before[idx_output + 3] << ", ";
    std::cout << g_output_before[idx_output + 4] << ", ";
    std::cout << g_output_before[idx_output + 5] << ", ";
    std::cout << g_output_before[idx_output + 6] << ", ";
    std::cout << g_output_before[idx_output + 7] << std::endl;

    std::cout << "\nDone!\n";
}