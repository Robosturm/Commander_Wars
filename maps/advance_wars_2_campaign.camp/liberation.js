var Constructor = function()
{
    this.immediateStart = function()
    {
        // called to check if the game should start immediatly without changing rules or modifying co's
        // return true for an immediate start
        return false;
    };

    this.getVictoryInfo = function()
    {
        return qsTr("Destroy the Weld of the Black Hole Factory.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We did it! There'll be no more units coming from that factory! "),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Aarrrgh! Nooooooo! "),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Give it up, Flak! You lose! "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("My invasion of Orange Star may have failed, but I'm not alone. There are other Black Hole COs, with more armies"),
                        "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What are you saying? Are Yellow Comet and Blue Moon under attack? "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hah! You'll find out soon enough. They'll be coming for you next! I'm gone. "),
                        "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hey! We're not done with you!... He got away again! We can't ignore his warning, can we? We've gotta contact the other armies. "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes. You're right. Let's ready reinforcements. In truth, though, our own Orange Star Army is in pretty ragged shape. If that battle had gone on much longer, materials and morale might have dropped to dangerous levels. I can't tell you how grateful I am for all of your aid! Nice work! "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's all due to your instruction, Nell. "),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No, it was all of you. You made the decisions and performed the deeds. "),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("C'mon! Let's hurry and check on the other countries! "),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yeah! Let's get busy and drive those Black Hole thugs off the planet! "),
                        "co_sami", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You said it! We'll knock the stuffing out of 'em, then head home for a victory feast! "),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // no co power
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var osList = campaignScript.getOSBuildList();
        map.getPlayer(0).setBuildList(osList);
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(1).setBuildList(bhList);
        // disable attacking of the weld
        map.getPlayer(1).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        var variables = map.getGameScript().getVariables();
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var weld = map.getTerrain(12, 1);
        if (weld.getTerrainID() !== "WELD")
        {
            // set victory to true
            map.getPlayer(1).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        // disable black factory
        var blackFactory = map.getTerrain(4, 2).getBuilding();
        blackFactory.setFireCount(0);

        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }
        else if (turn === 1 && player === 1)
        {
            gameScript.initBHDialog();
        }
        else if (turn === 2 && player === 0)
        {
            gameScript.day2Dialog();
        }
        else if (turn === 3 && player === 0)
        {
            gameScript.day3Dialog();
        }
        else if (turn === 3 && player === 1)
        {
            gameScript.day3BHDialog();
        }
        else if (turn === 5 && player === 1)
        {
            map.spawnUnit(5, 6, "ARTILLERY", map.getPlayer(1), 0);
        }
        else if (turn === 6 && player === 1)
        {
            map.spawnUnit(5, 6, "MISSILE", map.getPlayer(1), 0);
            map.spawnUnit(6, 6, "MECH", map.getPlayer(1), 0);
        }
        else if (turn === 7 && player === 1)
        {
            map.spawnUnit(5, 6, "FLAK", map.getPlayer(1), 0);
        }
        else if (turn === 8 && player === 1)
        {
            map.spawnUnit(5, 6, "LIGHT_TANK", map.getPlayer(1), 0);
        }
        else if (turn === 9 && player === 1)
        {
            map.spawnUnit(5, 6, "ROCKETTHROWER", map.getPlayer(1), 0);
        }
        else if (player === 1 && turn > 9)
        {
            gameScript.spawnFactory();
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("They've come at last... I'll win it all with this factory. "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You look like you're having a rough time, Flak. "),
                    "co_random", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? What rock did you crawl out from under, Adder?"),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Heh heh heh... I wanted to see the military strength of this world myself. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Should you be here? I thought you were supposed to be invading Yellow Comet? "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Leave off, Flak. You'd do well not to waste your energy on my affairs. Look! The natives are knocking on your doorstep... and they look restless. "),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grrr... I won't lose. "),
                    "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("'Won't'? Heh heh heh... You mustn't lose, Flak. That's closer to the truth."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("........."),
                    "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm looking forward to your performance. Heh heh heh..."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Whoa! What in the world is that?!? "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It seems to be some sort of an installation. What are they planning now? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I believe they call it a factory. This is the center of all the damage inflicted upon our forces. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So there's no doubt that it's the key to the entire Black Hole invasion! "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, but... There's still so much we don't know. I've got a spy on the inside, so I'll let you know if I hear anything. In the meanwhile, let's launch some sorties and see the enemy's response. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, ma'am! "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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

    this.initBHDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That unit's ready, right? Open the factory gate! "),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        map.spawnUnit(5, 6, "MECH", map.getPlayer(1), 0);
    };

    this.spawnFactory = function()
    {
        var units = ["LIGHT_TANK", "FLAK", "ARTILLERY", "MECH"];
        var index = globals.randInt(0, 3);
        var pos = globals.randInt(0, 2);
        map.spawnUnit(4 + pos, 6, units[index], map.getPlayer(1), 0);
    };

    this.day2Dialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's no base, and yet an enemy unit just came out of it. What's going on? "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So that's it... I understand! "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What is it, Nell? Is everything OK? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No, it isn't, Max. Our army's not the only thing being attacked here. The enemy's been stealing weapons, money, and other materials, too! Somehow, they transport everything here to this factory, then it all goes into that pipe and winds up... somewhere else. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Black Hole goons... They're even more dangerous than we originally thought. "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't worry! As long as we're in this together, there's no way we can lose! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("You're right, Andy. You've all made it this far. I know you'll emerge victorious. "),
                    "co_nell", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
    };

    this.day3Dialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I've got new intel, troops. I don't know what that factory's made of, but I do know that we can't damage it with the weapons we have here. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you serious? Well then, how are we gonna put it out of business? "),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's the problem at hand. That long pipe is also made of the same impervious substance. But take a look over there. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It looks like two separate pieces of pipe are connected there..."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Right. The two pieces appear to be bolted together. "),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Maybe Md tanks or bombers could blast that seam apart... "),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, and maybe by breaking the pipe, we can halt their troop production. First, we should gain control of that airport in the center of the map. That should give us an advantage. Next, we need to make good use of your CO Powers... Just so you know, your Power Meter fills faster when you're attacked. It does build up for the attacker, too, so firing first is never a mistake! "),
                    "co_nell", GameEnums.COMood_Sad, PLAYER.getDefaultColor(0));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK! Now that that's clear, I'm using my CO Power as often as I can! "),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        dialog3.queueAnimation(dialog4);
        dialog4.queueAnimation(dialog5);
        dialog5.queueAnimation(dialog6);
        dialog6.queueAnimation(dialog7);
        dialog7.queueAnimation(dialog8);
    };

    this.day3BHDialog = function()
    {
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Lord Flak, it appears that the foe has discovered the pipe's weak point. "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grrr... That section may be weak, but it's still not easy to destroy. Even so, if they manage to damage the pipe and halt troop production, we're in deep trouble. Hit 'em now, before they're ready! Smash 'em! "),
                    "co_flak", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir! "),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        dialog1.queueAnimation(dialog2);
        dialog2.queueAnimation(dialog3);
        map.spawnUnit(5, 6, "RECON", map.getPlayer(1), 0);
    };
}

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

