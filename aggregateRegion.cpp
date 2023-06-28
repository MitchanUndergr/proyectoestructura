#include "Quad.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

int main() {
    Point topLeft(-180.0, -180.0);
    Point botRight(180.0, 180.0);

    Quad quadtree(topLeft, botRight);

    std::ifstream inputFile("C:/Users/user/Downloads/ESTRUCTURA/experimental3/datos20.txt");
    std::string line;

    std::vector<Node*> nodos;

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);

        std::string poblacionStr, latitudeStr, longitudStr;
        std::getline(ss, poblacionStr, ';');    // leer como cadena
        std::getline(ss, latitudeStr, ';');
        std::getline(ss, longitudStr, ';');

        int poblacion = std::stoi(poblacionStr);
        size_t commaPos1 = latitudeStr.find(',');
        if (commaPos1 != std::string::npos) {
            latitudeStr[commaPos1] = '.';
        }
        double latitud = std::stod(latitudeStr);

        size_t commaPos2 = longitudStr.find(',');
        if (commaPos2 != std::string::npos) {
            longitudStr[commaPos2] = '.';
        }
        double longitud = std::stod(longitudStr);

        Point point(latitud, longitud);
        Node* nuevoNode = new Node(point, poblacion);

        nodos.push_back(nuevoNode);
        quadtree.insert(nuevoNode);
    }

    Point regionCenter(0, 0);
    int regionRadius[] = {2, 30, 50, 100, 180};

    std::ofstream outputFile("C:/Users/user/Downloads/ESTRUCTURA/experimental3/aggregateRegion20.txt");

    if (outputFile.is_open()) {
        for (int j = 0; j < 5; ++j) {
            outputFile << "Region " << regionRadius[j] << "x" << regionRadius[j] << ":" << std::endl;
            for (int k = 0; k < 20; ++k) {
                std::cout << "Tiempo de ejecucion aggregateRegion 20 datos con centro P(0,0) y area del cuadrado " << regionRadius[j] << "x" << regionRadius[j] << " (iteracion " << (k + 1) << "):" << endl;
                auto start1 = std::chrono::system_clock::now();

                for (int i = 0; i < nodos.size(); ++i) {
                    quadtree.aggregateRegion(nodos, regionCenter, regionRadius[j]);
                }

                auto end1 = std::chrono::system_clock::now();
                std::chrono::duration<double, std::milli> duration = end1 - start1;
                double tiempo = duration.count();
                cout << tiempo << "ms" << endl;

                outputFile << tiempo << endl;
            }
            outputFile << endl;
        }
        outputFile.close();
        cout << "Los tiempos se han guardado en el archivo aggregateRegion20.txt" << endl;
    } else {
        cout << "No se pudo abrir el archivo aggregateRegion20.txt" << endl;
    }

    for (const auto& nodo : nodos) {
        delete nodo;
    }

    return 0;
}
