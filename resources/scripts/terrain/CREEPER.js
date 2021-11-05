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
        var random = 0;
        var itemCount = surroundings.split("+").length - 1;
        if (surroundings === "")
        {
            random = globals.randInt(0, 1);
        }
        else if (itemCount === 2)
        {
            random = globals.randInt(0, 2);
        }
        else if (itemCount === 3)
        {
            random = globals.randInt(0, 5);
        }
        else if (itemCount === 4)
        {
            random = globals.randInt(0, 14);
        }
        else
        {
            surroundings = "";
        }
        random = 2;
        terrain.loadBaseSprite("creeper+" + random.toString() + surroundings);
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
        return qsTr("Well-surfaced roads provides optimum mobility but little cover.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    };

};
Constructor.prototype = TERRAIN;
var CREEPER = new Constructor();
