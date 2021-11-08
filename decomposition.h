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

// ������� ���������� ����� � ������
vector <vector <string>> getmap_vector(string filename);

// ����� �����������
vector <vector <Point>> find_obstacles(vector <vector<string>> map);

// ������� �������������� �����/������ �������
list <Point> obs_to_chain(vector <Point> obs);
list <Segment> obs_to_segments(list <Point> obs);

//������� ������ ����� ����� �������/������� ��������� ��������� �������
vector <Point> find_angles(list <Segment> obs);
vector <Point> find_angles(list <Point> obs);

// ���������� ������ �������� ����� ���������
double cos_between_points(Point p1, Point p2);

// ������� ��������� ������������ ����������
list <Segment> trapezoidal_decomposition(vector <vector <string>> map, vector <Point> angles);