var Constructor = function()
{
    this.coVictory = [  ["CO_ADAM",                 5,    qsTr("I'm good at math!")],
                        ["CO_ADDER",                5,    qsTr("Sidekick!")],
                        ["CO_AIRA",                 5,    qsTr("Windy!")],
                        ["CO_ALEXANDER",            5,    qsTr("Blue Politics!")],
                        ["CO_ALEXIS",               5,    qsTr("I love crystals!")],
                        ["CO_AMY",                  5,    qsTr("Quantum + EMP!")],
                        ["CO_ANDY"   ,              5,    qsTr("Party time!")],
                        ["CO_BEAST",                5,    qsTr("Robberies!")],
                        ["CO_BRENNER",              5,    qsTr("Helped a lot!")],
                        ["CO_CASSIDY",              5,    qsTr("Roses are red!")],
                        ["CO_CAULDER",              5,    qsTr("Mad doctor!")],
                        ["CO_COLIN",                5,    qsTr("Tons of Soldiers!")],
                        ["CO_CONRAD",               5,    qsTr("Vision Battle!")],
                        ["CO_DRAKE",                5,    qsTr("I'm a pirat!")],
                        ["CO_EAGLE",                5,    qsTr("Goggles in the air!")],
                        ["CO_EPOCH",                5,    qsTr("Tested it!")],
                        ["CO_FLAK",                 5,    qsTr("Blast through!")],
                        ["CO_FORSYTHE",             5,    qsTr("Noble war!")],
                        ["CO_GAGE",                 5,    qsTr("Snipped them!")],
                        ["CO_GRAVES",               5,    qsTr("Novel reader!")],
                        ["CO_GREYFIELD",            5,    qsTr("I'm an admiral!")],
                        ["CO_GRIMM",                5,    qsTr("Lightning grimm!")],
                        ["CO_GRIT",                 5,    qsTr("Wild western!")],
                        ["CO_HACHI",                5,    qsTr("Sold out!")],
                        ["CO_HAWKE",                5,    qsTr("I have my own plans!")],
                        ["CO_IK_486_B7",            5,    qsTr("Direct artillery!")],
                        ["CO_ISABELLA",             5,    qsTr("For Will!")],
                        ["CO_JAKE",                 5,    qsTr("Plains fighter!")],
                        ["CO_JAVIER",               5,    qsTr("Noble ritter!")],
                        ["CO_JESS",                 5,    qsTr("Green Land!")],
                        ["CO_JOEY",                 5,    qsTr("Fair play!")],
                        ["CO_JUGGER",               5,    qsTr("Booting...")],
                        ["CO_JULIA",                5,    qsTr("Propaganda mistress!")],
                        ["CO_KANBEI",               5,    qsTr("Samurai!")],
                        ["CO_KINDLE",               5,    qsTr("Urban fighter!")],
                        ["CO_KOAL",                 5,    qsTr("Not Adder!")],
                        ["CO_LASH",                 5,    qsTr("I'm a crazy genius!")],
                        ["CO_LIN",                  5,    qsTr("Teaser!")],
                        ["CO_MARY",                 5,    qsTr("Bloody Mary!")],
                        ["CO_MAX",                  5,    qsTr("MAAAAAAAAAAAX!")],
                        ["CO_MEIYO",                5,    qsTr("Veteran fighter!")],
                        ["CO_MELANTHE",             5,    qsTr("Nature fighter!")],
                        ["CO_MINA",                 5,    qsTr("Deceptively dangerous!")],
                        ["CO_MINAMOTO",             5,    qsTr("Rice Cakes!")],
                        ["CO_NANA",                 5,    qsTr("Splash damages!")],
                        ["CO_NAPOLEON",             5,    qsTr("French war!")],
                        ["CO_NELL",                 5,    qsTr("Lucky star!")],
                        ["CO_OLAF",                 5,    qsTr("Winter is coming!")],
                        ["CO_OZZY",                 5,    qsTr("Grooving!")],
                        ["CO_PENNY",                5,    qsTr("Mr. Bear!")],
                        ["CO_PETER",                5,    qsTr("General Peter!")],
                        ["CO_RACHEL",               5,    qsTr("Just a sister?")],
                        ["CO_RATTIGAN",             5,    qsTr("Out numbered?")],
                        ["CO_ROBOANDY",             5,    qsTr("Puppet master!")],
                        ["CO_ROBOSTURM",            5,    qsTr("I'm the programmer now!")],
                        ["CO_SABAKI",               5,    qsTr("Field repairs!")],
                        ["CO_SAMI",                 5,    qsTr("Infantry specalist!")],
                        ["CO_SANJURO",              5,    qsTr("Noble trader!")],
                        ["CO_SASHA",                5,    qsTr("Dominatrix!")],
                        ["CO_MINAMOTO",             5,    qsTr("Rice Cakes!")],
                        ["CO_SENSEI",               5,    qsTr("Cranky Paratrooper!")],
                        ["CO_SMITAN",               5,    qsTr("Yelled dominant!")],
                        ["CO_SONJA",                5,    qsTr("Sees all, knows all!")],
                        ["CO_SOPHIE",               5,    qsTr("Mail call!")],
                        ["CO_STURM",                5,    qsTr("Evil invader!")],
                        ["CO_TABITHA",              5,    qsTr("Extremly cruel!")],
                        ["CO_TASHA",                5,    qsTr("Revenge for John!")],
                        ["CO_VARLOT",               5,    qsTr("Caviar and war!")],
                        ["CO_VON_BOLT",             5,    qsTr("Immortal Evil!")],
                        ["CO_WALTER",               5,    qsTr("Long standing guardian!")],
                        ["CO_WAYLON",               5,    qsTr("Flying over the skies!")],
                        ["CO_WILL",                 5,    qsTr("Joung and winning!")],
                        ["CO_XAVIER",               5,    qsTr("Assasins!")],
                        ["CO_YUKIO",                5,    qsTr("The ultimate evil!")],
                        ["CO_ZANDRA",               5,    qsTr("Sandy times!")],];

    this.registerAchievements = function()
    {
        for (var i = 0; i < ACHIEVEMENT_VICTORY.coVictory.length; ++i)
        {
            var data = ACHIEVEMENT_VICTORY.coVictory[i];
            var description = qsTr("Win the given amount of games with %0.")
            description = replaceTextArgs(description, [Global[data[0]].getName()]);
            userdata.addAchievement("VICTORY_CO_" + data[0],
                                    data[1],
                                    data[2],
                                    description,
                                    data[0] + "+face",
                                    false);
        }

        userdata.addAchievement("GAME_VICTORY", 500, qsTr("The best!"),       qsTr("Win the given amount of games."),     "s_rang",       false);
        userdata.addAchievement("GAME_LOST",    100, qsTr("Biggest Looser"),  qsTr("Loose the given amount of games."),   "surrender",    false);
    };
    this.onVictory = function(team, humanWin)
    {
        if (humanWin && team >= 0)
        {
            var count = map.getPlayerCount();
            for (var i = 0; i < count; ++i)
            {
                var player = map.getPlayer(i);
                if (player.getBaseGameInput().getAiType() === GameEnums.AiTypes_Human)
                {
                    var co = player.getCO(0);
                    if (co !== null)
                    {
                        userdata.increaseAchievement("VICTORY_CO_" + co.getCoID(), 1);
                    }
                    co = player.getCO(1);
                    if (co !== null)
                    {
                        userdata.increaseAchievement("VICTORY_CO_" + co.getCoID(), 1);
                    }
                }
            }
            userdata.increaseAchievement("GAME_VICTORY", 1);
        }
        else if (team >= 0 && !humanWin)
        {
            userdata.increaseAchievement("GAME_LOST", 1);
        }
    };
};

Constructor.prototype = ACHIEVEMENT;
var ACHIEVEMENT_VICTORY = new Constructor();
