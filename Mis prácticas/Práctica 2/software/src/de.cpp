////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// de.cpp
//
// Implementación de la clase DE. Differential Evolution. Se encarga de aplicar
// el algoritmo de evolución diferencial
//
////////////////////////////////////////////////////////////////////////////////

#include "de.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Constructor
//
DE::DE(Arff data) : DE(Sorter(data)){

}

DE::DE(Sorter s) : DE(s, 50, 0.5, 15000, 0.5, 0, 0.3){

}

DE::DE(Sorter s, unsigned int sP, float pC, unsigned int mE, float f_p, float m,
                    float stD) : sorter(s), sizeP(sP), pCross(pC), maxEval(mE),
                                f(f_p), mean(m), stDev(stD), exper(0){

}

////////////////////////////////////////////////////////////////////////////
// Setters
//
void DE::setExp(int e){
    exper = e;
}

////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Inicializa el vector con la población
//
void DE::init(){

    // Se vacían los vectores
    population.clear();
    pSuccess.clear();

    // Se crean los cromosomas
    for(unsigned int i=0; i<sizeP; i++){

        //insert(sorter.randomW());
        population.push_back(sorter.randomW());

        // Se crea el struct que contiene los valores
        stSuc suc;
        suc.ev = sorter.evaluate(population.back());
        suc.pos = i;

        pSuccess.push_back(suc);

    }

    // Se ordena para tener a los mejor en las posiciones iniciales
    sortPSuccess();

}

////
// Ordena el vector donde están los aciertos y la posición que ocupa ese
// acierto
void DE::sortPSuccess(){

    sortPSuccess(pSuccess);

}

////
// Ordena el vector que recibe donde están los aciertos y la posición que
// ocupa ese acierto
void DE::sortPSuccess(vector<stSuc>& pNewSuccess){

    //show();

    sort(pNewSuccess.begin(), pNewSuccess.end(), stSucSort);

    //show();

    //cin.get();

}

////
// Genera un vector de números enteros aleatorios de tamaño n en un
// intervalo
//
vector<int> DE::generateInt(unsigned int n, int start, int end){

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
// Trunca un valor en el intervalo [0,1]
//
float DE::truncate(float v){

    float res = v;

    if(v>1){
        res = 1;
    } else if(v<0){
        res = 0;
    }

    return res;

}

////
// Cruza una solución con tres padres de la forma DE/Rand/1
//
void DE::crossRand(vector<float> &w, const vector<float> &p1,
                    const vector<float> &p2, const vector<float> &p3,
                    const unsigned int &pos){

    w[pos] = truncate(p1[pos] + f*(p2[pos]-p3[pos]));

}

////
// Cruza una solución con la mejor de la población y 2 padres de la forma
// DE/current-to-best/1
//
void DE::crossBest(vector<float> &w, const vector<float> &best,
                    const vector<float> &p1, const vector<float> &p2,
                    const unsigned int &pos){

    w[pos] = truncate(w[pos] + f*(best[pos]-w[pos]) + f*(p1[pos]-p2[pos]));

}

/////
// Aplica el algoritmo de evolución diferencial. El cruce que reciba por
// parámetro será una de las constantes definidas al inicio (RAND o BEST)
//
eval DE::evaluate(int cross){
    
    // Se crea el objeto que va a escribir en el fichero
    ofstream myFile;
    
    // Se abre el fichero
    if(cross==RAND)
        myFile.open ("./data/partitions/"+name+"/out-de-rand-partition-"+to_string(part)+"-exp-"+to_string(exper)+".txt");
    else
        myFile.open ("./data/partitions/"+name+"/out-de-best-partition-"+to_string(part)+"-exp-"+to_string(exper)+".txt");
    
    myFile << "n_gen,%agr,%clas,%red\n";

    // Se crea una nueva población de soluciones aleatorias
    init();

    // Número de atributos que tiene la solución
    unsigned int numAttr = sorter.getData().getNumAttr();

    // Solución que se guarda en cada generación
    vector<float> w;

    // Solución auxiliar que se utiliza para cálculos intermedios
    vector<float> wAux;

    // Se guarda la evaluación de la solución que se elige
    eval wEval;

    // Evaluación auxiliar que se utiliza para cálculos intermedios
    eval evalAux;

    // Se inicializa el contador de las evaluaciones con el tamaño de la
    // población al haberlo inicializado y calculado
    unsigned int numEval = sizeP;

    // Posición que se genera aleatoria sobre el elemento que se va a cruzar
    unsigned int posCross;
    
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
    eval bestEvalEver = pSuccess.front().ev;
    
    // Se guarda el valor de la reducción relacionado con la mejor solución
    float minRedBestEver = sorter.getMinRed();
    
    switch(exper){
        
        // Valor de reducción dinámico
        case 1:
            
            numVar = 0;

            maxVar = 0.8*sizeP;

            lastBest = pSuccess.front().ev;
            
            break;
        
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////

    // Mientras que pueda seguir haciendo evaluaciones sigue en el bucle,
    // dando paso a una nueva generación
    while(numEval < maxEval){

        // Se comienza un bucle del tamaño de la población
        for(unsigned int i=0; i<sizeP; i++){

            // Se seleccionan la solución a cambiar más 3 padres aleatoriamente
            matrix parents;

            // Se generan las 4 posiciones sin repetición donde se encuentran
            // los padres (para current-to-best no hace falta 3 padres, sino 2,
            // pero igualmente se utiliza este método y no se envía el último
            // generado)
            vector<int> posParents = generateInt(4, 0, sizeP-1);

            // Se guardan las 4 soluciones que corresponden con las posiciones
            // generadas. La primera será la que hay que modificar.
            for(unsigned int j=0; j<posParents.size(); j++){
                parents.push_back(population[pSuccess[posParents[j]].pos]);
            }

            posCross = posParents[0];

            // Se guarda la solución aparte para poder compararla después
            w = wAux = population[pSuccess[posCross].pos];
            wEval = evalAux = pSuccess[posCross].ev;

            // Se comienza un bucle del tamaño de la solución
            //#pragma omp parallel for
            for(unsigned int j=0; j<numAttr; j++){

                // Cruza con una probabilidad de pCross
                if(Randint(0,1) <= pCross){

                    // Se cruza y se genera una nueva solución dependiendo del
                    // cruce que se quiera hacer
                    if(cross == RAND){
                
                        // Se cruza wAux con los tres padres generados
                        crossRand(wAux, parents[1], parents[2], parents[3], j);

                    } else if(cross == BEST){
                      
                        // Se cruza wAux con el mejor y los dos padres generados
                        crossRand(wAux, population[pSuccess.front().pos],
                                parents[1], parents[2], j);

                    }

                }

            }
            
            // Se evalúa la nueva solución creada
            evalAux = sorter.evaluate(wAux);

            // Se aumenta el contador de las evaluaciones
            numEval++;
            
            // Si el elemento generado es mejor, se guarda
            if(get<0>(wEval) < get<0>(evalAux)){

                population[pSuccess[posCross].pos] = wAux;
                pSuccess[posCross].ev = evalAux;

            }

        }
        
        // Se ordena la nueva población
        sortPSuccess();

        //cout << "El número de evaluaciones es: " << numEval << endl;
        
        //cout << "El acierto es: " << get<0>(pSuccess.front().ev)*100 << "%" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Experimentación
        //
        
        switch(exper){
        
            // Valor de reducción dinámico
            case 1:
                
                // Se guarda la mejor solución hasta el momento
                if(get<0>(bestEvalEver) < get<0>(pSuccess.front().ev)){
                    
                    bestEvalEver = pSuccess.front().ev;
                    
                    // Se guarda el mejor valor de reducción hasta el momento
                    minRedBestEver = sorter.getMinRed();
                    
                }

                // Se comprueba si la nueva evaluación ha mejorado a la anterior
                if(get<0>(lastBest) == get<0>(pSuccess.front().ev)){

                    numVar++;

                } else{

                    // Si no son iguales se comienza a contar las variaciones
                    numVar = 0;

                    // Se modifica la mejor solución hasta ahora
                    lastBest = pSuccess.front().ev;

                }

                // Si ha habido 10 generaciones sin mejora, se aumenta el valor mínimo
                // que se tiene en cuenta para hacer la validación
                if(numVar >= maxVar){
                    
                    // Se modifica el valor de la reducción
                    sorter.setMinRed(sorter.getMinRed()+0.02);

                    //cout << "La tasa mínima de la reducción ahora es: " << sorter.getMinRed() << endl;

                    //cin.get();
                    
                }

                break;

        }

        //
        // Fin de la experimentación
        ////////////////////////////////////////////////////////////////////////
        
        // Se guarda en el fichero la mejor solución
        myFile << numEval/sizeP << "," << get<0>(pSuccess.front().ev)*100 << "," 
                << get<1>(pSuccess.front().ev)*100 << "," 
                << get<2>(pSuccess.front().ev)*100 << endl;

    }
    
    // Se cierra la escritura del fichero
    myFile.close();
    
    ////////////////////////////////////////////////////////////////////////////
    // Experimentación
    //

    switch(exper){
        
        // Sin experimentación
        case 0:
            
            bestEvalEver = pSuccess.front().ev;
            
            break;

        // Valor de reducción dinámico
        case 1:
            
            //cout << "minRed: " << minRedBestEver << endl;
            
            break;
            
    }
    
    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////

    return pSuccess.front().ev;

}

////
// Muestra los vectores de pesos
void DE::show(){
    show(sizeP, sorter.getData().getNumAttr());
}

void DE::show(unsigned int numP, unsigned int numA){

    /*for(unsigned int i=0; i<numP; i++){

        cout << "Posición " << i+1 << ": ";

        for(unsigned int j=0; j<numA; j++){

            cout << population[i][j] << " ";

        }

        cout << "Su resultado es: " << get<0>(sorter.evaluate(population[i]))*100 << "%" << endl;

    }*/

    cout << endl << "Vector con el % y la posición:" << endl;

    for(unsigned int i=0; i<numP; i++){

        cout << "Posición " << i+1 << ": ";

        cout << "Su resultado es: " << get<0>(pSuccess[i].ev)*100 << "%" << " y ocupa la posición " << pSuccess[i].pos+1 << endl;

    }

}
