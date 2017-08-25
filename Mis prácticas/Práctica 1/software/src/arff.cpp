////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 1. APC.
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
Arff::Arff(string file, bool posClass) : type(TRAIN){
    
    // Se lee el fichero arff
    ArffParser parser("data/"+file);

    // Se convierte en un objeto ArffData, para pasarlo a la clase Arff
    ArffData* trainParser = parser.parse();

    // Se guarda en número de atributos y el número de ejemplos de la muestra
    numAttr = trainParser->num_attributes()-1;
    numTrainInst = trainParser->num_instances();
    
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