#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <math.h>

struct reactant_st;

class CChemical
{
private:

public:
    CChemical(std::string name);
    ~CChemical();
    void set_minimum_package(uint16_t minimum_package);
    std::string chemical_name;
    bool raw_material;
    std::vector<reactant_st> created_by_chem;
    uint64_t neccessary_amount;
    uint16_t minimum_package;
    uint16_t count_creates;
    uint16_t count_amount_collector;
};

class CReactionList_Reader
{
private:
    std::string file_name;
    void find_output_chemical(std::string input_line);
    void find_input_chemicals(std::string input_line);
    void extract_number_and_name(std::string input_line, uint16_t idx_number_start, uint16_t* extract_number, std::string* name);
    void count_neccessary_amount(CChemical* chemical);

public:
    CReactionList_Reader(std::string file_name);
    ~CReactionList_Reader();
    void read_reaction_list();
    void count_overall_amount();
};

struct reactant_st
{
    CChemical* reactant;
    uint16_t quantity;
};

CChemical* get_chemical(std::string name);


std::vector<CChemical> g_all_chemicals;
const uint64_t max_ore = 1'000'000'000'000;
