var IngameInfoBar =
{
    // constants
    BoxOffset : 130,
    MinMapHeight : 220,
    MinMinimapHeight : 100,
    Width : 300,
    CursorInfoHeigth : 330,
    BaseGameInfoHeigth : 330,
    ItemInfoHeight : 270,
    // Player constants
    NoCoFaceSprite : "no_co+face",
    DayLabelId : "DAY_LABEL",
    PlayerNameLabelId : "PLAYER_NAME_LABEL_ID",
    PlayerCoSpriteId : "PLAYER_CO_SPRITE_ID",
    PlayerCoBoxId : "PLAYER_CO_BOX_ID",
    PlayerWeatherSpriteId : "PLAYER_WEATHER_ID",
    PlayerWeatherBoxId : "PLAYER_WEATHER_BOX_ID",
    PlayerBuildingLabelId : "PLAYER_BUILDING_ID",
    PlayerUnitLabelId : "PLAYER_UNIT_ID",
    PlayerFundsLabelId : "PLAYER_FUNDS_ID",
    PlayerArmyLabelId : "PLAYER_ARMY_ID",
    PlayerIncomeLabelId : "PLAYER_INCOME_ID",
    PlayerPlayerLabelId : "PLAYER_PLAYER_ID",
    PlayerTeamLabelId : "PLAYER_TEAM_ID",
    PlayerInfoNameIdx : 0,
    PlayerInfoCO0Idx : 1,
    PlayerInfoCO1Idx : 2,
    PlayerInfoCOBox0Idx : 3,
    PlayerInfoCOBox1Idx : 4,
    PlayerInfoWeatherIdx : 5,
    PlayerInfoWeatherBoxIdx : 6,
    PlayerInfoWeatherPred0Idx : 7,
    PlayerInfoWeatherPred0BoxIdx : 8,
    PlayerInfoWeatherPred1Idx : 9,
    PlayerInfoWeatherPred1BoxIdx : 10,
    PlayerBuildingLabelIdx : 11,
    PlayerUnitLabelIdx : 12,
    PlayerFundsLabelIdx : 13,
    PlayerArmyLabelIdx : 14,
    PlayerIncomeLabelIdx : 15,
    PlayerPlayerLabelIdx : 16,
    PlayerTeamLabelIdx : 17,
    PlayerInfoObjectCount : 18,
    // Terrain constants
    Gray : "#7F7F7F",
    MaxTerrainHp : 100,
    MaxResistance : 20,
    TerrainNameLabelId : "TERRAIN_NAME_ID",
    TerrainHealthId : "TERRAIN_HEALTH_ID",
    TerrainResistanceId : "TERRAIN_RES_ID",
    TerrainOwnerLabelId : "TERRAIN_OWNER_ID",
    TerrainNameLabelIdx : 0,
    TerrainHealthIdx : 1,
    TerrainResistanceIdx : 2,
    TerrainOwnerLabelIdx : 3,
    TerrainInfoObjectCount : 4,
    // Unit constants
    UnitInfoBoxId : "UNIT_INFO_BOX_ID",
    UnitNameLabelId : "UNIT_NAME_LABEL_ID",
    UnitHpId : "UNIT_HP_ID",
    UnitFuelId : "UNIT_FUEL_ID",
    UnitAmmo1Id : "UNIT_AMMO1_ID",
    UnitAmmo2Id : "UNIT_AMMO2_ID",
    UnitOwnerLabelId : "UNIT_OWNER_ID",
    UnitWeapon1LabelId : "UNIT_WEAPON1_LABEL_ID",
    UnitWeapon2LabelId : "UNIT_WEAPON2_LABEL_ID",
    UnitMovetypeLabelId : "UNIT_MOVETYPE_LABEL_ID",
    UnitMovepointsLabelId : "UNIT_MOVEPOINTS_LABEL_ID",
    UnitVisionLabelId : "UNIT_VISION_LABEL_ID",
    UnitLoadedUnitBoxId : "UNIT_LOADED_UNIT_BOX_ID",
    UnitLoadedUnitId : "UNIT_LOADED_UNIT_ID",
    UnitInfoBoxIdx : 0,
    UnitNameLabelIdx : 1,
    UnitHpIdx : 2,
    UnitFuelIdx : 3,
    UnitAmmo1Idx : 4,
    UnitAmmo2Idx : 5,
    UnitOwnerLabelIdx : 6,
    UnitWeapon1LabelIdx : 7,
    UnitWeapon2LabelIdx : 8,
    UnitMovetypeLabelIdx : 9,
    UnitMovepointsLabelIdx : 10,
    UnitVisionLabelIdx : 11,
    UnitLoadedUnitStartIdx : 12,
    UnitInfoObjectCount : 23,
    MaxUnitHp : 10,
    MaxShownLoadedUnits : 5,
    // Movement Info constants
    MovementInfoBoxId :  "MOVEMENT_INFO_BOX_ID",
    MovementInfoLabelName0Id :  "MOVEMENT_INFO_LABEL_NAME0_ID",
    MovementInfoLabelValue0Id :  "MOVEMENT_INFO_LABEL_Value0_ID",
    MovementInfoLabelName1Id :  "MOVEMENT_INFO_LABEL_NAME1_ID",
    MovementInfoLabelValue1Id :  "MOVEMENT_INFO_LABEL_Value1_ID",
    MovementInfoBoxIdx : 0,
    MovementInfoStartIdx : 1,
    MaxShownMovementInfos : 10,
    MovementInfoObjectCount : 1 + 10 * 4,
    // UI Size calculated values in initValues
    ShownPlayerCount : 1,
    GameInfoHeigth : 330,    
    DayLabel : null,
    PlayerInfoObjects : [],
    TerrainInfoObjects : [],
    UnitInfoObjects : [],
    MovementInfoObjects : [],
    getBaseMinimapHeight : function()
    {
        return settings.getStageHeight() - IngameInfoBar.CursorInfoHeigth - IngameInfoBar.GameInfoHeigth;
    },
    getMinimapHeight : function()
    {
        var height = IngameInfoBar.getBaseMinimapHeight();
        if (height < IngameInfoBar.MinMinimapHeight)
        {
            height = IngameInfoBar.MinMinimapHeight;
        }
        return height;
    },
    getScale : function(menu, map)
    {
        var height = IngameInfoBar.getBaseMinimapHeight();
        if (height < IngameInfoBar.MinMinimapHeight)
        {
            return settings.getStageHeight() / (IngameInfoBar.MinMinimapHeight + IngameInfoBar.CursorInfoHeigth + IngameInfoBar.GameInfoHeigth);
        }
        else
        {
            return 1;
        }
    },
    preInitValues : function(menu, map)
    {
        IngameInfoBar.ShownPlayerCount = 1;
        IngameInfoBar.GameInfoHeigth = IngameInfoBar.BaseGameInfoHeigth;
        var playerCount = map.getPlayerCount();
        var freeSize = settings.getStageHeight() - IngameInfoBar.CursorInfoHeigth - IngameInfoBar.GameInfoHeigth - IngameInfoBar.MinMapHeight;
        while ((IngameInfoBar.ShownPlayerCount < playerCount && IngameInfoBar.ItemInfoHeight <= freeSize))
        {
            freeSize = freeSize - IngameInfoBar.ItemInfoHeight;
            IngameInfoBar.GameInfoHeigth = IngameInfoBar.GameInfoHeigth + IngameInfoBar.ItemInfoHeight;
            IngameInfoBar.ShownPlayerCount = IngameInfoBar.ShownPlayerCount + 1;
        }
        return IngameInfoBar.ShownPlayerCount;
    },
    postInitValues : function(menu, map)
    {
        IngameInfoBar.PlayerInfoObjects = new Array(IngameInfoBar.ShownPlayerCount);        
        IngameInfoBar.DayLabel = menu.getObject(IngameInfoBar.DayLabelId);
        for (var i = 0; i < IngameInfoBar.ShownPlayerCount; ++i)
        {
            IngameInfoBar.PlayerInfoObjects[i] = new Array(IngameInfoBar.PlayerInfoObjectCount);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoNameIdx] = menu.getObject(IngameInfoBar.PlayerNameLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoCO0Idx] = menu.getObject(IngameInfoBar.PlayerCoSpriteId + "0_" + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoCO1Idx] = menu.getObject(IngameInfoBar.PlayerCoSpriteId + "1_" + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoCOBox0Idx] = menu.getObject(IngameInfoBar.PlayerCoBoxId + "0_" + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoCOBox1Idx] = menu.getObject(IngameInfoBar.PlayerCoBoxId + "1_" + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherIdx] = menu.getObject(IngameInfoBar.PlayerWeatherSpriteId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherBoxIdx] = menu.getObject(IngameInfoBar.PlayerWeatherBoxId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherPred0Idx] = menu.getObject(IngameInfoBar.PlayerWeatherSpriteId + i + "_0");
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherPred0BoxIdx] = menu.getObject(IngameInfoBar.PlayerWeatherBoxId + i + "_0");
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherPred1Idx] = menu.getObject(IngameInfoBar.PlayerWeatherSpriteId + i + "_1");
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerInfoWeatherPred1BoxIdx] = menu.getObject(IngameInfoBar.PlayerWeatherBoxId + i + "_1");
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerBuildingLabelIdx] = menu.getObject(IngameInfoBar.PlayerBuildingLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerUnitLabelIdx] = menu.getObject(IngameInfoBar.PlayerUnitLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerFundsLabelIdx] = menu.getObject(IngameInfoBar.PlayerFundsLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerArmyLabelIdx] = menu.getObject(IngameInfoBar.PlayerArmyLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerIncomeLabelIdx] = menu.getObject(IngameInfoBar.PlayerIncomeLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerPlayerLabelIdx] = menu.getObject(IngameInfoBar.PlayerPlayerLabelId + i);
            IngameInfoBar.PlayerInfoObjects[i][IngameInfoBar.PlayerTeamLabelIdx] = menu.getObject(IngameInfoBar.PlayerTeamLabelId + i);
        }
        IngameInfoBar.TerrainInfoObjects = new Array(IngameInfoBar.TerrainInfoObjectCount);
        IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainNameLabelIdx] = menu.getObject(IngameInfoBar.TerrainNameLabelId);
        IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainHealthIdx] = menu.getObject(IngameInfoBar.TerrainHealthId);
        IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainResistanceIdx] = menu.getObject(IngameInfoBar.TerrainResistanceId);
        IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainOwnerLabelIdx] = menu.getObject(IngameInfoBar.TerrainOwnerLabelId);

        IngameInfoBar.UnitInfoObjects = new Array(IngameInfoBar.UnitInfoObjectCount);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitInfoBoxIdx] = menu.getObject(IngameInfoBar.UnitInfoBoxId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitNameLabelIdx] = menu.getObject(IngameInfoBar.UnitNameLabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitHpIdx] = menu.getObject(IngameInfoBar.UnitHpId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitFuelIdx] = menu.getObject(IngameInfoBar.UnitFuelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo1Idx] = menu.getObject(IngameInfoBar.UnitAmmo1Id);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo2Idx] = menu.getObject(IngameInfoBar.UnitAmmo2Id);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitOwnerLabelIdx] = menu.getObject(IngameInfoBar.UnitOwnerLabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon1LabelIdx] = menu.getObject(IngameInfoBar.UnitWeapon1LabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon2LabelIdx] = menu.getObject(IngameInfoBar.UnitWeapon2LabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitMovetypeLabelIdx] = menu.getObject(IngameInfoBar.UnitMovetypeLabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitMovepointsLabelIdx] = menu.getObject(IngameInfoBar.UnitMovepointsLabelId);
        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitVisionLabelIdx] = menu.getObject(IngameInfoBar.UnitVisionLabelId);
        for (var i = 0; i < IngameInfoBar.MaxShownLoadedUnits; ++i)
        {
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2] = menu.getObject(IngameInfoBar.UnitLoadedUnitBoxId + i);
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2 + 1] = menu.getObject(IngameInfoBar.UnitLoadedUnitId + i);
        }
        IngameInfoBar.MovementInfoObjects = new Array(IngameInfoBar.MovementInfoObjectCount);
        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoBoxIdx] = menu.getObject(IngameInfoBar.MovementInfoBoxId);
        for (var i = 0; i < IngameInfoBar.MaxShownMovementInfos; ++i)
        {
            IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + i * 4] = menu.getObject(IngameInfoBar.MovementInfoLabelName0Id + i);
            IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + i * 4 + 1] = menu.getObject(IngameInfoBar.MovementInfoLabelValue0Id + i);
            IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + i * 4 + 2] = menu.getObject(IngameInfoBar.MovementInfoLabelName1Id + i);
            IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + i * 4 + 3] = menu.getObject(IngameInfoBar.MovementInfoLabelValue1Id + i);

        }
    },
    updatePlayerInfo : function(menu, map)
    {
        if (menu !== null)
        {
            IngameInfoBar.DayLabel.setHtmlText(map.getCurrentDay());
            var count = 0;
            var playerCount = map.getPlayerCount();
            var startPlayerId = map.getCurrentPlayer().getPlayerID();
            for (var i = startPlayerId; count < IngameInfoBar.ShownPlayerCount && i < playerCount; ++i)
            {
                IngameInfoBar.updatePlayerInfoForPlayer(menu, map, i, count);
                ++count;
            }
            for (var i = 0; count < IngameInfoBar.ShownPlayerCount && i < startPlayerId; ++i)
            {
                IngameInfoBar.updatePlayerInfoForPlayer(menu, map, i, count);
                ++count;
            }
        }
    },
    updatePlayerInfoForPlayer : function(menu, map, playerIdx, objIdx)
    {
        var player = map.getPlayer(playerIdx);
        var viewPlayer = menu.getGameMenu().getCurrentViewPlayer();
        var gameRules = map.getGameRules();
        var fogMode = gameRules.getFogMode();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoNameIdx].setHtmlText(player.getPlayerNameId());
        var spriteId = IngameInfoBar.NoCoFaceSprite;
        if (co0 !== null)
        {
            spriteId = co0.getCoID() + "+face";
        }
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoCO0Idx].loadSprite(spriteId, 87);
        spriteId = IngameInfoBar.NoCoFaceSprite;
        if (co1 !== null)
        {
            spriteId = co1.getCoID() + "+face";
        }
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoCO1Idx].loadSprite(spriteId, 87);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherIdx].loadSprite(gameRules.getCurrentWeather().getWeatherSymbol(), 85);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoCOBox0Idx].setColorTableFromPlayer(player, true);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoCOBox1Idx].setColorTableFromPlayer(player, true);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherBoxIdx].setColorTableFromPlayer(player, true);
        var weatherPrediction = gameRules.getWeatherPrediction();
        if (weatherPrediction)
        {
            var weather = gameRules.getWeatherAtDay(1, playerIdx);
            if (weather !== null)
            {
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred0BoxIdx].setIsVisible(true);
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred0BoxIdx].setColorTableFromPlayer(player, true);
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred0Idx].loadSprite(weather.getWeatherSymbol(), 37);
            }
            else
            {
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred0BoxIdx].setIsVisible(false);
            }
            weather = gameRules.getWeatherAtDay(2, playerIdx);
            if (weather !== null)
            {
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred1BoxIdx].setIsVisible(true);
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred1BoxIdx].setColorTableFromPlayer(player, true);
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred1Idx].loadSprite(weather.getWeatherSymbol(), 37);
            }
            else
            {
                IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred1BoxIdx].setIsVisible(false);
            }
        }
        else
        {
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred0BoxIdx].setIsVisible(false);
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerInfoWeatherPred1BoxIdx].setIsVisible(false);
        }
        var foggy = fogMode !== GameEnums.Fog_Off || fogMode !== GameEnums.Fog_OfMist;
        var fogInfo = (viewPlayer === null || (foggy && viewPlayer.getTeam() !== player.getTeam()));
        if (fogInfo)
        {
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerBuildingLabelIdx].setHtmlText(qsTr("?"));
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerUnitLabelIdx].setHtmlText(qsTr("?"));
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerFundsLabelIdx].setHtmlText(qsTr("?"));
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerArmyLabelIdx].setHtmlText(qsTr("?"));
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerIncomeLabelIdx].setHtmlText(qsTr("?"));
        }
        else
        {
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerBuildingLabelIdx].setHtmlText(player.getBuildingCount());
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerUnitLabelIdx].setHtmlText(player.getUnitCount());
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerFundsLabelIdx].setHtmlText(player.getFunds());
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerArmyLabelIdx].setHtmlText(player.calcArmyValue());
            IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerIncomeLabelIdx].setHtmlText(player.calcIncome());
        }
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerPlayerLabelIdx].setHtmlText(player.getPlayerID() + 1);
        IngameInfoBar.PlayerInfoObjects[objIdx][IngameInfoBar.PlayerTeamLabelIdx].setHtmlText(player.getTeam() + 1);
    },
    updateTerrainInfo : function(menu, map, x, y)
    {
        var updatedDetailedView = false;
        if (menu !== null)
        {
            var viewPlayer = menu.getGameMenu().getCurrentViewPlayer();
            if (viewPlayer !== null)
            {
                var visionHide = viewPlayer.getFieldVisibleType(x, y);
                if (visionHide !== GameEnums.VisionType_Shrouded)
                {                    
                    var terrain = map.getTerrain(x, y);
                    var unit = terrain.getUnit();
                    if (unit !== null && unit.isStealthed(viewPlayer))
                    {
                        unit = null;
                    }
                    updatedDetailedView = true;
                    IngameInfoBar.createTerrainInfo(menu, map, x, y, viewPlayer, terrain, unit);
                    var created = IngameInfoBar.createUnitInfo(menu, map, x, y, viewPlayer, terrain, unit);
                    IngameInfoBar.createMovementInfo(menu, map, terrain, created);
                }
            }
        }
        return updatedDetailedView;
    },
    createTerrainInfo : function(menu, map, x, y, viewPlayer, terrain, unit)
    {
        var building = terrain.getBuilding();
        var hp = 0;
        if (building !== null)
        {
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainNameLabelIdx].setHtmlText(building.getName());
            hp = building.getHp();
        }
        else
        {
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainNameLabelIdx].setHtmlText(terrain.getTerrainName());
            hp = terrain.getHp();
        }
        if (hp > 0)
        {
            var maxHp = IngameInfoBar.MaxTerrainHp;
            if (hp > maxHp)
            {
                maxHp = hp
            }
            var divider = hp / maxHp;
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainHealthIdx].setIsVisible(true);
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainHealthIdx].changeBar(divider, IngameInfoBar.getHealthColor(divider), IngameInfoBar.Gray);
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainHealthIdx].setText(qsTr("HP: ") + hp);
        }
        else
        {
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainHealthIdx].setIsVisible(false);
        }
        if (building !== null || (unit !== null && unit.getCapturePoints() > 0))
        {
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainResistanceIdx].setIsVisible(true);
            var resistance = IngameInfoBar.MaxResistance;
            if (unit !== null)
            {
                resistance = IngameInfoBar.MaxResistance - unit.getCapturePoints();
            }
            var divider = resistance / IngameInfoBar.MaxResistance;
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainResistanceIdx].changeBar(divider, "#00FFFF", IngameInfoBar.Gray);
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainResistanceIdx].setText(qsTr("Resistance: ") + resistance);
        }
        else
        {
            IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainResistanceIdx].setIsVisible(false);
        }
        var ownerText = qsTr("Neutral");
        if (building !== null)
        {
            var visionHide = viewPlayer.getFieldVisibleType(x, y);
            var owner = building.getOwner();
            if (owner !== null && visionHide === GameEnums.VisionType_Clear)
            {
                ownerText = qsTr("Player ") + (owner.getPlayerID() + 1);
            }
        }
        IngameInfoBar.TerrainInfoObjects[IngameInfoBar.TerrainOwnerLabelIdx].setHtmlText(ownerText);
    },
    createUnitInfo : function(menu, map, x, y, viewPlayer, terrain, unit)
    {
        var created = false;
        if (unit !== null)
        {
            created = true;
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitInfoBoxIdx].setIsVisible(true);
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitNameLabelIdx].setHtmlText(unit.getName());
            var owner = unit.getOwner();
            var hp = unit.getHp();
            var hpRounded = unit.getHpRounded();
            var divider = hp / IngameInfoBar.MaxUnitHp;
            var hpHidden = unit.getHpHidden(viewPlayer);
            var perfectHpVision = unit.getPerfectHpView(viewPlayer);
            if (hpHidden)
            {
                divider = 0.0;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitHpIdx].setText(qsTr("HP: ") + "?/" + IngameInfoBar.MaxUnitHp);
            }
            else if (perfectHpVision)
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitHpIdx].setText(qsTr("HP: ") + hp * 10 + "/" + IngameInfoBar.MaxUnitHp * 10);
            }
            else
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitHpIdx].setText(qsTr("HP: ") + hpRounded + "/" + IngameInfoBar.MaxUnitHp);
            }

            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitHpIdx].changeBar(divider, IngameInfoBar.getHealthColor(divider), IngameInfoBar.Gray);
            var fuel = unit.getFuel();
            var maxFuel = unit.getMaxFuel();
            if (maxFuel > 0)
            {
                divider = fuel / maxFuel;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitFuelIdx].setText(qsTr("Fuel: ") + fuel + "/" + maxFuel);
            }
            else
            {
                divider = 0;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitFuelIdx].setText(qsTr("Fuel: ") + "-/-");
            }
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitFuelIdx].changeBar(divider, "#0000FF", IngameInfoBar.Gray);

            var ammo1 = unit.getAmmo1();
            var maxAmmo1 = unit.getMaxAmmo1();
            if (maxAmmo1 > 0)
            {
                divider = ammo1 / maxAmmo1;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo1Idx].setText(qsTr("Ammo 1: ") + ammo1 + "/" + maxAmmo1);
            }
            else
            {
                divider = 0;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo1Idx].setText(qsTr("Ammo 1: ") + "-/-");
            }
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo1Idx].changeBar(divider, "#FFA500", IngameInfoBar.Gray);

            var ammo2 = unit.getAmmo2();
            var maxAmmo2 = unit.getMaxAmmo2();
            if (maxAmmo2 > 0)
            {
                divider = ammo2 / maxAmmo2;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo2Idx].setText(qsTr("Ammo 2: ") + ammo2 + "/" + maxAmmo2);
            }
            else
            {
                divider = 0;
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo2Idx].setText(qsTr("Ammo 2: ") + "-/-");
            }
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitAmmo2Idx].changeBar(divider, "#FFA500", IngameInfoBar.Gray);
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitOwnerLabelIdx].setHtmlText(qsTr("Player ") + (owner.getPlayerID() + 1));
            var weaponId = unit.getWeapon1ID();
            if (weaponId !== "")
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon1LabelIdx].setHtmlText(weaponManager.getWeaponName(weaponId));
            }
            else
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon1LabelIdx].setHtmlText("-");
            }
            weaponId = unit.getWeapon2ID();
            if (weaponId !== "")
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon2LabelIdx].setHtmlText(weaponManager.getWeaponName(weaponId));
            }
            else
            {
                IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitWeapon2LabelIdx].setHtmlText("-");
            }
            var position = Qt.point(x, y);
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitMovetypeLabelIdx].setHtmlText(movementTableManager.getMovementName(unit.getMovementType()));
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitMovepointsLabelIdx].setHtmlText(unit.getMovementpoints(position));
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitVisionLabelIdx].setHtmlText(unit.getVision(position));

            var loadingPlace = unit.getLoadingPlace();
            var loadingCount = unit.getLoadedUnitCount();
            if (loadingCount > loadingPlace)
            {
                loadingPlace = loadingCount;
            }
            var transportHidden = unit.getTransportHidden(viewPlayer);
            for (var i = 0; i < IngameInfoBar.MaxShownLoadedUnits; ++i)
            {
                if (i < loadingPlace)
                {
                    IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2].setIsVisible(true);
                    IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2].setColorTableFromPlayer(owner, true);
                    if (transportHidden)
                    {
                        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2 + 1].loadFromUnit(null);
                    }
                    else
                    {
                        IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2 + 1].loadFromUnit(unit.getLoadedUnit(i));
                    }
                }
                else
                {
                    IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitLoadedUnitStartIdx + i * 2].setIsVisible(false);
                }
            }
        }
        else
        {
            IngameInfoBar.UnitInfoObjects[IngameInfoBar.UnitInfoBoxIdx].setIsVisible(false);
        }
        return created;
    },
    createMovementInfo : function(menu, map, terrain, unitInfoCreated)
    {
        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoBoxIdx].setIsVisible(!unitInfoCreated);
        if (!unitInfoCreated)
        {
            var moveTypes = movementTableManager.getMovementTypes();
            var count = moveTypes.length;
            var moveIdx = 0;
            var itemIdx = 0;
            while (itemIdx < IngameInfoBar.MaxShownMovementInfos * 2)
            {
                var cost = -1;
                if (moveIdx < count)
                {
                    var id = moveTypes[moveIdx];
                    cost = movementTableManager.getBaseMovementPoints(id, terrain, terrain, null);
                    if (cost >= 0)
                    {
                        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2].setIsVisible(true);
                        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2 + 1].setIsVisible(true);
                        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2].setHtmlText(movementTableManager.getMovementName(id) + ":");
                        IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2 + 1].setHtmlText(cost);
                        ++itemIdx;
                    }
                }
                else
                {
                    IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2].setIsVisible(false);
                    IngameInfoBar.MovementInfoObjects[IngameInfoBar.MovementInfoStartIdx + itemIdx * 2 + 1].setIsVisible(false);
                    ++itemIdx
                }
                ++moveIdx;
            }
        }
    },

    getHealthColor : function(divider)
    {
        var color = "#FF0000";
        if (divider > 2.0 / 3.0)
        {
            color = "#008000";
        }
        else if (divider > 1.0 / 3.0)
        {
            color = "#FF8000";
        }
        return color;
    },
    getLoadedUnitPosY : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return lastY + lastHeight + 2;
        }
        else
        {
            return lastY;
        }
    },
    getLoadedUnitPosX : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 10;
        }
        else
        {
            return lastX + lastWidth;
        }
    },
    getMovementInfoY : function(loopIdx)
    {
        if (loopIdx === 0)
        {
            return 0;
        }
        else
        {
            return lastY + lastHeight + 1;
        }
    },
};
