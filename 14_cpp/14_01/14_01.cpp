#include "14_01.h"

using namespace std;

CReactionList_Reader::CReactionList_Reader(string file_name)
{
    this->file_name = file_name;
    CChemical new_chemical("ORE");
    new_chemical.raw_material = true;
    new_chemical.set_minimum_package(1);
    g_all_chemicals.push_back(new_chemical);
}

CReactionList_Reader::~CReactionList_Reader()
{
}

void CReactionList_Reader::read_reaction_list()
{
    ifstream input_file_1(file_name);
    string line;
    while ( getline(input_file_1, line) )
    {
        find_output_chemical(line);
    }
    input_file_1.close();


    ifstream input_file_2(file_name);
    while ( getline(input_file_2, line) )
    {
        find_input_chemicals(line);
    }
    input_file_2.close() ;   

}

void CReactionList_Reader::extract_number_and_name(std::string input_line, uint16_t idx_number_start, uint16_t* extract_number, string* name)
{
    int16_t idx_number_end = input_line.find(" ", idx_number_start);
    uint8_t length_number = idx_number_end - idx_number_start;

    *extract_number = stoi(input_line.substr(idx_number_start, length_number));

    uint16_t idx_name_begin = idx_number_end + 1;

    int16_t idx_name_end = input_line.find(",", idx_name_begin);

    if( idx_name_end == -1 )
        idx_name_end = input_line.find(" ", idx_name_begin);

    if( idx_name_end == -1 )
        idx_name_end = input_line.find("\n", idx_name_begin);

    uint8_t length_name = idx_name_end - idx_name_begin;

    *name = input_line.substr(idx_number_end+1, length_name);
}

void CReactionList_Reader::find_output_chemical(string input_line)
{
    int16_t idx_number_start = input_line.find(">") + 2;

    uint16_t extracted_number;
    string extracted_name;
    extract_number_and_name(input_line, idx_number_start, &extracted_number, &extracted_name);

    CChemical new_chemical(extracted_name);
    new_chemical.set_minimum_package(extracted_number);

    g_all_chemicals.push_back(new_chemical);
}



void CReactionList_Reader::find_input_chemicals(string input_line)
{
    int16_t idx_number_start = input_line.find(">") + 2;

    uint16_t extracted_number;
    string output_chemical_name;
    extract_number_and_name(input_line, idx_number_start, &extracted_number, &output_chemical_name);
    CChemical* output_chemical = get_chemical(output_chemical_name);

    uint8_t count_reactants;
    count_reactants = std::count(input_line.begin(), input_line.end(), ',') + 1;

    int16_t current_idx = 0;

    for (uint8_t i = 0; i < count_reactants; i++)
    {
        uint16_t reactant_quantity;
        string reactant_name;
        extract_number_and_name(input_line, current_idx, &reactant_quantity, &reactant_name);
        reactant_st new_reactant;
        new_reactant.quantity = reactant_quantity;
        new_reactant.reactant = get_chemical(reactant_name);
        new_reactant.reactant->count_creates++;

        output_chemical->created_by_chem.push_back(new_reactant);

        current_idx = input_line.find(",", current_idx+1) + 2;
    }
    

}


CChemical::CChemical(string name)
{
    chemical_name = name;
    raw_material = false;
    neccessary_amount = 0;
    count_creates = 0;
    count_amount_collector = 0;
}

CChemical::~CChemical()
{
}

void CChemical::set_minimum_package(uint16_t minimum_package)
{
    this->minimum_package = minimum_package;
}

CChemical* get_chemical(std::string name)
{
    for (uint16_t i = 0; i < g_all_chemicals.size(); i++)
    {
        if ( name == g_all_chemicals[i].chemical_name )
        {
            return &g_all_chemicals[i];
        }
    }
    return nullptr;    
}

void CReactionList_Reader::count_neccessary_amount(CChemical* current_product)
{
    CChemical* current_reactant;
    for (uint8_t i = 0; i < current_product->created_by_chem.size(); i++)
    {
        current_reactant = current_product->created_by_chem[i].reactant;
        current_reactant->count_amount_collector++;

        current_reactant->neccessary_amount += current_product->neccessary_amount * current_product->created_by_chem[i].quantity / current_product->minimum_package;
        if( current_reactant->count_amount_collector == current_reactant->count_creates )
        {
            if( current_reactant->neccessary_amount % current_reactant->minimum_package != 0 )
            {
                current_reactant->neccessary_amount = ceil(current_reactant->neccessary_amount / (float)current_reactant->minimum_package) * current_reactant->minimum_package;
            }
            count_neccessary_amount(current_reactant);
        }
    }
}

void CReactionList_Reader::count_overall_amount()
{
    CChemical* fuel = get_chemical("FUEL");
    count_neccessary_amount(fuel);     
}


int main()
{
    CReactionList_Reader reaction_reader("input.txt");
    reaction_reader.read_reaction_list();

    CChemical* fuel = get_chemical("FUEL");
    fuel->neccessary_amount = 1;

    reaction_reader.count_overall_amount();

    cout << "\nScript ends...\n";

}