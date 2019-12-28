#include "intcode_computer.h"

CPainting_robot g_paint_robot;
CIntcode_computer g_intcoder;

int main()
{
    g_intcoder.run_programm(&g_paint_robot);
    cout << "------------------\n";
    cout << "program ends\n";

    uint16_t painted_panels = g_paint_robot.count_painted_panels();
    cout << "------------------\n";
    cout << painted_panels << " panels painted\n";
    cout << "------------------\n";

    g_paint_robot.print_panels();
}