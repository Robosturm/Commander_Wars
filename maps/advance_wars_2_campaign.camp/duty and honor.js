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
                        qsTr("I-I don't believe it... They were... stronger than me? Must find a way... to escape... "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The enemy has been routed! "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I'm sorry, Father. I almost destroyed us all. "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What, your silos? Don't be ridiculous, Sonja. It is because of those silos that we won here today. "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Father... "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("There's no rest for the weary! Battle awaits, and we must ride forth to meet it! "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
        var ycList = campaignScript.getYCBuildList();
        map.getPlayer(0).setBuildList(ycList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var town1 = map.getTerrain(21, 0).getBuilding();
        var town2 = map.getTerrain(22, 0).getBuilding();
        var town3 = map.getTerrain(23, 0).getBuilding();
        var town4 = map.getTerrain(24, 0).getBuilding();
        var town5 = map.getTerrain(25, 0).getBuilding();
        var town6 = map.getTerrain(21, 2).getBuilding();
        var town7 = map.getTerrain(22, 2).getBuilding();
        var town8 = map.getTerrain(23, 2).getBuilding();
        var town9 = map.getTerrain(24, 2).getBuilding();
        var town10 = map.getTerrain(25, 2).getBuilding();
        if (town1 !== null && town1.getOwner().getPlayerID() === 1 &&
            town2 !== null && town2.getOwner().getPlayerID() === 1 &&
            town3 !== null && town3.getOwner().getPlayerID() === 1 &&
            town4 !== null && town4.getOwner().getPlayerID() === 1 &&
            town5 !== null && town5.getOwner().getPlayerID() === 1 &&
            town6 !== null && town6.getOwner().getPlayerID() === 1 &&
            town7 !== null && town7.getOwner().getPlayerID() === 1 &&
            town8 !== null && town8.getOwner().getPlayerID() === 1 &&
            town9 !== null && town9.getOwner().getPlayerID() === 1 &&
            town10 !== null && town10.getOwner().getPlayerID() === 1 )
        {
            map.getPlayer(0).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        var laser1 = map.getTerrain(6, 3).getBuilding();
        var laser2 = map.getTerrain(14, 6).getBuilding();
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
        else if (turn === 2 && player === 1)
        {
            GameAnimationFactory.createGameAnimationDialog(
                                qsTr("Father, the enemy is preparing to fire the laser! Look out!"),
                                "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        }

        if (turn % 2 === 1)
        {
            // reset lasers
            laser1.setFireCount(0);
            laser2.setFireCount(0);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander Adder! The laser cannon has been deployed, as you ordered."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Very well."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Adder, how you planning on using it up on that mountain?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Quite simple, really. I'm going to lure the enemy in and fire."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What makes you think they'll come all the way out here?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm going to lay siege to their cities. If they don't come, I'll tell the world they permitted their cities be taken. Either way, we can't lose."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hrmmm..."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("More importantly, did you deploy that infantry as I requested?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Put 'em just where you said. They're pretty far from the cities. That OK?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes. That's fine. March them along slowly. They're merely bait for the enemy. Wouldn't want them capturing those cities too quickly. Heh heh heh..."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father, hold your attack!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Why, Sonja? The Black Hole Army is about to lay siege to our cities."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's a trap. They're trying to lure us in."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But we can't just sit here!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Why not? That city isn't at all important, strategically speaking."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja! What are you? What are we?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are we? We're COs in the Yellow Comet Army."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Correct. And COs do not sit idly by and watch their people be destroyed! Even if it's a trap. Even if it's not strategically important."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We are COs. It is our job to protect the people of Yellow Comet. Am I wrong?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No, Father. I understand. Just be careful. I'm going to see if I can unravel this trap."),
                    "co_sonja", GameEnums.COMood_Happy, PLAYER.getDefaultColor(3));
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
        dialog16.queueAnimation(dialog17);
        dialog17.queueAnimation(dialog18);
        dialog18.queueAnimation(dialog19);
        dialog19.queueAnimation(dialog20);
        dialog20.queueAnimation(dialog21);
    };
    this.day2Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father, I know what the enemy has planned. They have a monstrous weapon called a laser cannon. If fires a deadly laser up, down, left, and right in a straight line."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I see... Yet, no matter the trap, I..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ooh! I'm not finished yet!!! It's a laser cannon, but it's only a prototype. It can't just be fired at any time. I think that's something we can use to our advantage."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That is helpful. You are splendid, my child!"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Father! I am NOT looking for flattery! I'm trying to warn you! If the 10 cities in the upper right are captured, we lose. Be careful!"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

