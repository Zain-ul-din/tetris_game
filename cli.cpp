#include <iostream>
#include <cstdlib>

// driver code
int main ()
{
    system ("g++ main.cpp -c -o main.o");
    system ("g++ main.o -o game.exe -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lwinmm -lgdi32");
    system ("pause");
    system ("game.exe");

    std::cin.get ();
    return EXIT_SUCCESS;
}
