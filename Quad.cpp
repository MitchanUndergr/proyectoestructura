#include "Quad.h"

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

