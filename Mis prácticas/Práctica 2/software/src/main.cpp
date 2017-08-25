////////////////////////////////////////////////////////////////////////////////
//
// Francisco Javier Caracuel Beltrán
//
// Curso 2016/2017 - Metaheurísticas - GII - CCIA
//
// Práctica 2. APC.
//
// main.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

#include "genetic_a.h"
#include "tsa.h"
#include "de.h"

int Seed;

const static unsigned int NUM_PARTITIONS = 5;
const static unsigned int NUM_DATA = 2*NUM_PARTITIONS;

// Es una manera rápida de ponerle nombre a los ficheros auxiliares que se crean
// en los algoritmos para hacer las gráficas de evolución de los algoritmos
string name;
int part;

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
// Inicializa los vectores a 0
//
void initMean(vector<float>& mean);

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto
//
void exec(string file, bool label);

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto para la práctica 2
//
void exec2(string file, bool label);

////////////////////////////////////////////////////////////////////////////////
// main
//
int main(int argc, char **argv) {
    
    if(argc != 3){
        cout << endl << "Faltan parámetros. Uso: ./bin/p2 archivo.arff semilla" << endl << endl;
        exit(1);
    }
    
    // Se guarda el archivo que se va a ejecutar
    string file(argv[1]);
    
    name = file;
    
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
    
    //cout <<  endl << "//////////////////////////////////////////////" << endl;
    //cout << "// " << file << " //" << endl << endl;
        
    // Si se pone a true ejecuta la práctica, si no, hace pruebas
    if(true){
        
        exec2(file, label);
        
    }else{
        
        part = 1;

        struct timeval start;
        float time;
        eval res;

        Arff data(file, label);

        // Se crea el clasificador/evaluador de los datos
        Sorter sorter(data);

        ////////////////////////////////////////////////////////////////////////
        //////////////// COMIENZO DE LOS DATOS DE ENTRENAMIENTO ////////////////
        ////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////
        // Algoritmo 1-NN
        ////////////////////////////////////////////////////////////////////////

        /*cout << endl << "/////////////////////////////" << endl;

        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo 1-NN" << endl << endl;

        // Por defecto, los datos están en train, pero se establece aquí para
        // futuras pruebas

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

        cout << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;





        ////////////////////////////////////////////////////////////////////////
        //
        cout << endl << "/////////////////////////////" << endl;
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

        cout << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;

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

        cout << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
    */
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

        cout << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;




        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Simulated Annealing
        ////////////////////////////////////////////////////////////////////////

        /*cout << endl << "/////////////////////////////" << endl;

        ////////////////////////////////////////////////////////////////////////
        //
        //cout << "// Simulated Annealing" << endl << endl;
        cout << "// Iterative Local Search" << endl << endl;

        //data.changeData();

        // Se crea el objeto que utiliza el algoritmo genético generacional
        TSA tsa1(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);

        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = tsa1.sa();

        res = tsa1.ils();

        // Se guarda el tiempo
        time = getTime(start);

        //
        ////////////////////////////////////////////////////////////////////////

        cout << endl << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;





        ////////////////////////////////////////////////////////////////////////
        // Differential Evolution
        ////////////////////////////////////////////////////////////////////////

        cout << endl << "/////////////////////////////" << endl;

        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Differential Evolution" << endl << endl;

        data.changeData();

        // Se crea el objeto que utiliza el algoritmo genético generacional
        DE de1(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);

        res = de1.evaluate(RAND);
        res = de1.evaluate(BEST);

        // Se guarda el tiempo
        time = getTime(start);

        //
        ////////////////////////////////////////////////////////////////////////

        cout << endl << "_obj: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;




*/
        cout << endl;





        //cout << "// Fin de " << file;
        //cout << endl << "//////////////////////////////////////////////" << endl << endl;    


        ////////////////////////////////////////////////////////////////////////////
        // spambase
        //

        /*cout <<  endl << "//////////////////////////////////////////////" << endl;
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
        cout << "//////////////////////////////////////////////////////////" << endl << endl;*/
        
    }
    
    
     
    ////////////////////////////////////////////////////////////////////////////
    // 
    ////////////////////////////////////////////////////////////////////////////
     
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
    
    meanR = vector<float>(14, 0);
    meanT = vector<float>(14, 0);
    
}

////////////////////////////////////////////////////////////////////////////////
// Inicializa los vectores a 0
//
void initMean(vector<float>& mean){
    
    mean = vector<float>(14, 0);
    
}

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto
//
void exec(string file, bool label){
    
    struct timeval start;
    
    float time;
    
    eval res;
    
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
        
        meanR[0] += get<0>(res);
        
        meanT[0] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[2] += get<0>(res);
        
        meanT[2] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
                
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
        
        meanR[1] += get<0>(res);
        
        meanT[1] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
                
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
        
        meanR[3] += get<0>(res);
        
        meanT[3] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[4] += get<0>(res);
        
        meanT[4] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[5] += get<0>(res);
        
        meanT[5] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[6] += get<0>(res);
        
        meanT[6] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[7] += get<0>(res);
        
        meanT[7] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[8] += get<0>(res);
        
        meanT[8] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[9] += get<0>(res);
        
        meanT[9] += time;
    
        cout << "Partición " << i+1 << "-1: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[0] += get<0>(res);
        
        meanT[0] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[2] += get<0>(res);
        
        meanT[2] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
                        
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
        
        meanR[1] += get<0>(res);
        
        meanT[1] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[3] += get<0>(res);
        
        meanT[3] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[4] += get<0>(res);
        
        meanT[4] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[5] += get<0>(res);
        
        meanT[5] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[6] += get<0>(res);
        
        meanT[6] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[7] += get<0>(res);
        
        meanT[7] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[8] += get<0>(res);
        
        meanT[8] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
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
        
        meanR[9] += get<0>(res);
        
        meanT[9] += time;
    
        cout << "Partición " << i+1 << "-2: " << "%_clas: " << setP(get<0>(res)) << "%, T: " << time << " segundos" << endl;
        
        
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

////////////////////////////////////////////////////////////////////////////////
// Ejecuta la prueba con una base de datos en concreto para la práctica 2
//
void exec2(string file, bool label){
    
    struct timeval start;
    
    float time;
    
    eval res;
    
    // Función objetivo (Agregado)
    vector<float> meanA;
    
    // Clasificación
    vector<float> meanC;
    
    // Reducción
    vector<float> meanR;
    
    // Tiempo
    vector<float> meanT;
    
    // Se inicializan los valores para calcular la media
    initMean(meanA);
    initMean(meanC);
    initMean(meanR);
    initMean(meanT);
    
    // Se cargan los datos de la bbdd
    Arff data(file, label);
        
    // Se crearán 5 particiones aleatorias de cada muestra
    for(unsigned int i=0; i<NUM_PARTITIONS; i++){
        
        // Se utiliza para guardar ficheros auxiliares con un nombre 
        // identificativo
        part = i+1;
                
        // Se crea el clasificador/evaluador de los datos
        Sorter sorter(data);
                
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo 1-NN
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Algoritmo 1-NN" << endl << endl;
                
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        // Aunque comienza con el vector de pesos aleatorio, se indica que se
        // use ese para contar el tiempo que tarda en crearlo
        //sorter.setTypeW(RANDOM);
        
        // Se evalúa el % de acierto de la solución aleatoria con los datos de
        // entrenamiento
        res = sorter.evaluate();
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[0] += get<0>(res);
        
        meanC[0] += get<1>(res);
        
        meanR[0] += get<2>(res);
        
        meanT[0] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Búsqueda local
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// BL" << endl << endl;
                        
        // Se crea el objeto que va a lanzar la búsqueda local, pasándole el
        // clasificador que ya tenemos con el algoritmo Relief
        LocalSearch ls(sorter);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto de la solución aleatoria con los datos de
        // entrenamiento      
        res = ls.getSorter().evaluate(ls.localSearch().first);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[6] += get<0>(res);
        
        meanC[6] += get<1>(res);
        
        meanR[6] += get<2>(res);
        
        meanT[6] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;

        
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
        
        meanA[1] += get<0>(res);
        
        meanC[1] += get<1>(res);
        
        meanR[1] += get<2>(res);
        
        meanT[1] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Simulated Annealing
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Simulated Annealing" << endl << endl;
        
        // Se crea el objeto que utiliza el SA
        TSA sa(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        //sa.setExp(1);

        // Se evalúa el SA
        res = sa.sa();
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[2] += get<0>(res);
        
        meanC[2] += get<1>(res);
        
        meanR[2] += get<2>(res);
        
        meanT[2] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Simulated Annealing - EXPERIMENTO
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Simulated Annealing - EXPERIMENTO" << endl << endl;
        
        // Se crea el objeto que utiliza el SA
        TSA saExp(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        saExp.setExp(1);

        // Se evalúa el SA
        res = saExp.sa();
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[9] += get<0>(res);
        
        meanC[9] += get<1>(res);
        
        meanR[9] += get<2>(res);
        
        meanT[9] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
          
        ////////////////////////////////////////////////////////////////////////
        // Iterative Local Search
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Iterative Local Search" << endl << endl;
        
        // Se crea el objeto que utiliza la ILS
        TSA ils(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        //ils.setExp(1);

        // Se evalúa la ILS
        res = ils.ils();
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[3] += get<0>(res);
        
        meanC[3] += get<1>(res);
        
        meanR[3] += get<2>(res);
        
        meanT[3] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                      
        ////////////////////////////////////////////////////////////////////////
        // Iterative Local Search - EXPERIMENTO
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Iterative Local Search - EXPERIMENTO" << endl << endl;
        
        // Se crea el objeto que utiliza la ILS
        TSA ilsExp(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        ilsExp.setExp(1);

        // Se evalúa la ILS
        res = ilsExp.ils();
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[10] += get<0>(res);
        
        meanC[10] += get<1>(res);
        
        meanR[10] += get<2>(res);
        
        meanT[10] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                              
        ////////////////////////////////////////////////////////////////////////
        // Differential Evolution, DE/rand/1
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Differential Evolution, DE/rand/1" << endl << endl;
        
        // Se crea el objeto que utiliza la DE
        DE deRand(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        //deRand.setExp(1);

        // Se evalúa la DE con el operador de cruce Rand
        res = deRand.evaluate(RAND);
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[4] += get<0>(res);
        
        meanC[4] += get<1>(res);
        
        meanR[4] += get<2>(res);
        
        meanT[4] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                     
        ////////////////////////////////////////////////////////////////////////
        // Differential Evolution, DE/rand/1 - EXPERIMENTO
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Differential Evolution, DE/rand/1 - EXPERIMENTO" << endl << endl;
        
        // Se crea el objeto que utiliza la DE
        DE deRandExp(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        deRandExp.setExp(1);

        // Se evalúa la DE con el operador de cruce Rand
        res = deRandExp.evaluate(RAND);
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[11] += get<0>(res);
        
        meanC[11] += get<1>(res);
        
        meanR[11] += get<2>(res);
        
        meanT[11] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                             
        ////////////////////////////////////////////////////////////////////////
        // Differential Evolution, DE/current-to-best/1
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Differential Evolution, DE/current-to-best/1" << endl << endl;
        
        // Se crea el objeto que utiliza la DE
        DE deBest(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        //deBest.setExp(1);

        // Se evalúa la DE con el operador de cruce Best
        res = deBest.evaluate(BEST);
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[5] += get<0>(res);
        
        meanC[5] += get<1>(res);
        
        meanR[5] += get<2>(res);
        
        meanT[5] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                             
        ////////////////////////////////////////////////////////////////////////
        // Differential Evolution, DE/current-to-best/1 - EXPERIMENTO
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// Differential Evolution, DE/current-to-best/1 - EXPERIMENTO" << endl << endl;
        
        // Se crea el objeto que utiliza la DE
        DE deBestExp(data);

        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
        
        deBestExp.setExp(1);

        // Se evalúa la DE con el operador de cruce Best
        res = deBestExp.evaluate(BEST);
                
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[12] += get<0>(res);
        
        meanC[12] += get<1>(res);
        
        meanR[12] += get<2>(res);
        
        meanT[12] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
                                     
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con BLX
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-BLX" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genGGBLX(data, GG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        //genGGBLX.setExp(1);
        
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genGGBLX.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[7] += get<0>(res);
        
        meanC[7] += get<1>(res);
        
        meanR[7] += get<2>(res);
        
        meanT[7] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;

        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Genético Generacional con BLX - EXPERIMENTO
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// GG-BLX - EXPERIMENTO" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genGGBLXExp(data, GG);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        genGGBLXExp.setExp(1);
        
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genGGBLXExp.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[13] += get<0>(res);
        
        meanC[13] += get<1>(res);
        
        meanR[13] += get<2>(res);
        
        meanT[13] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;
        
        ////////////////////////////////////////////////////////////////////////
        // Algoritmo Memético - Genético Generacional con BLX - (10, 0.1mej)
        ////////////////////////////////////////////////////////////////////////
        
        cout << endl << "/////////////////////////////" << endl;
                
        ////////////////////////////////////////////////////////////////////////
        //
        cout << "// AM-(10,0.1mej)" << endl << endl;
        
        // Se crea el objeto que utiliza el algoritmo genético generacional
        GeneticA genMA(data, GG);
        
        genMA.setTypeLocalSearch(2);
        
        ////////////////////////////////////////////////////////////////////////
        // Se comienza a contar el tiempo
        startTime(start);
                
        // Se evalúa el % de acierto del algoritmo genético GG con BLX
        res = genMA.evaluate(BLX);
        
        // Se guarda el tiempo
        time = getTime(start);
        
        //
        ////////////////////////////////////////////////////////////////////////
        
        meanA[8] += get<0>(res);
        
        meanC[8] += get<1>(res);
        
        meanR[8] += get<2>(res);
        
        meanT[8] += time;
    
        cout << "Partición " << i+1 << " -> Agr: " << setP(get<0>(res)) << "%, _class: " << setP(get<1>(res)) 
                << "%, _red: " << setP(get<2>(res)) << "%, T: " << time << " segundos" << endl;

        // Cuando ya ha terminado se hará el cambio de datos del conjunto de
        // entrenamiento/test para que en la siguiente iteración empiece de 
        // nuevo
        data.changeData();
        
        cout << "TERMINA" << endl;
        
    }
        
    cout << endl << "//////////////////////////////////////////////////////////" << endl;
    cout << "////////// Tiempos totales finales" << endl;
    
    cout << endl << "// 1-NN:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[0]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[0]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[0]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[0]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// Relief:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[1]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[1]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[1]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[1]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// ES:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[2]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[2]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[2]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[2]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// ILS:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[3]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[3]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[3]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[3]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// DE/rand/1:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[4]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[4]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[4]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[4]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// DE/current-to-best/1:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[5]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[5]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[5]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[5]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// BL:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[6]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[6]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[6]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[6]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// GG-BLX:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[7]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[7]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[7]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[7]/NUM_PARTITIONS << " segundos" << endl;
            
    cout << endl << "// AM-(10,0.1mej):" << endl;
    cout << "Media del % del Agr: " << setP(meanA[8]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[8]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[8]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[8]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// ES - EXPERIMENTO:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[9]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[9]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[9]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[9]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// ILS - EXPERIMENTO:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[10]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[10]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[10]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[10]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// DE/rand/1 - EXPERIMENTO:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[11]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[11]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[11]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[11]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// DE/current-to-best/1 - EXPERIMENTO:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[12]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[12]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[12]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[12]/NUM_PARTITIONS << " segundos" << endl;
    
    cout << endl << "// GG-BLX - EXPERIMENTO:" << endl;
    cout << "Media del % del Agr: " << setP(meanA[13]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _clas: " << setP(meanC[13]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del % de _red: " << setP(meanR[13]/NUM_PARTITIONS) << "%" << endl;
    cout << "Media del tiempo: " << meanT[13]/NUM_PARTITIONS << " segundos" << endl;
    
    cout <<  endl << endl << "//////////////////////////////////////////////" << endl << endl;
    
}