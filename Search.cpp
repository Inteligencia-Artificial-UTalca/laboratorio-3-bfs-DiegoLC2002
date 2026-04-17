#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cmath>

namespace std
{
    //this is needed to store a pair<int,int> in an associative container
    //such as unorered_set and unordered_map
	template<> struct hash<std::pair<int,int>>
	{
		std::size_t operator()( const std::pair<int,int> & p) const noexcept
		{
            hash<int> hasher;
			return hasher(p.first) ^ (hasher(p.second)<<1);

		}
	};
}

std::vector<std::pair<int,int>> Search::reconstruct(
    const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, 
    const std::pair<int,int> &start)
{	
    std::deque<std::pair<int,int>> nodes;
	auto node = start;//make copy

    while(true)
    {
        nodes.push_front(node);

        auto it = pathCache.find(node);
        
        if(it == pathCache.end()){ break;}

        node = it->second;
    }

    //revert path and return it
    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

std::vector<std::pair<int,int>> Search::BFS(
                                    const Map& map, 
                                    std::pair<int,int> start, 
                                    std::pair<int,int> goal)
{    
    std::cout<<"===========================\nRunning BFS...\n";
	//auto startTime = std::chrono::high_resolution_clock::now();

    //stores possible directions
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    //bool visited[map.h][map.w]{false};      //we'll just use a matrix og booleans to indicated if visited
    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    std::queue<std::pair<int,int>> OPEN;

    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;    ////hashmap to reconstruct path: child -> parent

    //add firts node to open list
    OPEN.push(start);
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        
        //obtener nodo actual
        auto pos = OPEN.front();
        OPEN.pop();

        //Verificar si se esta en el objetivo
        if(pos == goal)
        {
            std::cout << "Meta Encontrada.\n";

            auto result = reconstruct(pathCache, pos);

            std::cout << "Path size (BFS): " << result.size() << std::endl;

            return result;
        }

        //check if node is goal
		/*if(pos==goal){
			auto endTime = std::chrono::high_resolution_clock::now();
			int count=0;
            for(int i=0;i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
			std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
			std::cout<<"FOUND in "<<(endTime-startTime).count()/1000000.0<<"ms\n";
			return reconstruct(pathCache,pos);
		}*/

        //Explorar vecinos
		for(auto dir:dirs)
        {
            auto newPos = pos;
            newPos.first += dir.first;
            newPos.second += dir.second;

            //Si esta fuera de rango
            if(newPos.first < 0 || newPos.first >= map.h ||
               newPos.second < 0 || newPos.second >= map.w)
                { continue;}

            if(map._map[newPos.first][newPos.second] == 1){ continue;}

            //Ya visitado
            if(visited[newPos.first][newPos.second]){ continue;}

            //Agregar a la cola
            OPEN.push(newPos);
            visited[newPos.first][newPos.second] = true;
            pathCache[newPos] = pos;

		}
	}
	std::cout<<"NOT FOUND!!!!\n";
    return {};
    //let's just return start and goal to draw them
    /*std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;*/
}

//Funciones para implementar Greedy BFS (Laboratorio 4)
//Functor
struct Comparar
{
    //Esto hace que el priority queue (lista de prioridad) saque el nodo más cercano a la meta (goal)
    std::pair<int,int> goal;

    Comparar(std::pair<int,int> g) : goal(g){}

    bool operator()(std::pair<int,int> a, std::pair<int,int> b)
    {
        return Search::Heuristic(a, goal) > Search::Heuristic(b,goal);
    }
};



std::vector<std::pair<int,int>> Search::greedyBFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal)
{
    //Implementar aqui
    std::cout<<"===========================\nRunning BFS Greedy...\n";

    std::pair<int,int> direcciones[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visitados(map.h, std::vector<bool>(map.w, false));

    //Cola de prioridad (Priority Queue)
    std::priority_queue<
            std::pair<int,int>,
            std::vector<std::pair<int,int>>,
            Comparar
    > OPEN{(Comparar(goal))};

    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;

    OPEN.push(start);
    visitados[start.first][start.second] = true;

    while(!OPEN.empty())
    {
        auto pos = OPEN.top();
        OPEN.pop();

        if(pos == goal)
        {
            std::cout<<"Meta encontrada (Greedy).\n";
            return reconstruct(pathCache,pos);
        }

        for(auto dir:direcciones)
        {
            auto newPos = pos;
            newPos.first += dir.first;
            newPos.second += dir.second;

            //Si esta fuera de rango
            if(newPos.first < 0 || newPos.first >= map.h ||
               newPos.second < 0 || newPos.second >= map.w)
                { continue;}

            if(map._map[newPos.first][newPos.second] == 1){ continue;}

            //Ya visitado
            if(visitados[newPos.first][newPos.second]){ continue;}

            //Agregar a la cola
            OPEN.push(newPos);
            visitados[newPos.first][newPos.second] = true;
            pathCache[newPos] = pos;
        }
    }

    std::cout<<"NOT FOUND (Greedy)!!!!\n";
    return {};
}


float Search::Heuristic(std::pair<int,int> start, std::pair<int,int> goal)
{
    //Implementar heuristica Manhattan (Solo se mueve en 4 direcciones)
    //Debe calcular y devolver un valor heuristico.

    return abs(start.first - goal.first) + abs(start.second - goal.second);
}


//Funciones para Lab 5
std::vector<std::pair<int,int>> Search::AStar(const Map& map, std::pair<int,int> start, std::pair<int,int> goal)
{
    std::cout << "===========================\nEjecutando A*...\n";

    std::unordered_map<std::pair<int,int>, float> gCost;
    gCost[start] = 0.0f;

    return {};
}

