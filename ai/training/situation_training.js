var Init =
{
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "Amber Valley.map",           // map that will be used for training
    mutationChance  = 0.1,                          // chance for a weight to mutate at random
    mutationRate    = 0.2,                          // how much the weight gets altered.
    networkToTrain  = 0,
    fogOfWar        = GameEnums.Fog_Off,            // fog of war rule for training
    maxRuns         = 100000,                       // maximum amount of iterations
    logLevel        = 1,
    // ai's and names that will be used for training
    topAis          = 4,
    trainingAis     =  [["heavy_ai",    5],
                        ["heavy_ai1",   6],
                        ["heavy_ai2",   7],
                        ["heavy_ai3",   8],
                        ["heavy_ai4",   9],
                        ["heavy_ai5",   10],
                        //                        ["heavy_ai6",   11],
                        //                        ["heavy_ai7",   12],
                        //                        ["heavy_ai8",   13],
                        //                        ["heavy_ai9",   14],
    ],

    // internal data
    startAi = 0,
    matchData   = [],   // array with final score results
    start = false,
    coreData = [],
    rotationCount = 0,
    // enum states for remote game triggering
    runNextBattle = 0,
    sleep = 1,
    nextRun = 2,

    main = function(menu)
    {
        menu.enterSingleplayer();
    },

    gameMenu = function(menu)
    {
        menu.exitGame();
    },

    onVictory = function(menu)
    {
        Init.startAllCores();
    },

    startAllCores = function()
    {
        // reset all cores
        Init.coreData = [];
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
            }
        }
        for (var i = 0; i < Init.cores; ++i)
        {
            Init.coreData.push(["Core" + i.toString(), false, []]);
            Init.startRemoteGame(i);
        }
    },

    startRemoteGame = function(coreIndex)
    {
        if (Init.startAi < Init.trainingAis.length)
        {
            GameConsole.print("Preparing next match on core " + Init.coreData[coreIndex][0], Init.logLevel);
            Init.coreData[coreIndex][1] = false;
            Init.coreData[coreIndex][2] = [];
            Init.coreData[coreIndex][3] = Init.startAi;
            var map = Init.trainingFolder + Init.trainingMap;
            var script = "var Init =\n" +
                    "{\n" +
                    "main = function(menu)\n" +
                    "{\n" +
                    "menu.enterSingleplayer();\n" +
                    "},\n" +
                    "mapsSelection = function(menu)\n" +
                    "{\n" +
                    "menu.selectMap(\"" + Init.trainingFolder + "\", \"" + Init.trainingMap + "\");\n" +
                    "menu.buttonNext();\n" +
                    "menu.buttonNext();\n" +
                    "var gameRules = map.getGameRules();\n" +
                    "gameRules.addVictoryRule(\"VICTORYRULE_TURNLIMIT\");\n" +
                    "var victoryRule = gameRules.getVictoryRule(\"VICTORYRULE_TURNLIMIT\");\n" +
                    "victoryRule.setRuleValue(" + Init.turnLimit + ", 0);\n" +
                    "gameRules.setFogMode(" + Init.fogOfWar.toString() + ");\n" +
                    "gameRules.setRandomWeather(false);\n" +
                    "var selection = menu.getPlayerSelection();\n";
            var aiIdx = Init.startAi;
            GameConsole.print("Using ai " + aiIdx, Init.logLevel);
            GameConsole.print("Using ai-setting " + Init.trainingAis[aiIdx][0], Init.logLevel);
            script += "selection.selectPlayerAi(0, " + Init.trainingAis[aiIdx][1].toString() + ");\n";
            Init.coreData[coreIndex][2].push(aiIdx);
            script += "menu.startGame();\n" +
                    "},\n" +
                    "}";

            mainServer.startRemoteGame(script, Init.coreData[coreIndex][0]);
            ++Init.startAi;
        }
    },

    onRemoteGameFinished = function(score, core)
    {
        var coreIndex = 0;
        for (var i = 0; i < Init.cores; ++i)
        {
            if (Init.coreData[i][0] === core)
            {
                coreIndex = i;
                break;
            }
        }
        GameConsole.print("Core " + core + " finished", Init.logLevel);
        Init.coreData[coreIndex][1] = true;
        var state = Init.evaluateMatch(score, coreIndex);
        if (state === Init.nextRun)
        {
            Init.prepareNextRun();
            Init.startAllCores();
        }
        else if (state === Init.runNextBattle)
        {
            Init.startRemoteGame(coreIndex);
        }
    },

    evaluateMatch = function(score, coreIndex)
    {
        var nextRound = Init.runNextBattle;
        var ai = Init.coreData[coreIndex][3];
        Init.matchData[ai] = score;
        if (Init.startAi >= Init.trainingAis.length)
        {
            nextRound = Init.nextRun;
            for (var i = 0; i < Init.cores; ++i)
            {
                if (Init.coreData[i][1] === false)
                {
                    // not all cores are idle waiting for all games to be finished
                    nextRound = Init.sleep;
                    break;
                }
            }
        }
        return nextRound;
    },

    prepareNextRun = function()
    {
        GameConsole.print("Preparing next run", Init.logLevel);
        var bestAis = [];
        for (var i = 0; i < Init.matchData.length; ++i)
        {
            GameConsole.print("AI " + i + " got a score of " + Init.matchData[i] + " points", Init.logLevel);
            var aiScore = Init.matchData[i];
            if (aiScore > 0)
            {
                if (bestAis.length < Init.topAis)
                {
                    bestAis.push([Init.trainingAis[i], aiScore]);
                }
                else
                {
                    var lowestScore = aiScore;
                    var lowestIndex = -1;
                    for (var i2 = 0; i2 < bestAis.length; ++i2)
                    {
                        if (bestAis[i2][1] < lowestScore)
                        {
                            lowestScore = bestAis[i2][1];
                            lowestIndex = i2;
                        }
                    }
                    if (lowestIndex >= 0)
                    {
                        bestAis[lowestIndex] = [Init.trainingAis[i], aiScore];
                    }
                }
            }
        }
        var aiNames = [];
        GameConsole.print("Best selected ai's are: ", Init.logLevel);
        for (i = 0; i < bestAis.length; ++i)
        {
            aiNames.push(bestAis[i][0][0]);
            GameConsole.print(aiNames[i] + " with a score of " + bestAis[i][1] + " points", Init.logLevel);
        }
        var mutateCount = 0;
        for (i = 0; i < Init.trainingAis.length; ++i)
        {
            var mutate = true;
            for (var i2 = 0; i2 < aiNames.length; ++i2)
            {
                if (aiNames[i2] === Init.trainingAis[i][0])
                {
                    mutate = false;
                    break;
                }
            }
            if (mutate)
            {
                mutateCount = Init.mutate(mutateCount, i, aiNames);
            }
        }
        Init.runCount = Init.runCount + 1;
        GameConsole.print("Starting run: " + Init.runCount, Init.logLevel);
        // reset data
        if (Init.runCount < Init.maxRuns)
        {
            Init.startAi = 0;
            Init.matchData = [];
        }
    },

    mutate = function(mutateCount, i, aiNames)
    {
        GameConsole.print("Mutating ai: " + Init.trainingAis[i][0], Init.logLevel);
        var dummyAi = map.getPlayer(0).getBaseGameInput();
        var ai = globals.randInt(0, aiNames.length + 1);
        if (ai < aiNames.length)
        {
            ai = mutateCount % Init.topAis;
            var aiName = [aiNames[ai]];
            dummyAi.combineAi(aiName);
        }
        else
        {
            dummyAi.combineAi(aiNames);
        }
        dummyAi.setAiName(Init.trainingAis[i][0]);
        var neuralNetworkCount = dummyAi.getMaxNeuralNetworks();
        dummyAi.mutateNeuralNetwork(Init.networkToTrain, Init.mutationRate);
        ++mutateCount;
        return mutateCount;
    },
}
