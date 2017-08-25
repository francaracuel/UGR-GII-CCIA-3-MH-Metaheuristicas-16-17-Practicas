////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// de.h
//
// Clase DE. Differential Evolution. Se encarga de aplicar el algoritmo de
// evolución diferencial
//
////////////////////////////////////////////////////////////////////////////////


#ifndef DE_A
#define DE_A

#include "sorter.h"

// Constantes para el tipo de cruce
#define RAND 0
#define BEST 1

extern int Seed;

extern string name;
extern int part;

class DE{
    
private:
    
    // Clase Sorter que contiene el clasificador con el vector de pesos que
    // tiene que ajustar
    Sorter sorter;
    
    // Vector con la población
    vector<vector<float> > population;
    
    struct stSuc{
        
        eval ev;
        unsigned int pos;
        
        // Función para ordenar los elementos del vector
        bool operator() (stSuc e1, stSuc e2){
            return (get<0>(e1.ev) > get<0>(e2.ev));
        }
        
    } stSucSort;
    
    // Vector con el % de acierto de la poblacion
    vector<stSuc> pSuccess;
    
    // Tamaño de la población
    unsigned int sizeP;
    
    // Probabilidad de cruce
    float pCross;
    
    // Valor que se usa en el cruce
    float f;
    
    // Número máximo de evaluaciones que tiene permitido realizar
    unsigned int maxEval;
    
    // Media
    float mean;
    
    // Desviación típica
    float stDev;
    
    // Indica el tipo de experimentación. Por defecto, 0 (no hay)
    int exper;
            
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Inicializa el vector con la población
    //
    void init();
    
    ////
    // Ordena el vector donde están los aciertos y la posición que ocupa ese
    // acierto
    void sortPSuccess();
    
    ////
    // Ordena el vector que recibe donde están los aciertos y la posición que 
    // ocupa ese acierto
    void sortPSuccess(vector<stSuc>& pNewSuccess);
    
    ////
    // Genera un vector de números enteros aleatorios de tamaño n en un 
    // intervalo
    //
    vector<int> generateInt(unsigned int n, int start, int end);
    
    ////
    // Trunca un valor en el intervalo [0,1]
    //
    float truncate(float v);
    
    ////
    // Cruza una solución con tres padres de la forma DE/Rand/1
    //
    void crossRand(vector<float> &w, const vector<float> &p1, 
                        const vector<float> &p2, const vector<float> &p3, 
                        const unsigned int &pos);
    
    ////
    // Cruza una solución con la mejor de la población y 2 padres de la forma 
    // DE/current-to-best/1
    //
    void crossBest(vector<float> &w, const vector<float> &best, 
                        const vector<float> &p1, const vector<float> &p2, 
                        const unsigned int &pos);
        
public:
            
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    //    
    DE(Arff data);
    
    DE(Sorter s); 
    
    DE(Sorter s, unsigned int sP, float pC, unsigned int nE, float f_p, float me, 
                        float stD); 
    
    ////////////////////////////////////////////////////////////////////////////
    // Setters
    //
    void setExp(int e);
    
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    /////
    // Aplica el algoritmo de evolución diferencial. El cruce que reciba por
    // parámetro será una de las constantes definidas al inicio (RAND o BEST)
    //
    eval evaluate(int cross);
            
    ////
    // Muestra los vectores de pesos
    void show();
    void show(unsigned int numP, unsigned int numA);
            
};

#endif