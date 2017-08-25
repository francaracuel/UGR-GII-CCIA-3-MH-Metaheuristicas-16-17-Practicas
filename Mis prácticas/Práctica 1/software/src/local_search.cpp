////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 1. APC.
//
// local_search.cpp
//
// Implementación de la clase LocalSearch. Se encarga de aplicar una búsqueda 
// local a una solución dada en el vector de pesos que tiene el atributo sorter 
// con una serie de parámetros que se definen en la clase
//
////////////////////////////////////////////////////////////////////////////////

#include "local_search.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor
//
LocalSearch::LocalSearch(Arff data) : LocalSearch(Sorter(data)){
            
}

LocalSearch::LocalSearch(Sorter sorter) : LocalSearch(sorter, 0, 0.03, 15000, 20){
            
}

LocalSearch::LocalSearch(Sorter sorter, unsigned int numN) : 
                                LocalSearch(sorter, 0, 0.03, 15000, numN){
            
}
    
LocalSearch::LocalSearch(Sorter s, float m, float stD, unsigned int numE, 
                            unsigned int numN) : sorter(s), mean(m), stDev(stD), 
                                                numEval(numE){
        
    // Se genera el número de vecinos máximos como el número de vecinos *
    // el número de características
    numNeig = numN * s.getData().getNumAttr();
    
    init();
    
}

////////////////////////////////////////////////////////////////////////////////
// Getters
//

////
// Devuelve el clasificador
//
Sorter& LocalSearch::getSorter(){
    return sorter;    
}

////////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Inicialización de componentes
//
void LocalSearch::init(){
            
    //Seed = 76440940;
    
}

////
// Genera un número aleatorio a partir de una distribución normal de media 
// mean y desviación típica stDev
//
float LocalSearch::genNormalDistNum(){
    
    default_random_engine generator(Randint(0, sorter.getData().getNumInst()));
    
    normal_distribution<double> distribution(mean, stDev);
        
    return distribution(generator);
    
}

////
// Muta un vector de pesos
//
void LocalSearch::mutate(vector<float>& w){
    
    // Se le suma una cantidad aleatoria a una característica aleatoria del
    // vector de pesos
    unsigned int posMutate = Randint(0, w.size()-1);
    
    w[posMutate] += genNormalDistNum();
    
    // Se trunca para no salir del valor normalizado
    if(w[posMutate] > 1){
        w[posMutate] = 1;
    } else if(w[posMutate] < 0){
        w[posMutate] = 0;
    }
        
}

////
// Aplica la búsqueda local del primer mejor
//
pair<vector<float>, float> LocalSearch::localSearch(){
    
    unsigned int numEval = 0;
    
    return localSearch(numEval);
    
}

pair<vector<float>, float> LocalSearch::localSearch(const vector<float>& w, 
                                            unsigned int& numEval){
    
    sorter.setW(w);
    
    return localSearch(numEval);
    
}

pair<vector<float>, float> LocalSearch::localSearch(unsigned int& numEvalP){
    
    // Contador para el número de evaluaciones generadas
    //unsigned int iEval = 0;
    
    // Contador para el número máximo de vecinos generados
    unsigned int iNeig = 0;
    
    // Vector de pesos actual
    vector<float> w = sorter.getW();
        
    // Porcentaje de acierto con el vector original
    //float pI = sorter.evaluate();
    
    float pF = 0;
    
    // Se guarda el número de iteraciones que va a necesitar
    //unsigned int numI = 0;
        
    // Mientras que el número de evaluaciones sea menor de numEval, seguirá
    // comprobando.
    // Se hace de manera "rara" para poder añadirle que lo haga de manera
    // paralela
    //#pragma omp parallel for
    for(unsigned int iEval=0; iEval<numEval; iEval++){
    
    //while(iEval < numEval && iNeig < numNeig){
        
        // Vector de pesos que se va a mutar con la mejor solución hata ahora
        vector<float> wNew = w;
        
        // Se muta el vector con la mejor solución
        mutate(wNew);
        
        // Se le asigna al clasificador el vector mutado
        sorter.setW(wNew);
        
        // Se evalúa el vector mutado
        float p = sorter.evaluate();
        
        // Se aumenta el contador de evaluaciones
        numEvalP++;
                
        // Si mejora la tasa de acierto, se modifican los valores
        if(p>pF){
            
            // Se actualiza el porcentaje máximo
            pF = p;
            
            // Se actualiza el vector de pesos al mejor
            w = wNew;
            
            // Se resetea el contador de los vecinos sin mejora
            iNeig = 0;
            
        }
                
        iNeig++;
        
        // Si el número de vecinos es mayor que el que debe tener, se termina
        // el ciclo for.
        if(iNeig > numNeig){
            iEval = numEval;
        }
        
        //numI++;
                
    }
    
    return pair<vector<float>, float>(w, pF);
        
}