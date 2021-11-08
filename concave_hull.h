#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Alpha_shape_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/assertions.h>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <math.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef K::FT                                                FT;
typedef K::Point_2                                           Point;
typedef K::Segment_2                                         Segment;
typedef CGAL::Alpha_shape_vertex_base_2<K>                   Vb;
typedef CGAL::Alpha_shape_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>          Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>                 Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator            Alpha_shape_edges_iterator;
typedef Alpha_shape_2::Alpha_shape_vertices_iterator            Alpha_shape_vertices_iterator;

using namespace std;

template <class OutputIterator>
void alpha_edges(const Alpha_shape_2& A, OutputIterator out);

template <class OutputIterator>
bool file_input(OutputIterator out);

// функция, возвращающая грани препятствий
vector <Segment> alpha_shapes_proc();
// функция, возвращающая вершины препятствий
vector <Point> alpha_shapes_proc_vertex();

// упорядочивание множества граней
list <Segment> chain_segments(vector <Segment> segments);
// часто алгоритм делит истинные грани на несколько частей, потому их надо объединить
list <Segment> concat_segments(list <Segment> segments);
