#include "arcade_game.h"
#include "intcode_computer.h"

int main()
{
    uint8_t input = 2;
    CIntcode_computer intcoder;
    intcoder.run_programm(input);
    cout << "------------------\n";
    cout << "program ends";
}