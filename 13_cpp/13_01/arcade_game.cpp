#include "arcade_game.h"

CBasic_tile::CBasic_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type)
{
    this->x_pos     = x_pos;
    this->y_pos     = y_pos;
    this->tile_type = tile_type;

    if (tile_type == BLOCK)
    {
        indestructible_flag = false;
    }
    else
    {
        indestructible_flag = true;
    }

    if (tile_type != BALL)
    {
        movement = NO_MOVEMENT;
    }
}

CBasic_tile::~CBasic_tile()
{
}


CGame_map::CGame_map(/* args */)
{
}

CGame_map::~CGame_map()
{
}

CGame_map::add_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type)
{
    CBasic_tile new_tile(x_pos, y_pos, tile_type);
    all_tiles.push_back(new_tile);
}