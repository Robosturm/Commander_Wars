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
            var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Nooooooooooooooo! So close, I was so close... I underestimated the strength of these worms!"),
                        "co_sturm", GameEnums.COMood_Sad, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I... I've been beaten! My dream of world conquest... I vow that I will return! When that time comes, I will challenge you worms yet again!"),
                        "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

            var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I do believe we've reached the end of a long, dusty road. Goodness gracious! If it ain't my old friend -- big, bearded Olaf."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Is that you, Grit? What have I done...? I never realized Sturm was..."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Don't get all long in the face now, Olaf. Everything turned out all right in the end. And Boss, you still got a lot of work to get done."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("You... you're right."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Blue Moon's treated me well, and I'm in her debt. Well, what else can I do? What do you say? Can I help you rebuild her?"),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Hm? Do you mean you would forgive me my misguided deeds? Grit, you... you... (sniff, sniff)"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Maybe it was worth Sturm foolin' him just for this."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Did you say something?"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Nope. Not a word. Let's say we get movin' on down the road."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Did we win?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yeah, this time we really won!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("And Sturm's army is gone for good, too. Ah! What happened to Eagle?"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Ahoy, there mateys! It looks like you've weathered the storm! Or should I say, 'Sturm!'"),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Captain Drake!"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog17 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Don'y you worry your pretty little head over Eagle. He'll be back. He always keeps his word. No matter what!"),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog18 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Yes... Yes, he does!"),
                        "co_sami", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog19 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("At any rate, you did great! But we've still got rough seas ahead of us. We've got to put right the country borders that Sturm muddled with. It's going to take some time for the fighting to die down."),
                        "co_drake", GameEnums.COMood_Normal, PLAYER.getDefaultColor(2));
            var dialog20 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("Do you mean we're not done yet?"),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog21 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("It's not as bad as all that. At the very least, it'll be fun seeing you all again!"),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));
            var dialog22 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("I think it'll be fun, too!"),
                        "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog23 = GameAnimationFactory.createGameAnimationDialog(map, 
                        qsTr("That's a good lad! Looking forward to testing your mettle again! Until next we meet!"),
                        "co_drake", GameEnums.COMood_Happy, PLAYER.getDefaultColor(2));

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
        map.getPlayer(2).setBuildList(list);
        map.getPlayer(3).setBuildList(list);

        // blue moon spawn
        map.spawnUnit(2, 18, "LIGHT_TANK", map.getPlayer(1), 0);
        map.spawnUnit(4, 18, "LIGHT_TANK", map.getPlayer(1), 0);
        var co = map.getPlayer(1).getCO(0);
        if (co !== null && co.getCoID() === "CO_MAX")
        {
            map.spawnUnit(1, 19, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(2, 19, "HEAVY_TANK", map.getPlayer(1), 0);
            map.spawnUnit(3, 19, "LIGHT_TANK", map.getPlayer(1), 0);
            map.spawnUnit(4, 19, "HEAVY_TANK", map.getPlayer(1), 0);
            map.spawnUnit(5, 19, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(2, 20, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(4, 20, "FLAK", map.getPlayer(1), 0);
        }
        else if (co !== null && co.getCoID() === "CO_GRIT")
        {
            map.spawnUnit(1, 19, "ARTILLERY", map.getPlayer(1), 0);
            map.spawnUnit(2, 19, "ROCKETTHROWER", map.getPlayer(1), 0);
            map.spawnUnit(3, 19, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(4, 19, "ROCKETTHROWER", map.getPlayer(1), 0);
            map.spawnUnit(5, 19, "ARTILLERY", map.getPlayer(1), 0);
            map.spawnUnit(1, 20, "INFANTRY", map.getPlayer(1), 0);
            map.spawnUnit(2, 20, "MISSILE", map.getPlayer(1), 0);
            map.spawnUnit(3, 20, "MISSILE", map.getPlayer(1), 0);
            map.spawnUnit(4, 20, "INFANTRY", map.getPlayer(1), 0);
        }
        else
        {
            map.spawnUnit(1, 19, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(2, 19, "HEAVY_TANK", map.getPlayer(1), 0);
            map.spawnUnit(4, 19, "HEAVY_TANK", map.getPlayer(1), 0);
            map.spawnUnit(5, 19, "FLAK", map.getPlayer(1), 0);
            map.spawnUnit(2, 20, "MISSILE", map.getPlayer(1), 0);
            map.spawnUnit(4, 20, "MISSILE", map.getPlayer(1), 0);
            map.spawnUnit(1, 21, "ROCKETTHROWER", map.getPlayer(1), 0);
            map.spawnUnit(5, 21, "ROCKETTHROWER", map.getPlayer(1), 0);
        }

        // green earth spawn
        co = map.getPlayer(2).getCO(0);
        if (co !== null && co.getCoID() === "CO_EAGLE")
        {
            map.spawnUnit(18, 19, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(17, 20, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(19, 20, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(17, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(18, 21, "BOMBER", map.getPlayer(2), 0);
            map.spawnUnit(19, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(17, 22, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(19, 22, "K_HELI", map.getPlayer(2), 0);
            map.spawnUnit(16, 23, "FIGHTER", map.getPlayer(2), 0);
            map.spawnUnit(20, 23, "FIGHTER", map.getPlayer(2), 0);
        }
        else if (co !== null && co.getCoID() === "CO_DRAKE")
        {
            map.spawnUnit(17, 19, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(18, 19, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(19, 19, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(17, 20, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(19, 20, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(16, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(20, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(16, 22, "MISSILE", map.getPlayer(2), 0);
            map.spawnUnit(20, 22, "MISSILE", map.getPlayer(2), 0);
        }
        else if (co !== null && co.getCoID() === "CO_KANBEI")
        {
            map.getPlayer(2).setColor(PLAYER.getDefaultColor(3));
            map.spawnUnit(16, 19, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(17, 19, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(19, 19, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(20, 19, "HEAVY_TANK", map.getPlayer(2), 0);
            map.spawnUnit(16, 20, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(17, 20, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(18, 20, "RECON", map.getPlayer(2), 0);
            map.spawnUnit(19, 20, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(20, 20, "FLAK", map.getPlayer(2), 0);
        }
        else
        {
            map.spawnUnit(17, 19, "MECH", map.getPlayer(2), 0);
            map.spawnUnit(18, 19, "MECH", map.getPlayer(2), 0);
            map.spawnUnit(19, 19, "MECH", map.getPlayer(2), 0);
            map.spawnUnit(17, 20, "LIGHT_TANK", map.getPlayer(2), 0);
            map.spawnUnit(19, 20, "LIGHT_TANK", map.getPlayer(2), 0);
            map.spawnUnit(16, 21, "MISSILE", map.getPlayer(2), 0);
            map.spawnUnit(17, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(19, 21, "FLAK", map.getPlayer(2), 0);
            map.spawnUnit(20, 21, "MISSILE", map.getPlayer(2), 0);
            map.spawnUnit(17, 23, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(18, 23, "INFANTRY", map.getPlayer(2), 0);
            map.spawnUnit(19, 23, "INFANTRY", map.getPlayer(2), 0);
        }

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
        else if (turn === 1 && player === 3)
        {
            gameScript.day1BHDialog(map);
        }
    };

    this.initDialog = function(map)
    {
        var playername = settings.getUsername();
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog0 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("So... you're the one behind all this chaos. Do you have a name, or should we just call you Disaster?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("My name is... Sturm."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Well Sturm, we've finally hunted you down. Time to pay the piper, as they say."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Hunted down? Me? You've done nothing but enter the door that I opened for you."),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What?"),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Did you think I didn't notice you, little girl? You and your pathetic strategy games! Is that all you're good for?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("What? But... that's... you couldn't... Are you saying you've known all along?"),
                    "co_sonja", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("You've made one stupid mistake after another... Did you imagine yourself a match for me? You conceited fool! It's time to put an end to this farce. Time to pay the piper, was it?"),
                    "co_sturm", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("... I'm sorry, Father... I've broken my promise..."),
                    "co_sonja", GameEnums.COMood_Sad, PLAYER.getDefaultColor(3));
        var dialog9 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Quick, Sonja's in danger!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Sonja!"),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Grit? I'm so sorry. Take everyone and run... The enemy... Sturm... He's much more powerful than I'd imagined... Everyone's going to be..."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("This 'Sturm' is the ine who did this to you, right?"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Andy, please... You've got to get away."),
                    "co_sonja", GameEnums.COMood_Normal, PLAYER.getDefaultColor(3));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(map, 
                    qsTr("Who's running away? That guy's goin' down!"),
                    "co_andy", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        dialog0.queueAnimation(dialog1);
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

    };


    this.day1BHDialog = function(map)
    {
        GameAnimationFactory.createGameAnimationDialog(map, 
                            qsTr("Those fools... Their troops are all amassed right there... Bwah ha ha... They've played right into my hands! They'll rue the day they dreamed of opposing me!"),
                            "co_sturm", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
    };
};

Constructor.prototype = BASEGAMESCRIPT;
var gameScript = new Constructor();
