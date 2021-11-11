#include <GL/glut.h>
#include <bits/stdc++.h>

using namespace std;

#define polygon vector<Point>

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

struct Point {
    int x, y;
    int id;
    int flag; // 0：普通点 1：入点 2：出点

};

void Polygon(polygon p, vector<double> b) {
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(b[0], b[1], b[2]);
    for (int i = 0; i < p.size(); i++) {
        glBegin(GL_LINES);
        glVertex2i(p[i].x, p[i].y);
        glVertex2i(p[(i + 1) % p.size()].x, p[(i + 1) % p.size()].y);
        glEnd();
    }
}

namespace GetLineCross {
    bool IsRectCross(const Point &p1, const Point &p2, const Point &q1, const Point &q2) {
        bool ret = min(p1.x, p2.x) <= max(q1.x, q2.x) &&
                   min(q1.x, q2.x) <= max(p1.x, p2.x) &&
                   min(p1.y, p2.y) <= max(q1.y, q2.y) &&
                   min(q1.y, q2.y) <= max(p1.y, p2.y);
        return ret;
    }

    bool IsLineSegmentCross(const Point &pFirst1, const Point &pFirst2, const Point &pSecond1, const Point &pSecond2) {
        long line1, line2;
        line1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
                pFirst2.x * (pFirst1.y - pSecond1.y) +
                pSecond1.x * (pFirst2.y - pFirst1.y);
        line2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
                pFirst2.x * (pFirst1.y - pSecond2.y) +
                pSecond2.x * (pFirst2.y - pFirst1.y);
        if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
            return false;

        line1 = pSecond1.x * (pFirst1.y - pSecond2.y) +
                pSecond2.x * (pSecond1.y - pFirst1.y) +
                pFirst1.x * (pSecond2.y - pSecond1.y);
        line2 = pSecond1.x * (pFirst2.y - pSecond2.y) +
                pSecond2.x * (pSecond1.y - pFirst2.y) +
                pFirst2.x * (pSecond2.y - pSecond1.y);
        if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
            return false;
        return true;
    }

    bool GetCrossPoint(const Point &p1, const Point &p2, const Point &q1, const Point &q2, int &x, int &y) {
        if (IsRectCross(p1, p2, q1, q2)) {
            if (IsLineSegmentCross(p1, p2, q1, q2)) {
                //求交点
                long tmpLeft, tmpRight;
                tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
                tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) -
                           p1.x * (p2.y - p1.y) * (q2.x - q1.x);

                x = (int) ((double) tmpRight / (double) tmpLeft);

                tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
                tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) -
                           q2.y * (q1.x - q2.x) * (p2.y - p1.y);
                y = (int) ((double) tmpRight / (double) tmpLeft);
                return true;
            }
        }
        return false;
    }
}

vector<polygon> Weiler_Atherton(polygon p, polygon q) {

    vector<Point> pv, qv; // 裁剪多边形顶点表和主多边形顶点表
    list<int> tempPv, tempQv; // 辅助链表
    list<int>::iterator it;
    for (int i = 0; i < p.size(); i++) pv.push_back({p[i].x, p[i].y, i, 0}), tempPv.push_back(i);
    for (int i = 0; i < q.size(); i++) qv.push_back({q[i].x, q[i].y, i, 0}), tempQv.push_back(i);
    // 求多边形交点并分成入点和出点存储到表内，并将入点和出点建立双向指针
    for (int i = 0; i < p.size(); i++) { // 扫描裁剪多边形边
        int poly1_next_idx = int((i + 1) % p.size());
        vector<Point> newPoint;
        for (int j = 0; j < q.size(); j++) { // 扫描主多边形边
            int poly2_next_idx = int((j + 1) % q.size());
            int x, y;
            if (GetLineCross::GetCrossPoint(p[i], p[poly1_next_idx], q[j], q[poly2_next_idx], x, y)) {
                Point point = Point{x, y, (int) pv.size(), 1};
                pv.push_back(point);
                newPoint.push_back(point);
            }
        }
        if(newPoint.size() == 0) continue ;
        sort(newPoint.begin(), newPoint.end(), [&](Point a, Point b) {
            return a.x < b.x;
        });
        if(pv[poly1_next_idx].x < pv[i].x) reverse(newPoint.begin(), newPoint.end());
        it = tempPv.begin();
        for(; it != tempPv.end(); it++) {
            if(*it == i) {
                ++it;
                for(int j = 0;j < newPoint.size(); j++) {
                    tempPv.insert(it, newPoint[j].id);
                }
                break ;
            }
        }
    }
    it = tempPv.begin();
    vector<Point> pvv(pv.size());
    for(int i = 0;i < pv.size(); i++) {
        pvv[pv[i].id] = pv[i];
    }
    for(int i = 0;i < pv.size(); i++) {
        pv[i] = pvv[*it];
        it++;
    }

    for (int i = 0; i < q.size(); i++) { // 扫描主多边形边
        int poly1_next_idx = int((i + 1) % q.size());
        vector<Point> newPoint;
        for (int j = 0; j < p.size(); j++) { // 扫描裁剪多边形边
            int poly2_next_idx = int((j + 1) % p.size());
            int x, y;
            if (GetLineCross::GetCrossPoint(q[i], q[poly1_next_idx], p[j], p[poly2_next_idx], x, y)) {
                Point point = Point{x, y, (int) qv.size(), 1};
                qv.push_back(point);
                newPoint.push_back(point);
            }
        }
        if(newPoint.size() == 0) continue ;
        sort(newPoint.begin(), newPoint.end(), [&](Point a, Point b) {
            return a.x < b.x;
        });
        if(qv[poly1_next_idx].x < qv[i].x) reverse(newPoint.begin(), newPoint.end());
        it = tempQv.begin();
        for(; it != tempQv.end(); it++) {
            if(*it == i) {
                ++it;
                for(int j = 0;j < newPoint.size(); j++) {
                    tempQv.insert(it, newPoint[j].id);
                }
                break ;
            }
        }
    }
    it = tempQv.begin();
    vector<Point> qvv(qv.size());
    for(int i = 0;i < qv.size(); i++) {
        qvv[qv[i].id] = qv[i];
    }
    for(int i = 0;i < qv.size(); i++) {
        qv[i] = qvv[*it];
        it++;
    }

//    Polygon(pv, vector<double>{1.0, 0, 0});
//    Polygon(qv, vector<double>{0, 1.0, 0});

    /*
     * 将交点插入表中，并分成入点1和出点2
     */
    map<pair<int, int>, int> mapPoint; // 交点映射
    map<int, int> mapNextP, mapNextQ; // 交点下表映射 / 双向指针
    int opt = 0;
    for (int i = 0; i < pv.size(); i++) {
        if (pv[i].flag == 1) {
            pv[i].flag = (opt ? 1 : 2);
            opt ^= 1;
            mapPoint[{pv[i].x, pv[i].y}] = pv[i].flag;
        }
    }
    for (int i = 0; i < qv.size(); i++) {
        if (qv[i].flag == 1) {
            qv[i].flag = mapPoint[{qv[i].x, qv[i].y}];
        }
    }

    for (int i = 0; i < pv.size(); i++) {
        for (int j = 0; j < qv.size(); j++) {
            if (pv[i].x == qv[j].x && pv[i].y == qv[j].y) {
                mapNextP[i] = j;
                mapNextQ[j] = i;
            }
        }
    }

    for (int i = 0; i < (int) pv.size(); i++) cout << pv[i].flag << " ";
    cout << endl;
    for (int i = 0; i < (int) qv.size(); i++) cout << qv[i].flag << " ";
    cout << endl;

    vector<polygon > ans(1); // 裁剪结果多边形顶点表
    map<pair<int, int>, int> vis;
    int num = 0;
    // 遍历
    while(true) {
        if(mapPoint.empty()) break ;

        // 随机一个入点为起点
        int start;
        for(int i = 0;i < qv.size(); i++) {
            if(vis[pair<int, int>{qv[i].x, qv[i].y}] == 1) continue ;
            int k = mapPoint[pair<int, int>{qv[i].x, qv[i].y}];
            if(k == 1) {
                start = i;
                break ;
            }
            else mapPoint.erase(pair<int, int>{qv[i].x, qv[i].y});
        }

        // 从入点start开始走
        opt = 0;
        while(true) {
            if((opt == 0 && vis[{qv[start].x, qv[start].y}] == 1) || (opt == 1 && vis[{pv[start].x, pv[start].y}] == 1)) {
                ans.resize(ans.size() + 1);
                num++;
                break ;
            }
            if(opt == 0) {
                ans[num].push_back(Point{qv[start].x, qv[start].y});
                mapPoint.erase({qv[start].x, qv[start].y});
                vis[{qv[start].x, qv[start].y}] = 1;
            }
            else {
                ans[num].push_back(Point{pv[start].x, pv[start].y});
                mapPoint.erase({pv[start].x, pv[start].y});
                vis[{pv[start].x, pv[start].y}] = 1;
            }
            int nxt = (start + 1);
            if(opt == 0) nxt %= (int)qv.size();
            else nxt %= (int)pv.size();
            if(opt == 0) { // 入点走主多边形
                if(qv[nxt].flag == 2) {
                    opt ^= 1;
                    start = mapNextQ[nxt];
                }
                else start = nxt;
            }
            else { // 出点走裁剪多边形
                if(pv[nxt].flag == 1) {
                    opt ^= 1;
                    start = mapNextP[nxt];
                }
                else start = nxt;
            }
        }
    }

    return ans;
}


void display() {
    vector<Point> polygon1{{30,  60},
                           {50,  130},
                           {90,  50},
                           {190, 60},
                           {120, 20}};

    vector<Point> polygon2{{30,  120},
                           {180, 40},
                           {170, 20},
                           {20,  100},
                           {60,  90}};
    Polygon(polygon1, vector<double>{1.0, 0, 0});
    Polygon(polygon2, vector<double>{0, 1.0, 0});
    vector<polygon> ans = Weiler_Atherton(polygon1, polygon2);
    for (auto res: ans) {

        Polygon(res, vector<double>{0, 0, 1.0});
    }
    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Ploygon_clip");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

/*
    vector<Point> polygon1{{50, 15},{75, 55}, {125, 15},{155, 60},{140, 120},{65,  135},{20,  75}};
    vector<Point> polygon2{{30, 30}, {30, 120}, {150, 120}, {150, 30}};

    vector<Point> polygon1{{30, 60}, {50, 130}, {90, 50}, {190, 60}, {120, 20}};
    vector<Point> polygon2{{30, 120}, {180, 40}, {170, 20}, {20, 100}, {60, 90}};
 */
