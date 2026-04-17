#pragma once
#include <vector>
#include "Map.h"
#include <unordered_map>

class Search{

public:
    static std::vector<std::pair<int,int>> reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start);//reconstructs path
    static std::vector<std::pair<int,int>> BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal);//finds and return path in map from start to goal
    
    //Funciones Lab 4: BFS Greedy
    static std::vector<std::pair<int,int>> greedyBFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal); //find path using Greedy Best-First Search
    static float Heuristic(std::pair<int,int> start, std::pair<int,int> goal); //Calculate Heuristic value between two points

    //Funciones Lab 5: A* (A-star)
    static std::vector<std::pair<int,int>> AStar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal);

};