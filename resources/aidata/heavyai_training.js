var Init =
{
    // training setup data
    trainingFolder  = "maps/2_player/",     // map folder used
    trainingMap     = "Agitated.map",       // map that will be used for training
    mutationRate    = 0.05,                 // chance for a weight to mutate at random
    fogOfWar        = GameEnums.Fog_Off,    // fog of war rule for training
    maxRuns         = 1000,                 // maximum amount of iterations
    // ai's and names that will be used for training
    topAis          = 4,
    trainingAis     =  [["heavy_ai",    5],
                        ["heavy_ai1",   6],
                        ["heavy_ai2",   7],
                        ["heavy_ai3",   8],
                        ["heavy_ai4",   9],
                        ["heavy_ai5",   10],
                        ["heavy_ai6",   11],
                        ["heavy_ai7",   12],
                        ["heavy_ai8",   13],
                        ["heavy_ai9",   14],],
    // internal data
    startAi = 0,
    rotationStartAi = 0,
    rotationCount = 0,
    currentMatch = [],
    matchData   = [],
    runCount    = 0,
    main = function(menu)
    {
        if (Init.runCount < Init.maxRuns)
        {
            menu.enterSingleplayer();
        }
    },

    mapsSelection = function(menu)
    {
        GameConsole.print("Preparing next match", 0);
        var playerCount = map.getPlayerCount();
        Init.currentMatch = [];
        if (Init.matchData.length === 0)
        {
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                Init.matchData.push(0);
            }
        }
        menu.selectMap(Init.trainingFolder, Init.trainingMap);
        menu.slotButtonNext();
        menu.slotButtonNext();
        var selection = menu.getPlayerSelection();
        var gameRules = map.getGameRules();
        gameRules.setFogMode(Init.setFogMode);
        gameRules.setRandomWeather(false);
        for (var i = 0; i < playerCount; ++i)
        {
            var player = Init.rotationCount + i;
            if (i > 0)
            {
                player += Init.rotationStartAi;
            }
            else
            {
                player += Init.startAi;
            }

            if (player >= playerCount)
            {
                player -= playerCount;
            }
            selection.selectPlayerAi(i, trainingAis[player][1]);
            GameConsole.print("Using ai-setting " + trainingAis[player][0] + " for player " + i, 0);
            Init.currentMatch.push(player);
        }
        // todo add co mirror matches
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
        var team = map.getVictoryTeam();
        var nextRound = false;
        if (team >= 0)
        {
            var winnerAi = Init.currentMatch[team];
            GameConsole.print("Winning Ai is " + Init.trainingAis[player][0], 0);
            for (var i = 0; i < Init.trainingAis.length; ++i)
            {
                if (Init.trainingAis[player][0] === Init.trainingAis[i][0])
                {
                    Init.matchData[i] += 1;
                }
            }
        }
        Init.rotationCount += 1;
        if (Init.rotationCount === playerCount)
        {
            GameConsole.print("Going for next match up", 0);
            Init.rotationCount = 0;
            Init.rotationStartAi += 1;
            if (Init.rotationStartAi === Init.trainingAis.length - playerCount)
            {
                nextRound = true;
            }
        }
        return nextRound;
    },

    prepareNextRun = function()
    {
        GameConsole.print("Preparing next run", 0);
        // todo mutate ai's that haven't one the match
        Init.runCount = Init.runCount + 1;
        // reset data
        Init.startAi = 0;
        Init.rotationStartAi = 0;
        Init.rotationCount = 0;
        Init.currentMatch = [];
        Init.matchData = [];
    },
}
