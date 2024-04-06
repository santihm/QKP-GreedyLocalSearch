#ifndef BL_H
#define BL_H

#include <vector>
#include <utility> // for std::pair
#include "mochila.h"



vector<bool> solucionInicial(const vector<Objeto>& objeto, const int& n, int capacidad);
vector<pair<int, vector<vector<int>>>> VecinosPermutacion(const Mochila& actual, const int& capacidad);
vector<pair<int, vector<vector<int>>>> VecinosDobleIntercambio(const Mochila& actual, const int& capacidad);

void Permutacion(vector<bool>& v, const int& p1, const int& p0);
void Intercambio(vector<bool>& v, const int& p1, const vector<int>& p0);

using VecinosFunc = vector<pair<int, vector<vector<int>>>> (*)(const Mochila&, const int&);

vector<bool> busquedaLocal(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                            bool& maximo_local, VecinosFunc Vecinos);
                            


vector<bool> busquedaLocalMejor(Mochila& actual, const int n, const int W, const int& max_instancias, int& n_instancias,
                           bool& maximo_local, VecinosFunc Vecinos);



#endif // BL_H
