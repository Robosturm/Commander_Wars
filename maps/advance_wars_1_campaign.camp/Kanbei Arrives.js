var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediately without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.victory = function(team, map)
    {
        if (team === 0)
        {
            var playername = settings.getUsername();
            // called when a player wins
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Preposterous!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("So many of Kanbei's forces defeated in such a short time? Withdraw!"),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            dialog1.queueAnimation(dialog2);

            if (map.getCurrentDay() <= 8)
            {
                var campaignVariables = map.getCampaign().getVariables();
                var sonjaCounter = campaignVariables.createVariable("sonjaCounter");
                sonjaCounter.writeDataInt32(sonjaCounter.readDataInt32() + 1);
            }
        }
    };
    this.gameStart = function(map)
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var list = campaignScript.getBasicBuildList();
        map.getPlayer(0).setBuildList(list);
        map.getPlayer(1).setBuildList(list);
    };
    this.actionDone = function(action, map)
    {

    };

    this.turnStart = function(turn, player, map)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Hmm?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Like a plague, here comes the Orange Star Army! Not content with their own lands, they've come to steal mine!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Never, I say! Never! Kanbei's Imperial Forces shall drive them back!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Oh, no... I was just planning on passing through Yellow Comet, but..."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Kanbei's Imperial Forces spotted us before we even crossed the border."),
                    "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Kanbei's Imperial Forces?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Uh-huh. They're supposed to be the best-trained troops around. The high equipment costs and salaries must make them hard to maintain."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("I've heard of 'em before! Kanbei's supposed to be the reason no one messes with that little country. Even so, there's no way he's gonna beat me! Hey, ") + playername + qsTr("! Let's go get 'em!"),
                    "co_max", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));

        // doing the queueing of the dialog
        dialog0.queueAnimation(dialog1);
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

