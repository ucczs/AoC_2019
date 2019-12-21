#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

const uint16_t g_height     = 6;
const uint16_t g_width      = 25;
const uint16_t pixels_in_layer = g_height * g_width;
std::string g_line;

class CLayer
{
private:
    uint16_t height;
    uint16_t width;
    uint16_t counter_zeros;
public:
    CLayer(/* args */);
    ~CLayer();
    int8_t pixel_data[g_height*g_width];
    uint16_t get_number_counter(int8_t desired_number);
};

class CImage
{
private:
    uint16_t height_layer;
    uint16_t width_layer;
    uint16_t number_layers;
    std::vector<CLayer*> image_layers;
    void process_input(std::string input_string);
    CLayer final_image;
public:
    CImage(std::string input_string);
    ~CImage();
    uint16_t get_fewest_zero_Layer();
    uint16_t counter_number_in_layer(int8_t number, int16_t layer_idx);
    void generate_final_image();
    void save_final_image(std::string file_name);
};