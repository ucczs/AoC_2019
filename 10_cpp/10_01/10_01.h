#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

void read_in_map(std::string filename);
uint16_t get_maximum_views();

enum posCheck_t : bool { 
    ASTEROID_FOUND = true,
    NO_ASTEROID_FOUND = false
};

enum visibleCheck_t : bool {
    ASTEROID_VISIBLE = true,
    ASTEROID_NOT_VISIBLE = false
};

class CAsteroids
{
private:
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t visible_asteroids;
    posCheck_t check_position(uint16_t x_pos_test, uint16_t y_pos_test);
    visibleCheck_t check_asteroid_in_x_pos_range(uint16_t x_pos_target_asteroid, uint16_t y_pos_target_asteroid, float view_direction);

    // create visible list with pointer to asteroid
public:
    CAsteroids(uint16_t x_pos, uint16_t y_pos);
    ~CAsteroids();
    void check_visibility_asteroids();
    uint16_t get_visible_asteroids();
};

std::vector<CAsteroids> g_all_asteroids;
