#include </home/santihm/MH/p1/software/FUENTES/include/mochila.h>

using namespace std;

Mochila::Mochila() {}

Mochila::Mochila(vector<Objeto> objeto, vector<bool> asignacion) {
    this->objeto = objeto;
    this->asignacion = asignacion;
    beneficio = 0;
    for (int i = 0; i < objeto.size(); i++) {
      if (asignacion[i]) {
        beneficio += objeto[i].getB_Individual();
        for (int j = 0; j < objeto.size(); j++) {
          beneficio += objeto[i].getB_Interdependencias(j) * asignacion[j];
        }
      }
    }
}

int Mochila::Fitness() {
    for (int i = 0; i < objeto.size(); i++) {
      if (asignacion[i]) {
        beneficio += objeto[i].getB_Individual();
        for (int j = 0; j < objeto.size(); j++) {
          beneficio += objeto[i].getB_Interdependencias(j) * asignacion[j];
        }
      }
    }
    return beneficio;
}

vector<Objeto> Mochila::getObjeto() const { return objeto; }

vector<bool> Mochila::getAsignacion() const { return asignacion; }

int Mochila::getFitness() const { return beneficio; }

bool Mochila::operator==(const Mochila& other) const {
    return objeto == other.objeto && asignacion == other.asignacion;
}

int Mochila::getPeso() const{
  int peso = 0;
  for (int i=0; i<objeto.size(); i++){
    if (asignacion[i]){
      peso += objeto[i].getPeso();
    }
  }
  return peso;
}

int Mochila::size() const{
  return asignacion.size();
}

bool Mochila::swap(int i, int j, int capacidad){
  bool se_pudo = false;
  vector<bool> v = asignacion;
  v[i] = !v[i];
  v[j] = !v[j];
  Mochila nueva(objeto, v);
  if (nueva.getPeso() <= capacidad){
    asignacion = v;
    se_pudo = true;
  }
  return se_pudo;
}