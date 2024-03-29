#ifndef MOCHILA_H // Replace MOCHILA_H with the appropriate guard name for each file
#define MOCHILA_H
#include <iostream>
#include <vector>
#include </home/santihm/MH/p1/software/FUENTES/include/objeto.h>

using namespace std;

class Mochila{
    private:
        vector<Objeto> objeto;
        vector<bool> asignacion;
        int beneficio;
    public:
    Mochila();
    Mochila(vector<Objeto> objeto, vector<bool> asignacion);

    int Fitness();
    vector<Objeto> getObjeto() const;
    vector<bool> getAsignacion() const;
    int size() const;
    int getFitness() const;
    int getPeso() const;
    bool operator==(const Mochila& other) const;
    bool swap(int i, int j, int capacidad);
};
#endif // MOCHILA_H