#include <iostream>
#include <vector>

using namespace std;

enum PanelColor_t {
    COLOR_BLACK     = 0,
    COLOR_WHITE     = 1};

enum TurnDirection_t {
    TURN_LEFT   = 0,
    TURN_RIGHT  = 1};

enum RobotDirection_t {
    NORTH   = 0, 
    EAST    = 1,
    SOUTH   = 2,
    WEST    = 3};

class CSpaceship_panel
{
private:
    PanelColor_t current_color;
public:
    CSpaceship_panel(int32_t x_pos, int32_t y_pos);
    ~CSpaceship_panel();
    bool compare_position(int32_t x_pos, int32_t y_pos);
    void paint_panel(PanelColor_t new_color);
    PanelColor_t get_current_color();

    bool painted_flag;
    int32_t x_pos;
    int32_t y_pos;
};

bool compareInterval(CSpaceship_panel panel1, CSpaceship_panel panel2) ;



class CPainting_robot
{
private:
    vector<CSpaceship_panel> all_panels;
    int32_t current_x_pos;
    int32_t current_y_pos;
    RobotDirection_t current_direction;
    void get_outter_panel_positions(int32_t* x_max, int32_t* y_max, int32_t* x_min, int32_t* y_min);


public:
    CPainting_robot();
    ~CPainting_robot();
    CSpaceship_panel* panel_painted_check(int32_t x_pos, int32_t y_pos);
    void create_panel_element(int32_t x_pos, int32_t y_pos);
    void paint_current_panel(PanelColor_t new_color);
    void turn_and_move(TurnDirection_t turn_direction);
    PanelColor_t get_current_panel_color();
    uint32_t count_painted_panels();
    void print_panels();
};

