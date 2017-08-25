////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 1. APC.
//
// main.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

#include "genetic_a.h"

int Seed;

const static unsigned int NUM_PARTITIONS = 5;
const static unsigned int NUM_DATA = 2*NUM_PARTITIONS;

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Inicia el contador del tiempo
//
void startTime(struct timeval& start);

////////////////////////////////////////////////////////////////////////////////
// Devuelve el tiempo transcurrido desde que se inicio
//
float getTime(const struct timeval& start);

////////////////////////////////////////////////////////////////////////////////
// Convierte el porcentaje formateado con 2 decimales
//
float setP(float p);

////////////////////////////////////////////////////////////////////////////////
// Inicializa los vectores a 0
//
void initMean(vector<float>& meanR, vector<float>& meanT);

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto
//
void exec(string file, bool label);

////////////////////////////////////////////////////////////////////////////////
// main
//
int main(int argc, char **argv) {
    
    if(argc != 3){
        cout << endl << "Faltan parámetros. Uso: ./bin/p1 archivo.arff semilla" << endl << endl;
        exit(1);
    }
    
    // Se guarda el archivo que se va a ejecutar
    string file(argv[1]);
    
    // Se inicializa la semilla
    Seed = atoi(argv[2]);
    
    // Para controlar el lugar en el que está la etiqueta de la clase del
    // ejemplo se le indica por el constructor.
    // Normalmente estará al final, pero si es wdbc.arff estará al principio.
    bool label = false;
    
    if(file=="wdbc.arff"){
        label = true;
    }
    
    //variables inicializadoras del tiempo 
    struct timeval startTotal;
    //struct timeval startWDBC, startSONAR, startSPAMBASE;
    
    // Se comienza a medir el tiempo total de la ejecución
    startTime(startTotal);
    
    // Tiempos finales de cada base de datos
    //float timeWDBC, timeSONAR, timeSPAMBASE;
            
    ////////////////////////////////////////////////////////////////////////////
    // Ejecución de la práctica
    //
    
    cout <<  endl << "//////////////////////////////////////////////" << endl;
    cout << "// " << file << " //" << endl << endl;
        
    exec(file, label);
    
    cout << "// Fin de " << file;
    cout << endl << "//////////////////////////////////////////////" << endl << endl;    
    
    /*
    ////////////////////////////////////////////////////////////////////////////
    // spambase
    //

    cout <<  endl << "//////////////////////////////////////////////" << endl;
    cout << "// spambase //" << endl << endl;
    
    startTime(startSPAMBASE);
    
    exec("spambase-460.arff", false);
    
    timeSPAMBASE = getTime(startSPAMBASE);
    
    cout << "// Fin de spambase";
    cout <<  endl << "//////////////////////////////////////////////" << endl << endl;
        
    //
    ////////////////////////////////////////////////////////////////////////////
                
    ////////////////////////////////////////////////////////////////////////////
    // wdbc
    //

    cout <<  endl << "//////////////////////////////////////////////" << endl;
    cout << "// wdbc //" << endl << endl;
    
    startTime(startWDBC);
    
    exec("wdbc.arff", true);
    
    timeWDBC = getTime(startWDBC);
    
    cout << "// Fin de wdbc";
    cout <<  endl << "//////////////////////////////////////////////" << endl << endl;
        
    //
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    // sonar
    //

    cout <<  endl << "//////////////////////////////////////////////" << endl;
    cout << "// sonar //" << endl << endl;
    
    startTime(startSONAR);
    
    exec("sonar.arff", false);
    
    timeSONAR = getTime(startSONAR);
    
    cout << "// Fin de sonar";
    cout <<  endl << "//////////////////////////////////////////////" << endl << endl;
        
    //
    ////////////////////////////////////////////////////////////////////////////
                
    ////////////////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////////////////

    cout << endl << "//////////////////////////////////////////////////////////" << endl;
    cout << "// El tiempo que ha tardado SPAMBASE es: " << timeSPAMBASE << " segundos" << endl;
    cout << "// El tiempo que ha tardado SPAMBASE es: " << timeSPAMBASE/60 << " minutos" << endl;
    cout << "// El tiempo que ha tardado WDBC es: " << timeWDBC << " segundos" << endl;
    cout << "// El tiempo que ha tardado WDBC es: " << timeWDBC/60 << " minutos" << endl;
    cout << "// El tiempo que ha tardado SONAR es: " << timeSONAR << " segundos" << endl;
    cout << "// El tiempo que ha tardado SONAR es: " << timeSONAR/60 << " minutos" << endl;
    cout << "//////////////////////////////////////////////////////////" << endl << endl;
     
    ////////////////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////////////////
     */
    cout << endl << "//////////////////////////////////////////////////////////" << endl;
    cout << "// El tiempo que ha tardado en total es: " << getTime(startTotal) << " segundos" << endl;
    cout << "// El tiempo que ha tardado en total es: " << getTime(startTotal)/60 << " minutos" << endl;
    cout << "//////////////////////////////////////////////////////////" << endl << endl;
    
    ////////////////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////////////////
    

    return 0;

}

////////////////////////////////////////////////////////////////////////////////
// Inicia el contador del tiempo
//
void startTime(struct timeval& start){
    
    gettimeofday(&start, NULL);
        
}

////////////////////////////////////////////////////////////////////////////////
// Devuelve el tiempo transcurrido desde que se inicio
//
float getTime(const struct timeval& start){
    
    struct timeval end;
    
    gettimeofday(&end, NULL);

    return ((end.tv_sec  - start.tv_sec) * 1000000u + 
         end.tv_usec - start.tv_usec) / 1.e6;
    
}

////////////////////////////////////////////////////////////////////////////////
// Convierte el porcentaje formateado con 2 decimales
//
float setP(float p){
    return roundf(p*10000) / 100;
}

////////////////////////////////////////////////////////////////////////////////
// Inicializa los vectores a 0
//
void initMean(vector<float>& meanR, vector<float>& meanT){
    
    meanR = vector<float>(10, 0);
    meanT = vector<float>(10, 0);
    
}

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto
//
void exec(string file, bool label){
    
    struct timeval start;
    
    float res, time;
    
    vector<float> meanR;
    vector<float> meanT;
    
    // Se inicializan los valores para calcular la media
    initMean(meanR, meanT);
        
    // Se crearán 5 particiones aleatorias de cada muestra
    for(unsigned int i=0; i<NUM_PARTITIONS; i++){
                
        Arff data(file, label);
        
        // Se crea el clasificador/evaluador de los datos
        Sorter sorter(data);
        
        ////////////////////////////////////////////////////////////////////////
        //////////////// COMIENZO DE LOS DATOS DE ENTRENAMIENTO ////////////////
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo 1-NN
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo 1-NN" << endl << endl;
        
        // Por defecto, los datos están en train, pero se establece aquí para
        // futuras pruebas
        sorter.setType(TRAIN);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        // Aunque comienza con el vector de pesos aleatorio, se indica que se
        // use ese para contar el tiempo que tarda en crearlo
        sorter.setTypeW(RANDOM);
        
        // Se evalúa el % de acierto de la solución aleatoria con los datos de
        // entrenamiento
        res = sorter.evaluate();
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[0] += res;
        
        meanT[0] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Búsqueda local
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// BL" << endl << endl;
                        
        // Se crea el objeto que va a lanzar la búsqueda local, pasándole el
        // clasificador que ya tenemos con el algoritmo Relief
        LocalSearch lsTrain(sorter);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto de la solución aleatoria con los datos de
        // entrenamiento      
        res = lsTrain.getSorter().evaluate(lsTrain.localSearch().first);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[2] += res;
        
        meanT[2] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Relief
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo Relief" << endl << endl;
                        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        // Aunque comienza con el vector de pesos aleatorio, se indica que se
        // use ese para contar el tiempo que tarda en crearlo
        sorter.setTypeW(RELIEF);
        
        // Se evalúa el % de acierto de la solución relief con los datos de
        // entrenamiento
        res = sorter.evaluate();
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[1] += res;
        
        meanT[1] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con BLX
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-BLX" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainGGBLX(data, GG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainGGBLX.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[3] += res;
        
        meanT[3] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con CA
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-CA" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainGGCA(data, GG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainGGCA.evaluate(MC);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[4] += res;
        
        meanT[4] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Estacionario con BLX
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GE-BLX" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainSGBLX(data, SG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainSGBLX.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[5] += res;
        
        meanT[5] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Estacionario con CA
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GE-CA" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainSGMC(data, SG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainSGMC.evaluate(MC);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[6] += res;
        
        meanT[6] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 1.0)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,1.0)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainMA1(data, GG);
        
        genTrainMA1.setTypeLocalSearch(0);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainMA1.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[7] += res;
        
        meanT[7] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 0.1)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,0.1)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainMA2(data, GG);
        
        genTrainMA2.setTypeLocalSearch(1);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainMA2.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[8] += res;
        
        meanT[8] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 0.1mej)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,0.1mej)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTrainMA3(data, GG);
        
        genTrainMA3.setTypeLocalSearch(2);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTrainMA3.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[9] += res;
        
        meanT[9] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        ////////////////// FIN DE LOS DATOS DE ENTRENAMIENTO ///////////////////
        ////////////////////////////////////////////////////////////////////////
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //////////////////// COMIENZO DE LOS DATOS DE TEST /////////////////////
        ////////////////////////////////////////////////////////////////////////
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo 1-NN
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo 1-NN" << endl << endl;
        
        // Se cambian los datos a los de test
        sorter.setType(TEST);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        // Aunque comienza con el vector de pesos aleatorio, se indica que se
        // use ese para contar el tiempo que tarda en crearlo
        sorter.setTypeW(RANDOM);
        
        // Se evalúa el % de acierto de la solución aleatoria con los datos de 
        // test
        res = sorter.evaluate();
        
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[0] += res;
        
        meanT[0] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Búsqueda local
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// BL" << endl << endl;
                                
        // Se crea el objeto que va a lanzar la búsqueda local, pasándole el
        // clasificador que ya tenemos con el algoritmo Relief
        LocalSearch lsTest(sorter);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto de la solución aleatoria con los datos de
        // entrenamiento      
        res = lsTest.getSorter().evaluate(lsTest.localSearch().first);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[2] += res;
        
        meanT[2] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
                        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Relief
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo Relief" << endl << endl;
               
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        // Aunque comienza con el vector de pesos aleatorio, se indica que se
        // use ese para contar el tiempo que tarda en crearlo
        sorter.setTypeW(RELIEF);
        
        // Se evalúa el % de acierto de la solución relief con los datos de 
        // test
        res = sorter.evaluate();
        
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[1] += res;
        
        meanT[1] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
                
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con BLX
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-BLX" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestGGBLX(data, GG);
        
        // Se utilizan los datos de test
        genTestGGBLX.setDataType(TEST);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestGGBLX.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[3] += res;
        
        meanT[3] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con CA
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-CA" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestGGCA(data, GG);
        
        // Se utilizan los datos de test
        genTestGGCA.setDataType(TEST);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestGGCA.evaluate(MC);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[4] += res;
        
        meanT[4] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Estacionario con BLX
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GE-BLX" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestSGBLX(data, SG);
        
        // Se utilizan los datos de test
        genTestSGBLX.setDataType(TEST);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestSGBLX.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[5] += res;
        
        meanT[5] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Estacionario con CA
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GE-CA" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestSGMC(data, SG);
        
        // Se utilizan los datos de test
        genTestSGMC.setDataType(TEST);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestSGMC.evaluate(MC);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[6] += res;
        
        meanT[6] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 1.0)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,1.0)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestMA1(data, GG);
        
        // Se utilizan los datos de test
        genTestMA1.setDataType(TEST);
        
        genTestMA1.setTypeLocalSearch(0);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestMA1.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[7] += res;
        
        meanT[7] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 0.1)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,0.1)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestMA2(data, GG);
        
        // Se utilizan los datos de test
        genTestMA2.setDataType(TEST);
        
        genTestMA2.setTypeLocalSearch(1);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestMA2.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[8] += res;
        
        meanT[8] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 0.1mej)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,0.1mej)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genTestMA3(data, GG);
        
        // Se utilizan los datos de test
        genTestMA3.setDataType(TEST);
        
        genTestMA3.setTypeLocalSearch(2);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genTestMA3.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanR[9] += res;
        
        meanT[9] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(res) << "%, T: " << time << " segundos" << endl;
        
        
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        
        
        ////////////////////////////////////////////////////////////////////////
        /////////////////////// FIN DE LOS DATOS DE TEST ///////////////////////
        ////////////////////////////////////////////////////////////////////////
        
    }
        
    cout << endl << "//////////////////////////////////////////////////////////" << endl;
    cout << "////////// Tiempos totales finales" << endl;
    
    cout << endl << "// 1-NN:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[0]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[0]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// Relief:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[1]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[1]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// BL:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[2]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[2]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// GG-BLX:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[3]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[3]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// GG-CA:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[4]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[4]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// GE-BLX:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[5]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[5]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// GE-CA:" << endl;
    cout << "Media del % de acierto: " << setP(meanR[6]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[6]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// AM-(10,1.0):" << endl;
    cout << "Media del % de acierto: " << setP(meanR[7]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[7]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// AM-(10,0.1):" << endl;
    cout << "Media del % de acierto: " << setP(meanR[8]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[8]/NUM_DATA << " segundos" << endl;
    
    cout << endl << "// AM-(10,0.1mej):" << endl;
    cout << "Media del % de acierto: " << setP(meanR[9]/NUM_DATA) << "%" << endl;
    cout << "Media del tiempo: " << meanT[9]/NUM_DATA << " segundos" << endl;
    
    cout <<  endl << endl << "//////////////////////////////////////////////" << endl << endl;
    
}