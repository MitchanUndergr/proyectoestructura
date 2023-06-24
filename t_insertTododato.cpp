#include "Quad.h"
#include <chrono>
#include <random>
#include <vector>

int main() {
    Point topLeft(-120.0, -120.0);
    Point botRight(120.0, 120.0);

    Quad quadtree(topLeft, botRight);

    ifstream inputFile("C:/Users/user/Downloads/ESTRUCTURA/experimental/datosTodos.txt");
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
    }

    cout<<"tiempo ejecucion insert() todos los datos"<<endl;
    auto start1=chrono::system_clock::now();

    for (int i=0; i<nodos.size();i++) {
        quadtree.insert(nodos[i]);
    }
    auto end1=chrono::system_clock::now();
    chrono::duration<float,milli> duration=end1-start1;
    cout<<duration.count()<<"ms"<<endl;

    for (const auto& nodo : nodos) {
        delete nodo;
    }

    return 0;
}
