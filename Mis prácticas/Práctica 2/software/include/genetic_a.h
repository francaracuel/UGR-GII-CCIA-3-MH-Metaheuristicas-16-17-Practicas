////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// genetic_a.h
//
// Clase GeneticA. Genetic Algorithm. Se encarga de aplicar los distintos tipos 
// de algoritmos genéticos a la muestra.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef GENETIC_A
#define GENETIC_A

#include <random>
#include <algorithm>
#include <functional>

#include "sorter.h"
#include "local_search.h"

// Constantes para el tipo de cruce
#define BLX 0
#define MC 1
#define GG 0
#define SG 1

extern int Seed;

extern string name;
extern int part;

class GeneticA{
    
private:
    
    // Clase Sorter que contiene el clasificador con el vector de pesos que
    // tiene que ajustar
    Sorter sorter;
    
    // Vector con la población
    vector<vector<float> > population;
    
    struct stSuc{
        //float total;
        //float success;
        //float reduction;
        eval ev;
        unsigned int pos;
        
        // Función para ordenar los elementos del vector
        /*bool operator<(const stSuc& suc) const{
            get<0>(ev) < get<0>(suc.ev);
        }*/
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
    
    // Alfa para el cruce de cromosomas
    float crossAlpha;
    
    // Tipo de cruce que va a realizar
    short crossType;
    
    // Probabilidad de mutación
    float pMutate;
    
    // Número de evaluaciones que tiene permitido realizar
    unsigned int numEval;
    
    // Media
    float mean;
    
    // Desviación típica
    float stDev;
    
    // Tipo de algoritmo genético
    short gaType;
    
    // Búsqueda local
    LocalSearch ls;
    
    // Indica si la búsqueda local está activada o no
    bool activedLS;
    
    // Número de generaciones que tienen que generarse para aplicar la búsqueda
    // local
    unsigned int numGenLS;
    
    // Cantidad de cromosomas a los que se le aplica la búsqueda local
    float pWLS;
    
    // Tipo de búsqueda local que se utiliza
    short lsType;
    
    // Semilla para los valores aleatorios
    //int Seed;
    
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
    // Inserta un cromosoma en el lugar que le corresponde ordenado por % de 
    // acierto
    //
    //void insert(vector<float> w);
    
    ////
    // Ordena el vector donde están los aciertos y la posición que ocupa ese
    // acierto
    void sortPSuccess();
    
    ////
    // Ordena el vector que recibe donde están los aciertos y la posición que 
    // ocupa ese acierto
    void sortPSuccess(vector<stSuc>& pNewSuccess);
    
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
    // Selecciona por torneo "numP" de cromosomas.
    // Devuelve la posición de esos cromosomas elegidos.
    //
    matrix tournamentSelection(unsigned int numP);
    
    ////
    // Cruce BLX-alpha
    //
    void BLXCross(vector<float>& w1, vector<float>& w2);
    
    ////
    // Cruce basado en la media aritmética
    //
    void meanCross(vector<float>& w, const vector<float>& w1, const vector<float>& w2);
    
    ////
    // Muta un cromosoma
    //
    void mutate(vector<float>& w);
    
    ////
    // Compara dos cromosomas
    //
    bool compare(const vector<float>& w1, const vector<float>& w2);
    
    ////
    // Genera un vector con las posiciones a las que se va a aplicar la 
    // búsqueda local
    //
    vector<bool> genPosLS();
    
    ////
    // Le aplica la búsqueda local a las posiciones que se han generado con
    // genPosLS.
    // Devuelve las iteraciones que ha gastado.
    //
    unsigned int applyLS(matrix& newP, vector<stSuc>& newPSuccess,
                    const vector<bool>& posLS);
           
public:
            
    ////////////////////////////////////////////////////////////////////////////
    // Constructor
    //    
    GeneticA(Arff data, short ga);
    
    GeneticA(Sorter s, short ga); 
    
    GeneticA(Sorter s, short ga, unsigned int sW, float c, float cA, short cT, 
                float m, unsigned int nE, float me, float stD, int e); 
    
    ////////////////////////////////////////////////////////////////////////////
    // Setters
    //
    void setDataType(short t);
    
    void setCrossType(short t);
    
    void setLocalSearch(bool active);
    
    void setTypeLocalSearch(short type);
    
    void setExp(int e);
        
    ////////////////////////////////////////////////////////////////////////////
    // Utils
    //
    
    ////
    // Lanza el algoritmo que tenga activo con un operador de cruce concreto
    //
    eval evaluate(short type);
    
    ////
    // Lanza el algoritmo genético generacional
    //
    eval generational();
    
    ////
    // Lanza el algoritmo genético estacionario
    //
    eval stationary();
    
    ////
    // Muestra los vectores de pesos
    void show();
    void show(unsigned int numP, unsigned int numA);
            
};

#endif