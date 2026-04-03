#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2

    ///1) verificar cantidad de argumentos
    if(argc != 6)
    {
        std::cerr << "Formato correcto: " << argv[0] << "<mapa> x1 y1 x2 y2\n";
        return 1;
    }

    ///2) obtener argumentos
    std::string fileMapName = argv[1];
    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int x2 = atoi(argv[4]);
    int y2 = atoi(argv[5]);


    //Load map with class Map
    ///3) Cargar Mapa
    Map map(fileMapName);
    ColorMap colorMap(map);
    colorMap.print();

    //4)Ejecutar BFS
    auto path = Search::BFS(map,{x1,y1},{x2,y2});
    colorMap.print(path);
    
    //Calculate path distance
    //Print path distance
    
    return 0;
}