#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <unistd.h>

using namespace std;

typedef map <pair<string, string>, int> Dict;
typedef vector< vector<int> > matrix;

struct PPP {
	int Y;	// Cantidad de Yates
	int T; // Cantidad de Tiempos
	vector<int> vtrK; // Capacidad del yate i
	vector<int> vtrC; // Tripulación del yate i
};

string getCurrentDir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	return cwd;
}

int contarLineasArchivo(string nombre) {
	ifstream arch(nombre.c_str());
	int lin = 0;
	while(arch.good()) if(arch.get() == '\n') lin++;
	arch.close();
	return lin;
}

void printearMap(map<int,int> m) {
	for(map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
		cout << it->first << " => " << it->second << endl;
	cout << endl;
}

void printearVector(vector<int> v) {
	for(unsigned i = 0; i < v.size(); ++i) 
		cout << v[i] << "\t";
	cout << endl;
}

void printearMatriz(matrix m){
	cout << "\t";
	for (int i = 0; i < m[0].size(); ++i)
		cout << "T" << i << "\t";
	
	cout << endl;
	for (int i = 0; i < m.size(); ++i) {
		cout << "G" << i << "\t";
		for (int j = 0; j < m[i].size(); ++j)
			cout << m[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}

int abs(int x) {
	if (x > 0)
		return x;
	else
		return -x;
}

vector< vector<int> > inicializarMatriz(int valorInicial, int filas, int columnas) {
	vector< vector<int> > matriz;

	for (int i = 0; i < filas; ++i) {
		vector<int> fila;

		for (int j = 0; j < columnas; ++j) 
			fila.push_back(valorInicial);

		matriz.push_back(fila);
	}	
	return matriz;
}

int getCapacidadYate(int yate, vector<int> vtrK) {
	return vtrK[yate];
}


int getTripulacionYate(int yate, vector<int> vtrC) {
	return vtrC[yate];
}

int exist(int n, vector<int> v) {
	for(unsigned i = 0; i < v.size(); ++i) {
		if (n == v[i])
			return 1;
	}
	return 0;
}

int existMovTabu(pair<int, int> par, vector<pair<int, int> > v) {
	for(unsigned i = 0; i < v.size(); ++i) {
		if (par == v[i])
			return 1;
	}
	return 0;
}

// Retorna una lista de tuplas (n,k) con el elemento n en la posicion k del vector v
vector< pair<int, int> > getNMayores(int n, vector<int> v) {
	vector<pair<int, int> > retorno;
	vector<int> vCopia = v, mayoresYaRevisados;

	int posicion;
	for(unsigned i = 0; i < n; ++i) {
		int mayor = -9999;
		for(unsigned j = 0; j < v.size(); ++j) {
			if (!exist(j, mayoresYaRevisados) && mayor < v[j]) {
				mayor = v[j];
				posicion = j;		
			}
		}
		//cout << "(" << mayor << ", " << posicion << "),\t";
		retorno.push_back(make_pair(mayor, posicion));
		mayoresYaRevisados.push_back(posicion);
	}
	
	return retorno;
}

map<int, int> getPosicionGuests(int Y, vector<int> hosts) {
	map<int, int> retorno;
	int contador = 0;
	for(unsigned i = 0; i < Y; ++i) {
		if (!exist(i, hosts)) {
			retorno[contador] = i;
			contador++;
		}
	}
	return retorno;
}