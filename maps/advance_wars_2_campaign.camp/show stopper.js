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
        return qsTr("Destroy the Mini Cannons surrounding the Fortress.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What? It can't be! Our beautiful fortress... We were unlucky, nothing more. And so, for now, we must retreat. "),
                        "co_adder", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Whew... We won. Somehow, we won! "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Sonja! "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Father, did you observe the battle? "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I did. Those were dangerous tactics you employed, Daughter. However, I was mistaken. You were the perfect CO to fight this fight. "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Father... "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("However, this war is going to be more dangerous, and more violent. You must never let your guard down. Do you understand, Sonja? "),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir! I do! "),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            dialog1.queueAnimation(dialog2);
            dialog2.queueAnimation(dialog3);
            dialog3.queueAnimation(dialog4);
            dialog4.queueAnimation(dialog5);
            dialog5.queueAnimation(dialog6);
            dialog6.queueAnimation(dialog7);
            dialog7.queueAnimation(dialog8);
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_OfWar); // no fog of war or GameEnums.Fog_OfWar -> for on
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
        var variables = map.getGameScript().getVariables();
        var mapTown = map.getTerrain(6, 8).getBuilding();
        if (mapTown.getOwner() !== null &&
            mapTown.getOwner().getPlayerID() === 0)
        {
            var mapTownDialog = variables.createVariable("mapTownDialog");
            if (mapTownDialog.readDataBool() === false)
            {
                gameScript.mapTownDialog();
                // enable yc map lab
                var campaignVariables = map.getCampaign().getVariables();
                var ycLabFound = campaignVariables.createVariable("ycLabFound");
                ycLabFound.writeDataBool(true);
                mapTownDialog.writeDataBool(true);
            }
        }
        var miniCannon1 = map.getTerrain(9, 4).getBuilding();
        var miniCannon2 = map.getTerrain(9, 7).getBuilding();
        var miniCannon3 = map.getTerrain(10, 3).getBuilding();
        var miniCannon4 = map.getTerrain(10, 8).getBuilding();
        var miniCannon5 = map.getTerrain(13, 3).getBuilding();
        var miniCannon6 = map.getTerrain(13, 8).getBuilding();
        var miniCannon7 = map.getTerrain(14, 4).getBuilding();
        var miniCannon8 = map.getTerrain(14, 7).getBuilding();
        if (miniCannon1 === null &&
            miniCannon2 === null &&
            miniCannon3 === null &&
            miniCannon4 === null &&
            miniCannon5 === null &&
            miniCannon6 === null &&
            miniCannon7 === null &&
            miniCannon8 === null)
        {
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.mapTownDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander! This was found when we captured a new base. "),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ah! It's a map! A map showing the location of their lab! This must be where they're developing those new weapons we've heard about. Please deliver this to my father! He will send reinforcements for sure. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! I'll deliver it to CO Kanbei right away! "),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Now, I just have to do something with this mess. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
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
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you sure the enemy is here? "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes. Somewhere. My reports say that... Eeeek! "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja! Are you all right? "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ouch... "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Blood! Sonja, you're bleeding! Quickly! Prepare for a transfusion! "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sir? "),
                    "co_officier_yc", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Fa-Father! Stop! I just fell down. Please stop overreacting. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But I'm worried. If anything ever happened to you, I don't know what I would do. I knew it! You should be in the rear. It's much safer there. "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ooh! I am a Yellow Comet CO! I can handle this responsibility! I'll prove it to you! "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But, Sonja... I... "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Wait! Father, look at that! "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? That... That's... "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("A huge enemy stronghold? When did that get there? "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How dare they build that monstrosity on our sacred soil! It is time to clean the rust from my blade! "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Patience, Father! First, we need to reconnoiter. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));

        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("But... "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Intel is the most important part of combat. Rushing blindly to war just increases your casualties. Leave this to me. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Never! It is too dangerous. "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I think you should let her go. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sensei? I don't... "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Sonja is a respected CO in the Yellow Comet Army. Believe in your daughter, Kanbei. It's the right thing to do. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... Sonja, are you truly ready for this? "),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("One hundred percent! I'm your daughter, aren't I?! I'll prove I'm worthy to command. "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("How to capture that giant fortress... Looks like I need to smash the 8 cannons protecting it. If I can do that, the fortress will be powerless, and I'll win! It's either that or capture their HQ... "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh... It seems that this terrain has our friends in a quandary. Not that it matters. Whatever they decide to do, they can only fail. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("CO Adder of the Black Hole Army, I presume. I should have known Black Hole was behind this! "),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This fortress was built on the strength of the Black Hole nation. It will never fall to the likes of Yellow Comet. Once I've taken care of things here, I must destroy this map. It wouldn't do to have these fools showing up at our hidden lab... Now then, let's see what skills this foe possesses. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

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
        dialog21.queueAnimation(dialog22);
        dialog22.queueAnimation(dialog23);
        dialog23.queueAnimation(dialog24);
        dialog24.queueAnimation(dialog25);
        dialog25.queueAnimation(dialog26);
        dialog26.queueAnimation(dialog27);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

