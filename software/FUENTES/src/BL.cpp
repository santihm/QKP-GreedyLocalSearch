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


using namespace std;
using Random = effolkronium::random_static;



vector<bool> solucionInicial(const vector<Objeto>& objeto, const int& n, int capacidad){
  vector<bool> s(n, false);
  vector<Objeto> o = objeto;
  int indice;
  bool cabe = true;
  while(!o.empty()){
    Random::shuffle(o);
    if (capacidad>=o[o.size()-1].getPeso() && s[o[o.size()-1].getIndice()]==false){
      capacidad -= o[o.size()-1].getPeso();
      s[o[o.size()-1].getIndice()] = true;
      o.pop_back();
    }
    else{
      o.pop_back();
    }
  }
  return s;
}


vector<pair<int, vector<vector<int>>>> VecinosPermutacion(const Mochila& actual, const int& W) {
    vector<pair<int, vector<vector<int>>>> vecinos;
    const vector<Objeto>& objeto = actual.getObjeto();
    const vector<bool>& v = actual.getAsignacion();

    for (int i = 0; i < objeto.size(); i++) {
        if (v[i]) {
            for (int j = 0; j < objeto.size(); j++) {
                if (!v[j] && i != j) {  // Evitar intercambiar un objeto consigo mismo
                    if (actual.getPeso() - objeto[i].getPeso() + objeto[j].getPeso() <= W) {
                        vecinos.push_back({objeto[i].getIndice(), {{objeto[j].getIndice()}}});
                    }
                }
            }
        }
    }
    return vecinos;
}






vector<pair<int, vector<vector<int>>>> VecinosDobleIntercambio(const Mochila& actual, const int& capacidad) {
    vector<pair<int, vector<vector<int>>>> vecinos = VecinosPermutacion(actual, capacidad);
    const vector<Objeto>& objeto = actual.getObjeto(); // Se corrige el acceso a las referencias
    const vector<bool>& v = actual.getAsignacion(); // Se corrige el acceso a las referencias

    for (int i = 0; i < vecinos.size(); i++) {
        vector<vector<int>>& ceros = vecinos[i].second;
        int objeto_actual = vecinos[i].first;
        for (int j = 0; j < ceros.size(); j++) {
            int objeto_cero = ceros[j][0];
            int capacidad_actual = capacidad - objeto[objeto_actual].getPeso() + objeto[objeto_cero].getPeso();
            for (int k = 0; k < objeto.size(); k++) {
                if (!v[k] && k != objeto_actual && k != objeto_cero) {
                    int nuevo_peso = capacidad_actual + objeto[k].getPeso();
                    if (nuevo_peso >= 0 && nuevo_peso <= capacidad) {
                        vector<int> grupo = ceros[j];
                        if (grupo.size() < 2) { // Control de tamaño máximo del grupo
                            grupo.push_back(k);
                            vecinos[i].second.push_back(grupo);
                        }
                    }
                }
            }
        }
    }
    return vecinos;
}


void Permutacion(vector<bool>& v, const int& p1, const int& p0){
  //cout << "Antes permutacion" << endl;
  v[p1] = false;
  v[p0] = true;
  //cout << "Después permutacion" << endl;
}

void Intercambio(vector<bool>&v, const int&p0, const vector<int>& p1){
  v[p0] = !v[p0];
  for (int i=0; i<p1.size(); i++){
    v[p1[i]] = !v[p1[i]];
  }
}


using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocal(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos) {
    vector<bool> mejor_asignacion = actual.getAsignacion();
    int mejor_fitness = actual.getFitness();
    int nueva_fitness;
    int capacidad = W;
    n_instancias = 0;
    vector<Objeto> objeto = actual.getObjeto();
    maximo_local = false;

    while (n_instancias < max_instancias && !maximo_local) {
        vector<pair<int, vector<vector<int>>>> vecinos = Vecinos(actual, capacidad);
        
        bool mejora_encontrada = false;
        // Barajar los vecinos
        Random::shuffle(vecinos);
        
        for (auto& vecino : vecinos) {
            int indice_objeto = vecino.first;
            vector<vector<int>>& intercambios = vecino.second;
            
            // Barajar los intercambios
            Random::shuffle(intercambios);
            
            for (auto& intercambio : intercambios) {
                vector<bool> v = actual.getAsignacion();
                Intercambio(v, indice_objeto, intercambio);
                Mochila nueva(objeto, v);
                
                if (nueva.getFitness() > mejor_fitness) {
                    mejor_asignacion = v;
                    mejor_fitness = nueva.getFitness();
                    actual = nueva;
                    mejora_encontrada = true;
                    //cout << mejor_fitness << endl;
                    break;  // Salir del bucle de intercambios
                }
            }
            if (mejora_encontrada) {
                break;  // Salir del bucle de vecinos
            }
        }
        
        if (!mejora_encontrada) {
            maximo_local = true; // No se encontró mejora en esta iteración
        }
        
        ++n_instancias;
    }
    
    return mejor_asignacion;
}



using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocalMejor(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                                 bool& maximo_local, VecinosFunc Vecinos) {
    vector<bool> mejor_asignacion = actual.getAsignacion();
    int mejor_fitness = actual.getFitness();
    int capacidad = W;
    n_instancias = 0;
    vector<Objeto> objeto = actual.getObjeto();
    maximo_local = false;

    while (n_instancias < max_instancias && !maximo_local) {
        vector<pair<int, vector<vector<int>>>> vecinos = Vecinos(actual, capacidad);

        bool mejora_encontrada = false;
        Mochila mejor_mochila = actual; // Mantener un seguimiento de la mejor mochila encontrada en esta iteración

        for (const auto& vecino : vecinos) {
            const int indice_objeto = vecino.first;
            const vector<vector<int>>& intercambios = vecino.second;

            for (const auto& intercambio : intercambios) {
                vector<bool> v = actual.getAsignacion();
                Intercambio(v, indice_objeto, intercambio); // Función para aplicar intercambio
                Mochila nueva(objeto, v);

                if (nueva.getFitness() > mejor_fitness) { // Verificar si mejora el fitness
                    mejor_asignacion = v;
                    mejor_fitness = nueva.getFitness();
                    mejor_mochila = nueva; // Actualizar la mejor mochila encontrada
                    mejora_encontrada = true;
                    //cout << mejor_fitness << endl;
                }
            }
        }

        if (mejora_encontrada) {
            actual = mejor_mochila; // Actualizar el estado de la mochila solo si se encontró una mejora
        } else {
            maximo_local = true; // No se encontró mejora en esta iteración
        }

        ++n_instancias;
    }

    return mejor_asignacion;
}



