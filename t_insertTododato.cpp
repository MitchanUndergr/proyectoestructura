#include "Quad.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

int main() {
    std::vector<double> tiempos;

    for (int k = 0; k < 20; ++k) {
        Point topLeft(-120.0, -120.0);
        Point botRight(120.0, 120.0);

        Quad quadtree(topLeft, botRight);

        std::ifstream inputFile("C:/Users/user/Downloads/ESTRUCTURA/experimental/datosTodos.txt");
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
        }

        std::cout << "Tiempo de ejecucion insert() todos los datos (iteracion " << (k + 1) << "):" << std::endl;
        auto start1 = std::chrono::system_clock::now();

        for (int i = 0; i < nodos.size(); ++i) {
            quadtree.insert(nodos[i]);
        }

        auto end1 = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> duration = end1 - start1;
        double tiempo = duration.count();
        std::cout << tiempo << "ms" << std::endl;

        tiempos.push_back(tiempo);

        for (const auto& nodo : nodos) {
            delete nodo;
        }
    }

    std::ofstream outputFile("tiempos.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < tiempos.size(); ++i) {
            outputFile << "Iteración " << (i + 1) << ": " << tiempos[i] << "ms" << std::endl;
        }
        outputFile.close();
        std::cout << "Los tiempos se han guardado en el archivo tiempos.txt" << std::endl;
    } else {
        std::cout << "No se pudo abrir el archivo tiempos.txt" << std::endl;
    }

    return 0;
}
