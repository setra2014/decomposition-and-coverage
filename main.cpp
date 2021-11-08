#include "decomposition.h"
#include "concave_hull.h"
#include "graphic.h"

#define filename "map.csv"

int main()
{
    // считывание карты из файла
    vector <vector <string>> map_vector = getmap_vector(filename);

    // поиск препятствий
    vector <vector <Point>> obs = find_obstacles(map_vector);
    
    // вектор граничных клеток препятствий (одно препятствие записывается в список list <Point>
    vector <list <Point>> obstacles;
    // вектор граней препятствий
    vector <list <Segment>> obstacle_segments;


    for (int i = 0; i < obs.size(); i++)
    {
        // препятствия представлены в виде множества точек - нужно их упорядочить
        list <Point> obstacle = obs_to_chain(obs[i]);
        obstacles.push_back(obstacle);

        // участок кода для вывода графика исходных границ
        // так как имеем дело с точками, удобнее будет перевести их в отрезки 
        list <Segment> obstacle_segment = concat_segments(obs_to_segments(obstacle));
        obstacle_segments.push_back(obstacle_segment);
    }

    vector<list <Point>>:: iterator it_obs;
    vector <list <Segment>> segment;
    vector <Point> angles;
    for (it_obs = obstacles.begin(); it_obs != obstacles.end(); it_obs++)
    {
        ofstream out;          // поток для записи
        out.open("obs.txt"); // окрываем файл для записи
        if (out.is_open())
        {
            out << (*it_obs).size() << endl;

            list <Point> ::iterator it;
            for (it = (*it_obs).begin(); it != (*it_obs).end(); it++)
            {
                out << (*it).x() << " " << (*it).y() << endl;
            }
            out.close();
        }

        // создание вогнутой оболочки
        list <Segment> alpha_shape = concat_segments(chain_segments(alpha_shapes_proc()));
        segment.push_back(alpha_shape);
        // поиск углов
        vector <Point> angles_cur = find_angles(alpha_shape);
        angles.insert(angles.end(), angles_cur.begin(), angles_cur.end());
    }

    // список сечений
    list <Segment> slices = trapezoidal_decomposition(map_vector, angles);
    int index_orig_obs = 0; // сколько граней у препятствий (нужно для графика)
    for (auto iter = segment.begin(); iter != segment.end(); iter++) {
        index_orig_obs += (*iter).size();
    }
    segment.push_back(slices);
    // кусок кода для вывода исходных границ
    //segment.insert(segment.end(), obstacle_segments.begin(), obstacle_segments.end());

    // вывод графика
    init_window(segment, index_orig_obs);

    return 0;
}