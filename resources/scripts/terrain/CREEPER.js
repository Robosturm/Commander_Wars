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
    this.loadBaseTerrain = function(terrain, currentTerrainID, map)
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
    this.loadBaseSprite = function(terrain, map)
    {
        var surroundings = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("creeper" + surroundings);
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        // Check every side.
        var surroundings = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_Direct, false);
        // Load overlay south east, strict.
        if (surroundings.includes("+S") && surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("creeper+SE");
            }
        }
        // Load overlay north east, strict.
        if (surroundings.includes("+N") && surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("creeper+NE");
            }
        }
        // Load overlay south west, strict.
        if (surroundings.includes("+S") && surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("creeper+SW");
            }
        }
        // Load overlay northwest, strict.
        if (surroundings.includes("+N") && surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("CREEPER", false, false, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("creeper+NW");
            }
        }
    };
    this.startOfTurn = function(terrain, map)
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
        return ["creeper.png",
                "creeper+E",
                "creeper+E+W",
                "creeper+W"];
    };

};
Constructor.prototype = TERRAIN;
var CREEPER = new Constructor();
