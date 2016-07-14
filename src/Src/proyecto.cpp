#include "utilidades.cpp"

/*
 * Lee el archivo con formato:
 * Y
 * T
 * K_1,C_1;K_2,C_2;...;K_Y,c_Y
 *
 * In: Nombre del archivo (string)
 * Out: Tipo de dato PPP.
 *		PPP -> Y: Cantidad de Yates
 *		PPP -> Y: Cantidad de Yates
 *		PPP -> vtrK: Capacidad del Yate_i
 *		PPP -> vtrC: Tripulación del Yate_i
 */
PPP leerArchivo(string archivo) {
	ifstream arch;
	int Y, T;
	string config;
	
	arch.open(archivo.c_str());

	if (!arch.good()) {
		cout << "El archivo: '" << archivo << "' no existe. Saliendo del programa." << endl << endl;
		exit(1);
	}

	arch >> Y >> T >> config;
	arch.close();

	string separadorPuntoComa, separadorComa;
	istringstream streamPuntoComa(config);
	vector<int> vtrK, vtrC;

	// Spliteando por punto y coma la tercera línea
	while(getline(streamPuntoComa, separadorPuntoComa, ';')) {

		// Spliteando por coma cada config de un yate
		int i = 0;
		istringstream streamComa(separadorPuntoComa);
		while(getline(streamComa, separadorComa, ',')) {
			if (i == 0)
				vtrK.push_back(atoi(separadorComa.c_str()));
			else
				vtrC.push_back(atoi(separadorComa.c_str()));
			i++;
		}
	}
	PPP problema = {Y, T, vtrK, vtrC};
	
	return problema;
}

/* Lee el archivo de configuración para obtener el vector de hosts
 *
 * In: Nombre del archivo de configuración
 * Out: Mapeo del nombre de la configuración al vector de hosts asociado
 */
map<string, vector<int> > leerArchivoConfig(string archivo) {
	map<string, vector<int> > configuracionesHosts;
	ifstream fe(archivo.c_str());	
	vector<string> lectura;

	if (!fe.good()) {
		cout << "El archivo: '" << archivo << "' no existe. Saliendo del programa." << endl << endl;
		exit(1);
	}

	for(unsigned i = 0; i < 3 * contarLineasArchivo(archivo); ++i) {
		string palabra;
		fe >> palabra;
		lectura.push_back(palabra);
	}
	
	fe.close();

	// Se parsea a mano los string de hosts de las configuraciones a un vector de hosts
	for(unsigned j = 0; j < contarLineasArchivo(archivo); ++j) {
		string numero = "";
		int leyoGuion = 0;
		vector<int> hostsLinea;
		for(unsigned i = 0; i < lectura[3*j+2].size(); ++i) {
			
			if (lectura[3*j+2][i] == '-') {
				leyoGuion = 1;
				hostsLinea.push_back(atoi(numero.c_str())-1);
				numero = "";
				leyoGuion = 1;
			}
			else if (lectura[3*j+2][i] == ',') {
				if (leyoGuion) {
					for(unsigned i = hostsLinea.back()+1; i <= atoi(numero.c_str()); ++i)
						hostsLinea.push_back(i-1);
				}
				else
					hostsLinea.push_back(atoi(numero.c_str())-1);
				
				leyoGuion = 0;
				numero = "";			
			}
			else 
				numero += lectura[3*j+2][i];
		}

		if (leyoGuion) {
			for(unsigned i = hostsLinea.back()+1; i <= atoi(numero.c_str()); ++i)
				hostsLinea.push_back(i-1);
		}
		else
			hostsLinea.push_back(atoi(numero.c_str())-1);


		hostsLinea.erase(hostsLinea.begin());
		lectura[3*j].pop_back();

		configuracionesHosts[lectura[3*j]] = hostsLinea;
	}

	if (archivo == "Configuraciones.txt")
		configuracionesHosts["pp3"][0] = 0;

	return configuracionesHosts;
}

/* Restricción de que para todo x_{g,1}, x_{g,2}, ..., x_{g,T} sean diferentes
 * 
 * In: Matriz de enteros
 * Out: Cantidad de penalizaciones
 */
int allDiffPenalty(matrix matriz) {
	int contador = 0;
	for (int i = 0; i < matriz.size(); ++i) {
		for (unsigned j = 0; j < matriz[i].size(); ++j) {
			for (unsigned k = j + 1; k < matriz[i].size(); ++k) {
				if (matriz[i][j] == matriz[i][k])
					contador++;
			}	
		}	
	}



	return contador;
}

/* Restricción de que para todo par de invitados sólo se conozcan a lo más una vez
 * 
 * In: Matriz de enteros
 * Out: Cantidad de penalizaciones
 */
int crewMeetOncePenalty(matrix matriz) {
	int contador, penalizacion = 0;
	
	for(unsigned i = 0; i < matriz.size(); ++i) {

		for(unsigned j = i + 1; j < matriz.size(); ++j) {
			contador = 0;
			
			for(unsigned k = 0; k < matriz[0].size(); ++k) {
				if (matriz[i][k] == matriz[j][k])
					contador++;
			}

			if (contador <= 1)
				penalizacion += 0;
			else
				penalizacion += contador - 1;
		}
	}
		
	return penalizacion;
}

/* Restricción de que la tripulación a bordo no puede exceder de la capacidad del yate anfitrión
 * 
 * In: Anfitrion Hi (entero), diccionario de las posiciones de los yates invitados (map<int,int>),
 * 	   vectores de capacidades y tamaño tripulaciónvtrK y vtrC respectivamente, Matriz de enteros
 * Out: Cantidad de penalizaciones
 */
int hostCapacityPenalty(vector<int> hosts, map<int,int> posicionesInvitados, vector<int> vtrK, vector<int> vtrC, matrix matriz) {
	int sigma, tripulacion = 0, penalizacion = 0;

	// Para cada host (h) se verifica si en un tiempo T (en una columna) los valores x_{g,t} que
	// sean igual a h (para tiempo t la tripulacion g esta en h) no superen la capacidad de h
	for(unsigned h = 0; h < hosts.size(); ++h) {
		//cout << "Para h = " << hosts[h] << endl;
		for(unsigned i = 0; i < matriz[0].size(); ++i) {
			tripulacion = 0;
			//cout << "\tPara T" << i << endl;
			for(unsigned j = 0; j < matriz.size(); ++j) {
				if (matriz[j][i] == hosts[h]) {
					//cout << "\t\t(j,i) = (" << j << ", " << i << ")" << endl;
					tripulacion += getTripulacionYate(posicionesInvitados[j], vtrC);
				}
			}
			tripulacion += getTripulacionYate(hosts[h], vtrC);	// A la tripulación presente en el yate h se le añade la tripulación de h

			//cout << "\tPara T" << i << " hay a bordo: " << tripulacion << " con capacidad de " << getCapacidadYate(hosts[h], vtrK) << endl;
			//cout << endl;

			sigma = getCapacidadYate(hosts[h], vtrK) - tripulacion;

			if (sigma >= 0)
				penalizacion += 0;
			else
				//penalizacion += 1 + (abs(sigma) - 1) / 4;
				penalizacion += 1;
			
			//cout << "sigma = " << sigma <<" - penalizacion = " << penalizacion << endl;

		}
	}
	return penalizacion;
}

/* Retorna la suma de todas las penalizaciones */
int getAllPenalty(vector<int> hosts, map<int,int> posicionesInvitados, matrix matriz, PPP problema) {
	int allDif = allDiffPenalty(matriz);
	int meetOnlyOnce = crewMeetOncePenalty(matriz);
	int hostCapacity = hostCapacityPenalty(hosts, posicionesInvitados, problema.vtrK, problema.vtrC, matriz);
	int total = allDif + meetOnlyOnce + hostCapacity;
	//cout << "All Diff:\t\t\t\t" << allDif << endl;
	//cout << "Crew meet only once:\t" << meetOnlyOnce << endl;
	//cout << "Host Capacity:\t\t\t" << hostCapacity << endl;
	//cout << "Total: \t\t\t\t\t" << total << endl;
	return total;
}

/* Greedy.
 *
 * In: Mapeo de indice en la matriz a posiciones de los Invitados, vector de hosts, el problema, la cantidad de iteraciones y el largo de la lista tabu
 * Out: Tupla de dos elementos (cantidad penalizaciones, solución encontrada)
 */
matrix greedy(map<int, int> posicionesGuests, vector<int> hosts, PPP problema){
	matrix matriz;

	vector<int> recorridoGuest0;

	for(unsigned g = 0; g < posicionesGuests.size(); ++g) {
		vector<int> recorridoGuest;
		matriz.push_back(recorridoGuest);

		for(unsigned t = 0; t < problema.T; ++t) {
			matriz[g].push_back(hosts[0]);

			// Para cada (g,t) se busca aquel hosts que cumpla todas las restricciones (penalizaciones = 0)
			// si no lo encuentra deja asignado el mejor host posible.
			int menorPuntaje = 9999, mejorHost;
			for(unsigned h = 0; h < hosts.size(); ++h) {
				matriz[g][t] = hosts[h];
				//printearMatriz(matriz);

				int p = getAllPenalty(hosts, posicionesGuests, matriz, problema);
				if (p < menorPuntaje)  {
					menorPuntaje = p;
					mejorHost = hosts[h];
				}
				
				// Si la penalización actual es 0 entonces se termina de buscar un host para (g,t) y pasa al siguiente tiempo T
				if (p == 0) 
					break;
			}

			matriz[g][t] = mejorHost;
		}
	}

	cout << "Greedy:" << endl<<endl;
	printearMatriz(matriz);

	int allDif = allDiffPenalty(matriz);
	int meetOnlyOnce = crewMeetOncePenalty(matriz);
	int hostCapacity = hostCapacityPenalty(hosts, posicionesGuests, problema.vtrK, problema.vtrC, matriz);
	int total = getAllPenalty(hosts, posicionesGuests, matriz, problema);

	cout << "All Diff:\t\t" << allDif << endl;
	cout << "Crew meet only once:\t" << meetOnlyOnce << endl;
	cout << "Host Capacity:\t\t" << hostCapacity << endl;
	cout << "--------------------------" << endl;
	cout << "Total: \t\t\t" << total << endl<<endl;
	
	return matriz;
}

/* Tabu Search.
 *
 * In: Mapeo de indice en la matriz a posiciones de los Invitados, vector de hosts, el problema, la cantidad de iteraciones y el largo de la lista tabu
 * Out: Tupla de dos elementos (cantidad penalizaciones, solución encontrada)
 */
pair<int, matrix> tabuSearch(matrix solInicial, map<int, int> posicionesGuests, vector<int> hosts, PPP problema, int ITERACIONES, int LARGO_LISTA_TABU){
	matrix mejorSol, solActual, vecino;

	solActual = solInicial;
	
	int menorPuntaje = 99999, iteracion = 0, puntaje = getAllPenalty(hosts, posicionesGuests, solActual, problema);
	vector<pair<int, int> > listaTabu;

	mejorSol = solActual;

	while (iteracion < ITERACIONES){

		vecino = solActual;
		menorPuntaje = 9999;

		// Generando vecindario
		for(unsigned i = 0; i < solActual.size(); ++i) {
			for(unsigned j = 0; j < solActual[i].size(); ++j) {
		
				pair<int,int> mejorMovimiento;
				pair<int,int> movimiento = make_pair(i,j);
				int huboMejora = 0;

				// Se evalua si mejora el problema en cada asignación a un host distinto
				for(unsigned h = 0; h < hosts.size(); ++h) {

					if (hosts[h] != vecino[i][j]) {
						vecino[i][j] = hosts[h];
						int puntaje = getAllPenalty(hosts, posicionesGuests, vecino, problema);

						if (puntaje	< menorPuntaje && !existMovTabu(movimiento, listaTabu)) {
							solActual = vecino;
							menorPuntaje = puntaje;
							huboMejora = 1;
						}

						vecino = solActual;
					}
				}

				// Si dentro del vecindario encontró una mejora entonces se agrega dicho movimiento a la lista tabú
				if (huboMejora) {
					listaTabu.insert(listaTabu.begin(), movimiento);
					if (listaTabu.size() > LARGO_LISTA_TABU)
						listaTabu.pop_back();
				}
			}
		}

		// Si solActual es mejor que mejorSol se reasigna mejorSol
		if (getAllPenalty(hosts, posicionesGuests, solActual, problema) < getAllPenalty(hosts, posicionesGuests, mejorSol, problema))
			mejorSol = solActual;

		// En caso de que el puntaje de penalizaciones sea 0 (óptimo global) se termina de iterar
		if (menorPuntaje == 0)
			break;

		iteracion++;
		
	}
	cout << endl<<"Tabu Search:" << endl<<endl;
	printearMatriz(mejorSol);

	int allDif = allDiffPenalty(mejorSol);
	int meetOnlyOnce = crewMeetOncePenalty(mejorSol);
	int hostCapacity = hostCapacityPenalty(hosts, posicionesGuests, problema.vtrK, problema.vtrC, mejorSol);
	int total = getAllPenalty(hosts, posicionesGuests, mejorSol, problema);

	cout << "All Diff:\t\t" << allDif << endl;
	cout << "Crew meet only once:\t" << meetOnlyOnce << endl;
	cout << "Host Capacity:\t\t" << hostCapacity << endl;
	cout << "--------------------------" << endl;
	cout << "Total: \t\t\t" << total << endl<<endl;

	return make_pair(total, mejorSol);
}

/* Escribe el archivo de salida
 *
 * In: Nombre del archivo de instancia, vector de hosts, las posiciones de los invitados, la matriz solución, el problema, tiempo de ejecución y penalizaciones
 * Out: No posee retorno
 */
void escribirOutput(string NOMBRE_INSTANCIA, vector<int> hosts, map<int, int> posicionesGuest, matrix matriz, PPP problema, double tiempo, int penalizaciones) {
	ofstream archivo;
	archivo.open("Salida/" + NOMBRE_INSTANCIA + ".txt");
	
	/*	Recorre la solución y la mapea para identificar por tiempo los anfitriones que hospedan a los yates, por ejemplo:
	 *
	 *	Para T = 1
	 *		1 => A 
	 *		2 => A [2+1/ 10]
	 *		3 => A [4/ 10]
	 *		4 => 2
	 *		5 => 3
	 *		6 => 2
	 *		
	 *	Para T = 2
	 *		1 => A [1/ 10]
	 *		2 => A [4/ 10]
	 *		3 => A [2/ 10]
	 *		4 => 3
	 *		5 => 2
	 *		6 => 1
	 *		
	 *	Para T = 3
	 *		1 => A [2+4/ 10]
	 *		2 => A 
	 *		3 => A [1/ 10]
	 *		4 => 1
	 *		5 => 1
	 *		6 => 3
	 */
	for(unsigned i = 0; i < matriz[0].size(); ++i) {
		//cout << "Para T = " << i << endl;
		map<int, string> mapSalida;

		for(unsigned h = 0; h < hosts.size(); ++h)
			mapSalida[hosts[h]] = "A ";

		for(unsigned h = 0; h < hosts.size(); ++h) {	
			int tieneInvitados = 0;
			for(unsigned j = 0; j < matriz.size(); ++j) {
			
				if (matriz[j][i] == hosts[h]) {
					if (tieneInvitados == 0)
						mapSalida[hosts[h]] += "[";
					ostringstream convert;
					convert << getTripulacionYate(posicionesGuest[j], problema.vtrC);
					mapSalida[hosts[h]] += convert.str();
					mapSalida[hosts[h]] += "+";
					tieneInvitados = 1;
				}	
			}
			// Cerrando el string de yates host
			if (tieneInvitados) {
				mapSalida[hosts[h]].pop_back();
				mapSalida[hosts[h]] += "/ ";
				mapSalida[hosts[h]] += to_string(getCapacidadYate(hosts[h], problema.vtrK));
				mapSalida[hosts[h]] += "]";
				tieneInvitados = 0;
			}
			// Agregando al map los yates invitados
			for(unsigned g = 0; g < posicionesGuest.size(); ++g) {
				ostringstream convert;
				convert << matriz[g][i] + 1;	
				mapSalida[posicionesGuest[g]] = convert.str();
			}
		}

		
		// Se escribe el archivo
		archivo << "T = " << i + 1 << endl;
		archivo << "---------------------" << endl;

		for(map<int, string>::iterator it = mapSalida.begin(); it != mapSalida.end(); ++it)
			archivo << it->first  + 1 << " -> " << it->second << endl;

		archivo << endl;
		
	}

	archivo << "Botes anfitriones óptimos: " << hosts.size() << "." << endl;
	archivo << "Tiempo de ejecución: " << tiempo << " [s]." << endl;
	archivo << "Penalizaciones: " << penalizaciones << "." << endl;

	archivo.close();
}

int main(int argc, char const *argv[]) {
	int termino = 0, offsetHost = 0;

	if (argv[1] == string("CSPLib")) {
		while (!termino) {
			// Configuración
			string NOMBRE_INSTANCIA = argv[2];
			int ITERACIONES_TS = atoi(argv[3]), LARGO_LISTA_TABU = atoi(argv[4]);

			// Declaración de variables
			string PATH = "Instancias PPP/Instancias CSPLib/";
			clock_t t_ini, t_fin;
			map<int, int> posicionesGuests;
			vector<int> hosts;
			double secs;
			pair <int, matrix> resultado;
			matrix matrizInicial;
			PPP problema;
			int CANTHOSTS;

			// Leyendo archivo de la instancia
			problema = leerArchivo(PATH + NOMBRE_INSTANCIA + ".txt");

			// Seteando hosts y guests
			CANTHOSTS = problema.T + offsetHost;
			vector< pair<int, int> > yatesHosts = getNMayores(CANTHOSTS, problema.vtrK);
			for(unsigned i = 0; i < yatesHosts.size(); ++i) 
				hosts.push_back(yatesHosts[i].second);
			posicionesGuests = getPosicionGuests(problema.Y, hosts);

			// Ejecutando algoritmos
			t_ini = clock();
			matrizInicial = greedy(posicionesGuests, hosts, problema);
			resultado = tabuSearch(matrizInicial, posicionesGuests, hosts, problema, ITERACIONES_TS, LARGO_LISTA_TABU);
			t_fin = clock();

			if (resultado.first != 0) {
				offsetHost++;
				continue;
			}

			secs = (double) (t_fin - t_ini) / CLOCKS_PER_SEC;
			
			// Escribiendo el archivo de salida
			escribirOutput(NOMBRE_INSTANCIA, hosts, posicionesGuests, resultado.second, problema, secs, resultado.first);

			termino = 1;
		}	
	}

	else if (argv[1] == string("PPP")) {
		// Configuración
		int ITERACIONES_TS = atoi(argv[3]), LARGO_LISTA_TABU = atoi(argv[4]);

		// Declaración de variables
		string NOMBRE_INSTANCIA, ARCHIVO_SALIDA;
		string PATH = "Instancias PPP/";
		clock_t t_ini, t_fin;
		map<int, int> posicionesGuests;
		vector<int> hosts;
		double secs;
		pair <int, matrix> resultado;
		matrix matrizInicial;
		PPP problema;
		
		// Leyendo archivo de la instancia y seteando hosts y guests
		if (argv[2] == string("m1")) {
			NOMBRE_INSTANCIA = string(argv[1]) + "_m1";
			hosts = leerArchivoConfig(PATH + "Configuraciones/Configuraciones_m1.txt")["pp1_m"];
			ARCHIVO_SALIDA = NOMBRE_INSTANCIA;
		}
		else if (argv[2] == string("m2")) {
			NOMBRE_INSTANCIA = string(argv[1]) + "_m2";
			hosts = leerArchivoConfig(PATH + "Configuraciones/Configuraciones_m2.txt")["pp1_m2"];
			ARCHIVO_SALIDA = NOMBRE_INSTANCIA;
		}
		else {
			NOMBRE_INSTANCIA = string(argv[1]);
			hosts = leerArchivoConfig(PATH + "Configuraciones/Configuraciones.txt")[argv[2]];
			ARCHIVO_SALIDA = NOMBRE_INSTANCIA + "_" + string(argv[2]);
		}

		// Leyendo archivo de la instancia
		problema = leerArchivo(PATH + NOMBRE_INSTANCIA + ".txt");
		
		posicionesGuests = getPosicionGuests(problema.Y, hosts);

		// Ejecutando algoritmos
		t_ini = clock();
		matrizInicial = greedy(posicionesGuests, hosts, problema);
		cout << "Trabajando... Si es que el problema es PPP normal se demora entre 2 a 3 minutos." << endl << endl;
		resultado = tabuSearch(matrizInicial, posicionesGuests, hosts, problema, ITERACIONES_TS, LARGO_LISTA_TABU);
		t_fin = clock();

		secs = (double) (t_fin - t_ini) / CLOCKS_PER_SEC;

		// Escribiendo el archivo de salida
		escribirOutput(ARCHIVO_SALIDA, hosts, posicionesGuests, resultado.second, problema, secs, resultado.first);
	}
	

		

	return 0;
}
