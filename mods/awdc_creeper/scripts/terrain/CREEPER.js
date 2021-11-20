var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(CREEPER.getName());
        terrain.setHasStartOfTurn(true);
    };
    this.getName = function()
    {
        return qsTr("Creeper");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_Direct, false, true);
        if (surroundings === "+N+E+S+W")
        {
            var random = globals.randInt(0, 4);
            terrain.loadBaseSprite("creeper+" + random.toString() + surroundings);
        }
        else
        {
            var random = globals.randInt(0, 1);
            terrain.loadBaseSprite("creeper+" + random.toString() + surroundings);
        }      
    };
    this.startOfTurn = function(terrain)
    {
        var unit = terrain.getUnit();
        if (unit !== null)
        {
            var damage = 1;
            var hp = unit.getHpRounded();
            if (hp <= damage)
            {
                // set hp to very very low
                unit.setHp(0.001);
            }
            else
            {
                unit.setHp(hp - damage);
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.getDescription = function()
    {
        return qsTr("A virus infested tile which deals 1HP of damage to each unit standing on it at the beginning of each day.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["creeper+0",
                "creeper+0+N",
                "creeper+0+E",
                "creeper+0+S",
                "creeper+0+W",
                "creeper+0+E+S+W",
                "creeper+0+N+E",
                "creeper+0+N+S",
                "creeper+0+N+E+S",
                "creeper+0+N+E+S+W",
                "creeper+0+N+E+W",
                "creeper+0+N+S+W",
                "creeper+0+N+W",
                "creeper+0+E+S",
                "creeper+0+E+W",
                "creeper+0+S+W",
                "creeper+1",
                "creeper+1+N",
                "creeper+1+E",
                "creeper+1+S",
                "creeper+1+W",
                "creeper+1+E+S+W",
                "creeper+1+N+E",
                "creeper+1+N+S",
                "creeper+1+N+E+S",
                "creeper+1+N+E+S+W",
                "creeper+1+N+E+W",
                "creeper+1+N+S+W",
                "creeper+1+N+W",
                "creeper+1+E+S",
                "creeper+1+E+W",
                "creeper+1+S+W",
                "creeper+2+N+E+S+W",
                "creeper+3+N+E+S+W",
                "creeper+4+N+E+S+W"];
    };

};
Constructor.prototype = TERRAIN;
var CREEPER = new Constructor();