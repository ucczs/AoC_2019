#include "12_02.h"

using namespace std;

CMoonSimulator::CMoonSimulator()
{
    total_energy = -1;
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

void CMoonSimulator::calculate_velocity_x()
{
    for(int j=0;j<all_moons.size();j++)
    {
        for(int k=0;k<all_moons.size();k++)
        {
            uint16_t vel_counter = 0;
            if(j==k) continue;

            if(all_moons[j].x_pos > all_moons[k].x_pos) all_moons[j].x_vel--;
            else if(all_moons[j].x_pos < all_moons[k].x_pos) all_moons[j].x_vel++;
        }
    }
}

void CMoonSimulator::calculate_velocity_y()
{
    for(int j=0;j<all_moons.size();j++)
    {
        for(int k=0;k<all_moons.size();k++)
        {
            uint16_t vel_counter = 0;
            if(j==k) continue;

            if(all_moons[j].y_pos > all_moons[k].y_pos) all_moons[j].y_vel--;
            else if(all_moons[j].y_pos < all_moons[k].y_pos) all_moons[j].y_vel++;
        }
    }
}

void CMoonSimulator::calculate_velocity_z()
{
    for(int j=0;j<all_moons.size();j++)
    {
        for(int k=0;k<all_moons.size();k++)
        {
            uint16_t vel_counter = 0;
            if(j==k) continue;

            if(all_moons[j].z_pos > all_moons[k].z_pos) all_moons[j].z_vel--;
            else if(all_moons[j].z_pos < all_moons[k].z_pos) all_moons[j].z_vel++;
        }
    }
}

void CMoonSimulator::calculate_new_position_x()
{
    for (int i=0; i<all_moons.size(); i++)
    {
        all_moons[i].x_pos += all_moons[i].x_vel;
    }
}

void CMoonSimulator::calculate_new_position_y()
{
    for (int i=0; i<all_moons.size(); i++)
    {
        all_moons[i].y_pos += all_moons[i].y_vel;
    }
}

void CMoonSimulator::calculate_new_position_z()
{
    for (int i=0; i<all_moons.size(); i++)
    {
        all_moons[i].z_pos += all_moons[i].z_vel;
    }
}

void CMoonSimulator::perform_physic_step(uint16_t steps)
{
    for(int i = 0; i<steps;i++)
    {
        calculate_velocity_x();
        calculate_velocity_y();
        calculate_velocity_z();
        
        calculate_new_position_x();
        calculate_new_position_y();
        calculate_new_position_z();
    }
}

void CMoonSimulator::perform_physic_step_x_only(uint16_t steps)
{
    for(int i = 0; i<steps;i++)
    {
        calculate_velocity_x();
        calculate_new_position_x();
    }
}  

void CMoonSimulator::perform_physic_step_y_only(uint16_t steps)
{
    for(int i = 0; i<steps;i++)
    {
        calculate_velocity_y();
        calculate_new_position_y();
    }
}  

void CMoonSimulator::perform_physic_step_z_only(uint16_t steps)
{
    for(int i = 0; i<steps;i++)
    {
        calculate_velocity_z();
        calculate_new_position_z();
    }
}  

int32_t CMoonSimulator::calculate_energy()
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

    return total_energy;
}

int32_t CMoonSimulator::get_system_energy()
{
    return total_energy;
}

bool CMoonSimulator::check_x_initial()
{
    for (uint16_t i = 0; i < all_moons.size(); i++)
    {
        if (all_moons[i].x_vel != initial_moon_state[i].x_vel || all_moons[i].x_pos != initial_moon_state[i].x_pos)
        {
            return false;
        }
        
    }
    return true;   
}

bool CMoonSimulator::check_y_initial()
{
    for (uint16_t i = 0; i < all_moons.size(); i++)
    {
        if (all_moons[i].y_vel != initial_moon_state[i].y_vel || all_moons[i].y_pos != initial_moon_state[i].y_pos)
        {
            return false;
        }
        
    }
    return true;   
}

bool CMoonSimulator::check_z_initial()
{
    for (uint16_t i = 0; i < all_moons.size(); i++)
    {
        if (all_moons[i].z_vel != initial_moon_state[i].z_vel || all_moons[i].z_pos != initial_moon_state[i].z_pos)
        {
            return false;
        }
        
    }
    return true;   
}

void set_moons_test1(CMoonSimulator* moonSimulator)
{
    moonSimulator->add_moon(-1,0,2);
    moonSimulator->add_moon(2,-10,-7);
    moonSimulator->add_moon(4,-8,8);
    moonSimulator->add_moon(3,5,-1);
}

void set_moons_test2(CMoonSimulator* moonSimulator)
{
    moonSimulator->add_moon(-8,-10,0);
    moonSimulator->add_moon(5,5,10);
    moonSimulator->add_moon(2,-7,3);
    moonSimulator->add_moon(9,-8,-3);
}

void set_moons(CMoonSimulator* moonSimulator)
{
    moonSimulator->add_moon(-2,9,-5);
    moonSimulator->add_moon(16,19,9);
    moonSimulator->add_moon(0,3,6);
    moonSimulator->add_moon(11,0,11);
}

void CMoonSimulator::save_initial_state()
{
    copy(all_moons.begin(), all_moons.end(), back_inserter(initial_moon_state));
}

uint64_t least_common_mulitple(uint64_t a, uint64_t b) 
{
    uint64_t lcm;

    if( a>b ) lcm = a;
    else lcm = b;

    while(1) 
    {
        if( lcm % a == 0 && lcm % b == 0 ) 
        {
            cout<<"The LCM of "<<a<<" and "<<b<<" is "<<lcm << endl;
            break;
        }
        lcm++;
    }
    return lcm;
}

int main()
{
    CMoonSimulator moonSimulator;
    set_moons(&moonSimulator);
    
    // set_moons_test1(&moonSimulator);
    // set_moons_test2(&moonSimulator);

    uint64_t step_counter = 0;
    moonSimulator.save_initial_state();

    do
    {
        moonSimulator.perform_physic_step_x_only(1);
        step_counter++;
    } while (!moonSimulator.check_x_initial() );
    
    cout << "Took " << step_counter << " steps for x initial\n";
    moonSimulator.steps_x_initial = step_counter;

    step_counter = 0;
    do
    {
        moonSimulator.perform_physic_step_y_only(1);
        step_counter++;
    } while (!moonSimulator.check_y_initial() );
    
    cout << "Took " << step_counter << " steps for y initial\n";
    moonSimulator.steps_y_initial = step_counter;

    step_counter = 0;
    do
    {
        moonSimulator.perform_physic_step_z_only(1);
        step_counter++;
    } while (!moonSimulator.check_z_initial() );
    
    cout << "Took " << step_counter << " steps for z initial\n";
    moonSimulator.steps_z_initial = step_counter;

    uint64_t lcm_1 = least_common_mulitple(moonSimulator.steps_x_initial, moonSimulator.steps_z_initial);
    uint64_t lcm_2 = least_common_mulitple(lcm_1, moonSimulator.steps_y_initial);

    cout << "Initial state is reached at step " << lcm_2 << "\n";
}

// <x=-2, y=9, z=-5>
// <x=16, y=19, z=9>
// <x=0, y=3, z=6>
// <x=11, y=0, z=11>