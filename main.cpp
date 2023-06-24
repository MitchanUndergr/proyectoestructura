#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Point {
    double x;
    double y;
    Point(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};

struct Node {
    Point pos;
    int data;
    Node(Point _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    Node() { data = 0; }
};

class Quad {
    Point topLeft;
    Point botRight;

    Node* n;

    Quad* topLeftTree;
    Quad* topRightTree;
    Quad* botLeftTree;
    Quad* botRightTree;

public:
    Quad()
    {
        topLeft = Point(0, 0);
        botRight = Point(0, 0);
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
    }
    Quad(Point topL, Point botR)
    {
        n = NULL;
        topLeftTree = NULL;
        topRightTree = NULL;
        botLeftTree = NULL;
        botRightTree = NULL;
        topLeft = topL;
        botRight = botR;
    }
    void insert(Node*);
    Node* search(Point);
    bool inBoundary(Point);
    void BoundingBox(Point p, int d, Point& topLeft, Point& botRight);
    int totalPoints();
    int totalNodes();
    vector<Node *> List();
    int countRegion(vector<Node*>& nodes, Point p, int d);
    int aggregateRegion(vector<Node*>& nodes, Point p, int d);

};

void Quad::insert(Node* node){
    if (node == NULL)
        return;

    if (!inBoundary(node->pos))
        return;

    if (abs(topLeft.x - botRight.x) <= 1
        && abs(topLeft.y - botRight.y) <= 1) {
        if (n == NULL)
            n = node;
        return;
    }

    if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topLeftTree == NULL)
                topLeftTree = new Quad(
                        Point(topLeft.x, topLeft.y),
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        }


        else {
            if (botLeftTree == NULL)
                botLeftTree = new Quad(
                        Point(topLeft.x,
                              (topLeft.y + botRight.y) / 2),
                        Point((topLeft.x + botRight.x) / 2,
                              botRight.y));
            botLeftTree->insert(node);
        }
    }
    else {
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topRightTree == NULL)
                topRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                              topLeft.y),
                        Point(botRight.x,
                              (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        }

        else {
            if (botRightTree == NULL)
                botRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2),
                        Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}

Node* Quad::search(Point p){
    if (!inBoundary(p))
        return NULL;

    if (n != NULL)
        return n;

    if ((topLeft.x + botRight.x) / 2 >= p.x) {
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topLeftTree == NULL)
                return NULL;
            return topLeftTree->search(p);
        }

        else {
            if (botLeftTree == NULL)
                return NULL;
            return botLeftTree->search(p);
        }
    }
    else {
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topRightTree == NULL)
                return NULL;
            return topRightTree->search(p);
        }

        else {
            if (botRightTree == NULL)
                return NULL;
            return botRightTree->search(p);
        }
    }
};

bool Quad::inBoundary(Point p){
    return (p.x >= topLeft.x && p.x <= botRight.x
            && p.y >= topLeft.y && p.y <= botRight.y);
}

void Quad::BoundingBox(Point p, int d, Point& topLeft, Point& botRight) {
    topLeft.x = p.x - d;
    //cout<<topLeft.x<<endl;
    botRight.x = p.x + d;
    //cout<<botRight.x<<endl;

    topLeft.y = p.y - d;
    //cout<<topLeft.y<<endl;
    botRight.y = p.y + d;
    //cout<<botRight.y<<endl;
}



int Quad::totalPoints(){
    int count = 0;

    if (n != NULL)
        count++;

    if (topLeftTree != NULL)
        count += topLeftTree->totalPoints();
    if (topRightTree != NULL)
        count += topRightTree->totalPoints();
    if (botLeftTree != NULL)
        count += botLeftTree->totalPoints();
    if (botRightTree != NULL)
        count += botRightTree->totalPoints();

    return count;
}

int Quad::totalNodes(){
    int count = 1;

    if (topLeftTree != NULL)
        count += topLeftTree->totalNodes();
    if (topRightTree != NULL)
        count += topRightTree->totalNodes();
    if (botLeftTree != NULL)
        count += botLeftTree->totalNodes();
    if (botRightTree != NULL)
        count += botRightTree->totalNodes();

    return count;
}

vector<Node*> Quad::List() {
    vector<Node*> nodes;

    if (n != NULL) {
        nodes.push_back(n);
    }

    if (topLeftTree != NULL) {
        vector<Node*> topLeftNodes = topLeftTree->List();
        nodes.insert(nodes.end(), topLeftNodes.begin(), topLeftNodes.end());
    }
    if (topRightTree != NULL) {
        vector<Node*> topRightNodes = topRightTree->List();
        nodes.insert(nodes.end(), topRightNodes.begin(), topRightNodes.end());
    }
    if (botLeftTree != NULL) {
        vector<Node*> botLeftNodes = botLeftTree->List();
        nodes.insert(nodes.end(), botLeftNodes.begin(), botLeftNodes.end());
    }
    if (botRightTree != NULL) {
        vector<Node*> botRightNodes = botRightTree->List();
        nodes.insert(nodes.end(), botRightNodes.begin(), botRightNodes.end());
    }

    return nodes;
}



int Quad::countRegion(vector<Node*>& nodes, Point p, int d) {
    int count = 0;
    Point topLeft, botRight;
    BoundingBox(p, d, topLeft, botRight);

    for (const auto& node : nodes) {
        if (node->pos.x >= topLeft.x && node->pos.x <= botRight.x &&
            node->pos.y >= topLeft.y && node->pos.y <= botRight.y) {
            count++;
        }
    }

    return count;
}


int Quad::aggregateRegion(vector<Node*>& nodes, Point p, int d) {
    int suma_poblacion = 0;
    Point topLeft, botRight;
    BoundingBox(p, d, topLeft, botRight);

    for (const auto& node : nodes) {
        if (node->pos.x >= topLeft.x && node->pos.x <= botRight.x &&
            node->pos.y >= topLeft.y && node->pos.y <= botRight.y) {
            suma_poblacion += node->data;
        }
    }

    return suma_poblacion;
}



int main()
{
    Point topLeft(-120.0, -120.0);
    Point botRight(120.0, 120.0);

    Quad center(topLeft, botRight);


    ifstream inputFile("C:/Users/user/Downloads/ESTRUCTURA/proyecto/datos.txt");
    string line;

    while (getline(inputFile, line)) {
        stringstream ss(line);

        string poblacionStr, latitudeStr, longitudStr;
        getline(ss, poblacionStr, ';');    // leer como cadena
        getline(ss, latitudeStr, ';');
        getline(ss, longitudStr, ';');

        int poblacion = stoi(poblacionStr);
        //cout<<population<<endl;
        double latitud = stod(latitudeStr.replace(latitudeStr.find(','), 1, "."));    // reemplazar ',' por '.'
        //cout<<latitude<<endl;
        double longitud = stod(longitudStr.replace(longitudStr.find(','), 1, "."));
        //cout<<longitude<<endl;

        Point point(latitud, longitud);
        Node* nuevoNode = new Node(point, poblacion);

        center.insert(nuevoNode);
    }


    cout<<endl;

    vector<Node*> nodos = center.List();
    cout << "Lista de nodos en el QuadTree:" << endl;

    for (const auto& point : nodos) {
        cout << "Georeferencia: (" << point->pos.x << ", " << point->pos.y << "), Poblacion: " << point->data << endl;
    }


    cout<<endl;

    double x=40.9204;
    double y=39.1921;
    Point searchPoint(x,y);
    Node* result = center.search(searchPoint);
    if (result != NULL) {
        cout << "Poblacion encontrada: " << result->data << " en el punto P("<<x<<","<<y<<")"<< endl;
    } else {
        cout << "Poblacion no encontrada." << endl;
    }

    cout<<endl;

    int numPoints = center.totalPoints();
    int numNodes = center.totalNodes();
    cout << "Cantidad de puntos en el QuadTree: " << numPoints << endl;
    cout << "Cantidad de nodos en el QuadTree: " << numNodes << endl;

    cout<<endl;

    Point regionCenter(30, 30);
    int regionRadius = 20;
    int numPointsInRegion = center.countRegion(nodos,regionCenter, regionRadius);
    cout << "Cantidad de puntos en la region: " << numPointsInRegion << endl;
    cout<<endl;

    int populationInRegion = center.aggregateRegion(nodos,regionCenter, regionRadius);
    cout << "Poblacion estimada en la region: " << populationInRegion << endl;

    return 0;
}

