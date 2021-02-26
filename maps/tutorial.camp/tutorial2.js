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
        // called when a player wins
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Epoch!!! Stop ruining my fun. I'll shut you down if you continue..."),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Please noooo. I'll do what you want!"),
                    "co_epoch", GameEnums.COMood_Sad, PLAYER.getDefaultColor(6));
        dialog1.queueAnimation(dialog2);
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var tower = map.getTerrain(1, 4).getBuilding();
        var mine = map.getTerrain(6, 4).getBuilding();
        var radar = map.getTerrain(2, 5).getBuilding();
        if (tower.getOwner() === map.getPlayer(0))
        {
            // read the tower variable or create it
            var towerDialiog = variables.createVariable("towerDialiog");
            if (towerDialiog.readDataBool() === false)
            {
                // call tower dialog
                gameScript.towerDialog();
                // set variable to true since the dialog has been played
                towerDialiog.writeDataBool(true);
            }
        }
        if (mine.getOwner() === map.getPlayer(1))
        {
            // read the mine variable or create it
            var mineDialiog = variables.createVariable("mineDialiog");
            if (mineDialiog.readDataBool() === false)
            {
                // call mine dialog
                gameScript.mineDialog();
                // set variable to true since the dialog has been played
                mineDialiog.writeDataBool(true);
            }
        }
        if (radar.getOwner() === map.getPlayer(0))
        {
            // read the mine variable or create it
            var radarDialiog = variables.createVariable("radarDialiog");
            if (radarDialiog.readDataBool() === false)
            {
                // call mine dialog
                gameScript.radarDialog();
                // set variable to true since the dialog has been played
                radarDialiog.writeDataBool(true);
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
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee! You dropped my superior army!!! So I can have a lot of fun with Epoch."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes Lash, as you have ordered, a superior army."),
                    "co_officier_bh", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? Why does Epoch have a Laser?"),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog2.queueAnimation(dialog3);
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You ordered to place a Laser there."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog3.queueAnimation(dialog4);
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("... For me, not for him. Luckily he doesn't know he has to select the Laser and press fire to use it. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog4.queueAnimation(dialog5);
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huuuhuuu, Epoch, listen, in this tutorial you learn everything about CO units and buildings."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog5.queueAnimation(dialog6);
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm ready to learn more!"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog5.queueAnimation(dialog7);
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We both start with a CO-Unit on this map. You can detect the CO-Unit by the medal rank. ") +
                    qsTr("You can have a CO-Unit for each CO you have. A CO-Unit creates an area around it in which units ") +
                    qsTr("get buffed. Those buffs stack if a unit is inside the range of two CO-Units."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog7.queueAnimation(dialog8);
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Some COs like this sniper guy, Grit, ") +
                    qsTr("have global effects for certain or all units. Tee heee, luckily those effects often have a negative effect as well. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog8.queueAnimation(dialog9);
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Got it!!! COs are always stronger inside the CO-Zone!!!"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog9.queueAnimation(dialog10);
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You gain power for funds-damage you deal or got dealt. You gain more power when the damage is dealt inside the CO-Zone. ") +
                    qsTr("However you loose half your CO-Bar when your CO-Unit is destroyed. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog10.queueAnimation(dialog11);
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Epoch never loses his CO-Unit!"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog11.queueAnimation(dialog12);
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The CO-Zone of a CO raises when you gathered enough to use your CO-Power by 1 space and by 2 spaces for the CO-Superpower. ") +
                    qsTr("If you have two COs and both can use the Superpower, you can use the Tagpower instead. This will activate both superpowers ") +
                    qsTr("at the same time and for certain CO-Combinations give you additional strength."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog12.queueAnimation(dialog13);
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood, all units stay inside the CO-Zone! Attack and use the Laser."),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        dialog13.queueAnimation(dialog14);
    };
    this.mineDialog = function()
    {
        // give a small dialog again
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hee hee! I have a mine now. This gives me a lot of funds. Now I can build so many troops. Huh? Where's my factory?"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We built a factory near the HQ just as you ordered."),
                    "co_officier_bh", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Again for Epoch. Do I have to write down everything in detail? Can't you fools think for yourself?..."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog2.queueAnimation(dialog3);
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well then, give me CO-Power. Hihihi..."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog3.queueAnimation(dialog4);
        dialog4.setEndOfAnimationCall("gameScript", "getLashPower");
    };
    this.towerDialog = function()
    {
        // give a small dialog again
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Captured tower. I don't get anything from it. Why are there towers?"),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Towers give you a small amount of offensive and defensive power for all units. You dumb robot."),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
    };
    this.radarDialog = function()
    {
        // give a small dialog again
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("RADAR OWNED BY EPOCH!!! EPOCH IS THE BEST..."),
                    "co_epoch", GameEnums.COMood_Normal, PLAYER.getDefaultColor(6));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hee hee. We have no fog of war here, Epoch, so the enhanced vision range does nothing. Hiii hii! You won't win this way."),
                    "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
    };
    this.getLashPower = function()
    {
        // just give an insane amount of power -> will be capped at max.
        map.getPlayer(1).getCO(0).setPowerFilled(20);
    };
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
