#include <iostream>
#include <vector>
#include <math.h>
#include <numeric> 

struct Moon
{
    int16_t x_pos;
    int16_t y_pos;
    int16_t z_pos;

    int16_t x_vel;
    int16_t y_vel;
    int16_t z_vel;
};

class CMoonSimulator
{
private:
    std::vector<Moon> all_moons;
    std::vector<Moon> initial_moon_state;
    
    int32_t total_energy;


    void calculate_velocity_x();
    void calculate_velocity_y();
    void calculate_velocity_z();

    void calculate_new_position_x();
    void calculate_new_position_y();
    void calculate_new_position_z();

public:
    CMoonSimulator(/* args */);
    ~CMoonSimulator();
    void perform_physic_step(uint16_t steps);
    void perform_physic_step_x_only(uint16_t steps);
    void perform_physic_step_y_only(uint16_t steps);
    void perform_physic_step_z_only(uint16_t steps);

    int32_t calculate_energy();
    void add_moon(int16_t x_pos, int16_t y_pos, int16_t z_pos);
    int32_t get_system_energy();

    bool check_x_initial();
    bool check_y_initial();
    bool check_z_initial();

    int32_t steps_x_initial;
    int32_t steps_y_initial;
    int32_t steps_z_initial;

    void save_initial_state();
    
};


