#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <list>
#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "concave_hull.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef K::Point_2                                           Point;


using namespace std;

// функция считывания файла с картой
vector <vector <string>> getmap_vector(string filename);

// поиск препятствий
vector <vector <Point>> find_obstacles(vector <vector<string>> map);

// функции упорядочивания точек/граней границы
list <Point> obs_to_chain(vector <Point> obs);
list <Segment> obs_to_segments(list <Point> obs);

//функции поиска углов между гранями/которые создаются соседними точками
vector <Point> find_angles(list <Segment> obs);
vector <Point> find_angles(list <Point> obs);

// вычисление модуля косинуса между векторами
double cos_between_points(Point p1, Point p2);

// функция начальной декомпозиции трапециями
list <Segment> trapezoidal_decomposition(vector <vector <string>> map, vector <Point> angles);