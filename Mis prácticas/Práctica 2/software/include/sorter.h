////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// sorter.h
//
// Clase Sorter. Es el clasificador k-NN.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SORTER
#define SORTER

#include <omp.h>
#include <cmath>

#include <tuple>

#include <fstream>

#include "arff.h"

#define RANDOM 0
#define RELIEF 1

extern int Seed;

typedef tuple<float, float, float> eval;

class Sorter{
        
private:
        
    // Datos de la muestra
    Arff data;
    
    // Vector de pesos
    vector<float> w;
    
    // Muestra de datos que se encuentra activa (entrenamiento o test)
    short type;
    
    // Peso que tiene la tasa de clasificación con respecto a la tasa de 
    // reducción en el cálculo de la función objetivo
    float weightClass;
    
    // Valor mínimo que se tiene en cuenta para calcular la tasa de 
    // clasificación
    float minRed;
    
    // Semilla para la generación aleatoria de números
    //int Seed;
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Inicializa los valores
    //
    void init();
    
    ////
    // Devuelve la distancia Euclídea de dos ejemplos.
    //
    float de(const vector<float>& e1, const vector<float>& e2);
    
    ////
    // Devuelve la distancia Euclídea de dos ejemplos.
    // e es el ejemplo y posE es la posición sobre la que se calcula de la muestra
    //
    float de(vector<float> e, unsigned int posE);
    
    ////
    // Devuelve la distancia Euclídea de dos ejemplos.
    // e es el ejemplo y posE es la posición sobre la que se calcula de la muestra
    //
    float de(unsigned int posE, vector<float> e);
    
    ////
    // Devuelve la distancia de Euclídea de dos ejemplos.
    // e1 y e2 son las posiciones de los ejemplos.
    //
    float de(unsigned int e1, unsigned int e2);
    
    ////
    // Devuelve la distancia de Hamming de dos ejemplos.
    // e1 y e2 son las posiciones de los ejemplos.
    //
    unsigned short dh(unsigned int e1, unsigned int e2);
    
    ////
    // Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
    // cogiendo como datos un intervalo que recibe
    //
    pair<string, float> NN1(const vector<float>& e, const int& posFirst, 
                                const int& posLatest);
    
    ////
    // Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
    // de toda la muestra
    //
    pair<string, float> NN1(vector<float> e);
    
    ////
    // Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
    // de toda la muestra menos la posición que se indique
    //
    pair<string, float> NN1(vector<float> e, unsigned int posNotCheck);
    
public:
    
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    //
    Sorter(Arff dC);
    
    Sorter(Arff dC, vector<float> wC);
    
    ////////////////////////////////////////////////////////////////////////////
    // Getters
    //
    
    ////
    // Devuelve la muestra
    //
    Arff getData();
    
    ////
    // Devuelve el vector de pesos
    //
    vector<float> getW();
        
    ////
    // Devuelve el valor minRed
    //
    float getMinRed();
    
    ////
    // Devuelve el peso que tiene la tasa de clasificación en la función
    // objetivo
    //
    float getWeightClass();
    
    ////////////////////////////////////////////////////////////////////////////
    // Setters
    //
    
    ////
    // Asigna un vector de pesos
    //
    void setW(vector<float> wP);
    
    ////
    // Asigna el tipo de muestra que se va a activar
    void setType(short t);
    
    ////
    // Cambia el vector de pesos que se va a utilizar
    //
    void setTypeW(short t);
    
    ////
    // Cambia el valor de minRed
    //
    void setMinRed(float mR);
    
    ////
    // Cambia el peso que tiene la clasificación en la función objetivo
    //
    void setWeightClass(float c);
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Genera un vector de pesos aleatorio
    //
    vector<float> randomW();
    
    ////
    // Algoritmo greedy para obtener una valoración de los pesos
    //
    vector<float> relief();
            
    ////
    // Función de evaluación de la solución actual
    //
    eval evaluate();
    
    ////
    // Función de evaluación de la solución actual asignando directamente el
    // vector de pesos
    //
    eval evaluate(vector<float> wP);
    
    ////
    // Cambia los ejemplos de train/test para hacer nuevas pruebas
    //
    void changeData();
    
};

#endif