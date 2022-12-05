var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("gate+E+W", false);
        building.loadSpriteV2("gate+E+W+mask", GameEnums.Recoloring_Matrix);
        building.loadSprite("gate+E+W+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Gate");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.onDestroyed = function(building, map)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        var player = building.getOwner();
        map.replaceTerrainOnly("PLAINS", x, y);
        var terrain = map.getTerrain(x, y);
        terrain.loadSprites();
        terrain.loadBuilding("ZDESTROYED_GATE_E_W");
        terrain.getBuilding().setOwner(player);
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 2);
        animation.addScreenshake(30, 0.95, 1000, 200);
        animation.setSound("explosion+land.wav");
    };

    this.getDescription = function()
    {
        return qsTr("A gate through a wall can be crossed by all allied players. Can be destroyed by other players");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        var variables = terrain.getVariables();
        var variable = variables.getVariable("FOREGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 1);
            variable = variables.createVariable("FOREGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
            if (rand > 1)
            {
                rand = globals.randInt(0, 1);
                variable = variables.createVariable("FOREGROUND_ID");
                variable.writeDataInt32(rand);
            }
        }
        return "fore_gate+" + rand.toString();
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return Global[terrain.getTerrainID()].getTerrainAnimationBackground(unit, terrain, defender, map);
    };
}

Constructor.prototype = BUILDING;
var ZGATE_E_W = new Constructor();
