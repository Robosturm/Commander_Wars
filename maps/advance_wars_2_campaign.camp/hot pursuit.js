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
        return qsTr("Destroy all three Black Cannons.");
    };

    this.victory = function(team)
    {
        if (team === 0)
        {
            // called when a player wins
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Lord Sturm! The Black Cannons have been--"),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Did you leak intel on the missile platform to the enemy as I ordered?"),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir. We let one soldier be captured. He will tell all in his interrogation."),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Good. It's time to end this game. Order all units to the platform."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes, sir!"),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("They will follow the trail all the way to the missile platform. My best troops will greet them there with a storm of death. Then, before their very eyes, I will launch our missiles."),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));


            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("The enemy's constructing a missile platform?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes. They're planning to launch missiles at Cosmo Land and other continents."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Is this intel reliable?"),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("We heard it from a captured soldier. Intel we gathered on our own confirms his story."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("What are we waiting for? We have to attack that missile platform!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's not that simple, Andy."),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It's not? Why?"),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("'Cause it might be a trap."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You've confirmed the intel, correct?"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Would a soldier who's fought so long break so easily? I wonder..."),
                        "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I think it's best to assume that it's a snare."),
                        "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Well, even so, we can't sit on port forever. We need to end this quickly while troop morale is high."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Hmmm..."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));


            var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Aaaaah! I can't stand it! How long are we gonna talk about this?!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("If the enemy's at the missile platform, then let's blast it to bits!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Andy, your mouth's gettin' ahead of your brain again..."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

            var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("No. I think the boy's got the right idea."),
                        "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Huh?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I concur. We must attack sometime. If they're waiting for us at the platform, it saves us the time and the annoyance of looking for them."),
                        "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr(" I get what you're sayin'. If you're looking for crawdads, you gotta turn over some rocks. That's a pretty sound principle."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("I hear no objections. Then let us begin our advance on the missile platform."),
                        "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
            var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Waaaahooooo! Let's get 'em!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
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
            dialog27.queueAnimation(dialog28);
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
        // set army stuff
        campaignScript.setArmyData(0, campaignScript.getArmy(map.getPlayer(0).getCO(0)));
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        campaignScript.setArmyData(2, campaignScript.getArmy(map.getPlayer(2).getCO(0)));
        // set bh stuff
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(3).setBuildList(bhList);
        // disable attacking of the weld
        map.getPlayer(3).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };    

    this.actionDone = function(action)
    {
        var blackCannon1 = map.getTerrain(10, 1).getBuilding();
        var blackCannon2 = map.getTerrain(14, 1).getBuilding();
        var blackCannon3 = map.getTerrain(18, 1).getBuilding();
        if ((blackCannon1 === null || blackCannon1.getBuildingID() === "ZBLACK_BUILDING_DESTROYED") &&
            (blackCannon2 === null || blackCannon2.getBuildingID() === "ZBLACK_BUILDING_DESTROYED") &&
            (blackCannon3 === null || blackCannon3.getBuildingID() === "ZBLACK_BUILDING_DESTROYED"))
        {
            map.getPlayer(3).setIsDefeated(true);
        }
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
                    qsTr("Lord Sturm, the enemy army..."),
                    "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So they've arrived..."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'll smash 'em. Lord Sturm, let me have 'em."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The command is... mine."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I will ascertain how strong they've become with my own eyes. You wretches report to the missile platform and assist Lash."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Understood."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yes, sir."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Good riddance. Pathetic fools. They have served me poorly. When this is finished, I will make... adjustments."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Here we are at last. The Black Hole Army's Citadel."),
                    "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I don't care what they call it. All I know is, everything they pillaged from Orange Star is here."),
                    "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's been confirmed by our allies, too."),
                    "co_jess", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("So if we can capture this place, we can get all of our money back!"),
                    "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I'm not so sure of that."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What do you mean, Nell?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Just because it was all sent here doesn't mean it's still here."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Are you saying they might have moved it all to another location?"),
                    "co_eagle", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What makes you think that?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("We gotta assume they weren't gathering it just for kicks. They've got something planned, and more 'n likely, they've moved all our stuff to where they need it. That sound 'bout right, Nell?"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit's exactly right. Which means this battle is also about deciphering their ultimate plan."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmmm... Regardless, we must still attack. The longer we wait, the stronger their defenses become. Our target..."),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Our objective is to destroy the 3 Black Cannons."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hm? Just the cannons? Are you sure, Sonja?"),
                    "co_kanbei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("If we eliminate those, we should be able to rout our foe. If there's nothing here for them to protect, they'll run."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Ah. And if they do... That would tell us whether or not this was their true headquarters."),
                    "co_sensei", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I see..."),
                    "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's brilliant, Commander Sonja!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("As we can see, indirect fire will be useful in this battle. The other vital elements will be selecting the right first CO... and using decoys effectively against the Black Cannons."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("OK, let's go!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));

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
        dialog27.queueAnimation(dialog28);
        dialog28.queueAnimation(dialog29);
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();

