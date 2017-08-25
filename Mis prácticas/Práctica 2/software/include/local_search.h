////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// local_search.h
//
// Clase LocalSearch. Se encarga de aplicar una búsqueda local a una solución
// dada en el vector de pesos que tiene el atributo sorter con una serie de 
// parámetros que se definen en la clase
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LOCAL_SEARCH
#define LOCAL_SEARCH

#include <random>

#include "sorter.h"

extern int Seed;

class LocalSearch{
    
private:
    
    // Clase Sorter que contiene el clasificador con el vector de pesos que
    // tiene que ajustar
    Sorter sorter;
    
    // Media
    float mean;
    
    // Desviación típica
    float stDev;
    
    // Número de evaluaciones que tiene permitido realizar
    unsigned int numEval;
    
    // Número de vecinos en los que parará cuando no encuentre mejora
    unsigned int numNeig;
    
    // Semilla usada para la generación aleatoria de datos
    //int Seed;
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Inicialización de componentes
    //
    void init();
    
    ////
    // Genera un número aleatorio a partir de una distribución normal de media 
    // mean y desviación típica stDev
    //
    float genNormalDistNum();
    
    ////
    // Muta un vector de pesos
    //
    void mutate(vector<float>& w);
    
public:
    
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    //    
    LocalSearch(Arff data);
    
    LocalSearch(Sorter sorter);
    
    LocalSearch(Sorter sorter, unsigned int numN);    
    
    LocalSearch(Sorter s, float m, float stD, unsigned int numE, 
                    unsigned int numN);
    
    ////////////////////////////////////////////////////////////////////////////
    // Getters
    //
    
    ////
    // Devuelve el clasificador
    //
    Sorter& getSorter();
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Aplica la búsqueda local del primer mejor
    //
    pair<vector<float>, eval> localSearch();
    
    pair<vector<float>, eval> localSearch(unsigned int& numEval);
    
    pair<vector<float>, eval> localSearch(const vector<float>& w, unsigned int& numEval);
    
    pair<vector<float>, eval> localSearch(Sorter s, const vector<float>& w, unsigned int& numEval);
    
};

#endif