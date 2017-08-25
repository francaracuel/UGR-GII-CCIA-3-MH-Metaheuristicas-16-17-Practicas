////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 1. APC.
//
// arff.h
//
// Clase Arff. Utilizada para guardar de manera más cómoda los datos leídos de
// los ficheros arff.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ARFF
#define ARFF

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

#include "arff_parser.h"
#include "arff_data.h"

extern "C"
{
  #include "random.h"
}

// Constantes para el tipo de dato
#define TRAIN 0
#define TEST 1

extern int Seed;

using namespace std;

// typedef creado para manejar de manera más comoda los datos
typedef vector<vector<float> > matrix;

class Arff{
    
private:
            
    // Datos de entrenamiento de la muestra
    matrix train;
    
    // Etiquetas de cada elemento de los datos de entrenamiento
    vector<string> trainLabel;
        
    // Número de ejemplos de los datos de entrenamiento
    int numTrainInst;
    
    // Datos de test de la muestra
    matrix test;
    
    // Etiquetas de cada elemento de los datos de test
    vector<string> testLabel;
        
    // Número de ejemplos de los datos de test
    int numTestInst;
        
    // Número de atributos de los datos
    int numAttr;
    
    // Semilla para la aleatorización de los datos
    //int Seed;
    
    // Muestra de datos que se encuentra activa (entrenamiento o test)
    short type;
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
        
    ////
    // Devuelve un vector con los mayores atributos de la muestra
    //
    vector<float> getMaxAttr(short type);
    
    ////
    // Devuelve un vector con los menores atributos de la muestra
    //
    vector<float> getMinAttr(short type);
    
    ////
    // Normaliza los atributos de la muestra, de entrenamiento y test
    //
    void normalize();
    
    ////
    // Normaliza los atributos de la muestra
    //
    void normalize(short type);
    
public:
    
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    // 
    // posClass: true si el atributo que define la clase está al principio,
    // false si está al final
    //
    Arff(string file, bool posClass);
    
    ////////////////////////////////////////////////////////////////////////////
    // Getters
    //
        
    ////
    // Devuelve los datos de test o de entrenamiento por referencia
    //
    matrix& get(short type);
    
    ////
    // Devuelve la matriz de datos que se encuentre activa
    //
    matrix getData();
    
    ////
    // Devuelve un ejemplo en concreto de los datos que se encuentre activa
    //
    vector<float> getData(unsigned int posRow);
    
    ////
    // Devuelve un atributo en concreto de los datos que se encuentre activa
    //
    float getData(unsigned int posRow, unsigned int posCol);
    
    ////
    // Devuelve el vector de etiquetas de los datos que se encuentren activos
    //
    vector<string> getLabel();
    
    ////
    // Devuelve una etiqueta de los datos que se encuentre activa
    //
    string getLabel(unsigned int posLabel);
    
    ////
    // Devuelve el número de ejemplos de los datos que se encuentre activa
    //
    unsigned int getNumInst();
    
    ////
    // Devuelve la matriz de datos de entrenamiento
    //
    const matrix& getTrain();
    
    ////
    // Devuelve un ejemplo en concreto de los datos de entrenamiento
    //
    vector<float> getTrain(unsigned int posRow);
    
    ////
    // Devuelve un atributo en concreto de los datos de entramiento
    //
    float getTrain(unsigned int posRow, unsigned int posCol);
    
    ////
    // Devuelve el vector de etiquetas de los datos de entrenamiento
    //
    const vector<string>& getTrainLabel();
    
    ////
    // Devuelve una etiqueta de los datos de entrenamiento
    //
    string getTrainLabel(unsigned int posLabel);
    
    ////
    // Devuelve el número de ejemplos de los datos de entrenamiento
    //
    unsigned int getNumTrainInst();
    
    ////
    // Devuelve la matriz de datos de test
    //
    const matrix& getTest();
    
    ////
    // Devuelve un ejemplo en concreto de los datos de test
    //
    vector<float> getTest(unsigned int posRow);
    
    ////
    // Devuelve un atributo en concreto de los datos de test
    //
    float getTest(unsigned int posRow, unsigned int posCol);
    
    ////
    // Devuelve el vector de etiquetas de los datos de test
    //
    const vector<string>& getTestLabel();
    
    ////
    // Devuelve una etiqueta de los datos de test
    //
    string getTestLabel(unsigned int posLabel);
    
    ////
    // Devuelve el número de ejemplos de los datos de test
    //
    unsigned int getNumTestInst();
    
    ////
    // Devuelve el número de atributos
    //
    unsigned int getNumAttr();
        
    //// 
    // Devuelve el tipo de muestra que está activa
    const short& getType();
    
    ////////////////////////////////////////////////////////////////////////////
    // Setters
    //
    
    ////
    // Asigna el tipo de muestra que se va a activar
    void setType(short t);
        
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
        
    ////
    // Muestra por pantalla la muestra
    //
    void show(short type);
    
    ////
    // Muestra por pantalla la muestra hasta un límite de ejemplos y atibutos
    //
    void show(short type, unsigned int inst, unsigned int attr);
    
};

#endif