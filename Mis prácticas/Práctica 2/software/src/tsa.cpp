////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// tsa.cpp
//
// Implementación de la clase TSA. Trajectory based Search Algorithms. En esta 
// clase se encuentran los algoritmos de búsqueda basados en trayectorias.
//
////////////////////////////////////////////////////////////////////////////////

#include "tsa.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Constructor
//
TSA::TSA(Arff data) : TSA(Sorter(data)){
    
}

TSA::TSA(Sorter s) : sorter(s), muSA(0.3), phiSA(0.3), finalTemperatureSA(0.001),
                        maxEvalSA(15000), multSizeWNeigSA(10), 
                        multMaxSuccessSA(0.1), mean(0), stDev(0.03), 
                        ls(LocalSearch(s, mean, 0.03, 1000, 20)), maxItILS(15),
                        attrToChangeILS(0.1), bigStDev(0.4), exper(0){
    
    // Se calcula el número máximo de enfriamientos que va a realizar
    calculateMSA();
  
}

////////////////////////////////////////////////////////////////////////////
// Setters
//
void TSA::setExp(int e){
    exper = e;
}

////////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Inicializa valores por defecto que necesita Simulated Annealing
//
void TSA::initSA(){
    
    mean = 0;
    stDev = 1;
    
}

////
// Inicializa valores por defecto que necesita Iterative Local Search
//
void TSA::initILS(){
    
    mean = 0;
    stDev = 0.4;
    
}

////
// Inicializa la temperatura para el Simulated Annealing
//
void TSA::initTemperatureSA(float sol){
        
    // Se calcula la temperatura
    initialTemperatureSA = (muSA*sol)/-(log(phiSA));
    
    // Hay que comprobar que la temperatura que se genera es superior a la final
    if(initialTemperatureSA<finalTemperatureSA)
        initialTemperatureSA = finalTemperatureSA;
    
    //cout << "La temperatura inicial es: " << initialTemperatureSA << endl;
            
}

////
// Método de enfriamiento de la temperatura para Simulated Annealing
//
void TSA::coolTemperatureSA(float &temperature){
            
    float beta = (initialTemperatureSA-finalTemperatureSA) / 
                                (MSA*initialTemperatureSA*finalTemperatureSA);
    
    temperature = temperature / (1 + (beta*temperature));
    
    //temperature = Randint(0.9, 0.99) * temperature;
        
}

////
// Calcula el número máximo de enfriamientros del algoritmo Simulated
// Annealing
//
void TSA::calculateMSA(){
    
    maxNeighborsSA = multSizeWNeigSA * sorter.getData().getNumAttr();
    
    MSA = maxEvalSA / maxNeighborsSA;
    
    maxSuccessSA = multMaxSuccessSA*maxNeighborsSA;
    
}

////
// Trunca un valor en el intervalo [0,1]
//
float TSA::truncate(float v){
    
    float res = v;
    
    if(v>1){
        res = 1;
    } else if(v<0){
        res = 0;
    }
    
    return res;
    
}

////
// Muta una solución
//
void TSA::mutate(vector<float>& w){
    
    // Muta solo un elemento de la solución
    mutate(w, 1);
        
}

////
// Muta una solución, cambiando t atributos de ésta
//
void TSA::mutate(vector<float>& w, unsigned int t){
    
    //cout << "Número de elementos a mutar: " << t << endl;
    
    // Se le suma una cantidad aleatoria a una característica aleatoria del
    // vector de pesos
    
    // Si la mutación es solo para un elemento se hace una mutación normal,
    // con desviación típica stDev
    if(t==1){
        
        unsigned int posMutate = Randint(0, w.size()-1);
        
        w[posMutate] += genNormalDistNum();
    
        // Se trunca para no salir del valor normalizado
        w[posMutate] = truncate(w[posMutate]);
        
    // Si la mutación es fuerte, se aplica bigStDev a t elementos de la 
    // solución
    } else{
        
        vector<int> posMutate = generateInt(t, 0, 
                                        sorter.getData().getNumAttr()-1);

        for(unsigned int i=0; i<t; i++){

            //cout << "Entra al elemento: " << i << endl;

            w[posMutate[i]] += genNormalDistNum(mean, bigStDev);

            // Se trunca para no salir del valor normalizado
            w[posMutate[i]] = truncate(w[posMutate[i]]);

        }
        
    }
                    
}

////
// Genera un número aleatorio a partir de una distribución normal de media 
// mean y desviación típica stDev
//
float TSA::genNormalDistNum(){
    
    return genNormalDistNum(mean, stDev);
    
}

////
// Genera un número aleatorio a partir de una distribución normal de media 
// mean y desviación típica recibida por parámetro
//
float TSA::genNormalDistNum(float m, float sD){
    
    default_random_engine generator(Randint(0, sorter.getData().getNumInst()));
    
    normal_distribution<double> distribution(m, sD);
        
    return distribution(generator);
    
}

////
// Calcula la diferencia entre dos soluciones
//
float TSA::getDifference(vector<float> w1, vector<float> w2){
    
    float diff = 0;
    
    // Se recorren las dos soluciones y se suma la diferencia entre ellos
    for(unsigned int i=0; i<w1.size(); i++){
        
        diff += abs(w1[i]-w2[i]);
        
    }
    
    return diff;
    
}

////
// Genera un vector de números enteros aleatorios de tamaño n en un 
// intervalo
//
vector<int> TSA::generateInt(unsigned int n, int start, int end){
    
    vector<int> pos;
    
    for(unsigned int i=0; i<n; i++){
        
        int value;
        
        // Mientras que exista un número, seguirá generándolo
        while(find(pos.begin(), pos.end(), 
                (value = Randint(start, end))) != pos.end()){
            
        }
        
        pos.push_back(value);
        
    }
    
    /*for(int i=0; i<n; i++){
        cout << "Pos " << i+1 << ": " << pos[i] << endl;
    }*/
    
    return pos;
    
}

////
// Lanza el algoritmo Simulated Annealing
//
eval TSA::sa(){
    
    // Se crea el objeto que va a escribir en el fichero
    ofstream myFile;
    
    // Se abre el fichero
    myFile.open ("./data/partitions/"+name+"/out-sa-partition-"+to_string(part)+"-exp-"+to_string(exper)+".txt");
    myFile << "n_cooling,%agr,%clas,%red\n";
    
    // Se inicializan valores necesarios para Simulated Annealing
    initSA();
    
    // Se genera la solución inicial
    vector<float> bestW = sorter.randomW();
    
    vector<float> w = bestW;    
    
    // Se evalúa la solución inicial
    eval bestEval = sorter.evaluate(bestW);
    
    eval wEval = bestEval;
    
    // Se calcula la temperatura inicial
    initTemperatureSA(get<0>(bestEval));
    
    float temperature = initialTemperatureSA;
        
    // Contador con el número de evaluaciones. Comienza en 1 porque ya se ha
    // realiza la de la solución inicial aleatoria.
    unsigned int numEval = 1;
    
    // El número de éxitos comienza en 1 para que entre en el bucle
    unsigned int success = 1;
    
    int notSuccess = 0;
    
    unsigned int numCool = 0;
            
    // Solución auxiliar
    vector<float> wAux;
    
    ////////////////////////////////////////////////////////////////////////////
    // Experimentos
    //
    
    // Se guarda el número de no variaciones que hay en la mejor evaluación
    // en cada generación
    unsigned int numVar;
    
    // Número de variaciones por el que se aumenta el valor mínimo de la 
    // reducción
    unsigned int maxVar;
    
    // Se guarda la mejor de la última generación
    eval lastBest;
    
    // Se guarda la mejor solución de todas las que ha habido siempre
    eval bestEvalEver = bestEval;
    
    // Se guarda el valor de la reducción relacionado con la mejor solución
    float minRedBestEver = sorter.getMinRed();
    
    switch(exper){
        
        // Valor de reducción dinámico
        case 1:
            
            numVar = 0;

            maxVar = attrToChangeILS*maxNeighborsSA;

            lastBest = bestEval;
            
            break;
        
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////
    
    /*cout << "La primera de todas" << endl;
    
    cout << "La temperatura es: " << temperature << endl;
        
    cout << "El número de evaluaciones es: " << numEval << endl;

    cout << "El numero de éxitos es: " << success << endl;

    cout << "El porcentaje de la mejor solución es: " << get<0>(bestEval)*100 << 
            "%, tasa_acierto: " << get<1>(bestEval)*100 << 
            "%, tasa_reducción: " << get<2>(bestEval)*100 << "%" << endl << endl << endl;*/
    
    // Cuando el número de evaluaciones haya sido superado o
    // cuando el número de éxitos en el enfriamiento sea 0,
    // termina la ejecución del bucle
    while(numEval < maxEvalSA && success!= 0){
        
        unsigned int iNeighbors = 0;
        
        success = notSuccess = 0;
        
        // Por cada enfriamiento se generan el máximo de vecinos
        while(iNeighbors < maxNeighborsSA && success < maxSuccessSA){
            
            // Se copia la solución actual
            wAux = w;
            
            // Se muta la solución actual para generar un nuevo vecino
            mutate(wAux);
            
            // Se evalúa la solución generada
            eval evalAux = sorter.evaluate(wAux);
            
            // Se aumenta el contador de las evaluaciones
            numEval++;
            
            // Se calcula la diferencia entre las dos soluciones
            float difSol = get<0>(evalAux) - get<0>(wEval);
            
            //cout << "Probabilidad de aceptar: " << exp(difSol/temperature) << endl;
            
            // Si la diferencia es positiva (es mejor la nueva solución),
            // o si el valor aleatorio generado es menor que la exponencial de
            // la diferencia entre la temperatura por 1 (1-NN)
            if(difSol > 0 || Randint(0, 1) >= exp(difSol/temperature)){
                                                                
                // Se actualiza la nueva solución
                w = wAux;
                wEval = evalAux;
                
                // Se aumenta el contador de los éxitos 
                success++;
                
                // Si la solución actual es mejor que la mejor, se actualiza
                if(bestEval < wEval){
                    
                    bestW = w;
                    bestEval = wEval;
                    
                }
                
            } else{
                notSuccess++;
            }
            
            iNeighbors++;
            
        }
        
        // Se enfría la temperatura 
        coolTemperatureSA(temperature);
                
        /*cout << "La temperatura es: " << temperature << endl;
        
        cout << "El número de evaluaciones es: " << numEval << endl;
        
        cout << "El numero de éxitos es: " << success << endl;
        
        cout << "El numero de no éxitos es: " << notSuccess << endl;
        
        cout << "El numero máximo de éxitos es: " << maxSuccess << endl;
        
        cout << "El porcentaje de la mejor solución es: " << get<0>(bestEval)*100 << 
                "%, tasa_acierto: " << get<1>(bestEval)*100 << 
                "%, tasa_reducción: " << get<2>(bestEval)*100 << "%" << endl << endl << endl;*/
        
        ////////////////////////////////////////////////////////////////////////
        // Experimentación
        //
        
        switch(exper){
        
            // Valor de reducción dinámico
            case 1:
                
                // Se guarda la mejor solución hasta el momento
                if(get<0>(bestEvalEver) < get<0>(bestEval)){
                    
                    bestEvalEver = bestEval;
                    
                    // Se guarda el mejor valor de reducción hasta el momento
                    minRedBestEver = sorter.getMinRed();
                    
                }

                // Se comprueba si la nueva evaluación ha mejorado a la anterior
                if(get<0>(lastBest) == get<0>(bestEval)){

                    numVar++;

                } else{

                    // Si no son iguales se comienza a contar las variaciones
                    numVar = 0;

                    // Se modifica la mejor solución hasta ahora
                    lastBest = bestEval;

                }

                // Si ha habido 10 generaciones sin mejora, se aumenta el valor mínimo
                // que se tiene en cuenta para hacer la validación
                if(numVar >= maxVar){
                    
                    // Se modifica el valor de la reducción
                    sorter.setMinRed(sorter.getMinRed()+0.1);

                    //cout << "La tasa mínima de la reducción ahora es: " << sorter.getMinRed() << endl;

                    //cin.get();
                    
                }

                break;

        }

        //
        // Fin de la experimentación
        ////////////////////////////////////////////////////////////////////////
        
        // Se guarda en el fichero la mejor solución
        myFile << numCool << "," << get<0>(bestEval)*100 << "," 
                << get<1>(bestEval)*100 << "," 
                << get<2>(bestEval)*100 << endl;
        
        numCool++;
        
    }
    
    // Se cierra la escritura del fichero
    myFile.close();
    
    ////////////////////////////////////////////////////////////////////////////
    // Experimentación
    //

    switch(exper){
        
        // Sin experimentación
        case 0:
            
            bestEvalEver = bestEval;
            
            break;

        // Valor de reducción dinámico
        case 1:
            
            //cout << "minRed: " << minRedBestEver << endl;
            
            break;
            
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////
    
    return bestEvalEver;
    
}

////
// Lanza el algoritmo Iterative Local Search
//
eval TSA::ils(){
    
    // Se crea el objeto que va a escribir en el fichero
    ofstream myFile;
    
    // Se abre el fichero
    myFile.open ("./data/partitions/"+name+"/out-ils-partition-"+to_string(part)+"-exp-"+to_string(exper)+".txt");
    myFile << "n_it,%agr,%clas,%red\n";
    
    // Se inicializan valores necesarios para Iterative Local Search
    initILS();
    
    // Se calcula el número de elementos que se van a cambiar en la mutación
    // de la solución
    float t = attrToChangeILS * sorter.getData().getNumAttr();
    
    // Se genera la solución inicial
    vector<float> w = sorter.randomW();
        
    // Se evalúa la solución inicial
    eval wEval = sorter.evaluate(w);
        
    // Contador para las evaluaciones
    unsigned int numEval = 1;
    
    // Se aplica la búsqueda local sobre la solución inicial    
    pair<vector<float>, eval> res = ls.localSearch(w, numEval);
    
    // Se guarda el resultado
    vector<float> wAux = res.first;
    eval evalAux = res.second;
    
    // Se guarda en w la mejor solución
    if(get<0>(wEval) < get<0>(evalAux)){
        
        w = wAux;
        wEval = evalAux;
                
    }
    
    ////////////////////////////////////////////////////////////////////////////
    // Experimentos
    //
    
    // Se guarda el número de no variaciones que hay en la mejor evaluación
    // en cada generación
    unsigned int numVar;
    
    // Número de variaciones por el que se aumenta el valor mínimo de la 
    // reducción
    unsigned int maxVar;
    
    // Se guarda la mejor de la última generación
    eval lastBest;
    
    // Se guarda la mejor solución de todas las que ha habido siempre
    eval bestEvalEver = wEval;
    
    vector<float> bestWEver = w;
    
    // Se guarda el valor de la reducción relacionado con la mejor solución
    float minRedBestEver = sorter.getMinRed();
    
    switch(exper){
        
        // Valor de reducción dinámico
        case 1:
            
            numVar = 0;

            maxVar = 1;

            lastBest = wEval;
            
            break;
        
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////
    
    // Se aplican maxItILS iteraciones, como ya se ha aplicado una antes, se
    // comienza en 1
    for(unsigned int i=1; i<maxItILS; i++){
        
        // Se copia la mejor solución actual
        wAux = w;
        
        // Se hace una mutación fuerte
        mutate(wAux, t);
                        
        // Se vuelve a aplicar la búsqueda local 
        res = ls.localSearch(sorter, wAux, numEval);

        // Se guarda el resultado
        wAux = res.first;
        evalAux = res.second;
        
        // Se guarda en w la mejor solución
        if(get<0>(wEval) < get<0>(evalAux)){
        
            w = wAux;
            wEval = evalAux;

        }
        
        /*cout << "Número de evaluaciones: " << numEval << endl;
        cout << "La mejor solución es: " << get<0>(wEval) << endl;
        cout << "La diferencia entre las dos soluciones es: " << getDifference(w, wAux) << endl;*/
        
        ////////////////////////////////////////////////////////////////////////
        // Experimentación
        //
        
        switch(exper){
        
            // Valor de reducción dinámico
            case 1:
                
                // Se guarda la mejor solución hasta el momento
                if(get<0>(bestEvalEver) < get<0>(wEval)){
                    
                    bestEvalEver = wEval;
                    
                    bestWEver = w;
                    
                    // Se guarda el mejor valor de reducción hasta el momento
                    minRedBestEver = sorter.getMinRed();
                    
                }

                // Se comprueba si la nueva evaluación ha mejorado a la anterior
                if(get<0>(lastBest) == get<0>(wEval)){

                    numVar++;

                } else{

                    // Si no son iguales se comienza a contar las variaciones
                    numVar = 0;

                    // Se modifica la mejor solución hasta ahora
                    lastBest = wEval;

                }

                // Si ha habido 10 generaciones sin mejora, se aumenta el valor mínimo
                // que se tiene en cuenta para hacer la validación
                if(numVar >= maxVar){
                    
                    // Se modifica el valor de la reducción
                    sorter.setMinRed(sorter.getMinRed()+0.15);

                    //cout << "La tasa mínima de la reducción ahora es: " << sorter.getMinRed() << endl;

                    //cin.get();
                    
                }

                break;

        }

        //
        // Fin de la experimentación
        ////////////////////////////////////////////////////////////////////////////
        
        // Se guarda en el fichero la mejor solución
        myFile << i << "," << get<0>(wEval)*100 << "," 
                << get<1>(wEval)*100 << "," 
                << get<2>(wEval)*100 << endl;
        
    }
    
    // Se cierra la escritura del fichero
    myFile.close();
    
    ////////////////////////////////////////////////////////////////////////////
    // Experimentación
    //

    switch(exper){
        
        // Sin experimentación
        case 0:
            
            bestEvalEver = wEval;
            
            break;

        // Valor de reducción dinámico
        case 1:
            
            //cout << "minRed: " << minRedBestEver << endl;
            
            break;
            
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////
        
    return bestEvalEver;
    
}
