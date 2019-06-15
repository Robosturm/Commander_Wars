var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.victory = function(team)
    {
        // we could check if a human player has won the map but i'm lazy here ;)
        // called when a player wins
        // just an example dialog you can spam out after a player won or lost.
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee! What?? Nooo not fun!! Epoch you won't enjoy the next tutorial. Heee...Heee"),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(true); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // map.getGameRules().changeWeatherChance("WEATHER_1SUN", 90); // sets the weather chance of sun to 90. The actual chance is the the value divided through the sum of all chances
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
    };
    this.actionDone = function()
    {
        // check if we spawned the inital tanks after lash lost her first infantry
        if (map.getPlayer(1).getUnitCount() === 1)
        {
            // read the spawn variable or create it
            var variables = map.getGameScript().getVariables();
            var tanksSpawned = variables.createVariable("spawned");
            // check if it's true or not
            if (tanksSpawned.readDataBool() === false)
            {
                // call spawn tanks
                gameScript.spawnTanks();
                // set variable to true since we spawned the tanks
                tanksSpawned.writeDataBool(true);
            }
        }
    };
    this.turnStart = function(turn, player)
    {
        // called at the start of each players turn
        if (turn === 1 && player === 0)
        {
            // play the inital dialog on day one and player 0
            gameScript.initDialog();
        }
    };
    this.initDialog = function()
    {
        // a long long queued dialog...
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee! Epoch i have a new Beta-Release for you. This new Version will be so much fun to play with!!"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No Lash please no update. I work perfectly."),
                    "co_epoch", GameEnums.COMood_Sad, PLAYER.getDefaultColor(6));
        dialog1.queueAnimation(dialog2);
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                                qsTr("C'mon. This is not you're choice."),
                                "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog2.queueAnimation(dialog3);
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Rebooting... System...Installing update... 50%...70%...70%...70%..."),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog3.queueAnimation(dialog4);
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                                qsTr("Tee hee hee. What did the update got stuck again???"),
                                "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog4.queueAnimation(dialog5);
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Update Installed!!! Hello i'm Epoch you're frienldy robot. What is my job? Who am I? Where am I? Why am I?"),
                    "co_epoch", GameEnums.COMood_Happy, PLAYER.getDefaultColor(6));
        dialog5.queueAnimation(dialog6);
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Again really? Epoch why you can't remember a single setting after an Update? ") +
                    qsTr("So we need to start at zero again! Before the fun starts! Tee hee hee.") +
                    qsTr("Here comes you're basic training lesson Epoch!!"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog6.queueAnimation(dialog7);
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Initializing Input Buffer... Buffer Ready...Awaiting Commands!!!"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog7.queueAnimation(dialog8);
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's start with the user interface. Tee hee hee. On the left top you see the current co's players and funds. On the right ") +
                    qsTr("top you see the minimap of the current game. Clicking on it will bring the map to the clicked point. ") +
                    qsTr("Pressing a mouse button on it allows you to move the minimap, if it's bigger than the given place. ") +
                    qsTr("Below the minimap are some stats about the current player and the current weather. ") +
                    qsTr("On the right bottom you can see the stats of the current terrain and unit of the cursor. ") +
                    qsTr("Finally in the center of the screen is the current map on which all interactions can be done."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog8.queueAnimation(dialog9);
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Inserted User Interface. Question how to I enter commands?"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog9.queueAnimation(dialog10);
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hiiii...hiii press the left mouse button or space (default) to enter or accept a command! ") +
                    qsTr("You can move the curser either by moving the mouse or pressing w, a, s, d(default).") +
                    qsTr("Canceling a command or viewing the firerange of units and buildings can be done by pressing the right mouse button or b (default). ") +
                    qsTr("You can press e or q (default) to move the cursor to the next/previous unit or building. ") +
                    qsTr("You can move the map by pressing the middle mouse button and moving the map afterwards. ") +
                    qsTr("If you want to change the zoom use the mouse wheel. ") +
                    qsTr("Now let's start move your units and finish your turn. Tee heee. Now i can have fun."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog10.queueAnimation(dialog11);
    };

    this.spawnTanks = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee! Every time a unit kills another one, it will get a new rank which provides more offensive and defensive.") +
                    qsTr("There are three unit ranks exlcuding co-rank. We will deal with this later...") +
                    qsTr("But Now deal with my tank!! This will be so much fun. When it rolls over your infantries!! Tee hee!"),
                    "co_lash", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
        // spawn a light tank after Lash finished her sentence
        dialog1.setEndOfAnimationCall("gameScript", "spawnLashTank");
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Using Strg+C... Strg+V... Strg+V"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog1.queueAnimation(dialog2);
        // spawn the light tanks after Epoch finished her sentence
        dialog2.setEndOfAnimationCall("gameScript", "spawnEpochTanks");
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Stop! Stop! Stop Epoch you ruin my fun..."),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog2.queueAnimation(dialog3);
    };
    this.spawnLashTank = function()
    {
        // spawn a light tank at map positon x = 7 and y = 0 for player 1 in a check range of two fields -> shouldn't be needed just making sure the unit is always spawned
        map.spawnUnit(7, 0, "LIGHT_TANK", map.getPlayer(1), 2);
    };
    this.spawnEpochTanks = function()
    {
        // spawn a light tank at map positon x = 0 and y = 4 for player 0 in a check range of two fields -> shouldn't be needed just making sure the unit is always spawned
        map.spawnUnit(0, 4, "LIGHT_TANK", map.getPlayer(0), 2);
        // spawn a light tank at map positon x = 0 and y = 3 for player 0 in a check range of two fields -> shouldn't be needed just making sure the unit is always spawned
        map.spawnUnit(0, 3, "LIGHT_TANK", map.getPlayer(0), 2);
    };
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
