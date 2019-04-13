var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(4);
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/sensei.mp3")
                break;
        }
    };

    this.activatePower = function(co)
    {
        CO_SENSEI.spawnUnits(co, "INFANTRY", 9);
        audio.clearPlayList();
        CO_SENSEI.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        CO_SENSEI.spawnUnits(co, "MECH", 9);
        audio.clearPlayList();
        CO_SENSEI.loadCOMusic(co);
        audio.playRandom();
    };

    this.spawnUnits = function(co, unitID, hp)
    {
        var buildings = co.getPlayer().getBuildings();
        var animations = [];
        var counter = 0;
        buildings.randomize();
        for (var i = 0; i < buildings.size(); i++)
        {
            var building = buildings.at(i);
            if (building.getBuildingID() === "TOWN")
            {
                if (map.getTerrain(building.getX(), building.getY()).getUnit() === null)
                {
                    map.spawnUnit(building.getX(), building.getY(), unitID, co.getPlayer());
                    map.getTerrain(building.getX(), building.getY()).getUnit().setHp(hp);
                    var animation = GameAnimationFactory.createAnimation(building.getX(), building.getY());
                    if (animations.length < 5)
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                        animations.push(animation);
                    }
                    else
                    {
                        animation.addSprite("power8", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                        animations[counter].queueAnimation(animation);
                        animations[counter] = animation;
                        counter++;
                        if (counter >= animations.length)
                        {
                            counter = 0;
                        }
                    }
                }
            }
        }
        buildings.remove();
    };

    this.getCOUnitRange = function(co)
    {
        return 2;
    };
    this.getCOArmy = function()
    {
        return "YC";
    };

    this.getInfantryIDS = function()
    {
        return  ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
    }
    this.getSeaUnitIDS = function()
    {
        return ["AIRCRAFTCARRIER", "CRUISER", "BATTLESHIP", "CANNONBOAT", "DESTROYER", "SUBMARINE"];
    };

    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        var seaUnitIDs = CO_SENSEI.getSeaUnitIDS();
        var unitInfantryIDs = CO_SENSEI.getInfantryIDS();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                break;
            case GameEnums.PowerMode_Power:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    return 30;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 80;
                }
                break;
            default:
                if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 30;
                    }
                    return 20;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                    {
                        return 50;
                    }
                    return 30;
                }
                break;
        }
        if (seaUnitIDs.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
        return 0;
    };

    this.getMovementPointModifier = function(co, unit)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
        return 0;
    };
}

Constructor.prototype = CO;
var CO_SENSEI = new Constructor();
