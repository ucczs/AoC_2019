#include "repair_droid.h"


CRepair_droid::CRepair_droid()
{
    position_info starting_pos;
    starting_pos.x_pos = 0;
    starting_pos.y_pos = 0;
    starting_pos.pos_type = EMPTY;
    starting_pos.all_neightbours_flooded = false;
    starting_pos.is_flooded = false;
    starting_pos.walk_back_initial_way = false;
    starting_pos.just_flooded = false;
    map.push_back(starting_pos);

    current_x_pos = 0;
    current_y_pos = 0;
    current_direction = NORTH_DIRECTION;
    last_type = EMPTY;
    all_areas_flooded = false;
    time_to_flood = 0;
    

    map_info_str.max_x_coordinate = 0;
    map_info_str.max_y_coordinate = 0;
    map_info_str.min_x_coordinate = 0;
    map_info_str.min_y_coordinate = 0;

    steps_from_zero = 0;
}

CRepair_droid::~CRepair_droid()
{
}

void CRepair_droid::get_position_depending_on_direction(move_direction_t move_dir, int32_t* x_pos, int32_t* y_pos)
{
    switch (move_dir)
    {
    case NORTH_DIRECTION:
        (*y_pos)++;
        break;
    case SOUTH_DIRECTION:
        (*y_pos)--;
        break;
    case WEST_DIRECTION:
        (*x_pos)--;
        break;
    case EAST_DIRECTION:
        (*x_pos)++;
        break;                                                
    }
}

bool CRepair_droid::check_if_direction_was_used(move_direction_t move_dir, position_info* current_element)
{
    for (uint16_t i = 0; i < current_element->checked_directions.size(); i++)
    {
        if (current_element->checked_directions[i] == move_dir)
        {
            return true;
        }
    }
    return false;
}


move_direction_t CRepair_droid::generate_newest_output()
{
    position_info* current_element = get_current_element(current_x_pos, current_y_pos);


    if ( last_type != WALL && !check_if_direction_was_used(current_direction, current_element) )
    {
        return current_direction;
    }
    else
    {
        switch ( current_direction )
        {
        case NORTH_DIRECTION:
            current_direction = SOUTH_DIRECTION;
            if( !check_if_direction_was_used(current_direction, current_element) )
                break;
        case SOUTH_DIRECTION:
            current_direction = WEST_DIRECTION;
            if( !check_if_direction_was_used(current_direction, current_element) )
                break;
        case WEST_DIRECTION:
            current_direction = EAST_DIRECTION;
            if( !check_if_direction_was_used(current_direction, current_element) )
                break;
        case EAST_DIRECTION:
            current_direction = NORTH_DIRECTION;
            if( !check_if_direction_was_used(current_direction, current_element) )
                break;
        default:
            current_direction = current_element->initially_come_from;
            current_element->walk_back_initial_way = true;
        }
        return current_direction;
    }
}

bool CRepair_droid::check_if_coordiantes_exists(int32_t x_pos, int32_t y_pos)
{
    for (uint32_t i = 0; i < map.size(); i++)
    {
        if (map[i].x_pos == x_pos && map[i].y_pos == y_pos)
        {
            return true;
        }
    }
    return false;
}

position_info* CRepair_droid::get_current_element(int32_t x_pos, int32_t y_pos)
{
    for (uint32_t i = 0; i < map.size(); i++)
    {
        if (map[i].x_pos == x_pos && map[i].y_pos == y_pos)
        {
            return &map[i];
        }
    }
    return nullptr;
}

move_direction_t CRepair_droid::get_inverse_direction(move_direction_t direction)
{
    switch (direction)
    {
    case NORTH_DIRECTION:
        return SOUTH_DIRECTION;
        break;
    case SOUTH_DIRECTION:
        return NORTH_DIRECTION;
        break;
    case WEST_DIRECTION:
        return EAST_DIRECTION;
        break;
    case EAST_DIRECTION:
        return WEST_DIRECTION;
        break;
    }
    return UNDEFINED_DIRECTION;
}

void CRepair_droid::determine_map_borders()
{
    for (uint16_t i = 0; i < map.size(); i++)
    {
        if ( map[i].x_pos + 1 > map_info_str.max_x_coordinate)
            map_info_str.max_x_coordinate = map[i].x_pos + 1;

        if ( map[i].y_pos + 1 > map_info_str.max_y_coordinate)
            map_info_str.max_y_coordinate = map[i].y_pos + 1;

        if ( map[i].x_pos - 1 < map_info_str.min_x_coordinate)
            map_info_str.min_x_coordinate = map[i].x_pos - 1;

        if ( map[i].y_pos - 1 < map_info_str.min_y_coordinate)
            map_info_str.min_y_coordinate = map[i].y_pos - 1;
    }
}

void CRepair_droid::draw_map()
{
    determine_map_borders();
    std::cout << std::endl << std::endl << std::endl;
    uint16_t range_x = map_info_str.max_x_coordinate - map_info_str.min_x_coordinate;
    uint16_t range_y = map_info_str.max_y_coordinate - map_info_str.min_y_coordinate;

    for (uint16_t y = 0; y < range_y+1; y++)
    {
        for (uint16_t x = 0; x < range_x+1; x++)
        {
            position_info* current_coordinates = get_current_element(x + map_info_str.min_x_coordinate, y + map_info_str.min_y_coordinate);
            if (current_coordinates == nullptr)
                std::cout << " ";
            else if (current_coordinates->x_pos == 0 && current_coordinates->y_pos == 0)
            {
                std::cout << "U";
            }
            else if ( current_coordinates->x_pos == current_x_pos && current_coordinates->y_pos == current_y_pos)
            {
                std::cout << "D";
            }
            else if (current_coordinates->pos_type == WALL)
            {
                std::cout << "#";
            }
            else if (current_coordinates->pos_type == EMPTY)
            {
                if (current_coordinates->is_flooded)
                    std::cout << "O";
                else
                    std::cout << ".";
            }
            else if (current_coordinates->pos_type == OXYGEN_SYSTEM)
            {
                std::cout << "O";
            }
        }
        std::cout << std::endl;
    }
}

void CRepair_droid::count_steps()
{
    for (uint16_t i = 0; i < map.size(); i++)
    {
        if (map[i].pos_type == EMPTY && !map[i].walk_back_initial_way)
        {
            steps_from_zero++;
        }
    }
}

position_info* CRepair_droid::get_neightbour(position_info* oxygen_position, move_direction_t direction)
{
    switch (direction)
    {
    case NORTH_DIRECTION:
        return get_current_element(oxygen_position->x_pos, oxygen_position->y_pos + 1);
        break;
    case SOUTH_DIRECTION:
        return get_current_element(oxygen_position->x_pos, oxygen_position->y_pos - 1);
        break;
    case WEST_DIRECTION:
        return get_current_element(oxygen_position->x_pos - 1, oxygen_position->y_pos);
        break;
    case EAST_DIRECTION:
        return get_current_element(oxygen_position->x_pos + 1, oxygen_position->y_pos);
        break;
    }
    return nullptr;
}

void CRepair_droid::flood_neightbours(position_info* oxygen_position)
{
    for (uint8_t i = 0; i < COUNT_DIRECTIONS; i++)
    {
        position_info* neightbour = get_neightbour( oxygen_position, static_cast<move_direction_t>(i + 1) );
        if (neightbour != nullptr && neightbour->pos_type == EMPTY && !neightbour->is_flooded)
        {
            neightbour->is_flooded = true;
            neightbour->just_flooded = true;
        }
    }
    oxygen_position->all_neightbours_flooded = true;
}

void CRepair_droid::check_if_all_areas_flooded()
{
    all_areas_flooded = true;
    for (uint16_t i = 0; i < map.size(); i++)
    {
        map[i].just_flooded = false;
        if (map[i].pos_type == EMPTY && map[i].is_flooded == false)
        {
            all_areas_flooded = false;
        }
    }
}

void CRepair_droid::start_oxygen_source()
{
    for (uint16_t i = 0; i < map.size(); i++)
    {
        if (map[i].pos_type == OXYGEN_SYSTEM)
        {
            map[i].is_flooded = true;
            break;
        }
    } 
}

void CRepair_droid::flood_area_with_oxygen()
{
    start_oxygen_source();

    while (!all_areas_flooded)
    {
        for (uint16_t i = 0; i < map.size(); i++)
        {
            if (map[i].is_flooded && !map[i].all_neightbours_flooded && !map[i].just_flooded)
            {
                flood_neightbours(&map[i]);
            }
        }
        time_to_flood++;
        check_if_all_areas_flooded();
        //draw_map();
    }
}


void CRepair_droid::process_newest_input(position_type_t pos_type)
{
    position_info* current_element = get_current_element(current_x_pos, current_y_pos);
    current_element->checked_directions.push_back(current_direction);

    position_info new_element;
    int32_t dir_x = current_x_pos;
    int32_t dir_y = current_y_pos;
    get_position_depending_on_direction(current_direction, &dir_x, &dir_y);
    new_element.x_pos = dir_x;
    new_element.y_pos = dir_y;
    new_element.walk_back_initial_way = false;
    new_element.is_flooded = false;
    new_element.all_neightbours_flooded = false;

    last_type = pos_type;
    if ( !check_if_coordiantes_exists(dir_x, dir_y) )
    {
        switch (pos_type)
        {
        case WALL:
            new_element.pos_type = WALL;
            map.push_back(new_element);
            break;

        case EMPTY:
            new_element.pos_type = EMPTY;
            new_element.initially_come_from = get_inverse_direction(current_direction);
            new_element.checked_directions.push_back(new_element.initially_come_from);
            map.push_back(new_element);
            break;

        case OXYGEN_SYSTEM:
            new_element.pos_type = OXYGEN_SYSTEM;
            new_element.initially_come_from = get_inverse_direction(current_direction);
            new_element.checked_directions.push_back(new_element.initially_come_from);
            map.push_back(new_element);
            count_steps();
            draw_map();
            break;
        }
    }
    
    if( pos_type == EMPTY || pos_type == OXYGEN_SYSTEM )
    {
        current_x_pos = dir_x;
        current_y_pos = dir_y;
    }
}

