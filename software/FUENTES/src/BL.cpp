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


vector<pair<int, vector<vector<int>>>> VecinosPermutacion(const Mochila& actual, const int& capacidad) {
    vector<pair<int, vector<vector<int>>>> vecinos;
    const vector<Objeto>& objeto = actual.getObjeto();
    const vector<bool>& v = actual.getAsignacion();
    for (int i = 0; i < objeto.size(); i++) {
        if (!v[i]) {
            for (int j = 0; j < objeto.size(); j++) {
                if (v[j] && (capacidad + objeto[j].getPeso() - objeto[i].getPeso()) >= 0) {
                    vecinos.push_back({objeto[j].getIndice(), {{objeto[i].getIndice()}}});
                }
            }
        }
    }
    return vecinos;
}




vector<pair<int, vector<vector<int>>>> VecinosDobleIntercambio(const Mochila& actual, const int& capacidad) {
    vector<pair<int, vector<vector<int>>>> vecinos = VecinosPermutacion(actual, capacidad);
    const vector<Objeto> objeto = actual.getObjeto();
    const vector<bool> v = actual.getAsignacion();
    for (int i = 0; i < vecinos.size(); i++) {
        vector<vector<int>>& ceros = vecinos[i].second;
        for (int j = 0; j < ceros.size(); j++) {
            int capacidad_actual = capacidad - objeto[vecinos[i].first].getPeso() + objeto[ceros[j][0]].getPeso();
            for (int k = 0; k < objeto.size(); k++) {
                if (!v[k] && k != ceros[j][0] && capacidad_actual + objeto[k].getPeso() >= 0) {
                    vector<int> grupo = ceros[j];
                    if (grupo.size() < 2) { // Control de tamaño máximo del grupo
                        grupo.push_back(k);
                        vecinos[i].second.push_back(grupo);
                    }
                }
            }
        }
    }
    return vecinos;
}


vector<pair<int, vector<vector<int>>>> VecinosTripleIntercambio(const Mochila& actual, const int& capacidad){
  vector<pair<int, vector<vector<int>>>> vecinos = VecinosDobleIntercambio(actual, capacidad);
  const vector<Objeto> objeto = actual.getObjeto();
  const vector<bool> v = actual.getAsignacion();
  int capacidad_actual;
  bool encontrado;
  for (int i=0; i<vecinos.size(); i++){
    vector<vector<int>> ceros = vecinos[i].second;
    for (int j=0; j<ceros.size(); j++){
        if (ceros[j].size()>1){
            encontrado = false;
            capacidad_actual = (capacidad - objeto[vecinos[i].first].getPeso() + objeto[ceros[j][0]].getPeso() + objeto[ceros[j][1]].getPeso());
            for (int k=0; k<objeto.size() && !encontrado; k++){
                if (!v[k] && k!=ceros[j][0] && k!=ceros[j][1] && (capacidad_actual + objeto[k].getPeso()) >= 0){
                    //cout << vecinos[i].second[j].size() << endl;
                    vector<int> grupo = ceros[j];
                    grupo.push_back(objeto[k].getIndice());
                    vecinos[i].second.push_back(grupo);
                    encontrado = true;
                }
            }
        }
    }
  }
  return vecinos;
}

/*
vector<pair<int, vector<vector<int>>>> VecinosTripleIntercambio(const Mochila& actual, const int& capacidad){
  vector<pair<int, vector<vector<int>>>> vecinos = VecinosDobleIntercambio(actual, capacidad);
  const vector<Objeto> objeto = actual.getObjeto();
  const vector<bool> v = actual.getAsignacion();
  int capacidad_actual = capacidad;
  for (int i=0; i<vecinos.size(); i++){
    for (int j=0; j<objeto.size(); j++){
      capacidad_actual = capacidad;
      capacidad_actual -= vecinos[i].first;
      for (int k=0; k<vecinos[i].second[vecinos[i].second.size()-1].size(); k++){
        capacidad_actual -= objeto[vecinos[i].second[vecinos[i].second.size()-1][k]].getPeso();
      }
      if (!v[j] && (capacidad  + objeto[j].getPeso()) >= 0){
        vector<int> grupo = vecinos[i].second[vecinos[i].second.size()-1];
        grupo.push_back(j);
        vecinos[i].second.push_back(grupo);
      }
    }
  }

  return vecinos;
}
*/



void Permutacion(vector<bool>& v, const int& p1, const int& p0){
  //cout << "Antes permutacion" << endl;
  v[p1] = false;
  v[p0] = true;
  //cout << "Después permutacion" << endl;
}

void Intercambio(vector<bool>&v, const int&p0, const vector<int>& p1){
  v[p0] = false;
  for (int i=0; i<p1.size(); i++){
    v[p1[i]] = true;
  }
}

using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocal(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos){
  vector<bool> v = actual.getAsignacion();
  int capacidad = W;
  n_instancias = 0;
  vector<Objeto> objeto = actual.getObjeto();
  maximo_local = false;
  vector<pair<int, vector<vector<int>>>> vecinos;
  Mochila nueva;
  //cout << actual.getFitness()<< endl;
  
  vecinos = Vecinos(actual, capacidad);
  v = actual.getAsignacion();
  Random::shuffle(vecinos);
  Random::shuffle(vecinos[vecinos.size()-1].second);
  for (n_instancias; n_instancias<max_instancias && !maximo_local; n_instancias++){
    Intercambio(v, vecinos[vecinos.size()-1].first, vecinos[vecinos.size()-1].second[vecinos[vecinos.size()-1].second.size()-1]);
    Mochila nueva(objeto, v);
    if (nueva.getFitness()>actual.getFitness()){
      actual = nueva;
      capacidad = W - actual.getPeso();
      v = actual.getAsignacion();
      vecinos = Vecinos(actual, capacidad);
      Random::shuffle(vecinos);
      if (!vecinos.empty()) {
        Random::shuffle(vecinos[vecinos.size() - 1].second);
      }
    }
    else{
      vecinos[vecinos.size()-1].second.pop_back();
    }
    if (!vecinos.empty()){
      if(vecinos[vecinos.size()-1].second.size() == 0){
        vecinos.pop_back();
      }
    }
    //cout << actual.getFitness()<< endl;
    maximo_local = (vecinos.size() == 0);
  }
  return actual.getAsignacion();
}



using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocalFact(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos){
  vector<bool> v = actual.getAsignacion();
  int capacidad = W;
  n_instancias = 0;
  vector<Objeto> objeto = actual.getObjeto();
  maximo_local = false;
  vector<pair<int, vector<vector<int>>>> vecinos;
  Mochila nueva;
  int fitness_actual =actual.getFitness();
  //cout << actual.getFitness()<< endl;
  /*
  vecinos = Vecinos(actual, capacidad);
  v = actual.getAsignacion();
  Random::shuffle(vecinos);
  Random::shuffle(vecinos[vecinos.size()-1].second);
  for (n_instancias; n_instancias<max_instancias && !maximo_local; n_instancias++){
    Intercambio(v, vecinos[vecinos.size()-1].first, vecinos[vecinos.size()-1].second[vecinos[vecinos.size()-1].second.size()-1]);
    fitness_actual -= objeto[vecinos[vecinos.size()-1].first].getB_Individual();
    for (int i=0; i<objeto.size(); i++){}
    if (nueva.getFitness()>actual.getFitness()){
      Mochila nueva(objeto, v);
      actual = nueva;
      capacidad = W - actual.getPeso();
      v = actual.getAsignacion();
      vecinos = Vecinos(actual, capacidad);
      Random::shuffle(vecinos);
      if (!vecinos.empty()) {
        Random::shuffle(vecinos[vecinos.size() - 1].second);
      }
    }
    else{
      vecinos[vecinos.size()-1].second.pop_back();
    }
    if (!vecinos.empty()){
      if(vecinos[vecinos.size()-1].second.size() == 0){
        vecinos.pop_back();
      }
    }
    //cout << actual.getFitness()<< endl;
    maximo_local = (vecinos.size() == 0);
  }
  */
  return actual.getAsignacion();
}



using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);
vector<bool> busquedaLocalMejor(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos){
  vector<bool> v = actual.getAsignacion();
  int capacidad = W;
  n_instancias = 0;
  vector<Objeto> objeto = actual.getObjeto();
  maximo_local = false;
  vector<pair<int, vector<vector<int>>>> vecinos;
  Mochila nueva;
  
  vecinos = Vecinos(actual, capacidad);
  v = actual.getAsignacion();
  while (!maximo_local){
    Random::shuffle(vecinos);
    Mochila mejor_vecino = actual;
    int mejor_beneficio = 0;
    vector<bool> asignacion_vecino = v;
    int beneficio_vecino;
    for (int j=0; j<vecinos[vecinos.size()-1].second.size(); j++){
      asignacion_vecino = v;
      Intercambio(asignacion_vecino, vecinos[vecinos.size()-1].first, vecinos[vecinos.size()-1].second[j]);
      Mochila vecino(objeto, asignacion_vecino);
      beneficio_vecino = vecino.getFitness();
      if (beneficio_vecino > mejor_beneficio){
        mejor_vecino = vecino;
        mejor_beneficio = beneficio_vecino;
      }
    }
    vecinos.pop_back();
    if (mejor_beneficio>actual.getFitness()){
      actual = mejor_vecino;
      capacidad = W - actual.getPeso();
      v = actual.getAsignacion();
      vecinos = Vecinos(actual, capacidad);
    }
    //cout << actual.getFitness()<< endl;
    maximo_local = (vecinos.size() == 0);
  }
  return actual.getAsignacion();
}