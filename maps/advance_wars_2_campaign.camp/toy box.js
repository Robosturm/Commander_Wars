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
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Uh-oh. This might be trouble... Oh, well. I guess I'll have to find someplace new to play. Toodles! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("This is it. This is where my house used to be. "),
                        "co_olaf", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander... "),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("And over there, that was the town square. I played here all day with all the other kids in the neighborhood. It didn't matter how much snow was on the ground. It didn't bother us at all. Look, right here... Can you see the outlines of the square? "),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Olaf... "),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Black Hole! For this, there can be no forgiveness! "),
                        "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOUNITS"); // win by destroying all units
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var bmList = campaignScript.getBMBuildList();
        map.getPlayer(0).setBuildList(bmList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        var mapTown = map.getTerrain(3, 8).getBuilding();
        if (mapTown.getOwner() !== null &&
            mapTown.getOwner().getPlayerID() === 0)
        {
            var mapTownDialog = variables.createVariable("mapTownDialog");
            if (mapTownDialog.readDataBool() === false)
            {
                gameScript.mapTownDialog();
                // enable os map lab
                var campaignVariables = map.getCampaign().getVariables();
                var bmLabFound = campaignVariables.createVariable("bmLabFound");
                bmLabFound.writeDataBool(true);
                mapTownDialog.writeDataBool(true);
            }
        }
    };
    this.mapTownDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! We recovered this from a property we captured... "),
                    "co_officier_bm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? What's that? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, I'll be... What you've got there is a map pinpointing the enemy lab. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A lab? What do you mean? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's where the enemy's workin' on their new weapons. It would make things a mite easier if we could get some of those new weapons for our side, too. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmph! I don't like it. Using enemy weapons... "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Well, we know the location. Least we can do is stop in and take a look-see. "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
    this.turnStart = function(turn, player)
    {
        //
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's keep moving! Everyone, advance! There's a city up ahead. We can rest there. "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander, you're certainly in high spirits today. "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yeah, is it snowing up ahead or something?"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Pardon me, Commander... May I ask what lies ahead? "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hah! It's about time someone asked me that! To be honest, we'll soon arrive in the town where I was born!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What's that? Your hometown? "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's right. It's been a long time since I left to join the army. I imagine everyone will be surprised to see how far I've risen in the world. Ho ho, yes! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So you're really just excited about showing 'em how important you are. Watch out, y'all! Olaf's comin' home! "),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The city's coming into view, sir. It's... It's... "),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What!? What's happened here? "),
                    "co_olaf", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Welcome to my playground! Since you've come so far, would you like to play? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You... You fiend! What have you done? "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? Is there something wrong? You don't like my playground? "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("My home! The town square! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are you talking about? I don't see any of that stuff! Oh... Those must be the places that got broken during my field tests. Anyway, I've hidden a goody in one of these properties. Think you can find it? Tee hee hee! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit... Colin... Stay out of this! This fight is mine!!! "),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
        dialog7.queueAnimation(dialog8);
        dialog8.queueAnimation(dialog9);
        dialog9.queueAnimation(dialog10);
        dialog10.queueAnimation(dialog11);
        dialog11.queueAnimation(dialog12);
        dialog12.queueAnimation(dialog13);
        dialog13.queueAnimation(dialog14);
        dialog14.queueAnimation(dialog15);
        dialog15.queueAnimation(dialog16);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

