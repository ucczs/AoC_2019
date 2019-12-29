#include <iostream>
#include <vector>

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
    int32_t total_energy;
    void calculate_velocity();
    void calculate_new_position();

public:
    CMoonSimulator(/* args */);
    ~CMoonSimulator();
    void perform_physic_step(uint16_t steps);
    void calculate_energy();
    void add_moon(int16_t x_pos, int16_t y_pos, int16_t z_pos);
    int32_t get_system_energy();
};


