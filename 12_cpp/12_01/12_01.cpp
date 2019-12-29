#include "12_01.h"

using namespace std;

CMoonSimulator::CMoonSimulator()
{
}

CMoonSimulator::~CMoonSimulator()
{
}

void CMoonSimulator::add_moon(int16_t x_pos, int16_t y_pos, int16_t z_pos)
{
    Moon new_moon;
    new_moon.x_pos = x_pos;
    new_moon.y_pos = y_pos;
    new_moon.z_pos = z_pos;

    new_moon.x_vel = 0;
    new_moon.y_vel = 0;
    new_moon.z_vel = 0;

    all_moons.push_back(new_moon);
}

void CMoonSimulator::calculate_velocity()
{
    for(int j=0;j<all_moons.size();j++)
    {
        for(int k=0;k<all_moons.size();k++)
        {
            uint16_t vel_counter = 0;
            if(j==k) continue;

            if(all_moons[j].x_pos > all_moons[k].x_pos) all_moons[j].x_vel--;
            else if(all_moons[j].x_pos < all_moons[k].x_pos) all_moons[j].x_vel++;

            if(all_moons[j].y_pos > all_moons[k].y_pos) all_moons[j].y_vel--;
            else if(all_moons[j].y_pos < all_moons[k].y_pos) all_moons[j].y_vel++;

            if(all_moons[j].z_pos > all_moons[k].z_pos) all_moons[j].z_vel--;
            else if(all_moons[j].z_pos < all_moons[k].z_pos) all_moons[j].z_vel++;
        }
    }
}

void CMoonSimulator::calculate_new_position()
{
    for (int i=0; i<all_moons.size(); i++)
    {
        all_moons[i].x_pos += all_moons[i].x_vel;
        all_moons[i].y_pos += all_moons[i].y_vel;
        all_moons[i].z_pos += all_moons[i].z_vel;
    }
}

void CMoonSimulator::perform_physic_step(uint16_t steps)
{
    for(int i = 0; i<steps;i++)
    {
        calculate_velocity();
        calculate_new_position();
    }
}
    

void CMoonSimulator::calculate_energy()
{
    total_energy = 0;
    int32_t kin_energy = 0;
    int32_t pot_energy = 0;

    for (int i=0; i<all_moons.size(); i++)
    {
        kin_energy = abs(all_moons[i].x_vel) + abs(all_moons[i].y_vel) + abs(all_moons[i].z_vel);
        pot_energy = abs(all_moons[i].x_pos) + abs(all_moons[i].y_pos) + abs(all_moons[i].z_pos);
        total_energy = total_energy + kin_energy*pot_energy;
    }
}

int32_t CMoonSimulator::get_system_energy()
{
    return total_energy;
}


int main()
{
    CMoonSimulator moonSimulator;
    moonSimulator.add_moon(-2,9,-5);
    moonSimulator.add_moon(16,19,9);
    moonSimulator.add_moon(0,3,6);
    moonSimulator.add_moon(11,0,11);

    moonSimulator.perform_physic_step(1000);
    moonSimulator.calculate_energy();

    cout << "System energy is " << moonSimulator.get_system_energy() << endl;
}

// <x=-2, y=9, z=-5>
// <x=16, y=19, z=9>
// <x=0, y=3, z=6>
// <x=11, y=0, z=11>