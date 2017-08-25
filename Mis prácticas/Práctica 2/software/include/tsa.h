////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// tsa.h
//
// Clase TSA. Trajectory based Search Algorithms. En esta clase se encuentran
// los algoritmos de búsqueda basados en trayectorias.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef TSA_A
#define TSA_A

#include "sorter.h"
#include "local_search.h"

extern int Seed;

extern string name;
extern int part;

class TSA{
    
private:
    
    // Clase Sorter que contiene el clasificador con el vector de pesos que
    // tiene que ajustar
    Sorter sorter;
    
    // Búsqueda local
    LocalSearch ls;
    
    // Valores para el cálculo de la temperatura para SA
    float muSA;
    
    float phiSA;
    
    // Temperatura inicial
    float initialTemperatureSA;
    
    // Temperatura final
    float finalTemperatureSA;
    
    // Número de iteraciones (M) del algoritmo Simulated Annealing
    float MSA;
    
    // Número máximo de vecinos que puede generar
    unsigned int maxNeighborsSA;
    
    // Numero máximo de éxitos que puede generar
    unsigned int maxSuccessSA;
    
    // Valor por el que se multiplica el máximo de vecinos y da lugar al máximo
    // de éxitos
    float multMaxSuccessSA;
    
    // Evaluaciones máximas que tienen permitidos los algoritmos
    unsigned int maxEvalSA;
    
    // Valor por el que se multiplica el tamaño del problema y que da lugar al
    // número máximo de vecinos
    float multSizeWNeigSA;
    
    // Número máximo de iteraciones del Iterative Local Search
    unsigned int maxItILS;
    
    // Tasa de variación de los componentes de una solución
    float attrToChangeILS;
    
    // Media
    float mean;
    
    // Desviación típica
    float stDev;
    
    // Desviación típica para la mutación fuerte de Iterative Local Search
    float bigStDev;
    
    // Indica el tipo de experimentación. Por defecto, 0 (no hay)
    int exper;
            
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Inicializa valores por defecto que necesita Simulated Annealing
    //
    void initSA();
    
    ////
    // Inicializa valores por defecto que necesita Iterative Local Search
    //
    void initILS();
    
    ////
    // Inicializa la temperatura para el Simulated Annealing
    //
    void initTemperatureSA(float sol);
    
    ////
    // Método de enfriamiento de la temperatura para Simulated Annealing
    //
    void coolTemperatureSA(float &temperature);
    
    ////
    // Calcula el número máximo de enfriamientros del algoritmo Simulated
    // Annealing
    //
    void calculateMSA();
    
    ////
    // Trunca un valor en el intervalo [0,1]
    //
    float truncate(float v);
    
    ////
    // Genera un número aleatorio a partir de una distribución normal de media 
    // mean y desviación típica stDev
    //
    float genNormalDistNum();
    
    ////
    // Genera un número aleatorio a partir de una distribución normal de media 
    // m y desviación típica sD
    //
    float genNormalDistNum(float m, float sD);
    
    ////
    // Muta una solución
    //
    void mutate(vector<float>& w);
    
    ////
    // Muta una solución, cambiando t atributos de ésta
    //
    void mutate(vector<float>& w, unsigned int t);
    
    ////
    // Calcula la diferencia entre dos soluciones
    //
    float getDifference(vector<float> w1, vector<float> w2);
    
    ////
    // Genera un vector de números enteros aleatorios de tamaño n en un 
    // intervalo
    //
    vector<int> generateInt(unsigned int n, int start, int end);
                   
public:
            
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    //    
    TSA(Arff data);
    
    TSA(Sorter s);
    
    ////////////////////////////////////////////////////////////////////////////
    // Setters
    //
    void setExp(int e);
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Lanza el algoritmo Simulated Annealing
    //
    eval sa();
    
    ////
    // Lanza el algoritmo Iterative Local Search
    //
    eval ils();
                    
};

#endif