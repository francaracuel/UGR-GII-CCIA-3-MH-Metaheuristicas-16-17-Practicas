////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// arff.cpp
//
// Implementación de la clase Arff. Utilizada para guardar de manera más cómoda 
// los datos leídos de los ficheros arff.
//
////////////////////////////////////////////////////////////////////////////////

#include "arff.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor
// 
// posClass: true si el atributo que define la clase está al principio,
// false si está al final
//
Arff::Arff(string file, bool posClass) : type(TRAIN), partitions(5){
    
    // Se lee el fichero arff
    ArffParser parser("data/"+file);

    // Se convierte en un objeto ArffData, para pasarlo a la clase Arff
    ArffData* dataParser = parser.parse();

    // Se guarda en número de atributos y el número de ejemplos de la muestra
    numAttr = dataParser->num_attributes()-1;
    unsigned numInst = dataParser->num_instances();
    
    // En la práctica 1 se hizo un tipo de validación, que cambia en esta 
    // segunda, por lo que el método de generar los conjuntos de train y test
    // es diferente
        
    /*
    // Para clasificar el ejemplo como entrenamient o test, se guarda cuál es la
    // mitad de ejemplos de la muestra y un booleano que indique donde se 
    // añade
    unsigned int halfInst = numTrainInst/2;
    bool toTrain = true;
    
    // Se inicializa la semilla para decidir el lugar del ejemplo  
    //Seed = 76440940;
    
    // Se añaden todos los datos de la muestra en train
    unsigned int posFirst = 0;
    unsigned int posLatest = numAttr;
    
    if(posClass){
        posFirst++;
        posLatest++;
    }
    
    // Se recorre todo el objeto que ha leído el fichero y se hace el copiado
    for(unsigned int i=0; i<numTrainInst; i++){
        
        // Aleatoriamente se decide si el ejemplo que se guarda es de 
        // entrenamiento o de test.
        // Si aún queda sitio para que sea de los dos tipos, se decide
        // aleatoriamente
        if(train.size() <= halfInst && test.size() <= halfInst){
            
            if(Randfloat(-1, 1) > 0){
                toTrain = true;
            } else{
                toTrain = false;
            }
                        
        } else if(train.size() > halfInst){
            toTrain = false;
        } else{
            toTrain = true;
        }
        
        // Por cada ejemplo de la muestra se crea su correspondiente vector
        // de atributos
        vector<float> inst;
        
        for(unsigned int j=posFirst; j<posLatest; j++){
            
            // Se añade el atributo de la posición j del ejemplo i
            inst.push_back(trainParser->get_instance(i)->get(j)->getF());
            
        }
        
        // Se guarda el ejemplo completo en el lugar correspondiente
        if(toTrain)
            train.push_back(inst);
        else
            test.push_back(inst);
        
        // Se guarda en donde corresponde, la etiqueta que tiene el ejemplo 
        // actual
        if(toTrain){
            
            if(posClass){
                trainLabel.push_back(trainParser->get_instance(i)->get(0)->getS());
            } else{
                trainLabel.push_back(trainParser->get_instance(i)->get(numAttr)->getS());
            }
            
        } else{
            
            if(posClass){
                testLabel.push_back(trainParser->get_instance(i)->get(0)->getS());
            } else{
                testLabel.push_back(trainParser->get_instance(i)->get(numAttr)->getS());
            }
            
        }
        
        
    }
    
    // Se actualiza la cantidad de ejemplos que tiene cada uno
    numTrainInst = train.size();
    numTestInst = test.size();
    
    */
    
    // Posición de los atributos de cada ejemplo
    unsigned int posFirst = 0;
    unsigned int posLatest = numAttr;
    
    // Si la etiqueta está al inicio, el copiado se hará un atributo posterior
    if(posClass){
        posFirst++;
        posLatest++;
    }
    
    // Se añaden todos los ejemplos a una matriz general y auxiliar
    vector<pair<vector<float>, string> > dataAux;
    
    // Se recorre todo el objeto que ha leído el fichero y se hace el copiado
    for(unsigned int i=0; i<numInst; i++){
        
        // Por cada ejemplo de la muestra se crea su correspondiente vector
        // de atributos
        vector<float> inst;
        
        for(unsigned int j=posFirst; j<posLatest; j++){
            
            // Se añade el atributo de la posición j del ejemplo i
            inst.push_back(dataParser->get_instance(i)->get(j)->getF());
            
        }
        
        string c;
                
        // Dependiendo de donde esté la etiqueta que define la clase, se coge
        // la información de un sitio u otro
        if(posClass){
            c = dataParser->get_instance(i)->get(0)->getS();
        } else{
            c = dataParser->get_instance(i)->get(numAttr)->getS();
        }
        
        dataAux.push_back(pair<vector<float>, string>(inst, c));
                
    }
           
    // Se barajan los ejemplos del conjunto para hacer la separación
    random_shuffle(dataAux.begin(), dataAux.end());
    
    // Se guarda la cantidad de ejemplos que tendrá cada partición
    unsigned int instByPartition = numInst/partitions;
    
    // En los datos de train habrá (partitions-1)*instByPartition número de 
    // ejemplos. Es decir, si se tienen 5 particiones y el número total de 
    // ejemplos es 100, en el conjunto de train habrá 4*20 = 80 ejemplos.
    
    unsigned int posEndTrain = (partitions-1)*instByPartition;
    
    // Se crea el conjunto de train
    for(unsigned int i=0; i<posEndTrain; i++){
        train.push_back(dataAux[i].first);
        trainLabel.push_back(dataAux[i].second);
    }
    
    // Se crea el conjunto de test
    for(unsigned int i=posEndTrain; i<numInst; i++){
        test.push_back(dataAux[i].first);
        testLabel.push_back(dataAux[i].second);
    }
    
    // Se actualiza la cantidad de ejemplos que tiene cada uno
    numTrainInst = train.size();
    numTestInst = test.size();
    
    //cout << "El número total de ejemplos es: " << numInst << endl;
    
    //cout << "El número de train es: " << numTrainInst << " y el número de test es: " << numTestInst << endl;
    
    // Se deben normalizar todos los atributos de los ejemplos
    normalize();
    
}

////////////////////////////////////////////////////////////////////////////////
// Getters
//

////
// Devuelve los datos de test o de entrenamiento por referencia
//
matrix& Arff::get(short type){
        
    // Para saber los ejemplos que se quieren mostrar, se comprueba cuál es.
    // No me gusta hacer dos return, pero para esta forma de trabajar es una
    // solución.
    if(type == TRAIN){
        return train;
    } else{
        return test;
    }
    
}

////
// Devuelve la matriz de datos que se encuentre activa
//
matrix Arff::getData(){
    
    matrix m = train;
    
    if(type == TEST){
        m = test;
    }
    
    return m;
    
}

////
// Devuelve un ejemplo en concreto de los datos que se encuentre activa
//
vector<float> Arff::getData(unsigned int posRow){
    return getData()[posRow];
}

////
// Devuelve un atributo en concreto de los datos que se encuentre activa
//
float Arff::getData(unsigned int posRow, unsigned int posCol){
    return getData()[posRow][posCol];
}

////
// Devuelve el vector de etiquetas de los datos que se encuentre activa
//
vector<string> Arff::getLabel(){
    
    vector<string> l = trainLabel;
    
    if(type == TEST){
        l = testLabel;
    }
    
    return l;
    
}

////
// Devuelve una etiqueta de los datos que se encuentre activa
//
string Arff::getLabel(unsigned int posLabel){
    return getLabel()[posLabel];
}

////
// Devuelve el número de ejemplos de los datos que se encuentre activa
//
unsigned int Arff::getNumInst(){
    
    unsigned int numI = numTrainInst;
    
    if(type == TEST){
        numI = numTestInst;
    }
    
    return numI;
    
}

////
// Devuelve la matriz de datos de entrenamiento
//
const matrix& Arff::getTrain(){
    return train;
}

////
// Devuelve un ejemplo en concreto de los datos de entrenamiento
//
vector<float> Arff::getTrain(unsigned int posRow){
    return train[posRow];
}

////
// Devuelve un atributo en concreto de los datos de entrenamiento
//
float Arff::getTrain(unsigned int posRow, unsigned int posCol){
    return train[posRow][posCol];
}

////
// Devuelve el vector de etiquetas de los datos de entrenamiento
//
const vector<string>& Arff::getTrainLabel(){
    return trainLabel;
}

////
// Devuelve una etiqueta de los datos de entrenamiento
//
string Arff::getTrainLabel(unsigned int posLabel){
    return trainLabel[posLabel];
}

////
// Devuelve el número de ejemplos de los datos de entrenamiento
//
unsigned int Arff::getNumTrainInst(){
    return numTrainInst;
}

////
// Devuelve la matriz de datos de test
//
const matrix& Arff::getTest(){
    return test;
}

////
// Devuelve un ejemplo en concreto de los datos de test
//
vector<float> Arff::getTest(unsigned int posRow){
    return test[posRow];
}

////
// Devuelve un atributo en concreto de los datos de test
//
float Arff::getTest(unsigned int posRow, unsigned int posCol){
    return test[posRow][posCol];
}

////
// Devuelve el vector de etiquetas de los datos de test
//
const vector<string>& Arff::getTestLabel(){
    return testLabel;
}

////
// Devuelve una etiqueta de los datos de test
//
string Arff::getTestLabel(unsigned int posLabel){
    return testLabel[posLabel];
}

////
// Devuelve el número de ejemplos de los datos de test
//
unsigned int Arff::getNumTestInst(){
    return numTestInst;
}

////
// Devuelve el número de atributos de los datos
//
unsigned int Arff::getNumAttr(){
    return numAttr;
}

//// 
// Devuelve el tipo de muestra que está activa
const short& Arff::getType(){
    return type;
}

////////////////////////////////////////////////////////////////////////////////
// Setters
//

////
// Asigna el tipo de muestra que se va a activar
void Arff::setType(short t){
    type = t;
}

////////////////////////////////////////////////////////////////////////////////
// Utils
//

////
// Devuelve un vector con los mayores atributos de la muestra
//
vector<float> Arff::getMaxAttr(short t){
    
    const matrix &data = get(t);
    
    // Se comienza con el primer ejemplo
    vector<float> max = data[0];
    
    // Se recorren todos los datos
    for(unsigned int i=1; i<data.size(); i++){
        
        for(unsigned int j=0; j<numAttr; j++){
            
            // Si el atributo que se comprueba es mayor que el mayor actual, se
            // cambia
            if(data[i][j] > max[j]){
                max[j] = data[i][j];
            }
            
        }
        
    }
    
    return max;
    
}

////
// Devuelve un vector con los menores atributos de la muestra
//
vector<float> Arff::getMinAttr(short t){
    
    const matrix &data = get(t);
    
    // Se comienza con el primer ejemplo
    vector<float> min = data[0];
    
    // Se recorren todos los datos
    for(unsigned int i=1; i<data.size(); i++){
        
        for(unsigned int j=0; j<numAttr; j++){
            
            // Si el atributo que se comprueba es mayor que el mayor actual, se
            // cambia
            if(data[i][j] < min[j]){
                min[j] = data[i][j];
            }
            
        }
        
    }
    
    return min;
    
}

////
// Normaliza los atributos de la muestra, tanto de entrenamiento como de test
//
void Arff::normalize(){
    
    normalize(TRAIN);
    normalize(TEST);
    
}

////
// Normaliza los atributos de la muestra
//
void Arff::normalize(short t){
    
    vector<float> max = getMaxAttr(t);
    vector<float> min = getMinAttr(t);
    
    matrix &data = get(t);
    
    // Se recorren todos los datos de la muestra para normalizarlos
    for(unsigned int i=0; i<data.size(); i++){
        
        for(unsigned int j=0; j<numAttr; j++){
            
            data[i][j] = (data[i][j]-min[j])/(max[j]-min[j]);
            
        }
        
    }
    
}

////
// Cambia los ejemplos de train/test para hacer nuevas pruebas
//
void Arff::changeData(){
    
    // Se guarda la cantidad de ejemplos que tendrá cada partición
    unsigned int instByPartition = (numTrainInst+numTestInst)/partitions;
        
    // Se hace una copia de los datos que se van a convertir en la muestra de 
    // test
    matrix dataAux(train.begin(), train.begin()+instByPartition);
    vector<string> labelAux(trainLabel.begin(), trainLabel.begin()+instByPartition);
            
    // Se borran los ejemplos que ya no forman parte de la muestra de train
    train.erase(train.begin(), train.begin()+instByPartition);
    trainLabel.erase(trainLabel.begin(), trainLabel.begin()+instByPartition);
            
    // Se añaden los ejemplos que antes formaban parte de la muestra de test en
    // la muestra de train
    train.insert(train.end(), test.begin(), test.end());
    trainLabel.insert(trainLabel.end(), testLabel.begin(), testLabel.end());
        
    // Se vacían los vectores que contienen los datos de test
    test.clear();
    testLabel.clear();
        
    // Se añaden los datos que se han sacado de train a la muestra de test
    test.insert(test.begin(), dataAux.begin(), dataAux.end());
    testLabel.insert(testLabel.begin(), labelAux.begin(), labelAux.end());
        
    // Se actualiza la cantidad de ejemplos que tiene cada uno
    numTrainInst = train.size();
    numTestInst = test.size();
    
}

////
// Muestra por pantalla la muestra
//
void Arff::show(short t){
    
    show(t, getNumInst(), numAttr);
    
}
    
////
// Muestra por pantalla la muestra hasta un límite de ejemplos y atibutos
//
void Arff::show(short t, unsigned int inst, unsigned int attr){
        
    const matrix &data = get(t);
    
    cout << "Datos:" << endl;
                
    // Se recorren todos los datos de la muestra para normalizarlos
    for(unsigned int i=0; i<inst; i++){
        
        cout << "Ejemplo " << i+1 << ": ";
        
        for(unsigned int j=0; j<attr; j++){
            
            cout << data[i][j] << ", ";
            
        }
        
        cout << endl;
        
    }
    
}