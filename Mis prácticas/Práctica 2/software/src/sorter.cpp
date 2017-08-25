////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// sorter.cpp
//
// Implementación de la clase Sorter. Es el clasificador k-NN.
//
////////////////////////////////////////////////////////////////////////////////

#include "sorter.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor
//
Sorter::Sorter(Arff dC) : data(dC){

    init();

}

Sorter::Sorter(Arff dC, vector<float> wC) : data(dC), w(wC){

    init();

}

////////////////////////////////////////////////////////////////////////////////
// Getters
//

////
// Devuelve la muestra
//
Arff Sorter::getData(){
    return data;
}

////
// Devuelve el vector de pesos
//
vector<float> Sorter::getW(){
    return w;
}

////
// Devuelve el valor minRed
//
float Sorter::getMinRed(){
    return minRed;
}

////
// Devuelve el peso que tiene la tasa de clasificación en la función
// objetivo
//
float Sorter::getWeightClass(){
    return weightClass;
}

////////////////////////////////////////////////////////////////////////////////
// Setters
//

////
// Asigna un vector de pesos
//
void Sorter::setW(vector<float> wP){
    w = wP;
}

////
// Asigna el tipo de muestra que se va a activar
void Sorter::setType(short t){
    type = t;
    data.setType(t);
}

////
// Cambia el vector de pesos que se va a utilizar
//
void Sorter::setTypeW(short t){

    if(t == RANDOM){
        w = randomW();
    } else{
        w = relief();
    }

}

////
// Cambia el valor de minRed
//
void Sorter::setMinRed(float mR){

    if(mR > 0.95){
        minRed = 0.95;
    } else{
        minRed = mR;
    }

}

////
// Cambia el peso que tiene la clasificación en la función objetivo
//
void Sorter::setWeightClass(float c){

    if(c > 1){
        weightClass = 1;
    } else if(c < 0){
        weightClass = 0;
    } else{
        weightClass = c;
    }

}

////////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Inicializa los valores
//
void Sorter::init(){

    type = TEST;

    // Peso de la tasa de clasificación
    weightClass = 0.5;

    // Valor mínimo que se tiene en cuenta para calcular la tasa de
    // clasificación
    minRed = 0.1;

    //Seed = 76440940;

    //w = relief();
    //w = randomW();
    setW(vector<float>(data.getNumAttr(), 1));

}

////
// Devuelve la distancia Euclídea de dos ejemplos.
//
float Sorter::de(const vector<float>& e1, const vector<float>& e2){

    float res = 0;

    // Se multiplica la traspuesta de w con la diferencia de los atributos
    // de los ejemplos al cuadrado
    for(unsigned int i=0; i<e1.size(); i++){

        // Si la característica se tiene que tener en cuenta, se suma al
        // resultado
        if(w[i]>minRed){
            
            float sub = e1[i] - e2[i];

            // SUM(wi * (e1i - e2i)^2)
            res += w[i] * sub * sub;

        }

    }

    return sqrt(res);

}

////
// Devuelve la distancia Euclídea de dos ejemplos.
// e es el ejemplo y posE es la posición sobre la que se calcula de la muestra
//
float Sorter::de(vector<float> e, unsigned int posE){

    // Para hacer más rápido la operación, se comprueba aquí el tipo de datos
    // que se está comprobando
    float d;

    /*if(data.getType()==TRAIN){*/
        d = de(e, data.getTrain(posE));
    /*} else{
        d = de(e, data.getTest(posE));
    }*/

    return d;

}

////
// Devuelve la distancia Euclídea de dos ejemplos.
// e es el ejemplo y posE es la posición sobre la que se calcula de la muestra
//
float Sorter::de(unsigned int posE, vector<float> e){

    // Para hacer más rápido la operación, se comprueba aquí el tipo de datos
    // que se está comprobando
    float d;

    /*if(data.getType()==TRAIN){*/
        d = de(data.getTrain(posE), e);
    /*} else{
        d = de(data.getTest(posE), e);
    }*/

    return d;

}

////
// Devuelve la distancia Euclídea de dos ejemplos.
// e1 y e2 son las posiciones de los ejemplos.
//
float Sorter::de(unsigned int e1, unsigned int e2){

    // Para hacer más rápido la operación, se comprueba aquí el tipo de datos
    // que se está comprobando
    float d;

    /*if(data.getType()==TRAIN){*/
        d = de(data.getTrain(e1), data.getTrain(e2));
    /*} else{
        d = de(data.getTest(e1), data.getTest(e2));
    }*/

    return d;

}

////
// Devuelve la distancia de Hamming de dos ejemplos.
// e1 y e2 son las posiciones de los ejemplos.
// Si las dos etiquetas de los ejemplos son iguales devuelve 0, si no 1.
//
unsigned short Sorter::dh(unsigned int e1, unsigned int e2){

    // Para hacer más rápido la operación, se comprueba aquí el tipo de datos
    // que se está comprobando
    float d;

    /*if(data.getType()==TRAIN){*/
        d = data.getTrainLabel(e1)==data.getTrainLabel(e2) ? 0 : 1;
    /*} else{
        d = data.getTestLabel(e1)==data.getTestLabel(e2) ? 0 : 1;
    }*/

    return d;

}

////
// Genera un vector de pesos aleatorio
//
vector<float> Sorter::randomW(){

    vector<float> wRandom(data.getNumAttr());

    for(int i=0; i<data.getNumAttr(); i++){
        wRandom[i] = Randfloat(0, 1);
    }

    return wRandom;

}

////
// Algoritmo greedy para obtener la primera valoración de los pesos
//
vector<float> Sorter::relief(){

    unsigned int numInstAux;
    unsigned int numAttr = data.getNumAttr();
    matrix dataAux;
    vector<string> labelAux;

    if(type == TRAIN){

        numInstAux = data.getNumTestInst();
        dataAux = data.getTest();
        labelAux = data.getTestLabel();

    } else{

        numInstAux = data.getNumTrainInst();
        dataAux = data.getTrain();
        labelAux = data.getTrainLabel();

    }

    // Se crea el vector de pesos que se va a calcular inicializándolo a 0
    vector<float> wRelief(numAttr, 0);

    // Vector de pesos que contiene el mayor valor de cada componente. Se
    // inicializa con el primer ejempolo
    vector<float> wMax = dataAux[0];

    // Se recorre cada dato de la muestra para actualizar wRelief
    for(unsigned int i=0; i<numInstAux; i++){

        // Se busca el enemigo más cercano
        int up = i-1;
        unsigned int down = i+1;
        bool findedEnemy = false;
        bool findedFriend = false;
        unsigned int posEnemy;
        unsigned int posFriend;

        // Se comprueba el enemigo más cercano
        while(!findedEnemy && (up>=0 || down<numInstAux)){

            // Si el dato que se revisa no tiene la misma etiqueta que el
            // actual
            if(up>=0 && (labelAux[up] != labelAux[i])){
                findedEnemy = true;
                posEnemy = up;
            }

            // Si el dato que se revisa no tiene la misma etiqueta que el
            // actual
            if(down<numInstAux && (labelAux[down] != labelAux[i])){
                findedEnemy = true;
                posEnemy = down;
            }

            up--;
            down++;

        }

        // Se resetean los contadores
        up = i-1;
        down = i+1;

        // Se comprueba el amigo más cercano
        while(!findedFriend && (up>=0 || down<numInstAux)){

            // Si el dato que se revisa tiene la misma etiqueta que el actual
            if(up>=0 && (labelAux[up] == labelAux[i])){
                findedFriend = true;
                posFriend = up;
            }

            // Si el dato que se revisa tiene la misma etiqueta que el actual
            if(down<numInstAux && (labelAux[down] == labelAux[i])){
                findedFriend = true;
                posFriend = down;
            }

            up--;
            down++;

        }

        // Se actualiza la información del vector de pesos wRelief
        for(unsigned int j=0; j<numAttr; j++){

            float resEnemy = dataAux[i][j] - dataAux[posEnemy][j];
            float resFriend = dataAux[i][j] - dataAux[posFriend][j];

            wRelief[j] = wRelief[j] + abs(resEnemy) - abs(resFriend);

            // Se aprovecha este paso para ir guardando el máximo valor de cada
            // componente
            if(wRelief[j] > wMax[j]){
                wMax[j] = wRelief[j];
            }

        }

    }

    // Ya se tiene el vector con los máximos valores de las componentes.
    // En este paso se normalizan las componentes. Las negativos se truncan a 0.
    for(unsigned int i=0; i<numAttr; i++){

        wRelief[i] = wRelief[i]<=0 ? 0 : wRelief[i]/wMax[i];

    }

    return wRelief;

}

////
// Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
// cogiendo como datos un intervalo que recibe
//
pair<string, float> Sorter::NN1(const vector<float>& e, const int& posFirst,
                                    const int& posLatest){

    string labelRes;

    float dMin;

    matrix dataAux;

    vector<string> label;

    if(type == TRAIN){

        // Se guarda una copia de los datos de test
        dataAux = data.getTest();

        // Se guarda el vector de clases. Al estar en train, se coge el test
        label = data.getTestLabel();

    } else{

        // Se guarda una copia de los datos de test
        dataAux = data.getTrain();

        // Se guarda el vector de clases. Al estar en train, se coge el test
        label = data.getTrainLabel();

    }

    // Se comienza con la clase del primer ejemplo
    labelRes = label[posFirst];

    // Hay que quedarse con la clase del ejemplo con menor distancia.
    // Se comienza con la distancia del primer ejemplo
    dMin = de(dataAux[posFirst], e);

    // Se calcula la distancia con todas las muestras
    for(unsigned int i=posFirst+1; i<posLatest; i++){

        // Se calcula la distancia entre estos dos ejemplos
        float dAux = de(dataAux[i], e);

        // Si la nueva distancia es menor que la que se tiene, se actualiza la
        // clase
        if(dAux<dMin){
            dMin = dAux;
            labelRes = label[i];
        }

    }

    return pair<string, float>(labelRes, dMin);

}

////
// Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
// de toda la muestra
//
pair<string, float> Sorter::NN1(vector<float> e){

    return NN1(e, 0, data.getNumTrainInst());

}

////
// Calcula la clase de un ejemplo con la regla del vecino más próximo (1-NN)
// de toda la muestra menos la posición que se indique
//
pair<string, float> Sorter::NN1(vector<float> e, unsigned int posNotCheck){

    pair<string, float> res;

    const unsigned int numInst = data.getNumTrainInst();

    // Si no se quiere comprobar la posición inicial, se calcula desde el
    // segundo hasta el último
    if(posNotCheck == 0){

        res = NN1(e, 1, numInst);

    // Si no se quiere comprobar la última posición, se calcula desde el primero
    // hasta el penúltimo
    } else if(posNotCheck == numInst-1){

        res = NN1(e, 0, numInst-1);

    // Si el valor está entre los ejemplos de la muestra, se guarda el mínimo
    // entre los dos intervales
    } else{

        pair<string, float> NN1_1 = NN1(e, 0, posNotCheck);
        pair<string, float> NN1_2 = NN1(e, posNotCheck+1, numInst);

        res = NN1_1.second<NN1_2.second ? NN1_1 : NN1_2;

    }

    return res;

}

////
// Función de evaluación de la solución actual
//
eval Sorter::evaluate(){

    // Contador de ejemplos bien clasificados
    float wellClass = 0;

    // Contador de atributos utilizados (menores que minRed)
    float notUsedRed = 0;

    // Número de ejemplos de la muestra
    unsigned int numInst;
    unsigned int numInstToCompare;

    // Número de atributos de la solución
    unsigned int numAttrRed = w.size();

    matrix dataAux;
    vector<string> labelAux;

    // Para no hacer muchas comprobaciones se separa el código aquí
    if(type==TRAIN){

        // Se comprueban todos los datos de entrenamiento
        dataAux = data.getTrain();
        labelAux = data.getTrainLabel();

        numInst = data.getNumTrainInst();
        numInstToCompare = data.getNumTestInst();

    } else{

        // Se comprueban todos los datos de entrenamiento
        dataAux = data.getTest();
        labelAux = data.getTestLabel();

        numInst = data.getNumTestInst();
        numInstToCompare = data.getNumTrainInst();

    }

    // Se recorren todos los ejemplos de la muestra
    #pragma omp parallel for
    for(unsigned int i=0; i<numInst; i++){

        // Por cada ejemplo se calcula su clase. Si está bien etiquetada, se
        // aumenta 1 el contador
        if(labelAux[i] == NN1(dataAux[i], 0, numInstToCompare).first){
            #pragma omp atomic
            wellClass++;
        }

    }

    // Se recorren todos los elementos de la solución para comprobar cuál está
    // por debajo de minRed (por defecto, 0.1)
    // Se podría controlar el número de atributos que no se tienen en cuenta
    // en la función que calcula la distancia euclídea, pero es mas problemático
    // arrastrar los datos
    #pragma omp parallel for
    for(unsigned int i=0; i<numAttrRed; i++){
        //cout << "ATRIBUTO: " << w[i] << ", minRed: " << minRed << endl;
        // Por cada atributo se comprueba si es menor que minRed, en ese caso
        // aumenta en 1 el contador
        if(w[i] < minRed){
            #pragma omp atomic
            notUsedRed++;
            //cout << "NO SE USA" << endl;
        }

    }

    wellClass = (wellClass/numInst);



    //cout << endl << "Número de atributos no usados: " << notUsedRed << ", Número de atributos totales: " << numAttrRed << endl;

    notUsedRed = (notUsedRed/numAttrRed);

    //cout << endl << "%_obj: " << wellClass+notUsedRed << ", %_class: " << wellClass << ", %_red: " << notUsedRed << endl;

    return eval((wellClass*weightClass)+(notUsedRed*(1-weightClass)), wellClass, notUsedRed);

}

////
// Función de evaluación de la solución actual asignando directamente el
// vector de pesos
//
eval Sorter::evaluate(vector<float> wP){

    // Se asigna el vector de pesos
    setW(wP);

    // Se evalúa la muestra
    return evaluate();

}

////
// Cambia los ejemplos de train/test para hacer nuevas pruebas
//
void Sorter::changeData(){

    data.changeData();

}
