#include <iostream>
#include <vector>

using namespace std;

enum ball_movement_t {
    NO_MOVEMENT = 0,
    NORTH_EAST_MOVEMENT = 1,
    SOUTH_EAST_MOVEMENT = 2,
    SOUT_WEST_MOVEMENT = 3,
    NORTH_WEST_MOVEMENT = 4
};

enum tile_type_t {
    EMPTY = 0,
    WALL = 1,
    BLOCK = 2,
    PADDLE_HORIZONTAL = 3,
    BALL = 4
};

class CBasic_tile
{
private:
    uint32_t x_pos;
    uint32_t y_pos;
    bool indestructible_flag;
    ball_movement_t movement;
    tile_type_t tile_type;

public:
    CBasic_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type);
    ~CBasic_tile();
};



class CGame_map
{
private:
    vector<CBasic_tile> all_tiles;
    int16_t x_max;
    int16_t y_max;
    int16_t x_min;
    int16_t y_min;

public:
    CGame_map(/* args */);
    ~CGame_map();
    add_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type);
};


