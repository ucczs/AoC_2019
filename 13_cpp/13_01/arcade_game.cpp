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
    else
    {
        movement = SOUTH_EAST_MOVEMENT;
    }
    
}

CBasic_tile::~CBasic_tile()
{
}

tile_type_t CBasic_tile::get_tile_type()
{
    return tile_type;
}

void CBasic_tile::set_tile_type(tile_type_t tile_type)
{
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

CGame_map::CGame_map()
{
    std::fill( instructions, instructions + sizeof( instructions ), -2 );
    score = 0;
    draw_flag = false;

}

CGame_map::~CGame_map()
{
}

bool CGame_map::update_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type)
{
    for (uint16_t i = 0; i < all_tiles.size(); i++)
    {
        if( all_tiles[i].x_pos == x_pos && all_tiles[i].y_pos == y_pos)
        {
            all_tiles[i].set_tile_type(tile_type);
            return true;
        }
    }
    return false;
}

void CGame_map::add_tile(uint32_t x_pos, uint32_t y_pos, tile_type_t tile_type)
{
    if( !update_tile(x_pos, y_pos, tile_type) )
    {
        CBasic_tile new_tile(x_pos, y_pos, tile_type);
        all_tiles.push_back(new_tile);
    }
}

void CGame_map::add_instruction(int32_t instruction)
{
    for (int8_t i = 0; i < sizeof(instructions)/sizeof(instructions[0]); i++)
    {
        if (instructions[i] != -2)
        {
            continue;
        }
        else
        {
            instructions[i] = instruction;
            break;
        }
    }

    if (instructions[ sizeof(instructions) / sizeof(instructions[0])-1 ] != -2)
    {
        if ( instructions[0] != -1 )
        {
            add_tile(instructions[0], instructions[1], (tile_type_t)instructions[2]);
            if ( all_tiles.size() == NUMBER_OF_PILES )
            {
                draw_flag = true;
            }

            std::fill( instructions, instructions + sizeof( instructions ), -2 );

            if (draw_flag == true)
            {
                //draw_map();
                //Sleep(50);
            }
        }
        // score update
        else if ( instructions[0] == -1 && instructions[1] == 0 )
        {
            score = instructions[2];
            std::fill( instructions, instructions + sizeof( instructions ), -2 );
        }
    }
}

int16_t CGame_map::count_element_types(tile_type_t count_type)
{
    uint16_t counter = 0;
    for (uint16_t i = 0; i < all_tiles.size(); i++)
    {
        if ( all_tiles[i].get_tile_type() == count_type )
        {
            counter++;
        }
        
    }
    
    return counter;
}


void CGame_map::sort_tiles()
{
    std::sort(all_tiles.begin(), all_tiles.end(), compareInterval);
}

void CGame_map::reset_console_cursor()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
    
}

int8_t CGame_map::get_joystick_position()
{
    uint8_t x_pos_ball;
    uint8_t x_pos_paddle;

    for (uint16_t i = 0; i < all_tiles.size(); i++)
    {
        if( all_tiles[i].get_tile_type() == BALL)
        {
            x_pos_ball = all_tiles[i].x_pos;
        }
        else if (all_tiles[i].get_tile_type() == PADDLE_HORIZONTAL )
        {
            x_pos_paddle = all_tiles[i].x_pos;
        }
    }

    if( x_pos_ball-x_pos_paddle < 0 )
        return -1;
    else if( x_pos_ball-x_pos_paddle > 0 )
        return 1;
    else return 0;

}


void CGame_map::draw_map()
{
    sort_tiles();
    reset_console_cursor();

    for (int32_t i = 0; i < all_tiles.size(); i++)
    {
        if (i == 0 || all_tiles[i].y_pos != all_tiles[i-1].y_pos)
        {
            cout << endl;
        }

        switch ( all_tiles[i].get_tile_type() )
        {
        case EMPTY:
            cout << " ";
            break;
        case WALL:
            cout << "|";
            break;
        case BLOCK:
            cout << "X";
            break;
        case PADDLE_HORIZONTAL:
            cout << "=";
            break;
        case BALL:
            cout << "O";
            break;
        default:
            break;
        }       
    }
    cout << endl << endl << endl;
    cout << "Score: " << score << endl;

}

bool compareInterval(CBasic_tile tile1, CBasic_tile tile2) 
{ 
    if (tile1.y_pos < tile2.y_pos)
    {
        return true;
    }
    else if (tile1.y_pos > tile2.y_pos)
    {
        return false;
    }
    else if (tile1.x_pos < tile2.x_pos)
    {
        return true;
    }
    else return false;
} 