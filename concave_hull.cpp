#include "concave_hull.h"
#include "decomposition.h"

template <class OutputIterator>
void alpha_edges(const Alpha_shape_2& A, OutputIterator out)
{   
    
    Alpha_shape_edges_iterator it = A.alpha_shape_edges_begin(),
        end = A.alpha_shape_edges_end();

    for (; it != end; ++it)
        *out++ = A.segment(*it);
}

template <class OutputIterator>
void alpha_vertex(const Alpha_shape_2& A, OutputIterator out)
{

    Alpha_shape_vertices_iterator it = A.alpha_shape_vertices_begin(),
        end = A.alpha_shape_vertices_end();

    for (; it != end; ++it)
        *out++ = A.point(*it);
}

template <class OutputIterator>
bool file_input(OutputIterator out)
{
    ifstream is("obs.txt", ios::in);
    if (is.fail())
    {
        cerr << "unable to open file for input" << endl;
        return false;
    }
    int n;
    is >> n;
    cout << "Было " << n << " точек" << endl;
    copy_n(std::istream_iterator<Point>(is), n, out);
    return true;
}

list <Segment> chain_segments(vector <Segment> segments)
{
    list <Segment> chain;
    // выбираем начальную грань
    auto iter = segments.begin();
    chain.push_back(*iter);
    segments.erase(iter);

    // ищем в векторе граней ту, которая начинается с той точки, на которой заканчивается текущая грань
    while (!segments.empty())
    {
        for (auto it = segments.begin(); it != segments.end(); it++)
        {
            if ((*it).end() == chain.front().start()) {
                chain.push_front(*it);
                it = segments.erase(it);
                break;
            }
            else if ((*it).start() == chain.back().end())
            {
                chain.push_back(*it);
                it = segments.erase(it);
                break;
            }
        }
    }
    return chain;
}

list <Segment> concat_segments(list <Segment> segments) {
    list <Segment> concat;
    // выбираем начальную грань
    auto it = segments.begin();
    concat.push_back(*it);
    segments.erase(it);
    float eps = 0.01;

    for (auto it = segments.begin(); it != segments.end(); it++)
    {
        // переводим грани в вектора, направленные вдоль них
        Point p1(concat.back().end().x() - concat.back().start().x(), concat.back().end().y() - concat.back().start().y());
        Point p2((*it).end().x() - (*it).start().x(), (*it).end().y() - (*it).start().y());
        // вычисляем модуль косинуса угла между этими векторами. Если он близок к 1 (вплоть до погрешности eps), то соединяем эти грани в одну 
        if (1 - cos_between_points(p1, p2) < eps) {
            Segment seg(concat.back().start(), (*it).end());
            concat.pop_back();
            concat.push_back(seg);
        }
        else {
            concat.push_back(*it);
        }
    }

    // пробуем соединить последнюю и первую грани
    Point p1(concat.front().end().x() - concat.front().start().x(), concat.front().end().y() - concat.front().start().y());
    Point p2(concat.back().end().x() - concat.back().start().x(), concat.back().end().y() - concat.back().start().y());
    if (1 - cos_between_points(p1, p2) < eps) {
        Segment seg(concat.back().start(), concat.front().end());
        concat.pop_back();
        concat.pop_front();
        concat.push_front(seg);
    }

    return concat;
}

vector <Segment> alpha_shapes_proc()
{
    list <Point> points;
    vector<Segment> segments;
    // считывание файла с координатами точек
    if (!file_input(back_inserter(points)))
        return segments;

    // построение вогнутой оболочки
    Alpha_shape_2 A(points.begin(), points.end(),
        FT(10000),
        Alpha_shape_2::GENERAL);
    alpha_edges(A, back_inserter(segments));
    
    return segments;
}

vector <Point> alpha_shapes_proc_vertex()
{
    list <Point> points;
    vector<Point> segments;
    if (!file_input(back_inserter(points)))
        return segments;

    Alpha_shape_2 A(points.begin(), points.end(),
        FT(10000),
        Alpha_shape_2::GENERAL);
    alpha_vertex(A, back_inserter(segments));

    // cout << "Стало " << segments.size() << " точек в alpha shape" << endl;
    // cout << "Optimal alpha: " << *A.find_optimal_alpha(1) << endl;
    return segments;
}
