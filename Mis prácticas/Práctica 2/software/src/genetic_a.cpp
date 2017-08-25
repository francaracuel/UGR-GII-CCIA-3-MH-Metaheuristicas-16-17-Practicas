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
                                            0.001, 15000, 0, 0.3, 0){

}

GeneticA::GeneticA(Sorter s, short ga, unsigned int sP, float c, float cA,
                    short cT, float m, unsigned int nE, float me, float stD,
                    int e) :
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
                                        lsType(0),
                                        exper(e){

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

void GeneticA::setExp(int e){
    exper = e;
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

        // Se crea el struct que contiene los valores
        stSuc suc;
        suc.ev = sorter.evaluate(population.back());
        suc.pos = i;

        pSuccess.push_back(suc);

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
void GeneticA::sortPSuccess(vector<stSuc>& pNewSuccess){

    //show();

    sort(pNewSuccess.begin(), pNewSuccess.end(), stSucSort);

    //show();

    //cin.get();

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

        // No se pueden coger dos padres iguales
        while(pos1 == (pos2 = Randint(0, sizeP-1))){

        }

        // El que tenga mayor éxito es el que se guarda
        if(get<0>(pSuccess[pos1].ev) >= get<0>(pSuccess[pos2].ev)){
            selection.push_back(population[pSuccess[pos1].pos]);
        } else{
            selection.push_back(population[pSuccess[pos2].pos]);
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
eval GeneticA::evaluate(short type){

    eval res;

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
eval GeneticA::generational(){

    // Se crea el objeto que va a escribir en el fichero
    ofstream myFile;

    // Se abre el fichero
    myFile.open ("./data/partitions/"+name+"/out-gg-blx-partition-"+to_string(part)+"-exp-"+to_string(exper)+".txt");
    myFile << "n_gen,%agr,%clas,%red\n";

    // Se calcula el número de cruces que se van a hacer.
    // Realmente tendría que ser "pCross * (sizeP/2)", pero dado que proceso las
    // parejas de dos en dos, para que realmente se hagan las que son, no se
    // divide entre 2.
    unsigned int numCrosses = round(pCross * sizeP);

    // Se calcula el número de cromosomas que van a mutar en cada iteracion
    unsigned int numMutations = round(pMutate * sizeP * sorter.getData().getNumAttr());

    //unsigned int iEval = 0;

    unsigned int numGen = 0;

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

            maxVar = sorter.getWeightClass()*sizeP;

            lastBest = pSuccess.front().ev;

            break;

    }

    //
    // Fin de la experimentación
    ////////////////////////////////////////////////////////////////////////////

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
        vector<stSuc> newPSuccess(sizeP);

        // Primero se aplica la ls y después se evalúan los cromosomas que no
        // tienen su evaluación al no haberle aplicado la ls

        ////////////////////////////////////////////////////////////////////////
        // LocalSearch
        //

        // Si la ls no está habilitada, se crea un vector completamente a false
        vector <bool> posLS(sizeP, false);

        if(activedLS && crossType == BLX && numGen%numGenLS==0){

            // Se guardan las posiciones a las que se le aplica la ls
            posLS = genPosLS();

            //cout << "Estamos en la generación " << numGen << endl;

            // Se suman las evaluaciones que se hayan hecho al aplicar la
            // búsqueda local
            iEval += applyLS(newP, newPSuccess, posLS);

        }

        //
        ////////////////////////////////////////////////////////////////////////

        // Se evalúan solo los cromosomas a los que no se les ha aplicado la ls
        //#pragma omp parallel for
        for(unsigned int i=0; i<sizeP; i++){

            if(!posLS[i]){

                //newPSuccess.push_back(pair<float, unsigned int>(sorter.evaluate(newP[i]), i));

                // Se comprueba si el cromosoma ya existe y así se evita hacer
                // una evaluación
                unsigned int j=0;

                bool equals = false;

                // Se compara el nuevo cromosoma con los que ya existen
                while(!equals && j<sizeP){

                    if(compare(newP[i], population[pSuccess[j].pos])){
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

                    newPSuccess[i].ev = pSuccess[j-1].ev;

                // Si no existía, se evalúa y se aumenta el contador de las
                //evaluaciones
                } else{

                    newPSuccess[i].ev = sorter.evaluate(newP[i]);

                }

                //newPSuccess[i].first = sorter.evaluate(newP[i]);
                newPSuccess[i].pos = i;

            }

        }

        // Si no está activa la ls, se ordena (si no, ya se ha ordenado antes)
        if(!activedLS)
            sortPSuccess(newPSuccess);

        // Si el mejor de la generación anterior es mejor que el mejor de la
        // nueva generación, se quita la peor nueva por la mejor antigua
        if(get<0>(pSuccess.front().ev)>get<0>(newPSuccess.front().ev)){

            // Se añade en la posición del peor, el mejor de la generación
            // anterior
            newP[newPSuccess.back().pos] = population[pSuccess.front().pos];

            // Hay que actualizar el nuevo valor de acierto
            newPSuccess.back().ev = pSuccess.front().ev;

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
                    sorter.setMinRed(sorter.getMinRed()+0.005);

                    //cout << "La tasa mínima de la reducción ahora es: " << sorter.getMinRed() << endl;

                    //cin.get();

                }

                break;

        }

        //
        // Fin de la experimentación
        ////////////////////////////////////////////////////////////////////////

        // Se guarda en el fichero la mejor solución
        myFile << numGen << "," << get<0>(pSuccess.front().ev)*100 << ","
                << get<1>(pSuccess.front().ev)*100 << ","
                << get<2>(pSuccess.front().ev)*100 << endl;

        // Se aumenta el contador de la generación
        numGen++;

    }

    /*cout << endl << "Antes de mejorar la población, el mejor tiene " << best*100 << "% de acierto" << endl;

    cout << endl << "Después de mejorar la población, el mejor tiene " << pSuccess.front().first*100 << "% de acierto" << endl;

    cout << endl << "Número de generaciones: " << numGen << endl;*/

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

    return bestEvalEver;

}

////
// Lanza el algoritmo genético estacionario
//
eval GeneticA::stationary(){

    // Se calcula el número de cromosomas que van a mutar en cada iteracion
    unsigned int numMutations = round(pMutate * 2 * sorter.getData().getNumAttr());

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
        vector<stSuc> newPSuccess(sizeP);

        //#pragma omp parallel for
        for(unsigned int i=0; i<2; i++){

            unsigned int j=0;

            bool equals = false;

            // Se compara el nuevo cromosoma con los que ya existen
            while(!equals && j<sizeP){

                if(compare(newP[i], population[pSuccess[j].pos])){
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

                newPSuccess[i].ev = pSuccess[j-1].ev;

            // Si no existía, se evalúa y se aumenta el contador de las
            //evaluaciones
            } else{

                newPSuccess[i].ev = sorter.evaluate(newP[i]);

            }

            //newPSuccess[i].first = sorter.evaluate(newP[i]);
            newPSuccess[i].pos = i;

        }

        // Se ordenan los padres
        sortPSuccess(newPSuccess);

        bool last = false;

        // Se compara el peor de la generación actual con el peor de los padres
        if(get<0>(pSuccess.back().ev)<get<0>(newPSuccess.back().ev)){

            last = true;

            // Se añade en la posición del peor, el peor padre
            population[pSuccess.back().pos] = newP[newPSuccess.back().pos];

            // Hay que actualizar el nuevo valor de acierto
            pSuccess.back().ev = newPSuccess.back().ev;

        }

        // Si el peor padre no ha mejorado al peor de la generación anterior
        // se comprueba si el mejor padre es mejor que el peor
        if(!last){

            if(get<0>(pSuccess.back().ev)<get<0>(newPSuccess.front().ev)){

                // Se añade en la posición del peor, el mejor padre
                population[pSuccess.back().pos] = newP[newPSuccess.front().pos];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess.back().ev = newPSuccess.front().ev;

            }

        } else{

            // Hay que comprobar si el mejor padre es mejor que el segundo peor
            // de la generación anterior. Si es mejor se actualiza el valor,
            // si es peor se sabe que, al menos, es mejor que el último (que ya
            // es el peor padre)
            if(get<0>(pSuccess[sizeP-2].ev)<get<0>(newPSuccess.front().ev)){

                // Se añade en la posición del peor, el mejor padre
                population[pSuccess[sizeP-2].pos] = newP[newPSuccess.front().pos];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess[sizeP-2].ev = newPSuccess.front().ev;

            } else{

                // Se añade en la posición del peor, el mejor padre
                population[pSuccess.back().pos] = newP[newPSuccess.front().pos];

                // Hay que actualizar el nuevo valor de acierto
                pSuccess.back().ev = newPSuccess.front().ev;

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

    return pSuccess.front().ev;

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
// genPosLS.
// Devuelve el número de evaluaciones que ha gastado.
//
unsigned int GeneticA::applyLS(matrix& newP,
                                vector<stSuc>& newPSuccess,
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
            pair<vector<float>, eval> wLS = ls.localSearch(population[newPSuccess[i].pos], numEval);

            // Se actualiza el nuevo cromosoma
            population[newPSuccess[i].pos] = wLS.first;

            // Se actualiza el nuevo % de acierto
            newPSuccess[i].ev = wLS.second;

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
