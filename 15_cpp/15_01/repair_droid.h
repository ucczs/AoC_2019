#include <iostream>
#include <vector>

#define COUNT_DIRECTIONS 4

enum position_type_t{
    UNDEFINED = -1,
    WALL = 0,
    EMPTY = 1,
    OXYGEN_SYSTEM = 2
};

enum move_direction_t{
    UNDEFINED_DIRECTION = 0,
    NORTH_DIRECTION = 1,
    SOUTH_DIRECTION = 2,
    WEST_DIRECTION = 3,
    EAST_DIRECTION = 4
};

struct position_info
{
    int32_t x_pos;
    int32_t y_pos;
    position_type_t pos_type;
    std::vector<move_direction_t> checked_directions;
    move_direction_t initially_come_from;
    bool walk_back_initial_way;
    bool is_flooded;
    bool all_neightbours_flooded;
    bool just_flooded;
};

struct map_infos
{
    int32_t min_x_coordinate;
    int32_t min_y_coordinate;
    int32_t max_x_coordinate;
    int32_t max_y_coordinate;
};

class CRepair_droid
{
private:
    std::vector<position_info> map;
    int32_t current_x_pos;
    int32_t current_y_pos;
    position_type_t last_type;    
    move_direction_t current_direction;
    map_infos map_info_str;
    uint8_t steps_from_zero;
    bool all_areas_flooded;
    uint16_t time_to_flood;

    void determine_map_borders();
    void get_position_depending_on_direction(move_direction_t move_dir, int32_t* x_pos, int32_t* y_pos);
    bool check_if_coordiantes_exists(int32_t x_pos, int32_t y_pos);
    position_info* get_current_element(int32_t x_pos, int32_t y_pos);
    bool check_if_direction_was_used(move_direction_t move_dir, position_info* current_element);
    move_direction_t get_inverse_direction(move_direction_t direction);
    void count_steps();
    void flood_neightbours(position_info* oxygen_position);
    position_info* get_neightbour(position_info* oxygen_position, move_direction_t direction);
    void check_if_all_areas_flooded();
    void start_oxygen_source();

public:
    CRepair_droid();
    ~CRepair_droid();
    move_direction_t generate_newest_output();
    void process_newest_input(position_type_t pos_type);
    void draw_map();
    void flood_area_with_oxygen();

};

