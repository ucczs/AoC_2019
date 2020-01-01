#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include <stdlib.h>
#include <Windows.h>

using namespace std;

#define NUMBER_OF_PILES 798

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
    bool indestructible_flag;
    ball_movement_t movement;
    tile_type_t tile_type;
    uint32_t target_x_pos;
    uint32_t target_y_pos;

public:
    CBasic_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type);
    ~CBasic_tile();
    tile_type_t get_tile_type();
    void set_tile_type(tile_type_t tile_type);
    uint32_t x_pos;
    uint32_t y_pos;

};



class CGame_map
{
private:
    vector<CBasic_tile> all_tiles;
    bool draw_flag;

    int16_t score;
    void add_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type);
    int32_t instructions[3];
    void sort_tiles();
    void reset_console_cursor();
    bool update_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type);
public:
    CGame_map();
    ~CGame_map();
    void add_instruction(int32_t instruction);
    int16_t count_element_types(tile_type_t type);
    void draw_map();
    int8_t get_joystick_position();
};


bool compareInterval(CBasic_tile tile1, CBasic_tile tile2) ;
