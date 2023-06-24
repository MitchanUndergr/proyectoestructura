#include "Quad.h"
#include <chrono>
#include <random>
#include <vector>

int main() {
    Point topLeft(-120.0, -120.0);
    Point botRight(120.0, 120.0);

    Quad quadtree(topLeft, botRight);

    ifstream inputFile("C:/Users/user/Downloads/ESTRUCTURA/Experimental2/datos20.txt");
    string line;

    vector<Node*> nodos;

    while (getline(inputFile, line)) {
        stringstream ss(line);

        string poblacionStr, latitudeStr, longitudStr;
        getline(ss, poblacionStr, ';');    // leer como cadena
        getline(ss, latitudeStr, ';');
        getline(ss, longitudStr, ';');

        int poblacion = stoi(poblacionStr);
        size_t commaPos1 = latitudeStr.find(',');
        if (commaPos1 != string::npos) {
            latitudeStr[commaPos1] = '.';
        }
        double latitud = stod(latitudeStr);

        size_t commaPos2 = longitudStr.find(',');
        if (commaPos2 != string::npos) {
            longitudStr[commaPos2] = '.';
        }
        double longitud = stod(longitudStr);

        Point point(latitud, longitud);
        Node* nuevoNode = new Node(point, poblacion);

        nodos.push_back(nuevoNode);
        quadtree.insert(nuevoNode);
    }


    Point regionCenter(0, 0);
    int regionRadius[] = {2,30,50, 100, 180};

    for(int j=0; j<5;j++) {

        cout << "tiempo ejecucion countRegion 20 datos con centro P(0,0) y area del cuadrado "<<regionRadius[j]<< "x" <<regionRadius[j] << endl;
        auto start1 = chrono::system_clock::now();

        for (int i = 0; i < nodos.size(); i++) {
            quadtree.countRegion(nodos, regionCenter, regionRadius[j]);
        }
        auto end1 = chrono::system_clock::now();
        chrono::duration<float, milli> duration = end1 - start1;
        cout << duration.count() << "ms" << endl;

    }
    for (const auto& nodo : nodos) {
        delete nodo;
    }

    return 0;
}