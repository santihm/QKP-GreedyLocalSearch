#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "../tools/random.hpp"
#include <chrono>
#include <cmath>
#include <random>
#include <unordered_set>
#include <utility>
#include <cstring>
#include <chrono>
#include <ctime>
#include "../include/BL.h"
#include "../include/greedy.h"
#include "../include/mochila.h"
#include "../include/objeto.h"




using namespace std;
using namespace std::chrono;
using Random = effolkronium::random_static;

vector<vector<int>> convertir_matriz_simetrica(const vector<vector<int>>& b_interdependencias) {
  int n = b_interdependencias.size();
  vector<vector<int>> matriz_simetrica(n, vector<int>(n, 0));

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      matriz_simetrica[i][j] = b_interdependencias[i][j - i - 1];
      matriz_simetrica[j][i] = matriz_simetrica[i][j]; // Fill the symmetric counterpart
    }
  }

  return matriz_simetrica;
}




int main(int argc, char* argv[]){

  if (argc <= 2) {
    cerr << "Error: Se requieren argumentos. Ayuda:" << endl;
    cout << "$ejecutable $algoritmo(G o BL) $archivo ($semilla)\n";
    return 1;
  }

  // Obtener el primer argumento
  string algoritmo = argv[1];

  // Variable booleana

  // Evaluar el argumento
  if (!(algoritmo == "BL" || algoritmo == "BL-DOBLE" || algoritmo == "BL-MEJOR" || algoritmo == "BL-MEJOR-DOBLE" || algoritmo == "G")) {
    cerr << "Error: Argumento no válido. Debe ser 'BL' o 'G'. Ayuda: " << endl;
    cout << "$ejecutable $algoritmo(G o BL) $archivo ($semilla)" << endl;
    return 1;
  }

    int n; //Nº objetos
    vector<int> peso;
    vector<vector<int >> b_interdependencias;
    vector<int> b_individual;
    

    int W; // capacidad
    char * ruta_fichero(argv[2]);
    ifstream f;
    f.open(ruta_fichero);

    if (f.fail()) {
    cerr << "Error al abrir el fichero." << endl;
     return 1;
    }

    string linea;
    getline(f, linea);
    f >> n; // Lee tamaño

    int b;
    //Leemos beneficios individuales
    for (int i = 0; i < n; ++i) {
        int beneficio;
        f >> b;
        b_individual.push_back(b);
    }
    // Leemos beneficios por interdependicias
    for (int i = 0; i < n; ++i) {
        vector<int> fila;
        for (int j = i + 1; j < n; ++j) {
            f >> b;
            fila.push_back(b);
        }
        b_interdependencias.push_back(fila);
    }

    b_interdependencias = convertir_matriz_simetrica(b_interdependencias);

    f >> W;
    //Leemos capacidad mochila
    f >> W;
    //cout << "W: " << W << endl;

    //Leemos pesos
    for (int i = 0; i < n; ++i) {
        f >> b;
        peso.push_back(b);
    }

    //Cerramos fichero
    while (getline(f, linea)) {}
    f.close();


    
    vector<Objeto> objeto;
    for (int i=0; i<n; i++){
        Objeto o(i, b_individual[i], b_interdependencias[i], peso[i]);
        objeto.push_back(o);
    }

    int peso_final;
    int beneficio_final = 0;
    vector<bool> asignacion;
    int n_instancias;
    const int max_instancias = 90000;
    int max_grupos = 1;
    bool maximo_local;
    milliseconds tiempo;

    if (algoritmo == "G"){
      auto momentoInicio = high_resolution_clock::now();
      asignacion = greedy(objeto, n, W, peso_final);
      auto momentoFin = high_resolution_clock::now();
      tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
    }
    else{
      long int seed;

      if (argc <= 3) {
        //cout <<"Sin semilla establecida" <<endl;
      }
      else {
        seed = atoi(argv[3]);
        Random::seed(seed);
        //cout <<"Usando semilla: " <<seed <<endl;
      }
      vector<bool> s_inicial = solucionInicial(objeto, n, W);
      Mochila mochila(objeto, s_inicial);
      if (algoritmo == "BL"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = busquedaLocal(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BL-DOBLE"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = busquedaLocal(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosDobleIntercambio);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BL-MEJOR"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = busquedaLocalMejor(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosPermutacion);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      else if (algoritmo == "BL-MEJOR-DOBLE"){
        auto momentoInicio = high_resolution_clock::now();
        asignacion = busquedaLocalMejor(mochila, n, W, max_instancias, n_instancias, maximo_local, VecinosDobleIntercambio);
        auto momentoFin = high_resolution_clock::now();
        tiempo = duration_cast<std::chrono::milliseconds>(momentoFin - momentoInicio);
      }
      
    }
    Mochila mochila(objeto, asignacion);
    beneficio_final = mochila.getFitness();
    peso_final = mochila.getPeso();
    /*
    for (int i = 0; i < objeto.size(); i++) {
      if (asignacion[i]) {
        beneficio_final += objeto[i].getB_Individual();
        for (int j = 0; j < objeto.size(); j++) {
          beneficio_final += objeto[i].getB_Interdependencias(j) * asignacion[j];
        }
      }
    }
    */
    
    /*
    cout << "\nPeso final = " << peso_final;
    cout << "\nBeneficio final = " << beneficio_final << endl;
    cout << "Asignacion:\n";
    for (int i=0; i<asignacion.size(); i++){
        cout << asignacion[i] << " ";
    }
    cout << endl;
    if ((algoritmo=="BL" or algoritmo=="BL_DOBLE") && maximo_local){
      cout << "Se ha alcanzado un Máximo Local\n";
    }
    cout << endl;
    */
    
    
    
    cout << beneficio_final<< " " <<tiempo.count() <<endl;

}