#include "decomposition.h"
#include "concave_hull.h"
#include "graphic.h"

#define filename "map.csv"

int main()
{
    // ���������� ����� �� �����
    vector <vector <string>> map_vector = getmap_vector(filename);

    // ����� �����������
    vector <vector <Point>> obs = find_obstacles(map_vector);
    
    // ������ ��������� ������ ����������� (���� ����������� ������������ � ������ list <Point>
    vector <list <Point>> obstacles;
    // ������ ������ �����������
    vector <list <Segment>> obstacle_segments;


    for (int i = 0; i < obs.size(); i++)
    {
        // ����������� ������������ � ���� ��������� ����� - ����� �� �����������
        list <Point> obstacle = obs_to_chain(obs[i]);
        obstacles.push_back(obstacle);

        // ������� ���� ��� ������ ������� �������� ������
        // ��� ��� ����� ���� � �������, ������� ����� ��������� �� � ������� 
        list <Segment> obstacle_segment = concat_segments(obs_to_segments(obstacle));
        obstacle_segments.push_back(obstacle_segment);
    }

    vector<list <Point>>:: iterator it_obs;
    vector <list <Segment>> segment;
    vector <Point> angles;
    for (it_obs = obstacles.begin(); it_obs != obstacles.end(); it_obs++)
    {
        ofstream out;          // ����� ��� ������
        out.open("obs.txt"); // �������� ���� ��� ������
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

        // �������� �������� ��������
        list <Segment> alpha_shape = concat_segments(chain_segments(alpha_shapes_proc()));
        segment.push_back(alpha_shape);
        // ����� �����
        vector <Point> angles_cur = find_angles(alpha_shape);
        angles.insert(angles.end(), angles_cur.begin(), angles_cur.end());
    }

    // ������ �������
    list <Segment> slices = trapezoidal_decomposition(map_vector, angles);
    int index_orig_obs = 0; // ������� ������ � ����������� (����� ��� �������)
    for (auto iter = segment.begin(); iter != segment.end(); iter++) {
        index_orig_obs += (*iter).size();
    }
    segment.push_back(slices);
    // ����� ���� ��� ������ �������� ������
    //segment.insert(segment.end(), obstacle_segments.begin(), obstacle_segments.end());

    // ����� �������
    init_window(segment, index_orig_obs);

    return 0;
}