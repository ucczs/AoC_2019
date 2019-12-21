#include "08_01.h"

using namespace std;


CLayer::CLayer()
{
    height = g_height;
    width = g_width;
    counter_zeros = 0;
}

CLayer::~CLayer()
{
}

uint16_t CLayer::get_number_counter(int8_t desired_number)
{
    counter_zeros = 0;
    for (int i = 0; i < pixels_in_layer; i++)
    {
        if (pixel_data[i] == desired_number)
        {
            counter_zeros++;
        }
    }
    return counter_zeros;
}

CImage::CImage(string input_string)
{
    height_layer = g_height;
    width_layer = g_width;
    uint32_t size_input = input_string.size();
    number_layers = size_input / g_height / g_width;

    process_input(input_string);
}

CImage::~CImage()
{
}

void CImage::process_input(string input_string)
{
    for (uint32_t i = 0; i < number_layers; i++)
    {
        CLayer* new_layer = new CLayer();
        uint32_t start_index = height_layer * width_layer * i;
        for (uint32_t j = start_index; j < start_index+pixels_in_layer; j++)
        {
            new_layer->pixel_data[j-start_index] = (int)(g_line[j] - '0');
        }
        image_layers.push_back(new_layer);
    }
    cout << "Layers created\n";
}

uint16_t CImage::get_fewest_zero_Layer()
{
    int16_t minimum_zeros_idx = -1;
    int32_t minimum_zeros = -1;
    int16_t layer_zeros = 0;
    for (uint16_t i = 0; i < number_layers; i++)
    {
        layer_zeros = image_layers[i]->get_number_counter(0);
        if (layer_zeros < minimum_zeros || minimum_zeros_idx < 0)
        {
            minimum_zeros = layer_zeros;
            minimum_zeros_idx = i;
        }
        
    }
    return(minimum_zeros_idx);
}

uint16_t CImage::counter_number_in_layer(int8_t number, int16_t layer_idx)
{
    return(image_layers[layer_idx]->get_number_counter(number));
}

void CImage::generate_final_image()
{
    uint16_t layer_iterator = 0;
    for (uint16_t pixel_iterator = 0; pixel_iterator < pixels_in_layer; pixel_iterator++)
    {
        layer_iterator = 0;
        while (image_layers[layer_iterator]->pixel_data[pixel_iterator] == 2)
        {
            layer_iterator++;
        }
        final_image.pixel_data[pixel_iterator] = image_layers[layer_iterator]->pixel_data[pixel_iterator];
    }
}

void CImage::save_final_image(string file_name)
{
    ofstream output_file(file_name);

    if(output_file.is_open())
    {
        for (uint16_t i = 0; i < height_layer; i++)
        {
            string string_line = "";
            for (uint16_t j = 0; j < width_layer; j++)
            {
                if(final_image.pixel_data[g_width*i + j] == 0)
                {
                    string_line = string_line + "  ";
                }
                else
                {
                    string_line = string_line + to_string(final_image.pixel_data[g_width*i + j]) + " ";
                }
            }
            output_file << string_line << endl;
        }
        output_file.close();
    }
}

void open_file(string file_name)
{
    ifstream my_file(file_name);
    if (my_file.is_open())
    {
        getline(my_file, g_line);
        my_file.close();
    }
    else
    {
        cout << "\nFile not found!\n";
        exit(0);
    }
}

int main()
{
    open_file("input.txt");
    CImage image(g_line);
    uint16_t minimum_zeros_layer_idx = image.get_fewest_zero_Layer();
    uint16_t result = image.counter_number_in_layer(1,minimum_zeros_layer_idx) * image.counter_number_in_layer(2,minimum_zeros_layer_idx);
    cout << "In layer with fewest 0's, number of 1's and 2's multplied are:\n" << result << "\n";

    image.generate_final_image();
    image.save_final_image("output.txt");    
}