w = currentAttributeSolution
best = bestAttributeSolution
p1 = parent1
p2 = parent2
return truncate(w + f*(best-w) + f*(p1-p2))


// Cruce rand

p1 = parent1
p2 = parent2
p3 = parent3
return truncate(p1 + f*(p2-p3))


// DE

population = generateRandomPopulation()
pSuccess = evaluate(population)
numEval = size(population)
numParents = 2|3 // Dependiendo del tipo de cruce
while numEval < maxEval
{
    for i in size(population)
    {
        parents = getParents(population, numParents) // Dependiendo del cruce
                                                     // elige tres padres o
                                                     // dos padres y el mejor
        posW = randomInt(1, size(population)) // Se valida que no sea
                                              // un padre elegido
        w = population[posW]
        wEval = pSuccess[posW]
        wAux
        for j in numAttributes
        {
            if randomInt(0,1) <= probabilityOfCrossing
                wAux[j] = cross(w[j], parents[1][j], parents[2][j],
                                                        parents[3][j])
        }
        wEvalAux = evaluate(wAux)
        numEval++
        if wEval < wEvalAux
        {
            population[posW] = wAux
            pSuccess[posW] = wEvalAux
        }
    }
    sortPopulation()
}
return first(population), first(pSuccess)



// Mutación fuerte

w = currentSolution
posMutate = generateRandomPositionsWithoutRepetitions(t, 1, numAttributes)
for i in posMutate
{
    w[i] = w[i] + NormalDistribution(mean, standardDeviation)
    truncate(w[i], 0, 1)
}
return w


// ILS

t = calculateAttributesToChange()
w = generateRandomSolution()
wEval = LocalSearch(w)
for i in maxIterations
{
    wAux = mutate(w, t)
    evalAux = LocalSearch(wAux)
    if(evalAux > wEval)
    {
        w = wAux
        wEval = evalAux
    }
}
return w, wEval


// Enfriamiento de la tempertura - ES

beta = (initialTemperature-finalTemperature) / M*initialTemperature*finalTemperature
temperature = temperature / (1 + (beta*temperature))



// Temperatura inicial - ES

initialTemperature = (mu*eval)/-ln(phi)



// Enfriamiento Simulado

w = generateRandomSolution()
wEval = evaluate(w)
bestW = w
bestEval = wEval
t = initTemperature(wEval)
numEval = 1
success = inf
while numEval < maxEval && success != 0
{
    iNeighbors = 0
    success = 0
    while iNeighbors < maxNeighbors && success < maxSuccess
    {
        wAux = mutate(w)
        wEvalAux = evaluate(wAux)
        numEval++
        differenceEval = wEvalAux - wEval
        if differenceEval > 0 || randomInt(0,1) >= exp(differenceEval/t)
        {
            w = wAux
            wEval = wEvalAux
            success++
            if(bestW < wEval)
            {
                bestW = w
                bestEval = wEval
            }
        }
        iNeighbors++
    }
    t = coolTemperature(t)
}
return bestW, bestEval



// Algoritmo Distancia Euclídea para 1-NN

e1 = example1
e2 = example2
w = currentSolution
res = 0
for i in numAttributes
{
    if w[i] > minReduction
    {
        sub = e1[i] - e2[i]
        res = res + (w[i] * sub* sub)
    }
}
res = sqrt(res)
return res



// Algoritmo Memético

    .
    .
    .
        mutate(newPopulation[randomInt(1, size(population))])
    }
    posToApplyLocalSearch = generatePosToApplyLocalSearch()
    evaluations = LocalSearch(posToApplyLocalSearch, newPopulation,
                                newSuccessPopulation)
    iEval = iEval + evaluations
    for i in size(newPopulation)
    {
        if i not posToApplyLocalSearch
            evaluate(newPopulation[i])
    }
    if first(population) > first(newPopulation)
    .
    .
    .


// Algoritmo Genético Generacional

population = currentPopulation
successPopulation = currentSuccessPopulation
numCrosses = round(probabilityOfCrossing*size(population))
numMutations = round(probabilityOfMutation*size(population)*numAttributes)
iEval = 0
while iEval < numEval
{
    newPopulation = selection()
    newSuccessPopulation = container(size(newPopulation))
    for i in numCrosses
    {
        newPopulation[i] = cross(newPopulation[i])
    }
    for i in numMutations
    {
        newPopulation[i] = mutate(newPopulation[i])
    }
    for i in size(newPopulation)
    {
        equals = false
        j = 0
        while not equals && j<size(population)
        {
            if newPopulation[i] == population[j]
                equals = true
            j++
        }
        if equals
            newSuccessPopulation[i] = successPopulation[j-1]
        else
            newSuccessPopulation[i] = evaluate(newPopulation[i])
    }
    if first(population) > first(newPopulation)
        update(first(population), last(newPopulation))
    population = newPopulation
    successPopulation = newSuccessPopulation
    iEval = iEval + size(population)
}
return bestSolution(), bestSuccess()



// Mecanimo de selección de los AGs

selection = container(numParents)
population = currentPopulation
for i in numParents
{
    pos1 = randomInt(1, numPopulation)
    pos2
    while(pos1 == (pos2 = randomInt(1, numPopulation))){
    }
    if success(population[pos1]) >= success(population[pos2])
        selection[i] = population[pos1]
    else
        selection[i] = population[pos2]
}
return selection







// Función objetivo

well = 0
notUsed = 0
for i in numInstances
{
    if label[i] == NN1(data[i])
    {
        well++;
    }
}
for i in numAttributes
{
    if w[i] < minReduction
    {
        notUsed++
    }
}
return well/numInstances*weightWell + notUsed/numAttributes*(1-weightWell)
