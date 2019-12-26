#include "10_01.h"

using namespace std;

CAsteroids::CAsteroids(uint16_t x_pos, uint16_t y_pos)
{
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    visible_asteroids = 0;
}

CAsteroids::~CAsteroids()
{
}

posCheck_t CAsteroids::check_position(uint16_t x_pos_test, uint16_t y_pos_test)
{
    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        if (g_all_asteroids[i].x_pos == x_pos_test && g_all_asteroids[i].y_pos == y_pos_test)
        {
            return ASTEROID_FOUND;
        }
    }
    return NO_ASTEROID_FOUND;
}

visibleCheck_t CAsteroids::check_asteroid_in_x_pos_range(uint16_t x_pos_target_asteroid, uint16_t y_pos_target_asteroid, float view_direction)
{
    for (uint16_t x_test = min(x_pos_target_asteroid,x_pos)+1; x_test < max(x_pos,x_pos_target_asteroid); x_test++)
    {
        float y_test = y_pos_target_asteroid + (x_test-x_pos_target_asteroid)*view_direction;
        if (ceilf(y_test) == (y_test))
        {
            if (check_position(x_test, y_test))
            {
                return ASTEROID_NOT_VISIBLE;
            }
        }
    }
    return ASTEROID_VISIBLE;
}

uint16_t get_maximum_views()
{
    uint16_t maximum_views = 0;
    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        if (g_all_asteroids[i].get_visible_asteroids() > maximum_views)
        {
            maximum_views = g_all_asteroids[i].get_visible_asteroids();
        }
    }
    return maximum_views;
}

uint16_t CAsteroids::get_visible_asteroids()
{
    return visible_asteroids;
}

void CAsteroids::check_visibility_asteroids()
{
    visible_asteroids = 0;
    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        uint16_t x_pos_target_asteroid = g_all_asteroids[i].x_pos;
        uint16_t y_pos_target_asteroid = g_all_asteroids[i].y_pos;
        if (x_pos == x_pos_target_asteroid && y_pos == y_pos_target_asteroid)
        {
            continue; // dont check for own asteroid
        }
        
        if (x_pos != x_pos_target_asteroid)
        {
            float view_direction = (float)(y_pos - y_pos_target_asteroid) / (x_pos - x_pos_target_asteroid);
            if( check_asteroid_in_x_pos_range(x_pos_target_asteroid, y_pos_target_asteroid, view_direction) )
            {
                visible_asteroids++;
                continue;
            }
        }
        else if (x_pos == x_pos_target_asteroid)
        {
            bool view_blocked = false;
            for (uint16_t y_test = min(y_pos_target_asteroid,y_pos)+1; y_test < max(y_pos,y_pos_target_asteroid); y_test++)
            {
                if (check_position(x_pos, y_test))
                {
                    view_blocked = true;
                    break;
                }
            }
            if (!view_blocked) 
            {
                visible_asteroids++;
            }
        }
    }
}

void read_in_map(string filename)
{
    ifstream fin;
    fin.open(filename, ios::in);

    char my_character ;
    int number_of_lines = 0;

    uint16_t x_pos = 0;
    uint16_t y_pos = 0;

	while (!fin.eof() )
    {
        fin.get(my_character);
        cout << my_character;

        if (my_character != '\n')
        {
            if (my_character == '#')
            {
                CAsteroids new_asteroid(x_pos, y_pos);
                g_all_asteroids.push_back(new_asteroid);
            }
            x_pos++;
        }
        else
        {
            y_pos++;
            x_pos = 0;
        }
    }    
}


int main()
{
    read_in_map("input.txt");

    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        g_all_asteroids[i].check_visibility_asteroids();
    }

    uint16_t maxium_views = get_maximum_views();

    cout << "Maximum visible asteroids: " << maxium_views << endl;
    cout << "program ends";
}