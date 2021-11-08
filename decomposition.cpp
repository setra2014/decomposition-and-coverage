#include "decomposition.h"

// �������� ����, �������� �� ������ ��������
bool isneighborhood_points(Point p1, Point p2)
{
    return (abs(p1.x() - p2.x()) <= 1 && abs(p1.y() - p2.y()) <= 1) && (abs(p1.x() - p2.x()) + abs(p1.y() - p2.y()) != 0);
};


vector <vector <string>> getmap_vector(string filename)
{
    vector <vector <string>> map;
    ifstream work_file(filename);
    string line;
    char delimiter = '\t';
    //������ ������ ������, ����� ��������� ��, ��� ��� ��� �������� ����������
    getline(work_file, line);
    // ������ ��� ������
    while (getline(work_file, line))
    {
        stringstream stream(line); // �������������� ������ � �����
        vector <string> line_map;
        string l;
        // ���������� ���� �������� � ���� ������
        getline(stream, l, delimiter);
        while (getline(stream, l, delimiter))
        {
            line_map.push_back(l);
        }
        map.push_back(line_map);
    }
    work_file.close();
    return map;
}

// �������� ����, ��� ������ �������� ���������
bool isedge(vector<string> neighborhood)
{
    // ���� ����� ��������� �� ���� �����, �� ��� �������� ��������
    if (neighborhood.size() < 4)
    {
        return true;
    }

    // ���� ������ ������ ���� ��������� ������, �� ��� ������ �������� ��������
    for (int i = 0; i < neighborhood.size(); i++)
    {
        if (neighborhood[i] == "0") return true;
    }
    return false;
}

// ����� �����������
vector <vector <Point>> find_obstacles(vector <vector<string>> map)
{
    // ����� ������ ������
    vector <Point> Points_edge;
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[0].size(); x++) {
            if (map[y][x] == "0") continue;

            vector <string> neighborhood;

            if (y != 0) neighborhood.push_back(map[y - 1][x]);
            if (y != map.size() - 1) neighborhood.push_back(map[y + 1][x]);
            if (x != 0) neighborhood.push_back(map[y][x - 1]);
            if (x != map[0].size() - 1) neighborhood.push_back(map[y][x + 1]);

            if (isedge(neighborhood)) {
                Points_edge.push_back(Point(x, y));
            }
        }

    }

    // ���������� ������� � �����������
    vector <vector <Point>> obstacles;
    int ind = 0;
    // ���������� ������ � ������, ���� Points_edge �� ������ ������
    while (Points_edge.size() != 0)
    {
        vector <Point> start_obs;
        start_obs.push_back(Points_edge[0]);
        obstacles.push_back(start_obs);
        auto iter = Points_edge.cbegin();
        Points_edge.erase(iter);

        int pred_size = -1;
        int cur_size = Points_edge.size();
        // ��������� � ������ ������� ����������� ������, ���� Points_edge �� ����� ��������
        while (pred_size != cur_size && !Points_edge.empty())
        {
            cur_size = Points_edge.size();
            // ������� ������ ������ �� ��������� ���������
            for (auto iter = Points_edge.cbegin(); iter != Points_edge.cend(); iter++)
            {
                // � ���������� � ������ ������� �� �������� ����������� �������
                for (int j = 0; j < obstacles[ind].size(); j++) 
                {
                    if (isneighborhood_points(obstacles[ind][j], *iter))
                    {
                        obstacles[ind].push_back(*iter);

                        iter = Points_edge.erase(iter);
                        if (iter != Points_edge.cbegin()) {
                            iter--;
                        }
                        break;
                    }
                }
                if (Points_edge.empty()) break;
            }

            pred_size = Points_edge.size();
        }
        ind++;
        cout << ind << endl;
    }
    
    return obstacles;
}

// ������������� �������
double mannhatan(Point p1, Point p2) {
    return abs(p1.x() - p2.x()) + abs(p1.y() - p2.y());
}

// ������� �������������� �����/������ �������
list <Point> obs_to_chain(vector <Point> obs)
{
    list <Point> chain;
    chain.push_back(obs[0]);
    obs.erase(obs.begin());
    int pred_size = 0;
    while (pred_size != obs.size()) {
        pred_size = obs.size();
        vector <Point> neighbours_back;
        vector <Point> neighbours_front;
        for (auto i = obs.begin(); i < obs.end(); i++) {
            if (isneighborhood_points(chain.back(), *i)) {
                neighbours_back.push_back(*i);
                i = obs.erase(i);
                if (i != obs.cbegin()) {
                    i--;
                }
            }
            else if (isneighborhood_points(chain.front(), *i)) {
                neighbours_front.push_back(*i);
                i = obs.erase(i);
                if (i != obs.cbegin()) {
                    i--;
                }
            }
            if (obs.empty()) break;
        }
        if (neighbours_back.size() == 0) {
        }
        else if (neighbours_back.size() == 1) {
            chain.push_back(neighbours_back[0]);
        }
        else if (mannhatan(chain.back(), neighbours_back[0]) >= mannhatan(chain.back(), neighbours_back[1])) {
            chain.push_back(neighbours_back[1]);
            chain.push_back(neighbours_back[0]);
        }
        else {
            chain.push_back(neighbours_back[0]);
            chain.push_back(neighbours_back[1]);
        }

        if (neighbours_front.size() == 0) {

        }
        else if (neighbours_front.size() == 1) {
            chain.push_front(neighbours_front[0]);
        }
        else if (mannhatan(chain.front(), neighbours_front[0]) >= mannhatan(chain.front(), neighbours_front[1])) {
            chain.push_front(neighbours_front[1]);
            chain.push_front(neighbours_front[0]);
        }
        else {
            chain.push_front(neighbours_front[0]);
            chain.push_front(neighbours_front[1]);
        }
    }
    return chain;
}

list <Segment> obs_to_segments(list <Point> obs) {
    list <Segment> segments;
    auto it_end = obs.end();
    it_end--;
    for (auto i = obs.begin(); i != it_end; i++) {
        auto iter_next = *(++i);
        i--;
        Segment edge((*i), iter_next);
        
        segments.push_back(edge);
    }
    Segment edge(obs.back(), obs.front());
    segments.push_back(edge);
    return segments;
}

// ������ �������� ���� ����� ���������
double cos_between_points(Point p1, Point p2)
{
    return abs(p1.x() * p2.x() + p1.y() * p2.y()) / sqrt(p1.x() * p1.x() + p1.y() * p1.y()) / sqrt(p2.x() * p2.x() + p2.y() * p2.y());
}

// ����� �����
vector <Point> find_angles(list <Segment> obs) {
    vector <Point> angles;

    for (auto it = obs.begin(); it != obs.end(); it++) {
        angles.push_back((*it).end());
    }
    
    return angles;
}

vector <Point> find_angles(list <Point> obs) {
    list <Point>::iterator iter;
    vector <Point> angels;
    float eps = 0.001;
    for (iter = obs.begin(); iter != obs.end(); iter++) {
        if (iter == obs.begin()) {
            Point iter_next = *(++iter);
            iter--;
            Point iter_prev = obs.back();

            Point vec2((*iter).x() - iter_next.x(), (*iter).y() - iter_next.y());
            int x = iter_prev.x() - (*iter).x();
            int y = iter_prev.y() - (*iter).y();
            Point vec1(x, y);
            if (1-cos_between_points(vec1, vec2) > eps) {
                angels.push_back((*iter));
            }
        }
        else if (*iter == obs.back()) {
            Point iter_prev = *(--iter);
            iter++;
            Point iter_next = obs.front();
            Point vec1(iter_prev.x() - (*iter).x(), iter_prev.y() - (*iter).y());
            Point vec2((*iter).x() - iter_next.x(), (*iter).y() - iter_next.y());
            if (1 - cos_between_points(vec1, vec2) > eps) {
                angels.push_back((*iter));
            }
        }
        else {
            Point iter_prev = *(--iter);
            iter++;
            Point iter_next = *(++iter);
            iter--;
            Point vec1(iter_prev.x() - (*iter).x(), iter_prev.y() - (*iter).y());
            Point vec2((*iter).x() - iter_next.x(), (*iter).y() - iter_next.y());
            if (1 - cos_between_points(vec1, vec2) > eps) {
                angels.push_back((*iter));
            }
        }
    }
    return angels;
}

// ������������
list <Segment> trapezoidal_decomposition(vector <vector <string>> map, vector <Point> angles) 
{
    list <Segment> slices;
    int row_size = map[0].size(), column_size = map.size();
    for (auto angle = angles.begin(); angle != angles.end(); angle++)
    {
        int x = (*angle).x(), y = (*angle).y();
        // ����� ���� ������
        Point start;
        if (x == row_size-1 || map[y][x+1] == "100")
        {
            start = Point(x, y);
        }
        else
        {
            int i = x + 1, j = y;
            while (true) {
                if (map[j][i] == "100" || i == row_size - 1) {
                    start = Point(i, j);
                    break;
                }
                i++;
            }
        }
        
        // ����� ���� �����
        Point end;
        if (x == 0 || map[y][x - 1] == "100")
        {
            end = Point(x, y);
        }
        else
        {
            int i = x - 1, j = y;
            while (true) {
                if (map[j][i] == "100" || i == 0) {
                    end = Point(i, j);
                    break;
                }
                i--;
            }
        }

        slices.push_back(Segment(start, end));

        // ��� � �������������
        // ����� ���� ����
        if (y == column_size-1 || map[y+1][x] == "100")
        {
            start = Point(x, y);
        }
        else
        {
            int i = x, j = y + 1;
            while (true) {
                if (map[j][i] == "100" || j == column_size-1) {
                    start = Point(i, j);
                    break;
                }
                j++;
            }
        }

        // ����� ���� �����
        if (y == 0 || map[y-1][x] == "100")
        {
            end = Point(x, y);
        }
        else
        {
            int i = x, j = y - 1;
            while (true) {
                if (map[j][i] == "100" || j == 0) {
                    end = Point(i, j);
                    break;
                }
                j--;
            }
        }

        slices.push_back(Segment(start, end));
        
    }

    return slices;
}