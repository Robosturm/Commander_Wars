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
                        qsTr("Gimme a break! We lost again? Retreat, retreat, retreeeeeat!!!"),
                        "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Mistress Lash! Wait for us!"),
                        "co_officier_bh", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
            var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Yes! A victory for our army! A victory for Blue Moon."),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Commander Olaf... Wow, he even cries big..."),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("More than anyone else I know, the boss lives for his country. Those are tears of pure joy."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Really? Commander Olaf is a truly great leader! Of course, so are you, Commander Grit."),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Cut that out, Colin. I ain't used to anyone singing my praises."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));

            var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Colin's right!! In fact, you're both great commanders! You're the mold from which great COs are made! You're the children of Blue Moon! You're MY children!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));

            var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("YOUR children? You tryin' to scare old Colin half to death? "),
                        "co_grit", GameEnums.COMood_Sad, PLAYER.getDefaultColor(1));
            var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Howdy, Sami. Listen, sweetheart... I want to thank y'all for being so neighborly and helpin' us out. This war's gonna drag on a bit, so we'll return the favor someday soon. So long."),
                        "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Uh, Commander Grit! Wait for me!"),
                        "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Thanking others for their goodwill is nothing to be shy about! The ability to express gratitude is as beautiful as the aurora itself! Grit! Colin!"),
                        "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
            var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("It looks like peace has returned to Blue Moon."),
                        "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Their COs get along well, don't they? They look like a happy family."),
                        "co_andy", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
            var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("You think so? They looked a bit dysfunctional to me."),
                        "co_max", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
            var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                        qsTr("Blue Moon's safe and sound. Let's head for home."),
                        "co_sami", GameEnums.COMood_Happy, PLAYER.getDefaultColor(0));
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
        }
    };
    this.gameStart = function()
    {
        // called before a game starts
        //we're going to set the game rules here.
        map.getGameRules().setNoPower(false); // co power on
        map.getGameRules().setRandomWeather(false); // no random weather
        map.getGameRules().setFogMode(GameEnums.Fog_Off); // no fog of war or GameEnums.Fog_OfWar -> for on
        // map.getGameRules().changeWeatherChance("WEATHER_1SUN", 90); // sets the weather chance of sun to 90. The actual chance is the the value divided through the sum of all chances
        // here we decide how you can win the game
        map.getGameRules().addVictoryRule("VICTORYRULE_NOHQ"); // win by capturing all hq's of a player
        var bmList = campaignScript.getBMBuildList();
        map.getPlayer(0).setBuildList(bmList);
        campaignScript.setArmyData(1, campaignScript.getArmy(map.getPlayer(1).getCO(0)));
        var bhList = campaignScript.getBHBuildList();
        map.getPlayer(2).setBuildList(bhList);
        map.getPlayer(2).getBaseGameInput().setEnableNeutralTerrainAttack(false);
    };
    this.actionDone = function(action)
    {
        // check if the buildings changed there owner to a certain player and call a dialog on it
        var weld = map.getTerrain(8, 1);
        if (weld.getTerrainID() !== "WELD")
        {
            // set victory to true
            map.getPlayer(2).setIsDefeated(true);
        }
    };

    this.turnStart = function(turn, player)
    {
        var blackFactory = map.getTerrain(8, 3).getBuilding();
        blackFactory.setFireCount(0);
        if (turn === 1 && player === 0)
        {
            gameScript.initDialog();
        }

        // factory spawning comes here
        if (turn === 1 && player === 2)
        {
            map.spawnUnit(8, 6, "MECH", map.getPlayer(2), 0);
        }
        else if (turn === 2 && player === 2)
        {
            map.spawnUnit(7, 6, "RECON", map.getPlayer(2), 0);
            map.spawnUnit(8, 6, "RECON", map.getPlayer(2), 0);
            map.spawnUnit(9, 6, "RECON", map.getPlayer(2), 0);
        }
        else if (turn === 4 && player === 2)
        {
            map.spawnUnit(8, 6, "LIGHT_TANK", map.getPlayer(2), 0);
            map.spawnUnit(9, 6, "ARTILLERY", map.getPlayer(2), 0);
        }
        else if (turn === 5 && player === 2)
        {
            map.spawnUnit(9, 6, "HEAVY_TANK", map.getPlayer(2), 0);
        }
        else if (turn === 7 && player === 2)
        {
            map.spawnUnit(9, 6, "INFANTRY", map.getPlayer(2), 0);
        }
        else if (turn === 8 && player === 2)
        {
            map.spawnUnit(8, 6, "NEOTANK", map.getPlayer(2), 0);
        }
        else if (turn === 11 && player === 2)
        {
            map.spawnUnit(8, 6, "FLAK", map.getPlayer(2), 0);
        }
        else if (turn > 11 && player === 2)
        {
            gameScript.spawnFactory();
        }

    };

    this.spawnFactory = function()
    {
        // first deal with the spawning count
        var amount = globals.randInt(0, 5);
        if (amount <= 4 && amount >= 2)
        {
            amount = 0;
        }
        else if (amount === 5)
        {
            amount = 1;
        }
        else
        {
            amount = -1;
        }
        // than spawn some random units :)
        for (var i = 0; i <= amount; i++)
        {
            var units = ["LIGHT_TANK", "FLAK", "ARTILLERY", "MECH", "HEAVY_TANK", "INFANTRY"];
            var index = globals.randInt(0, 5);
            var pos = globals.randInt(0, 2);
            map.spawnUnit(7 + pos, 6, units[index], map.getPlayer(2), 0);
        }
    };

    this.initDialog = function()
    {
        // moods are GameEnums.COMood_Normal, GameEnums.COMood_Happy, GameEnums.COMood_Sad
        var dialog1 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Uh-oh! When did they arrive here at the factory? Are we in trouble?"),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog2 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("ou're a reckless little thing, aren't you? How do you plan on explaining this to Hawke?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog3 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Flak! The factory's too close to those silos! What happens if it gets hit with a missile?"),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog4 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I hadn't thought of that..."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog5 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Tee hee hee! Those missiles won't even scratch my Lash Armor! And that pipe seam is totally wrapped in the stuff! I can't wait to start using my factory to make them miserable!"),
                    "co_lash", GameEnums.COMood_Happy, PLAYER.getDefaultColor(4));
        var dialog6 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Don't worry, Adder. Lash has a plan."),
                    "co_flak", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog7 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I can deploy any unit I like, right? Tee hee hee..."),
                    "co_lash", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));
        var dialog8 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" I wonder if those two can handle this."),
                    "co_adder", GameEnums.COMood_Normal, PLAYER.getDefaultColor(4));

        var dialog9 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! Are we ready? Grit!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog10 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Yessiree, Boss. Ready when you are."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog11 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Um... The Commander-in-chief of Orange Star is approaching."),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog12 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Orange Star? Hmm... Show her in."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog13 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("It's been a long time, Olaf."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog14 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Hmph! Nell... What are you doing here? I didn't request reinforcements."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog15 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("No, but I did."),
                    "co_grit", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog16 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! Why, you no-good... Are you familiar with the concept of 'chain of command'?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog17 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr(" Commander Olaf, Commander Grit did what he thought best for Blue Moon!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog18 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("I know that, boy. Now shush!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog19 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Grit! Are we ready? Grit!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog20 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("This is a vital strategic location. Orange Star offers its aid."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog21 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Seeing as this is our territory, I'm of a mind to send you packing... However, we are a bit shorthanded, so I'll accept your offer."),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog22 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("The Yellow Comet Army is also on its way here."),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog23 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? That self-styled samurai?"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog24 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("That's right! We'll defeat Black Hole together!"),
                    "co_nell", GameEnums.COMood_Normal, PLAYER.getDefaultColor(0));
        var dialog25 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Commander Olaf, the troops are ready when you are, sir!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog26 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Very well! Let's not fall behind. Advance on all fronts!"),
                    "co_olaf", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog27 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Brilliant work, Commander Grit... Seeing Blue Moon's danger and rallying our allies so quickly!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
        var dialog28 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("Huh? Oh, that... Anything to keep from getting shot up, son."),
                    "co_grit", GameEnums.COMood_Happy, PLAYER.getDefaultColor(1));
        var dialog29 = GameAnimationFactory.createGameAnimationDialog(
                    qsTr("What? That was your reasoning? Commander Grit! Wait up, sir!"),
                    "co_colin", GameEnums.COMood_Normal, PLAYER.getDefaultColor(1));
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

