#include "16_01.h"

void creat_current_pattern(uint16_t calc_number)
{
    current_pattern.clear();
    for (uint8_t base_pattern_idx = 0; base_pattern_idx < length_base_pattern; base_pattern_idx++)
    {
        for (uint16_t i = 0; i < calc_number; i++)
        {
            current_pattern.push_back(base_pattern[base_pattern_idx]);
        }
    }
}

int main()
{
    output_all_phases.push_back(input);

    bool all_input_encode = false;

    uint16_t input_iterator = 0;
    uint16_t base_pattern_iterator = 0;
    uint8_t base_pattern_increase_cnt = 0;

    int32_t sum = 0;
    uint8_t output_number = 0;

    for (uint16_t cnt_phase = 0; cnt_phase < max_phases; cnt_phase++)
    {
        output_one_phase.clear();
        for (uint16_t calc_no_in_phase = 1; calc_no_in_phase <= size_input; calc_no_in_phase++)
        {
            creat_current_pattern(calc_no_in_phase);
            base_pattern_iterator = 1;
            
            while (input_iterator < size_input)
            {
                int8_t pattern_val = current_pattern[base_pattern_iterator];
                int8_t value = output_all_phases.back()[input_iterator];

                sum += current_pattern[base_pattern_iterator] * output_all_phases.back()[input_iterator];

                base_pattern_iterator++;
                if (base_pattern_iterator == 4 * calc_no_in_phase)
                    base_pattern_iterator = 0;
                
                input_iterator++;
            }
            output_number = abs(sum)%10;
            sum = 0;
            input_iterator = 0;
            output_one_phase.push_back(output_number);
        }
        output_all_phases.push_back(output_one_phase);
    }

    std::cout << "\nDone!\n";
}