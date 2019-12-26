#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include <bits/stdc++.h>

enum posCheck_t : bool { 
    ASTEROID_FOUND = true,
    NO_ASTEROID_FOUND = false
};

enum visibleCheck_t : bool {
    ASTEROID_VISIBLE = true,
    ASTEROID_NOT_VISIBLE = false
};

enum quadrant_position_t  {
                UNDEFINED_QUADRANT  = 0, 
                FIRST_QUADRANT      = 1, 
                SECOND_QUADRANT     = 2, 
                THIRD_QUADRANT      = 3, 
                FORTH_QUADRANT      = 4}; 

class CAsteroids
{
private:

    uint16_t visible_asteroids;
    posCheck_t check_position(uint16_t x_pos_test, uint16_t y_pos_test);
    visibleCheck_t check_asteroid_in_x_pos_range(uint16_t x_pos_target_asteroid, uint16_t y_pos_target_asteroid, float view_direction);

    // create visible list with pointer to asteroid
public:
    CAsteroids(uint16_t x_pos, uint16_t y_pos);
    ~CAsteroids();
    void check_visibility_asteroids();
    uint16_t get_visible_asteroids();
    bool base_flag;
    bool vaporized_flag;
    void get_position(uint16_t* x_pos, uint16_t* y_pos);
    void calculate_base_relations(uint16_t x_pos_base, uint16_t y_pos_base);
    float angle_to_base;
    quadrant_position_t quadrant_base;
    void define_base_quadrant(uint16_t x_pos_base, uint16_t y_pos_base);
    float distance_to_base;


    uint16_t x_pos;
    uint16_t y_pos;
};

std::vector<CAsteroids> g_all_asteroids;

void read_in_map(std::string filename);
uint16_t get_maximum_views();
void fill_base_relations();
void vaporize_asteroids();
bool compare_asteroids(CAsteroids asteroid_1, CAsteroids asteroid_2);
void remove_vaporized_asteroids();

