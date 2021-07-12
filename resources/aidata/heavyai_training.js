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
        menu.selectMap(Init.trainingFolder, Init.trainingMap);
        menu.slotButtonNext();
        menu.slotButtonNext();
        var selection = menu.getPlayerSelection();
        var gameRules = map.getGameRules();
        gameRules.setFogMode(Init.setFogMode);
        gameRules.setRandomWeather(false);
        // todo select ai's based on missing matches
        selection.selectPlayerAi(0, 5);
        selection.selectPlayerAi(1, 5);
        menu.startGame();
    },

    onVictory = function(menu)
    {
        // todo get victorious ai and store it
        // todo check if all matches have been done and prepare next run if so
        menu.exitMenue();
    },

    prepareNextRun = function()
    {
        // todo mutate ai's that haven't one the match
        Init.runCount = Init.runCount + 1
    },
}
