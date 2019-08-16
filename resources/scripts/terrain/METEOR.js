var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(METEOR.getName());
        terrain.setHp(100);
    };
    this.getName = function()
    {
        return qsTr("Meteor");
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PLASMA", false, false, GameEnums.Directions_Direct, false);
		// get rid of the north identifier
		surroundings = surroundings.replace("+N", "");
        terrain.loadBaseSprite("meteor" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_meteor";
    };
    this.onDestroyed = function(terrain)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = terrain.getX();
        var y = terrain.getY();
        var plasmaFields = [Qt.point(x, y)];
        var callRet = [];
        var i = 0;
        if (map.onMap(x - 1, y))
        {
            callRet = METEOR.getPlasmaFields(x - 1, y);
            for (i = 0; i < callRet.length; i++)
            {
                plasmaFields.push(callRet[i]);
            }
        }
        if (map.onMap(x + 1, y))
        {
            callRet = METEOR.getPlasmaFields(x + 1, y);
            for (i = 0; i < callRet.length; i++)
            {
                plasmaFields.push(callRet[i]);
            }
        }
        if (map.onMap(x, y - 1))
        {
            callRet = METEOR.getPlasmaFields(x, y - 1);
            for (i = 0; i < callRet.length; i++)
            {
                plasmaFields.push(callRet[i]);
            }
        }
        if (map.onMap(x, y + 1))
        {
            callRet = METEOR.getPlasmaFields(x, y + 1);
            for (i = 0; i < callRet.length; i++)
            {
                plasmaFields.push(callRet[i]);
            }
        }
        var field;
        for (i = 0; i < plasmaFields.length; i++)
        {
            field = plasmaFields[i];
            map.replaceTerrain("PLAINS_PLASMA", field.x, field.y);
        }
        for (i = 0; i < plasmaFields.length; i++)
        {
            field = plasmaFields[i];
            map.getTerrain(field.x, field.y).loadSprites();
        }
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+land.wav");
    };
    this.getPlasmaFields = function (x, y)
    {
        var plasmaFields = [];
        var terrain = map.getTerrain(x, y);
        if (terrain.getID() === "PLASMA")
        {
            var pfs = terrain.createTerrainFindingSystem();
            var points = pfs.getAllQmlVectorPoints();
            for (var i = 0; i < points.size(); i++)
            {
                plasmaFields.push(points.at(i));
            }
            points.remove();
            pfs.killTerrainFindingSystem();
        }
        return plasmaFields;
    };
    this.getDescription = function()
    {
        return qsTr("Meteor pieces which can be attacked and destroyed <div c='00ff00'>to eliminate plasma.</div>");
    };
};
Constructor.prototype = TERRAIN;
var METEOR = new Constructor();
