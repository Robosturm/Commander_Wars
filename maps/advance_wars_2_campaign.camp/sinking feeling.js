var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return true;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Destroy all 9 Battleships in the harbour.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander, the enemy battleships have all been sunk. "),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well done. Prepare to withdraw. "),
                        "co_jess", GameEnums.COMood_Sad, PLAYER.getDefaultColor(2));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Withdraw? "),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We've achieved our goal. Let's vanish before they send out more troops. "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, ma'am. We'll get started right away. "),
                        "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Stick and move... That's just like you, Jess. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No sense getting caught up in the moment and getting your troops hurt. We seriously reduced the enemy's firepower... That's enough for now. "),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("When you're right, you're right. I'll help get your troops out of there. "),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));

            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Aaaaah!!! What are you doing?! We can't fix the battleships if they're all sunk, can we?! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What... What should we do? "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What else is there to do? We attack with all of our strength! They've made me mad, and now they're gonna pay big time! "),
                        "co_lash", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Mistress Lash, that's... "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hm? "),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The enemy's gone. "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What? "),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("There's no sign of enemy units. It seems they've done what they set out to do and left. "),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You've got to be joking! They just sank my boats and took off? I... am... so... angry!!! "),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
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
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        map.getGameRules().addVictoryRule("VICTORYRULE_TURNLIMIT");
        var turnLimit = map.getGameRules().getVictoryRule("VICTORYRULE_TURNLIMIT"); // loose if the turn limit is gone
        turnLimit.setRuleValue(17, 0);

        var geList = campaignScript.getGEBuildList();
        map.getPlayer(0).setBuildList(geList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
    };
    this.actionDone = function(action)
    {
        var unit1 = map.getTerrain(10, 9).getUnit();
        var unit2 = map.getTerrain(11, 9).getUnit();
        var unit3 = map.getTerrain(12, 9).getUnit();
        var unit4 = map.getTerrain(10, 10).getUnit();
        var unit5 = map.getTerrain(11, 10).getUnit();
        var unit6 = map.getTerrain(12, 10).getUnit();
        var unit7 = map.getTerrain(10, 11).getUnit();
        var unit8 = map.getTerrain(11, 11).getUnit();
        var unit9 = map.getTerrain(12, 11).getUnit();
        if (unit1 === null &&
            unit2 === null &&
            unit3 === null &&
            unit4 === null &&
            unit5 === null &&
            unit6 === null &&
            unit7 === null &&
            unit8 === null &&
            unit9 === null)
        {
            map.getPlayer(1).setIsDefeated(true);
        }

        var variables = map.getGameScript().getVariables();
        var mapTown = map.getTerrain(0, 9).getBuilding();
        if (mapTown.getOwner() !== null &&
            mapTown.getOwner().getPlayerID() === 0)
        {
            var mapTownDialog = variables.createVariable("mapTownDialog");
            if (mapTownDialog.readDataBool() === false)
            {
                gameScript.mapTownDialog();
                // enable os map lab
                var campaignVariables = map.getCampaign().getVariables();
                var geLabFound = campaignVariables.createVariable("geLabFound");
                geLabFound.writeDataBool(true);
                mapTownDialog.writeDataBool(true);
            }
        }
    };
    this.mapTownDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! We found this in a property we captured... "),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? A lab... They've been developing weapons within our borders. Since they went to the trouble, let's take the weapons along with this map. We need to let Eagle know about this. He definitely won't stand for it. "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am. Understood. "),
                    "co_officier_ge", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("These Black Hole goons really are trouble. "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
    };

    this.turnStart = function(turn, player)
    {
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.day1Dialog();
        }
        else if (turn === 16 && player === 1)
        {
            gameScript.day16Dialog();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Mistress Lash, the battleships in need of repair have all been docked. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Let's get started then. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Before that, we'd better reply to Commander Hawke's question. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Oh, what a pain! What did he want again? "),
                    "co_Lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The estimated time on the repairs. Can they be done in 30 days? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("17 days. "),
                    "co_Lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Excuse me? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Under my brilliant direction, the repairs will take 17 days. Not 30, 17. "),
                    "co_Lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("However, the workers... Can they handle that pace? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If we don't have enough manpower, we'll simply borrow some soldiers. The units sent to hide the lab map have returned, haven't they? Anyway, we'll have them done in 17 days! "),
                    "co_Lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes... um... OK. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));


        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We've received word the battleships have docked. Not long now. "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Jess, can you pull this off with this many troops? It's still not too late for Eagle or me to provide reinforcements. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The more troops, the longer it takes to get organized. Time is of the essence here. We must attack before the repairs are done! "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's true, but... If the enemy captures that airport, they'll deploy B copters and bombers. At any rate, you're in for rough seas. "),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Take a look, Drake. For air units, I've got my A-airs. I'm going to bathe those battleships in artillery fire! Watch and see! "),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
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

    this.day1Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? We're under attack?! That's not fair! We're in the middle of repairs! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are your orders, Mistress Lash? "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What are my orders? Counterattack, you twerp! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So, we'll need to push back the deadline on repairs... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));


        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No way! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But... But... the deadline... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("STOP SAYING THAT! Just do as you're told. NOW! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };
    this.day16Dialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Mistress Lash. We have one day remaining to finish repairs. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I know, I know. I'm busy right now. Come back later. "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Perhaps we should rethink the deadline after all... "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're such a pest! We're almost finished! If you have enough time to keep jabbering at me, then help! "),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yaaah! Let me go! Please! "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

