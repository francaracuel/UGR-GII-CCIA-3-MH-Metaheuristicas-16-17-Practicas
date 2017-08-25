////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 1. APC.
//
// genetic_a.h
//
// Implementación de la clase GeneticA. Genetic Algorithm. Se encarga de aplicar
// los distintos tipos de algoritmos genéticos a la muestra.
//
////////////////////////////////////////////////////////////////////////////////

#include "genetic_a.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Constructor
//
GeneticA::GeneticA(Arff data, short ga) : GeneticA(Sorter(data), ga){
            
}

GeneticA::GeneticA(Sorter s, short ga) : GeneticA(s, ga, 30, 0.7, 0.3, BLX, 
                                            0.001, 15000, 0, 0.3){
    
}

GeneticA::GeneticA(Sorter s, short ga, unsigned int sP, float c, float cA, 
                    short cT, float m, unsigned int nE, float me, float stD) : 
                                        sorter(s),
                                        gaType(ga),
                                        sizeP(sP),
                                        pCross(c),
                                        crossAlpha(cA),
                                        crossType(cT),
                                        pMutate(m),
                                        numEval(nE),
                                        mean(me),
                                        stDev(stD),
                                        ls(LocalSearch(s, 2)),
                                        activedLS(false),
                                        lsType(0){
    
    // Inicialización de la semilla
    //Seed = 76440940;
    
    init();
    
}

////////////////////////////////////////////////////////////////////////////
// Setters
//
void GeneticA::setDataType(short t){
    init();
    sorter.setType(t);    
}

void GeneticA::setCrossType(short t){
    init();
    crossType = t;   
}

void GeneticA::setLocalSearch(bool active){
    
    activedLS = active;
    
    if(active){
        
        sizeP = 10;
        
        
    } else{
        
        sizeP = 30;
        
    }
    
}

void GeneticA::setTypeLocalSearch(short type){
    
    // Se activa automáticamente la búsqueda local
    setLocalSearch(true);
    
    lsType = type;     
    
}

////////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Inicializa el vector con la población
//
void GeneticA::init(){
    
    if(gaType == SG){
        pCross = 1;
    }
    
    // Se vacían los vectores
    population.clear();
    pSuccess.clear();
    
    // Se crean los cromosomas
    for(unsigned int i=0; i<sizeP; i++){
        
        //insert(sorter.randomW());
        population.push_back(sorter.randomW());
        pSuccess.push_back(pair<float, unsigned int>(sorter.evaluate(population.back()), i));
        
    }
         
    // Se ordena para tener a los mejor en las posiciones iniciales
    sortPSuccess();
        
    // Número de generaciones en las que se aplica la local search
    numGenLS = 10;
    
    // Cantidad de cromosomas a los que se le aplica la búsqueda local
    pWLS = 0.1;
            
}

////
// Inserta un cromosoma en el lugar que le corresponde ordenado por % de 
// acierto
//
/*void GeneticA::insert(vector<float> w){
    
    // Se evalúa el éxito del vector de pesos que se recibe
    float res = sorter.evaluate(w);
    
    // Comienza en la primera posición
    unsigned int pos = 0;
    
    bool finded = false;
    
    // Comprobará la población hasta encontrar el lugar donde corresponde 
    // insertarlo
    while(pos<population.size() && !finded){
        
        if(sorter.evaluate(population[pos]) <= res){
            finded = true;
        } else{
            pos++;
        }
                
    }
        
    // Se inserta el vector y el % en el lugar que le corresponde
    population.insert(population.begin()+pos, w);
    pSuccess.insert(pSuccess.begin()+pos, res);
        
}*/

////
// Ordena el vector donde están los aciertos y la posición que ocupa ese
// acierto
void GeneticA::sortPSuccess(){
    
    sortPSuccess(pSuccess);
    
}

////
// Ordena el vector que recibe donde están los aciertos y la posición que 
// ocupa ese acierto
void GeneticA::sortPSuccess(vector<pair<float, unsigned int> >& pNewSuccess){
    
    sort(pNewSuccess.begin(), pNewSuccess.end(), greater<pair<float, unsigned int> >());
    
}
 
////
// Selecciona por torneo "numP" de cromosomas.
// Devuelve la posición de esos cromosomas elegidos.
//
matrix GeneticA::tournamentSelection(unsigned int numP){
    
    matrix selection;
    
    // Se escogen tantos cromosomas como se reciba en "numP"
    for(unsigned int i=0; i<numP; i++){
        
        // Por cada padre que se devuelve, se hace un torneo binario para 
        // elegirlo
        unsigned int pos1 = Randint(0, sizeP-1);
        unsigned int pos2 = Randint(0, sizeP-1);
        
        // El que tenga mayor éxito es el que se guarda
        if(pSuccess[pos1].first >= pSuccess[pos2].first){
            selection.push_back(population[pSuccess[pos1].second]);
        } else{
            selection.push_back(population[pSuccess[pos2].second]);
        }
        
    }
    
    return selection;
    
}

////
// Trunca un valor en el intervalo [0,1]
//
float GeneticA::truncate(float v){
    
    float res = v;
    
    if(v>1){
        res = 1;
    } else if(v<0){
        res = 0;
    }
    
    return res;
    
}

////
// Genera un número aleatorio a partir de una distribución normal de media 
// mean y desviación típica stDev
//
float GeneticA::genNormalDistNum(){
    
    default_random_engine generator(Randint(0, sorter.getData().getNumInst()));
    
    normal_distribution<double> distribution(mean, stDev);
        
    return distribution(generator);
    
}

////
// Cruce BLX-alpha
//
void GeneticA::BLXCross(vector<float>& w1, vector<float>& w2){
    
    // Se recorre cada gen del cromosoma
    for(unsigned int i=0; i<sorter.getData().getNumAttr(); i++){
        
        // Máximo valor de los dos padres
        float cMax = max(w1[i], w2[i]);
        
        // Menor valor de los dos padres
        float cMin = min(w1[i], w2[i]);
        
        // Se guarda truncada la diferencia entre cMax y cMin
        float I = cMax - cMin;
        
        w1[i] = truncate(Randfloat(cMin - I*crossAlpha, cMax + I*crossAlpha));
        w2[i] = truncate(Randfloat(cMin - I*crossAlpha, cMax + I*crossAlpha));
        
    }
    
}

////
// Cruce basado en la media aritmética
//
void GeneticA::meanCross(vector<float>& w, const vector<float>& w1, const vector<float>& w2){
    
    // Se recorre cada gen del cromosoma
    for(unsigned int i=0; i<sorter.getData().getNumAttr(); i++){
        
        w[i] = (w1[i]+w2[i]) / 2;
        
    }
    
}

////
// Muta un cromosoma
//
void GeneticA::mutate(vector<float>& w){
    
    // Se le suma una cantidad aleatoria a una característica aleatoria del
    // vector de pesos
    unsigned int posMutate = Randint(0, w.size()-1);
    
    w[posMutate] += genNormalDistNum();
    
    // Se trunca para no salir del valor normalizado
    w[posMutate] = truncate(w[posMutate]);
        
}

////
// Compara dos cromosomas
//
bool GeneticA::compare(const vector<float>& w1, const vector<float>& w2){
    
    bool equals = true;
    
    unsigned int i = 0;
    unsigned int numAttr = w1.size();
    
    while(equals && i<numAttr){
        
        if(w1[i] != w2[i]){
            equals = false;
        }
        
        i++;
        
    }
    
    return equals;
    
}

////
// Lanza el algoritmo que tenga activo con un operador de cruce concreto
//
float GeneticA::evaluate(short type){
    
    float res;
    
    setCrossType(type);
    
    if(gaType == GG){
        res = generational();
    } else{
        res = stationary();
    }
    
    return res;
    
}

////
// Lanza el algoritmo genético generacional
//
float GeneticA::generational(){
        
    float best = pSuccess.front().first;   
                
    // Se calcula el número de cruces que se van a hacer.
    // Realmente tendría que ser "pCross * (sizeP/2)", pero dado que proceso las
    // parejas de dos en dos, para que realmente se hagan las que son, no se
    // divide entre 2.
    unsigned int numCrosses = round(pCross * sizeP);
    
    // Se calcula el número de cromosomas que van a mutar en cada iteracion
    unsigned int numMutations = round(pMutate * sizeP * sorter.getData().getNumAttr());
    
    //unsigned int iEval = 0;
    
    unsigned int numGen = 0;
                    
    // Se realizan numEval iteraciones
    for(unsigned int iEval=0;iEval<numEval; iEval+=sizeP){
                        
        matrix newP;
        
        // Solo para el caso del cruce aritmético
        matrix parents;
                
        if(crossType == BLX){
                
            // Se crea un vector con la nueva población generada por torneo binario
            newP = tournamentSelection(sizeP);

        } else{
            
            // Como este cruce solo crea un hijo, se crea el doble de padres
            parents = tournamentSelection(sizeP*2);
            
            newP = matrix(sizeP, vector<float>(parents[0].size()));

        }
        
        // Se recorren las posiciones obtenidas de dos en dos para hacer el 
        // cruce
        #pragma omp parallel for
        for(unsigned int i=0; i<numCrosses; i=i+2){
                                        
            /*cout << endl << "Pareja: " << i/2+1 << endl;

            cout << "Antes del cruce: " << endl;
            cout << "1: " << newP[i][0] << " " << newP[i][1] << " " << newP[i][2] << " " << newP[i][3] << endl;
            cout << "2: " << newP[i+1][0] << " " << newP[i+1][1] << " " << newP[i+1][2] << " " << newP[i+1][3] << endl;*/

            // Se cruzan los padres
            if(crossType == BLX){
                
                BLXCross(newP[i], newP[i+1]);
                
            } else{
                                
                meanCross(newP[i], parents[i*2], parents[i*2+1]);
                                        
            }

            /*cout << "Después del cruce: " << endl;
            cout << "1: " << newP[i][0] << " " << newP[i][1] << " " << newP[i][2] << " " << newP[i][3] << endl;
            cout << "2: " << newP[i+1][0] << " " << newP[i+1][1] << " " << newP[i+1][2] << " " << newP[i+1][3] << endl;*/
                                        
        }
                
        // Se van a mutar "numMutations" cromosomas aleatoriamente        
        #pragma omp parallel for
        for(unsigned int i=0; i<numMutations; i++){
            
            mutate(newP[Randint(0, sizeP-1)]);
            
        }
        
        // Se genera una matriz que indica el porcentaje de acierto para
        // ordenarla
        //vector<pair<float, unsigned int> > newPSuccess;
        vector<pair<float, unsigned int> > newPSuccess(sizeP);
        
        //#pragma omp parallel for
        for(unsigned int i=0; i<sizeP; i++){
            
            //newPSuccess.push_back(pair<float, unsigned int>(sorter.evaluate(newP[i]), i));
            
            // Se comprueba si el cromosoma ya existe y así se evita hacer una
            // evaluación
            unsigned int j=0;
            
            bool equals = false;
            
            // Se compara el nuevo cromosoma con los que ya existen
            while(!equals && j<sizeP){
                
                if(compare(newP[i], population[pSuccess[j].second])){
                    equals = true;
                }
                
                j++;
                                                
            }
            
            // Si ya existía el cromosoma no se evalúa
            if(equals){
                
                //cout << "SON IGUALES: " << endl;
                
                //cout << newP[i][0] << " " << newP[i][1] << " " << newP[i][2] << " " << newP[i][3] << endl;
                //cout << population[pSuccess[j-1].second][0] << " " << population[pSuccess[j-1].second][1] << " " << population[pSuccess[j-1].second][2] << " " << population[pSuccess[j-1].second][3] << endl;
                
                //cout << endl;
                
                newPSuccess[i].first = pSuccess[j-1].first;
                
            // Si no existía, se evalúa y se aumenta el contador de las 
            //evaluaciones
            } else{
                
                newPSuccess[i].first = sorter.evaluate(newP[i]);
                                                                
            }
            
            //newPSuccess[i].first = sorter.evaluate(newP[i]);
            newPSuccess[i].second = i;
                                    
        }
                
        ////////////////////////////////////////////////////////////////////////
        // LocalSearch
        //
        
        if(activedLS && crossType == BLX && numGen%numGenLS==0){
            
            //cout << "Estamos en la generación " << numGen << endl;
                                                
            // Se suman las evaluaciones que se hayan hecho al aplicar la 
            // búsqueda local
            iEval += applyLS(newP, newPSuccess, genPosLS());
                                                            
        }
        
        //
        ////////////////////////////////////////////////////////////////////////
                                
        // Si está activa la ls, se ordena (si no, ya se ha ordenado antes)
        if(!activedLS)
            sortPSuccess(newPSuccess);

        // Si el mejor de la generación anterior es mejor que el peor de la 
        // nueva generación, se intercambia
        if(pSuccess.front().first>newPSuccess.back().first){
            
            // Se añade en la posición del peor, el mejor de la generación
            // anterior
            newP[newPSuccess.back().second] = population[pSuccess.front().second];
            
            // Hay que actualizar el nuevo valor de acierto
            newPSuccess.back().first = pSuccess.front().first;
            
            // Se vuelve a ordenar
            sortPSuccess(newPSuccess);
            
        }
        
        // Se actualiza la nueva población
        population = newP;
        pSuccess = newPSuccess;
        
        /*if(iEval%90==0){
            cout << endl << "Va por la iteración " << iEval;
            cout << endl << "En esta iteración, el mejor es " << sorter.evaluate(population[pSuccess.front().second])*100 << "% de acierto" << endl;
            
            //cout << endl << "Las soluciones ahora son: " << endl;
            
            /*for(int i=0; i<sizeP; i++){
                cout << "Posición: " << i+1 << " " << pSuccess[i].first*100 << "%" << endl;
            }*/
            
        //}
                
        numGen++;
                        
    }
    
    /*cout << endl << "Antes de mejorar la población, el mejor tiene " << best*100 << "% de acierto" << endl; 
    
    cout << endl << "Después de mejorar la población, el mejor tiene " << pSuccess.front().first*100 << "% de acierto" << endl;
    
    cout << endl << "Número de generaciones: " << numGen << endl;*/
    
    return pSuccess.front().first;
    
}

////
// Lanza el algoritmo genético estacionario
//
 float GeneticA::stationary(){
    
    float best = pSuccess.front().first;
    
    // Se calcula el número de cromosomas que van a mutar en cada iteracion
    unsigned int numMutations = round(pMutate * sizeP * sorter.getData().getNumAttr());
    
    // Si intenta mutar mas cromosomas de los que puede
    if(numMutations > 2){
        numMutations = 2;
    }
        
    unsigned int numGen = 0;
    
    for(unsigned int iEval=0; iEval<numEval; iEval+=2){
        
        // Se crea un vector con los dos padres que se van a cruzar
        matrix newP;
        
        // Como solo se generan dos padres nuevos en cada generación, se cruzan
        // "manualmente"
        if(crossType == BLX){
            
            newP = tournamentSelection(2);
            
            BLXCross(newP[0], newP[1]);
            
        } else{
                        
            // Como este cruce solo crea un hijo, se crea el doble de padres
            matrix parents = tournamentSelection(4);
                        
            newP = matrix(2, vector<float>(parents[0].size()));
                        
            meanCross(newP[0], parents[0], parents[1]);
                        
            meanCross(newP[1], parents[2], parents[3]);
                        
        }
        
        // Se mutan los que corresponda
        #pragma omp parallel for
        for(unsigned int i=0; i<numMutations; i++){
            mutate(newP[i]);
        }
        
        // Se evalúan los padres creados        
        vector<pair<float, unsigned int> > newPSuccess(sizeP);
        
        //#pragma omp parallel for
        for(unsigned int i=0; i<2; i++){
            
            unsigned int j=0;
            
            bool equals = false;
            
            // Se compara el nuevo cromosoma con los que ya existen
            while(!equals && j<sizeP){
                
                if(compare(newP[i], population[pSuccess[j].second])){
                    equals = true;
                }
                
                j++;
                                                
            }
            
            // Si ya existía el cromosoma no se evalúa
            if(equals){
                
                //cout << "SON IGUALES: " << endl;
                
                //cout << newP[i][0] << " " << newP[i][1] << " " << newP[i][2] << " " << newP[i][3] << endl;
                //cout << population[pSuccess[j-1].second][0] << " " << population[pSuccess[j-1].second][1] << " " << population[pSuccess[j-1].second][2] << " " << population[pSuccess[j-1].second][3] << endl;
                
                //cout << endl;
                
                newPSuccess[i].first = pSuccess[j-1].first;
                
            // Si no existía, se evalúa y se aumenta el contador de las 
            //evaluaciones
            } else{
                
                newPSuccess[i].first = sorter.evaluate(newP[i]);
                                                                
            }
            
            //newPSuccess[i].first = sorter.evaluate(newP[i]);
            newPSuccess[i].second = i;
            
        }
        
        // Se ordenan los padres
        sortPSuccess(newPSuccess);
        
        bool last = false;
        
        // Se compara el peor de la generación actual con el peor de los padres
        if(pSuccess.back().first<newPSuccess.back().first){
            
            last = true;
            
            // Se añade en la posición del peor, el peor padre
            population[pSuccess.back().second] = newP[newPSuccess.back().second];
            
            // Hay que actualizar el nuevo valor de acierto
            pSuccess.back().first = newPSuccess.back().first;
            
        }
        
        // Si el peor padre no ha mejorado al peor de la generación anterior
        // se comprueba si el mejor padre es mejor que el peor
        if(!last){
            
            if(pSuccess.back().first<newPSuccess.front().first){
            
                // Se añade en la posición del peor, el mejor padre
                population[pSuccess.back().second] = newP[newPSuccess.front().second];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess.back().first = newPSuccess.front().first;

            }
            
        } else{
            
            // Hay que comprobar si el mejor padre es mejor que el segundo peor
            // de la generación anterior. Si es mejor se actualiza el valor,
            // si es peor se sabe que, al menos, es mejor que el último (que ya
            // es el peor padre)
            if(pSuccess[sizeP-2].first<newPSuccess.front().first){
            
                // Se añade en la posición del peor, el mejor padre
                population[pSuccess[sizeP-2].second] = newP[newPSuccess.front().second];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess[sizeP-2].first = newPSuccess.front().first;

            } else{
                
                // Se añade en la posición del peor, el mejor padre
                population[pSuccess.back().second] = newP[newPSuccess.front().second];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess.back().first = newPSuccess.front().first;
                
            }
            
        }
        
        // Se ordena la nueva población con los padres nuevos (si se han 
        // añadido)
        sortPSuccess();
        
        /*if(iEval%100==0){
            cout << endl << "Va por la iteración " << iEval;
            cout << endl << "En esta iteración, el mejor es " << sorter.evaluate(population[pSuccess.front().second])*100 << "% de acierto" << endl;
            
            //cout << endl << "Las soluciones ahora son: " << endl;
            
            /*for(int i=0; i<sizeP; i++){
                cout << "Posición: " << i+1 << " " << pSuccess[i].first*100 << "%" << endl;
            }*/
            
        //}
                
        numGen++;
        
    }
    
    /*cout << endl << "Antes de mejorar la población, el mejor tiene " << best*100 << "% de acierto" << endl; 
    
    cout << endl << "Después de mejorar la población, el mejor tiene " << pSuccess.front().first*100 << "% de acierto" << endl;
    
    cout << endl << "Número de generaciones: " << numGen << endl;*/
    
    return pSuccess.front().first;
    
}

////
// Genera un vector con las posiciones a las que se va a aplicar la 
// búsqueda local
//
vector<bool> GeneticA::genPosLS(){
    
    vector<bool> posLS;
    
    switch(lsType){
                
        // Se aplica sobre todos los cromosomas de la población
        case 0:

            posLS = vector<bool>(sizeP, true);

            break;

        // Se aplica sobre todos con probabilidad pWLS
        case 1:

            posLS = vector<bool>(sizeP, false);
            
            for(unsigned int i=0; i<sizeP; i++){
                
                if(Randfloat(0, 1) < pWLS){
                    posLS[i] = true;
                }
                
            }

            break;

        // Se aplica sobre los pWLS*N mejores
        case 2:
                    
            posLS = vector<bool>(sizeP, false);
            
            for(unsigned int i=0; i<pWLS*sizeP; i++){
                
                posLS[i] = true;
                
            }

            break;

    }
    
    /*cout << endl << "Se aplica sobre: " << endl;
    for(int i=0; i<sizeP; i++){
        
        if(posLS[i])
            cout << i+1 << ": SI, ";
        else
            cout << i+1 << ": NO, ";
        
    }
    
    cout << endl;*/
    
    return posLS;
    
}

////
// Le aplica la búsqueda local a las posiciones que se han generado con
// genPosLS
//
unsigned int GeneticA::applyLS(matrix& newP, 
                                vector<pair<float, unsigned int> >& newPSuccess,
                                const vector<bool>& posLS){
    
    unsigned int numEval = 0;
    
    // Si está activo el tipo 2 en ls, se ordena antes
    if(lsType==2)
        sortPSuccess(newPSuccess);
    
    // Se recorren las posiciones a las que hay que aplicarle la búsqueda local
    for(unsigned int i=0; i<sizeP; i++){
        
        // Si a esta posición se le aplica la búsqueda local
        if(posLS[i]){
            
            // Se ejecuta la búsqueda local y se guarda el cromosoma resultante
            // y el % de acierto
            pair<vector<float>, float> wLS = ls.localSearch(population[newPSuccess[i].second], numEval);
            
            // Se actualiza el nuevo cromosoma
            population[newPSuccess[i].second] = wLS.first;
            
            // Se actualiza el nuevo % de acierto
            newPSuccess[i].first = wLS.second;
                                    
        }
                
    }
    
    // Si está activo el tipo diferente al 2 en ls, se ordena después
    if(lsType!=2)
        sortPSuccess(newPSuccess);
        
    return numEval;
        
}


////
// Muestra los vectores de pesos
void GeneticA::show(){
    show(sizeP, sorter.getData().getNumAttr());    
}

void GeneticA::show(unsigned int numP, unsigned int numA){
    
    for(unsigned int i=0; i<numP; i++){
        
        cout << "Posición " << i+1 << ": ";
        
        for(unsigned int j=0; j<numA; j++){
            
            cout << population[i][j] << " ";
            
        }
        
        cout << "Su resultado es: " << sorter.evaluate(population[i])*100 << "%" << endl;
        
    }
    
    cout << endl << "Vector con el % y la posición:" << endl;
            
    for(unsigned int i=0; i<numP; i++){
        
        cout << "Posición " << i+1 << ": ";
                
        cout << "Su resultado es: " << pSuccess[i].first*100 << "%" << " y ocupa la posición " << pSuccess[i].second+1 << endl;
        
    }
        
}
