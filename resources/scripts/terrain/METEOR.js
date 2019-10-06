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
        else if (currentTerrainID === "SEA")
        {
            terrain.loadBaseTerrain("SEA");
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
            var placeTerrain = map.getTerrain(field.x, field.y);
            var baseID = placeTerrain.getBaseTerrainID();
            if (baseID === "PLAINS" ||
                baseID === "SNOW" ||
                baseID === "DESERT")
            {
                map.replaceTerrain("PLAINS_PLASMA", field.x, field.y);
            }
            else
            {
                map.replaceTerrain(baseID, field.x, field.y);
            }
        }
        var testFields =  globals.getCircle(1, 1);
        for (i = 0; i < plasmaFields.length; i++)
        {
            field = plasmaFields[i];
            map.getTerrain(field.x, field.y).loadSprites();
            // update sprites for surrounding meteors
            for (var i2 = 0; i2 < testFields.size(); i2++)
            {
                var pos = testFields.at(i2);
                var posX = field.x + pos.x;
                var posY = field.y + pos.y;
                if (map.onMap(posX, posY))
                {
                    var terrain2 = map.getTerrain(posX, posY);
                    if (terrain2.getID() === "METEOR")
                    {
                        terrain2.loadSprites();
                    }
                }
            }
        }
        testFields.remove();
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
