var Init =
{
    // training setup data
    trainingFolder  = "maps/2_player/",             // map folder used
    trainingMap     = "NB4DS.map",                  // map that will be used for training
    mutationRate    = 0.1,                          // chance for a weight to mutate at random
    fogOfWar        = GameEnums.Fog_OfWar,          // fog of war rule for training
    maxRuns         = 1000,                         // maximum amount of iterations
    turnLimit       = 30,
    // ai's and names that will be used for training
    topAis          = 1,
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
    rotationStartAi = 0,
    rotationCount = 0,
    currentMatch = [],
    currentBattleData = [],
    matchData   = [],
    runCount    = 0,
    logLevel    = 1,
    cos = ["CO_ANDY", "CO_JESS"],
    start = false,
    main = function(menu)
    {
        if (Init.start === false)
        {
            Init.selectCos();
            Init.start = true;
        }
        if (Init.runCount < Init.maxRuns)
        {
            menu.enterSingleplayer();
        }
    },

    mapsSelection = function(menu)
    {
        GameConsole.print("Preparing next match", Init.logLevel);
        menu.selectMap(Init.trainingFolder, Init.trainingMap);
        menu.buttonNext();
        menu.buttonNext();
        var gameRules = map.getGameRules();
        gameRules.addVictoryRule("VICTORYRULE_TURNLIMIT_CAPTURE_RACE");
        var victoryRule = gameRules.getVictoryRule("VICTORYRULE_TURNLIMIT_CAPTURE_RACE");
        victoryRule.setRuleValue(Init.turnLimit, 0);
        var selection = menu.getPlayerSelection();
        var playerCount = map.getPlayerCount();
        Init.currentMatch = [];
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
            }
        }
        if (Init.currentBattleData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.currentBattleData.push(0);
            }
        }
        gameRules.setFogMode(Init.fogOfWar);
        gameRules.setRandomWeather(false);
        for (var i = 0; i < playerCount; ++i)
        {
            var playerIdx = Init.rotationCount + i;
            if (playerIdx >= playerCount)
            {
                playerIdx -= playerCount;
            }
            var aiIdx = Init.rotationStartAi + i;
            if (i == 0)
            {
                aiIdx = Init.startAi;
            }
            GameConsole.print("Using ai at index " + aiIdx + " for player " + playerIdx, Init.logLevel);
            selection.selectPlayerAi(playerIdx, Init.trainingAis[aiIdx][1]);
            GameConsole.print("Using ai-setting " + Init.trainingAis[aiIdx][0] + " for player " + playerIdx, Init.logLevel);
            Init.currentMatch.push(aiIdx);

            selection.playerCO1Changed(Init.cos[0], i);
            selection.playerCO2Changed(Init.cos[1], i);
        }
        menu.startGame();
    },

    onVictory = function(menu)
    {
        if (Init.evaluateMatch())
        {
            Init.prepareNextRun();
        }
        menu.exitMenue();
    },

    evaluateMatch = function()
    {
        var playerCount = map.getPlayerCount();
        var team = map.getGameRules().getVictoryTeam();
        var nextRound = false;
        if (team >= 0)
        {
            var winnerAi = Init.currentMatch[team];
            GameConsole.print("Winning Ai is " + Init.trainingAis[winnerAi][0], Init.logLevel);
            Init.currentBattleData[winnerAi] += 1;
        }
        Init.rotationCount += 1;
        if (Init.rotationCount === playerCount)
        {
            Init.rotationCount = 0;
            Init.rotationStartAi += 1;
            for (var i = 0; i < Init.currentBattleData.length; ++i)
            {
                if (Init.currentBattleData[i] > 0)
                {
                    var wonBattles = Init.currentBattleData[i];
                    var possibleBattles = Init.currentMatch.length
                    var score = 0;
                    if (wonBattles === 1)
                    {
                        score = 1;
                    }
                    else if (wonBattles > wonBattles / 2)
                    {
                        score = (wonBattles - 1) * 3;
                    }
                    else
                    {
                        score = (wonBattles - 1) * 2;
                    }
                    GameConsole.print("Won " + wonBattles + " of " + possibleBattles + " possible Battles. Score=" + score, Init.logLevel);
                    Init.matchData[i] += score;
                }
            }
            Init.currentBattleData = [];
            if (Init.rotationStartAi > Init.trainingAis.length - playerCount)
            {
                GameConsole.print("Going for next match up", Init.logLevel);
                Init.startAi += 1;
                Init.rotationStartAi = Init.startAi;
                if (Init.startAi > Init.trainingAis.length - playerCount)
                {
                    nextRound = true;
                }
            }
        }
        GameConsole.print("Going for next match up Rotation count: " + Init.rotationCount, Init.logLevel);
        GameConsole.print("Going for next match up start ai: " + Init.startAi, Init.logLevel);
        GameConsole.print("Going for next match up Rotation start ai: " + Init.rotationStartAi, Init.logLevel);
        return nextRound;
    },

    prepareNextRun = function()
    {
        GameConsole.print("Preparing next run", Init.logLevel);
        var bestAis = [];
        for (var i = 0; i < Init.matchData.length; ++i)
        {
            GameConsole.print("AI " + i + " won " + Init.matchData[i] + " games", Init.logLevel);
            var aiScore = Init.matchData[i];
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
                    bestAis[lowestIndex] = [trainingAis[i], aiScore];
                }
            }
        }
        var aiNames = [];
        GameConsole.print("Best selected ai's are: ", Init.logLevel);
        for (i = 0; i < bestAis.length; ++i)
        {
            aiNames.push(bestAis[i][0][0]);
            GameConsole.print(aiNames[i] + " with " + bestAis[i][1] + " won matches", Init.logLevel);
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
                for (i2 = 0; i2 < neuralNetworkCount; ++i2)
                {
                    if (globals.randInt(0, 30) < 20)
                    {
                        dummyAi.mutateNeuralNetwork(i2, Init.mutationRate);
                    }
                }
                ++mutateCount;
            }
        }
        Init.runCount = Init.runCount + 1;
        GameConsole.print("Starting run: " + Init.runCount, Init.logLevel);
        // reset data
        Init.startAi = 0;
        Init.rotationStartAi = 0;
        Init.rotationCount = 0;
        Init.currentMatch = [];
        Init.matchData = [];
        Init.selectCos();
    },

    selectCos = function()
    {
        var cos = coSpriteManager.getCoIds();
        var index = globals.randInt(0, cos.length - 1);
        while (cos[index] === "CO_RANDOM")
        {
            index = globals.randInt(0, cos.length - 1);
        }
        Init.cos[0] = cos[index];
        var index2 = globals.randInt(0, cos.length - 1);
        while (index === index2 ||
               cos[index2] === "CO_RANDOM")
        {
            index2 = globals.randInt(0, cos.length - 1);
        }
        Init.cos[1] = cos[index2];
    }
}

