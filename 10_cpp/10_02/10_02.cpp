#include "10_02.h"

using namespace std;

CAsteroids::CAsteroids(uint16_t x_pos, uint16_t y_pos)
{
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    visible_asteroids = 0;
    base_flag = false;
    vaporized_flag = false;
    quadrant_base = UNDEFINED_QUADRANT;
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
    uint16_t former_max_idx = 0;
    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        if (g_all_asteroids[i].get_visible_asteroids() > maximum_views)
        {
            maximum_views = g_all_asteroids[i].get_visible_asteroids();
            g_all_asteroids[i].base_flag = true;
            g_all_asteroids[former_max_idx].base_flag = false;
            former_max_idx = i;
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

void CAsteroids::get_position(uint16_t* x_pos, uint16_t* y_pos)
{
    *x_pos = this->x_pos;
    *y_pos = this->y_pos;
}


void read_in_map(string filename)
{
    ifstream fin;
    fin.open(filename, ios::in);

    if (fin.is_open())
    {
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
    else
    {
        cout << "File not found!\n";
    }
    
}

void CAsteroids::calculate_base_relations(uint16_t x_pos_base, uint16_t y_pos_base)
{
    if (x_pos != x_pos_base)
    {
        angle_to_base = (float)(y_pos - y_pos_base) / (x_pos - x_pos_base);
    }
    else
    {
        angle_to_base = 999999; // approximation of vertical steepness
    }
    distance_to_base = sqrt(pow(((float)y_pos - y_pos_base),2) + pow(((float)x_pos - x_pos_base), 2));
}

void CAsteroids::define_base_quadrant(uint16_t x_pos_base, uint16_t y_pos_base)
{
    // first quadrant in map:   y < y_base, x > x_base
    // second quadrant in map:  y > y_base, x > x_base
    // third quadrant in map:   y > y_base, x < x_base
    // forth quadrant in map:   y < y_base, x < x_base    
    if (y_pos < y_pos_base && x_pos >= x_pos_base )
    {
        quadrant_base = FIRST_QUADRANT;
        if (x_pos == x_pos_base) angle_to_base = -999999;
        
    }
    else if (y_pos >= y_pos_base && x_pos > x_pos_base )
    {
        quadrant_base = SECOND_QUADRANT;
    }
    else if (y_pos > y_pos_base && x_pos <= x_pos_base )
    {
        quadrant_base = THIRD_QUADRANT;
        if (x_pos == x_pos_base) angle_to_base = 999999;
    }
    else if (y_pos <= y_pos_base && x_pos < x_pos_base )
    {
        quadrant_base = FORTH_QUADRANT;
    }
}

void fill_base_relations()
{
    CAsteroids* base;
    uint16_t x_pos_base;
    uint16_t y_pos_base;

    // find base coordinates
    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        if (g_all_asteroids[i].base_flag)
        {
            base = &g_all_asteroids[i];
            base->get_position(&x_pos_base, &y_pos_base);
        }
    }

    for (uint16_t i = 0; i < g_all_asteroids.size(); i++)
    {
        if (!g_all_asteroids[i].base_flag)
        {
            g_all_asteroids[i].calculate_base_relations(x_pos_base, y_pos_base);
            g_all_asteroids[i].define_base_quadrant(x_pos_base, y_pos_base);
        }
    }
}

bool compare_asteroids(CAsteroids asteroid_1, CAsteroids asteroid_2) 
{
    // asteroids in different quadrants
    if (asteroid_1.quadrant_base < asteroid_2.quadrant_base ) return true;
    else if (asteroid_1.quadrant_base > asteroid_2.quadrant_base) return false;
    // asteroids in same quadrant
    else
    {
        if (asteroid_1.angle_to_base < asteroid_2.angle_to_base) return true;
        else if (asteroid_1.angle_to_base > asteroid_2.angle_to_base) return false;
        else if (asteroid_1.distance_to_base < asteroid_2.distance_to_base) return true;
        else return false;
    }
} 

void remove_vaporized_asteroids()
{
    for (uint16_t i = g_all_asteroids.size(); i > 0; i--)
    {
        if (g_all_asteroids[i-1].vaporized_flag == true)
        {
            g_all_asteroids.erase(g_all_asteroids.begin()+i-1);
        }
    }
}

void vaporize_asteroids()
{
    uint16_t vaporizer_counter = 0;

    while (g_all_asteroids.size() > 0)
    {
        sort(g_all_asteroids.begin(), g_all_asteroids.end(), compare_asteroids);
        
        for (uint16_t i = 1; i < g_all_asteroids.size(); i++)
        {
            if (i == 1 || g_all_asteroids[i].angle_to_base != g_all_asteroids[i-1].angle_to_base)
            {
                g_all_asteroids[i].vaporized_flag = true;
                vaporizer_counter++;

                if (vaporizer_counter == 200)
                {
                    cout << "200th vaporized asteroid is located in (" << g_all_asteroids[i].x_pos << "," << g_all_asteroids[i].y_pos << ").\n";
                }
            }
        }
        remove_vaporized_asteroids();
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

    fill_base_relations();
    cout << "Base infos available\n";

    vaporize_asteroids();

    cout << "Maximum visible asteroids: " << maxium_views << endl;
    cout << "program ends";
}