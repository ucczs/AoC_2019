#include "painting_roboter.h"

CSpaceship_panel::CSpaceship_panel(int32_t x_pos, int32_t y_pos)
{
    painted_flag = false;
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    current_color = COLOR_BLACK;
}

CSpaceship_panel::~CSpaceship_panel()
{
}

bool CSpaceship_panel::compare_position(int32_t x_pos, int32_t y_pos)
{
    if (this->x_pos == x_pos && this->y_pos == y_pos) return true;
    else return false;
}

void CSpaceship_panel::paint_panel(PanelColor_t new_color)
{
    current_color = new_color;
    painted_flag = true;
}

PanelColor_t CSpaceship_panel::get_current_color()
{
    return current_color;
}

CPainting_robot::CPainting_robot(/* args */)
{
    current_x_pos = 0;
    current_y_pos = 0;
    RobotDirection_t NORTH;

    CSpaceship_panel new_panel(current_x_pos, current_y_pos);
    all_panels.push_back(new_panel);
}

CPainting_robot::~CPainting_robot()
{
}

CSpaceship_panel* CPainting_robot::panel_painted_check(int32_t x_pos, int32_t y_pos)
{
    for (uint32_t i = 0; i < all_panels.size(); i++)
    {
        if (all_panels[i].compare_position(x_pos, y_pos)) 
        {
            return &all_panels[i];
        }
    }
    return nullptr;
}

void CPainting_robot::create_panel_element(int32_t x_pos, int32_t y_pos)
{
    CSpaceship_panel new_panel(x_pos, y_pos);
    all_panels.push_back(new_panel);
}

void CPainting_robot::turn_and_move(TurnDirection_t turn_direction)
{
    switch (current_direction)
    {
    case NORTH:
        if (turn_direction == TURN_LEFT)
        {
            current_direction = WEST;
            current_x_pos--;
        }
        else if (turn_direction == TURN_RIGHT)
        {
            current_direction = EAST;
            current_x_pos++;
        }
        break;
    case EAST:
        if (turn_direction == TURN_LEFT)
        {
            current_direction = NORTH;
            current_y_pos++;
        }
        else if (turn_direction == TURN_RIGHT)
        {
            current_direction = SOUTH;
            current_y_pos--;
        }    
        break;
    case SOUTH:
        if (turn_direction == TURN_LEFT)
        {
            current_direction = EAST;
            current_x_pos++;
        }
        else if (turn_direction == TURN_RIGHT)
        {
            current_direction = WEST;
            current_x_pos--;
        }    
        break;
    case WEST:
        if (turn_direction == TURN_LEFT)
        {
            current_direction = SOUTH;
            current_y_pos--;
        }
        else if (turn_direction == TURN_RIGHT)
        {
            current_direction = NORTH;
            current_y_pos++;
        }    
        break;
    default:
        cout << "\nError in turning!\n";
        break;
    }

    if (panel_painted_check(current_x_pos, current_y_pos) == nullptr)
    {
       create_panel_element(current_x_pos, current_y_pos);
    }
}

void CPainting_robot::paint_current_panel(PanelColor_t new_color)
{
    CSpaceship_panel* current_panel = panel_painted_check(current_x_pos, current_y_pos);
    if (current_panel != nullptr)
    {
        current_panel->paint_panel(new_color);
    }
    else
    {
        CSpaceship_panel new_panel(current_x_pos, current_y_pos);
        new_panel.paint_panel(new_color);
        all_panels.push_back(new_panel);
    }
}

PanelColor_t CPainting_robot::get_current_panel_color()
{
    CSpaceship_panel* current_panel = panel_painted_check(current_x_pos, current_y_pos);
    return current_panel->get_current_color();
}

uint32_t CPainting_robot::count_painted_panels()
{
    uint32_t painted_counter = 0;
    for (uint32_t i = 0; i < all_panels.size(); i++)
    {
        if (all_panels[i].painted_flag == true)
        {
            painted_counter++;
        }
    }
    return painted_counter;
}

