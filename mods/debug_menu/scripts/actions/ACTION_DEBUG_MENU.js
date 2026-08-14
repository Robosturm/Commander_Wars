// Debug command menu, v2: categories on step 0, commands on step 1, then
// command-specific MENU or FIELD steps. Requires debug_menu_registry.js.
// Menu placement is done in scripts/general/debug_menu_install.js.
var Constructor = function()
{
    this.CAT_INSPECT = "DEBUG_CAT_INSPECT";
    this.CAT_PLAYER = "DEBUG_CAT_PLAYER";
    this.CAT_UNIT = "DEBUG_CAT_UNIT";
    this.CAT_MATCH = "DEBUG_CAT_MATCH";
    this.CAT_PERKS = "DEBUG_CAT_PERKS";
    this.CAT_LOGGING = "DEBUG_CAT_LOGGING";
    this.CAT_TOGGLES = "DEBUG_CAT_TOGGLES";

    this.CMD_INSPECT_TILE = "DEBUG_CMD_INSPECT_TILE";
    this.CMD_INSPECT_PLAYER = "DEBUG_CMD_INSPECT_PLAYER";
    this.CMD_INSPECT_AI = "DEBUG_CMD_INSPECT_AI";
    this.CMD_INSPECT_RULES = "DEBUG_CMD_INSPECT_RULES";
    this.CMD_DUMP = "DEBUG_CMD_DUMP";
    this.CMD_FUNDS_ADD = "DEBUG_CMD_FUNDS";
    this.CMD_FUNDS_ZERO = "DEBUG_CMD_FUNDS_ZERO";
    this.CMD_FUNDS_RICH = "DEBUG_CMD_FUNDS_RICH";
    this.CMD_CHARGE = "DEBUG_CMD_CHARGE";
    this.CMD_DEFEAT = "DEBUG_CMD_DEFEAT";
    this.CMD_HEAL = "DEBUG_CMD_HEAL";
    this.CMD_REFRESH_ALL = "DEBUG_CMD_REFRESH_ALL";
    this.CMD_UNIT_SET_HP = "DEBUG_CMD_UNIT_SET_HP";
    this.CMD_UNIT_REFRESH = "DEBUG_CMD_UNIT_REFRESH";
    this.CMD_UNIT_PROMOTE = "DEBUG_CMD_UNIT_PROMOTE";
    this.CMD_UNIT_KILL = "DEBUG_CMD_UNIT_KILL";
    this.CMD_UNIT_DELETE = "DEBUG_CMD_UNIT_DELETE";
    this.CMD_UNIT_OWNER = "DEBUG_CMD_UNIT_OWNER";
    this.CMD_UNIT_SPAWN = "DEBUG_CMD_UNIT_SPAWN";
    this.CMD_FOG = "DEBUG_CMD_FOG";
    this.CMD_WEATHER = "DEBUG_CMD_WEATHER";
    this.CMD_PERK_LIST = "DEBUG_CMD_PERK_LIST";
    this.CMD_PERK_GRANT = "DEBUG_CMD_PERK_GRANT";
    this.CMD_PERK_REVOKE = "DEBUG_CMD_PERK_REVOKE";
    this.CMD_PERK_BUDGET = "DEBUG_CMD_PERK_BUDGET";
    this.CMD_LOG_LEVEL = "DEBUG_CMD_LOG_LEVEL";
    this.CMD_LOG_MODULE_AI = "DEBUG_CMD_LOG_MODULE_AI";
    this.CMD_LOG_MODULE_JS = "DEBUG_CMD_LOG_MODULE_JS";
    this.CMD_MOD_FLAGS = "DEBUG_CMD_MOD_FLAGS";
    this.CMD_KEEP_POWER = "DEBUG_CMD_KEEP_POWER";
    this.CMD_KEEP_FUNDS = "DEBUG_CMD_KEEP_FUNDS";
    this.CMD_ICONS = "DEBUG_CMD_ICONS";
    this.CMD_DEVELOPER_MODE = "DEBUG_CMD_DEVELOPER_MODE";

    this.STEP_MENU = DEBUG_MENU.STEP_MENU;
    this.STEP_FIELD = DEBUG_MENU.STEP_FIELD;
    // input steps consumed by the category pick plus the command pick
    this.PATH_STEPS = 2;
    // placeholder entry so an empty submenu never soft-locks the input loop
    this.MENU_EMPTY_ID = "DEBUG_MENU_EMPTY";

    this.MENU_ICON = "options";
    this.FIELD_COLOR_SELECT = "#C800FF00";
    this.FIELD_COLOR_DESTROY = "#C8FF0000";

    this.FUNDS_ADD_AMOUNT = 10000;
    this.FUNDS_RICH_AMOUNT = 999999;
    this.MAX_HP = 10;
    this.HP_PRESETS = [1, 5, 10];
    this.CO_SLOT_COUNT = 2;
    this.CHARGE_ALMOST_EPSILON = 0.05;
    this.PERK_COST_LIFTED = 9999;
    this.PERK_COUNT_LIFTED = 99;
    this.MODULE_AI_BIT = 8;
    this.MODULE_JS_BIT = 32;
    this.LOG_LEVEL_NAMES = ["DEBUG", "INFO", "WARNING", "ERROR", "FATAL"];
    this.DISABLED_PLAYER = -1;
    this.STATE_KEEP_POWER = "keepPowerPlayer";
    this.STATE_KEEP_FUNDS = "keepFundsPlayer";

    this.CHARGE_PRESET_EMPTY = "DEBUG_CHARGE_EMPTY";
    this.CHARGE_PRESET_COP = "DEBUG_CHARGE_COP";
    this.CHARGE_PRESET_ALMOST = "DEBUG_CHARGE_ALMOST";
    this.CHARGE_PRESET_FULL = "DEBUG_CHARGE_FULL";

    this.SPAWN_UNITS = [
        "INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "LIGHT_TANK",
        "HEAVY_TANK", "NEOTANK", "MEGATANK", "APC", "FLAK", "ARTILLERY",
        "ROCKETTHROWER", "MISSILE", "ANTITANKCANNON", "K_HELI", "T_HELI",
        "FIGHTER", "BOMBER", "STEALTHBOMBER", "LANDER", "CRUISER",
        "DESTROYER", "SUBMARINE", "BATTLESHIP"];

    // matches the diplomacy mod's icon set so the toggle covers its overlays
    this.AI_MODE_ICONS = [
        {mode: GameEnums.GameAi_Offensive, icon: "offensive"},
        {mode: GameEnums.GameAi_Defensive, icon: "defensive"},
        {mode: GameEnums.GameAi_Hold, icon: "hold"},
        {mode: GameEnums.GameAi_TargetEnemyHq, icon: "hq"},
        {mode: GameEnums.GameAi_Patrol, icon: "patrol"},
        {mode: GameEnums.GameAi_PatrolLoop, icon: "patrol_loop"},
        {mode: GameEnums.GameAi_Scripted, icon: "scripted"}];
    this.AI_ICON_IDS = [];
    for (var modeIndex = 0; modeIndex < this.AI_MODE_ICONS.length;
         ++modeIndex)
    {
        this.AI_ICON_IDS.push(this.AI_MODE_ICONS[modeIndex].icon);
    }

    // plain JS state below is not serialized: debug only, resets on reload
    this.iconsHidden = true;
    this.iconsTouched = false;
    this.savedUpdateOrderIcon = null;
    this.savedFogMode = null;
    this.keepPowerPlayer = this.DISABLED_PLAYER;
    this.keepFundsPlayer = this.DISABLED_PLAYER;

    // chain-patch sentinels installed by custom_gamemodes and deckbuilder
    this.PATCH_SENTINELS = [
        {owner: "PLAYER", props: [
            "__cmode_koth_patched", "__cmode_sustain_patched",
            "__cmode_sudden_patched", "__cmode_grow_patched",
            "__cmode_shrink_patched", "__cmode_meteor_patched",
            "__cmode_chaos_patched", "__cmode_revgrav_patched",
            "__cmode_random_co_patched", "__db_patched", "__db_endpatched"]},
        {owner: "ACTION_BUILD_UNITS", props: [
            "__cmode_patched", "__db_curse_patched"]},
        {owner: "ACTION_CAPTURE", props: ["__db_curse_patched"]},
        {owner: "ACTION_NEXT_PLAYER", props: ["__db_pending_pick_patched"]}];

    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Debug...");
    };
    this.getIcon = function(map)
    {
        return ACTION_DEBUG_MENU.MENU_ICON;
    };

    // ------------------------------------------------------------------
    // step routing

    this.builtinCategories = function()
    {
        var self = ACTION_DEBUG_MENU;
        return [
            {id: self.CAT_INSPECT, text: qsTr("Inspect...")},
            {id: self.CAT_PLAYER, text: qsTr("Player...")},
            {id: self.CAT_UNIT, text: qsTr("Unit...")},
            {id: self.CAT_MATCH, text: qsTr("Match...")},
            {id: self.CAT_PERKS, text: qsTr("Perks...")},
            {id: self.CAT_LOGGING, text: qsTr("Logging...")},
            {id: self.CAT_TOGGLES, text: qsTr("Toggles...")}];
    };

    this.effectiveCategory = function(command)
    {
        var self = ACTION_DEBUG_MENU;
        var builtins = self.builtinCategories();
        for (var i = 0; i < builtins.length; ++i)
        {
            if (builtins[i].id === command.category)
            {
                return command.category;
            }
        }
        return DEBUG_MENU.CATEGORY_MODS;
    };

    this.getCategories = function()
    {
        var self = ACTION_DEBUG_MENU;
        var categories = self.builtinCategories();
        for (var i = 0; i < DEBUG_MENU.commands.length; ++i)
        {
            if (self.effectiveCategory(DEBUG_MENU.commands[i]) ===
                DEBUG_MENU.CATEGORY_MODS)
            {
                categories.push({id: DEBUG_MENU.CATEGORY_MODS,
                                 text: qsTr("Mods...")});
                break;
            }
        }
        return categories;
    };

    this.getCommand = function(id)
    {
        var self = ACTION_DEBUG_MENU;
        if (Object.prototype.hasOwnProperty.call(self.commands, id))
        {
            return self.commands[id];
        }
        return DEBUG_MENU.findCommand(id);
    };

    this.commandsForCategory = function(category)
    {
        var self = ACTION_DEBUG_MENU;
        var found = [];
        for (var i = 0; i < self.commandOrder.length; ++i)
        {
            var command = self.commands[self.commandOrder[i]];
            if (command.category === category)
            {
                found.push(command);
            }
        }
        for (var i2 = 0; i2 < DEBUG_MENU.commands.length; ++i2)
        {
            var registered = DEBUG_MENU.commands[i2];
            if (self.effectiveCategory(registered) === category)
            {
                found.push(registered);
            }
        }
        return found;
    };

    this.readPath = function(action)
    {
        var self = ACTION_DEBUG_MENU;
        var step = action.getInputStep();
        var path = {category: null, command: null, spec: null, args: []};
        action.startReading();
        if (step < 1)
        {
            return path;
        }
        path.category = action.readDataString();
        if (step < self.PATH_STEPS)
        {
            return path;
        }
        path.command = action.readDataString();
        path.spec = self.getCommand(path.command);
        if (path.spec === null)
        {
            return path;
        }
        var argCount = step - self.PATH_STEPS;
        for (var i = 0; i < argCount && i < path.spec.steps.length; ++i)
        {
            if (path.spec.steps[i].type === self.STEP_FIELD)
            {
                path.args.push({x: action.readDataInt32(),
                                y: action.readDataInt32()});
            }
            else
            {
                path.args.push(action.readDataString());
            }
        }
        return path;
    };

    this.getStepInputType = function(action, map)
    {
        var self = ACTION_DEBUG_MENU;
        var step = action.getInputStep();
        if (step < self.PATH_STEPS)
        {
            return self.STEP_MENU;
        }
        var path = self.readPath(action);
        if (path.spec === null)
        {
            return "";
        }
        var index = step - self.PATH_STEPS;
        if (index >= path.spec.steps.length)
        {
            return "";
        }
        return path.spec.steps[index].type;
    };

    this.isFinalStep = function(action, map)
    {
        var self = ACTION_DEBUG_MENU;
        var step = action.getInputStep();
        if (step < self.PATH_STEPS)
        {
            return false;
        }
        var path = self.readPath(action);
        if (path.spec === null)
        {
            // unknown command, finish instead of hanging the input loop
            return true;
        }
        return (step - self.PATH_STEPS) >= path.spec.steps.length;
    };

    this.getStepData = function(action, data, map)
    {
        var self = ACTION_DEBUG_MENU;
        var step = action.getInputStep();
        if (step === 0)
        {
            var categories = self.getCategories();
            for (var i = 0; i < categories.length; ++i)
            {
                data.addData(categories[i].text, categories[i].id,
                             self.MENU_ICON, 0, true);
            }
            return;
        }
        var path = self.readPath(action);
        if (step === 1)
        {
            var commands = self.commandsForCategory(path.category);
            for (var i2 = 0; i2 < commands.length; ++i2)
            {
                var command = commands[i2];
                var icon = (command.icon !== undefined) ?
                    command.icon : self.MENU_ICON;
                data.addData(command.text(map), command.id, icon, 0, true);
            }
            return;
        }
        if (path.spec === null)
        {
            return;
        }
        var stepSpec = path.spec.steps[step - self.PATH_STEPS];
        if (stepSpec === undefined)
        {
            return;
        }
        if (stepSpec.type === self.STEP_MENU)
        {
            var options = stepSpec.options(map, path.args);
            if (options.length === 0)
            {
                data.addData(qsTr("(nothing available)"),
                             self.MENU_EMPTY_ID, self.MENU_ICON, 0, true);
                return;
            }
            for (var i3 = 0; i3 < options.length; ++i3)
            {
                var option = options[i3];
                var optionIcon = (option.icon !== undefined) ?
                    option.icon : self.MENU_ICON;
                data.addData(option.text, option.id, optionIcon, 0, true);
            }
        }
        else
        {
            var points = stepSpec.points(map, path.args);
            for (var i4 = 0; i4 < points.length; ++i4)
            {
                data.addPoint(Qt.point(points[i4].x, points[i4].y));
            }
            data.setShowZData(false);
            var color = (stepSpec.color !== undefined) ?
                stepSpec.color : self.FIELD_COLOR_SELECT;
            data.setColor(color);
        }
    };

    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+default");
        cursorData.setXOffset(0);
        cursorData.setYOffset(0);
        cursorData.setScale(1.0);
    };

    this.perform = function(action, map)
    {
        var self = ACTION_DEBUG_MENU;
        var path = self.readPath(action);
        if (path.spec === null)
        {
            self.print("unknown command " + path.command);
            return;
        }
        for (var i = 0; i < path.args.length; ++i)
        {
            if (path.args[i] === self.MENU_EMPTY_ID)
            {
                self.print("nothing available for " + path.command);
                return;
            }
        }
        path.spec.perform(map, path.args, action);
    };

    // ------------------------------------------------------------------
    // shared helpers

    this.print = function(message)
    {
        GameConsole.print(DEBUG_MENU.LOG_PREFIX + message,
                          DEBUG_MENU.LOG_LEVEL_ERROR);
    };

    this.resolveGlobal = function(name)
    {
        return DEBUG_MENU.resolveNamespace(name);
    };

    this.coreaiHasFunction = function(name)
    {
        var coreai = ACTION_DEBUG_MENU.resolveGlobal("COREAI");
        return (coreai !== undefined && coreai !== null &&
                typeof coreai[name] === "function");
    };

    this.coreaiHasProperty = function(name)
    {
        var coreai = ACTION_DEBUG_MENU.resolveGlobal("COREAI");
        return (coreai !== undefined && coreai !== null &&
                coreai[name] !== undefined);
    };

    this.getDiplomacy = function()
    {
        var diplomacy = ACTION_DEBUG_MENU.resolveGlobal("Diplomacy");
        if (diplomacy !== undefined && diplomacy !== null &&
            diplomacy.updateOrderIcon !== undefined)
        {
            return diplomacy;
        }
        return null;
    };

    this.forEachCo = function(player, callback)
    {
        var self = ACTION_DEBUG_MENU;
        for (var index = 0; index < self.CO_SLOT_COUNT; ++index)
        {
            var co = player.getCO(index);
            if (co !== null)
            {
                callback(co, index);
            }
        }
    };

    this.allUnitPoints = function(map)
    {
        var points = [];
        for (var p = 0; p < map.getPlayerCount(); ++p)
        {
            var units = map.getPlayer(p).getUnits();
            for (var i = 0; i < units.size(); ++i)
            {
                var unit = units.at(i);
                points.push({x: unit.getX(), y: unit.getY()});
            }
        }
        return points;
    };

    this.allFieldPoints = function(map)
    {
        var points = [];
        for (var y = 0; y < map.getMapHeight(); ++y)
        {
            for (var x = 0; x < map.getMapWidth(); ++x)
            {
                points.push({x: x, y: y});
            }
        }
        return points;
    };

    this.playerOptions = function(map, excludeCurrent)
    {
        var options = [];
        var currentId = map.getCurrentPlayer().getPlayerID();
        for (var i = 0; i < map.getPlayerCount(); ++i)
        {
            if (excludeCurrent === true && i === currentId)
            {
                continue;
            }
            options.push({id: String(i),
                          text: qsTr("Player ") + (i + 1)});
        }
        return options;
    };

    this.readVariableValue = function(variable, type)
    {
        if (type === DEBUG_MENU.T_I32)
        {
            return variable.readDataInt32();
        }
        if (type === DEBUG_MENU.T_U32)
        {
            return variable.readDataUint32();
        }
        if (type === DEBUG_MENU.T_F32)
        {
            return variable.readDataFloat();
        }
        if (type === DEBUG_MENU.T_BOOL)
        {
            return variable.readDataBool();
        }
        if (type === DEBUG_MENU.T_STR)
        {
            return variable.readDataString();
        }
        if (type === DEBUG_MENU.T_LI32)
        {
            return "[" + self_joinList(variable.readDataListInt32()) + "]";
        }
        if (type === DEBUG_MENU.T_LSTR)
        {
            return "[" + self_joinList(variable.readDataListString()) + "]";
        }
        return "?";
    };

    // reads use getVariable plus a null check only: createVariable would
    // append container state and is forbidden outside real writes
    this.dumpVariables = function(label, variables, containerKey)
    {
        var self = ACTION_DEBUG_MENU;
        if (variables === null)
        {
            return 0;
        }
        var entries = DEBUG_MENU.variables[containerKey];
        var printed = 0;
        for (var i = 0; i < entries.length; ++i)
        {
            var entry = entries[i];
            if (entry.name !== undefined)
            {
                printed += self.dumpOneVariable(label, variables,
                                               entry.name, entry.type);
            }
            else
            {
                for (var index = entry.from; index <= entry.to; ++index)
                {
                    printed += self.dumpOneVariable(
                        label, variables, entry.prefix + index, entry.type);
                }
            }
        }
        return printed;
    };

    this.dumpOneVariable = function(label, variables, name, type)
    {
        var self = ACTION_DEBUG_MENU;
        var variable = variables.getVariable(name);
        if (variable === null)
        {
            return 0;
        }
        self.print(label + "|" + name + "(" + type + ")=" +
                   self.readVariableValue(variable, type));
        return 1;
    };

    this.dumpPlayerAndCos = function(map, player, label)
    {
        var self = ACTION_DEBUG_MENU;
        self.print(label + "|funds=" + player.getFunds() +
                   "|team=" + player.getTeam() +
                   "|defeated=" + player.getIsDefeated() +
                   "|controlType=" + player.getControlType());
        self.dumpVariables(label, player.getVariables(),
                           DEBUG_MENU.CONTAINER_PLAYER);
        self.forEachCo(player, function(co, index)
        {
            var coLabel = label + "|co" + index + "(" + co.getCoID() + ")";
            self.print(coLabel +
                       "|powerFilled=" + co.getPowerFilled() +
                       "|powerStars=" + co.getPowerStars() +
                       "|superStars=" + co.getSuperpowerStars() +
                       "|perks=[" + self_joinList(co.getPerkList()) + "]");
            self.dumpVariables(coLabel, co.getVariables(),
                               DEBUG_MENU.CONTAINER_CO);
        });
    };

    this.setPowerMeters = function(map, fill, player)
    {
        var self = ACTION_DEBUG_MENU;
        if (player === undefined)
        {
            player = map.getCurrentPlayer();
        }
        self.forEachCo(player, function(co, index)
        {
            // powerFilled is measured in stars; full = every star lit
            var stars = fill ?
                co.getPowerStars() + co.getSuperpowerStars() : 0;
            co.setPowerFilled(stars);
        });
    };

    // called from the PLAYER.startOfTurn wrapper in the install script
    this.onStartOfTurn = function(player, map)
    {
        var self = ACTION_DEBUG_MENU;
        self.applyTurnToggles(player, map);
        if (self.iconsTouched)
        {
            self.refreshAiIcons(map);
        }
    };

    this.applyTurnToggles = function(player, map)
    {
        var self = ACTION_DEBUG_MENU;
        if (player.getPlayerID() === self.keepPowerPlayer)
        {
            self.setPowerMeters(map, true, player);
        }
        if (player.getPlayerID() === self.keepFundsPlayer)
        {
            player.setFunds(self.FUNDS_RICH_AMOUNT);
        }
    };

    this.performKeepToggle = function(map, stateKey)
    {
        var self = ACTION_DEBUG_MENU;
        if (self[stateKey] !== self.DISABLED_PLAYER)
        {
            self[stateKey] = self.DISABLED_PLAYER;
        }
        else
        {
            self[stateKey] = map.getCurrentPlayer().getPlayerID();
            self.applyTurnToggles(map.getCurrentPlayer(), map);
        }
        self.print(stateKey + "=" + self[stateKey]);
    };

    this.keepToggleText = function(stateKey, offText, onText)
    {
        var self = ACTION_DEBUG_MENU;
        return (self[stateKey] !== self.DISABLED_PLAYER) ? offText : onText;
    };

    this.unitAt = function(map, point)
    {
        return map.getTerrain(point.x, point.y).getUnit();
    };

    // ------------------------------------------------------------------
    // command implementations

    this.performInspectTile = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var x = args[0].x;
        var y = args[0].y;
        var terrain = map.getTerrain(x, y);
        self.print("inspect " + x + "," + y +
                   "|terrain=" + terrain.getTerrainID());
        self.dumpVariables("terrain@" + x + "," + y,
                           terrain.getVariables(),
                           DEBUG_MENU.CONTAINER_TERRAIN);
        var building = terrain.getBuilding();
        if (building !== null)
        {
            var buildingOwner = building.getOwner();
            var ownerId = (buildingOwner !== null) ?
                buildingOwner.getPlayerID() : self.DISABLED_PLAYER;
            self.print("building@" + x + "," + y +
                       "|id=" + building.getBuildingID() +
                       "|owner=" + ownerId);
            self.dumpVariables("building@" + x + "," + y,
                               building.getVariables(),
                               DEBUG_MENU.CONTAINER_BUILDING);
        }
        var unit = terrain.getUnit();
        if (unit !== null)
        {
            var unitLabel = "unit@" + x + "," + y;
            self.print(unitLabel + "|id=" + unit.getUnitID() +
                       "|owner=" + unit.getOwner().getPlayerID() +
                       "|hp=" + unit.getHp() +
                       "|fuel=" + unit.getFuel() +
                       "|ammo1=" + unit.getAmmo1() +
                       "|ammo2=" + unit.getAmmo2() +
                       "|rank=" + unit.getUnitRank() +
                       "|hasMoved=" + unit.getHasMoved() +
                       "|aiMode=" + unit.getAiMode());
            self.dumpVariables(unitLabel, unit.getVariables(),
                               DEBUG_MENU.CONTAINER_UNIT);
            self.dumpPlayerAndCos(map, unit.getOwner(),
                                  "owner p" + unit.getOwner().getPlayerID());
        }
    };

    this.performInspectPlayer = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var player = map.getCurrentPlayer();
        self.dumpPlayerAndCos(map, player, "p" + player.getPlayerID());
    };

    this.performInspectAi = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var markers = [
            {label: "Counterpoint AI",
             present: self.coreaiHasFunction("_scoreUnitAgainstEnemies")},
            {label: "Classic/Diversity/strat family",
             present: self.coreaiHasFunction("forceComposition")},
            {label: "pvp_ai family",
             present: self.coreaiHasFunction("forceScoutProduction")},
            {label: "Chaos Comp AI",
             present: self.coreaiHasProperty("TRANSPORTER_CHANCE") &&
                      !self.coreaiHasFunction("forceComposition")},
            {label: "ai_overhaul alpha coordinator",
             present: self.resolveGlobal("Coordinator") !== undefined &&
                      self.resolveGlobal("Coordinator") !== null}];
        for (var i = 0; i < markers.length; ++i)
        {
            if (markers[i].present)
            {
                self.print("ai namespace|" + markers[i].label);
            }
        }
        for (var p = 0; p < map.getPlayerCount(); ++p)
        {
            var player = map.getPlayer(p);
            var label = "ai p" + p;
            self.print(label + "|controlType=" + player.getControlType());
            var input = player.getBaseGameInput();
            if (input !== null &&
                typeof input.getSimpleProductionSystem === "function")
            {
                var system = input.getSimpleProductionSystem();
                self.print(label + "|prodSystem init=" + system.getInit() +
                           "|enabled=" + system.getEnabled() +
                           "|producedThisTurn=" +
                           system.getCurrentTurnProducedUnitsCounter());
                self.dumpVariables(label, system.getVariables(),
                                   DEBUG_MENU.CONTAINER_SYSTEM);
            }
        }
    };

    this.performInspectRules = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var rules = map.getGameRules();
        var count = rules.getVictoryRuleSize();
        self.print("victory rules|count=" + count);
        for (var i = 0; i < count; ++i)
        {
            var rule = rules.getVictoryRuleAtIndex(i);
            var ruleId = (typeof rule.getRuleID === "function") ?
                rule.getRuleID() : ("rule#" + i);
            self.print("rule|" + ruleId);
            self.dumpVariables("rule|" + ruleId, rule.getVariables(),
                               DEBUG_MENU.CONTAINER_RULE);
        }
        for (var s = 0; s < self.PATCH_SENTINELS.length; ++s)
        {
            var sentinel = self.PATCH_SENTINELS[s];
            var owner = self.resolveGlobal(sentinel.owner);
            if (owner === undefined || owner === null)
            {
                continue;
            }
            for (var p2 = 0; p2 < sentinel.props.length; ++p2)
            {
                var prop = sentinel.props[p2];
                if (owner[prop] !== undefined)
                {
                    self.print("patch|" + sentinel.owner + "." + prop +
                               "=" + owner[prop]);
                }
            }
        }
    };

    this.performCharge = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var preset = args[0];
        self.forEachCo(map.getCurrentPlayer(), function(co, index)
        {
            var copStars = co.getPowerStars();
            var stars = 0;
            if (preset === self.CHARGE_PRESET_COP)
            {
                stars = copStars;
            }
            else if (preset === self.CHARGE_PRESET_ALMOST)
            {
                stars = copStars - self.CHARGE_ALMOST_EPSILON;
                if (stars < 0)
                {
                    stars = 0;
                }
            }
            else if (preset === self.CHARGE_PRESET_FULL)
            {
                stars = copStars + co.getSuperpowerStars();
            }
            co.setPowerFilled(stars);
            self.print("charge co" + index + " raw=" + co.getPowerFilled());
        });
    };

    this.performWeather = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        var rules = map.getGameRules();
        var index = parseInt(args[0]);
        rules.changeWeather(index, map.getPlayerCount());
        rules.onWeatherChanged();
        self.print("weather " + rules.getWeather(index).getWeatherId());
    };

    this.toggleFog = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        var rules = map.getGameRules();
        var current = rules.getFogMode();
        if (current !== GameEnums.Fog_Off)
        {
            self.savedFogMode = current;
            rules.setFogMode(GameEnums.Fog_Off);
        }
        else
        {
            var restored = (self.savedFogMode !== null) ?
                self.savedFogMode : GameEnums.Fog_OfWar;
            rules.setFogMode(restored);
        }
        rules.createFogVision();
        self.print("fog mode=" + rules.getFogMode());
    };

    this.healAndResupply = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        var units = map.getCurrentPlayer().getUnits();
        for (var index = 0; index < units.size(); ++index)
        {
            var unit = units.at(index);
            unit.setHp(self.MAX_HP);
            unit.refill();
        }
    };

    this.refreshAllUnits = function(map)
    {
        var units = map.getCurrentPlayer().getUnits();
        for (var index = 0; index < units.size(); ++index)
        {
            units.at(index).setHasMoved(false);
        }
    };

    this.performPerkList = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        for (var p = 0; p < map.getPlayerCount(); ++p)
        {
            self.forEachCo(map.getPlayer(p), function(co, index)
            {
                var perks = co.getPerkList();
                for (var i = 0; i < perks.length; ++i)
                {
                    var perkObj = self.resolveGlobal(perks[i]);
                    var active = "unknown";
                    if (perkObj !== undefined && perkObj !== null &&
                        typeof perkObj.isActive === "function")
                    {
                        active = String(perkObj.isActive(co));
                    }
                    self.print("perk p" + p + " co" + index + "|" +
                               perks[i] + "|active=" + active);
                }
            });
        }
    };

    this.coSlotOptions = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        var options = [];
        self.forEachCo(map.getCurrentPlayer(), function(co, index)
        {
            options.push({id: String(index),
                          text: qsTr("CO ") + index + " (" +
                                co.getCoID() + ")"});
        });
        return options;
    };

    this.performPerkChange = function(map, args, grant)
    {
        var self = ACTION_DEBUG_MENU;
        var co = map.getCurrentPlayer().getCO(parseInt(args[0]));
        var method = grant ? "addPerk" : "removePerk";
        if (co === null || typeof co[method] !== "function")
        {
            self.print("perk change failed: no CO or engine too old");
            return;
        }
        co[method](args[1]);
        self.print((grant ? "perk granted|" : "perk revoked|") + args[1]);
    };

    this.modFlagOptions = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        var options = [];
        for (var i = 0; i < DEBUG_MENU.flags.length; ++i)
        {
            var flag = DEBUG_MENU.flags[i];
            if (!flag.isPresent())
            {
                continue;
            }
            var value;
            try
            {
                value = flag.get();
            }
            catch (error)
            {
                continue;
            }
            options.push({id: flag.id, text: flag.label + ": " + value});
        }
        return options;
    };

    this.performModFlag = function(map, args)
    {
        var self = ACTION_DEBUG_MENU;
        for (var i = 0; i < DEBUG_MENU.flags.length; ++i)
        {
            var flag = DEBUG_MENU.flags[i];
            if (flag.id === args[0])
            {
                try
                {
                    flag.set(!flag.get());
                    self.print("flag " + flag.id + "=" + flag.get());
                }
                catch (error)
                {
                    self.print("flag " + flag.id + " failed: " + error);
                }
                return;
            }
        }
    };

    this.clearAiIcons = function(unit)
    {
        var self = ACTION_DEBUG_MENU;
        for (var index = 0; index < self.AI_ICON_IDS.length; ++index)
        {
            unit.unloadIcon(self.AI_ICON_IDS[index]);
        }
    };

    this.applyAiIcon = function(unit, map)
    {
        var self = ACTION_DEBUG_MENU;
        self.clearAiIcons(unit);
        var mode = unit.getAiMode();
        var iconId = null;
        for (var index = 0; index < self.AI_MODE_ICONS.length; ++index)
        {
            if (self.AI_MODE_ICONS[index].mode === mode)
            {
                iconId = self.AI_MODE_ICONS[index].icon;
                break;
            }
        }
        if (iconId !== null)
        {
            var size = map.getImageSize();
            unit.loadIcon(iconId, size / 2, size / 2, -1, -1);
        }
    };

    this.setAiIconsHidden = function(map, hidden)
    {
        var self = ACTION_DEBUG_MENU;
        self.iconsHidden = hidden;
        self.iconsTouched = true;
        self.refreshAiIcons(map);
        self.print("icons " + (hidden ? "HIDDEN" : "SHOWN"));
    };

    this.syncDiplomacyIconUpdater = function()
    {
        var self = ACTION_DEBUG_MENU;
        var diplomacy = self.getDiplomacy();
        if (diplomacy !== null)
        {
            if (self.iconsHidden && self.savedUpdateOrderIcon === null)
            {
                self.savedUpdateOrderIcon = diplomacy.updateOrderIcon;
                diplomacy.updateOrderIcon = function(unit, unusedMap)
                {
                    if (unit !== null)
                    {
                        ACTION_DEBUG_MENU.clearAiIcons(unit);
                    }
                };
            }
            else if (!self.iconsHidden && self.savedUpdateOrderIcon !== null)
            {
                diplomacy.updateOrderIcon = self.savedUpdateOrderIcon;
                self.savedUpdateOrderIcon = null;
            }
        }
    };

    this.refreshAiIcons = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        self.syncDiplomacyIconUpdater();
        var diplomacy = self.getDiplomacy();
        for (var p = 0; p < map.getPlayerCount(); ++p)
        {
            var units = map.getPlayer(p).getUnits();
            for (var index = 0; index < units.size(); ++index)
            {
                var unit = units.at(index);
                if (self.iconsHidden)
                {
                    self.clearAiIcons(unit);
                }
                else if (diplomacy !== null)
                {
                    diplomacy.updateOrderIcon(unit, map);
                }
                else
                {
                    self.applyAiIcon(unit, map);
                }
            }
        }
    };

    this.dumpState = function(map)
    {
        var self = ACTION_DEBUG_MENU;
        self.print("day " + map.getCurrentDay());
        for (var p = 0; p < map.getPlayerCount(); ++p)
        {
            var player = map.getPlayer(p);
            self.print("player " + p +
                       "|funds=" + player.getFunds() +
                       "|units=" + player.getUnitCount());
            var units = player.getUnits();
            for (var index = 0; index < units.size(); ++index)
            {
                var unit = units.at(index);
                self.print("p" + p + "|" +
                           unit.getUnitID() + "@" + unit.getX() + "," +
                           unit.getY() + "|hp=" + unit.getHpRounded() +
                           "|fuel=" + unit.getFuel() +
                           "|ammo1=" + unit.getAmmo1() +
                           "|ammo2=" + unit.getAmmo2() +
                           "|aiMode=" + unit.getAiMode());
            }
        }
    };

    // ------------------------------------------------------------------
    // command table

    this.commands = {};
    this.commandOrder = [];
    var owner = this;
    var define = function(id, category, textFn, steps, performFn, icon)
    {
        owner.commands[id] = {id: id, category: category, text: textFn,
                              steps: steps, perform: performFn,
                              icon: icon};
        owner.commandOrder.push(id);
    };

    var stepAllUnits = function(color)
    {
        return {type: owner.STEP_FIELD, color: color,
                points: function(map, args)
                {
                    return ACTION_DEBUG_MENU.allUnitPoints(map);
                }};
    };
    var stepAllFields = {type: owner.STEP_FIELD,
        points: function(map, args)
        {
            return ACTION_DEBUG_MENU.allFieldPoints(map);
        }};
    var stepAllPlayers = {type: owner.STEP_MENU,
        options: function(map, args)
        {
            return ACTION_DEBUG_MENU.playerOptions(map, false);
        }};
    var stepOtherPlayers = {type: owner.STEP_MENU,
        options: function(map, args)
        {
            return ACTION_DEBUG_MENU.playerOptions(map, true);
        }};
    var stepCoSlots = {type: owner.STEP_MENU,
        options: function(map, args)
        {
            return ACTION_DEBUG_MENU.coSlotOptions(map);
        }};

    define(this.CMD_INSPECT_TILE, this.CAT_INSPECT,
        function(map) { return qsTr("Inspect tile"); },
        [stepAllFields],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performInspectTile(map, args);
        });
    define(this.CMD_INSPECT_PLAYER, this.CAT_INSPECT,
        function(map) { return qsTr("Inspect current player"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performInspectPlayer(map, args);
        });
    define(this.CMD_INSPECT_AI, this.CAT_INSPECT,
        function(map) { return qsTr("Inspect AI state"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performInspectAi(map, args);
        });
    define(this.CMD_INSPECT_RULES, this.CAT_INSPECT,
        function(map) { return qsTr("Inspect victory rules"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performInspectRules(map, args);
        });
    define(this.CMD_DUMP, this.CAT_INSPECT,
        function(map) { return qsTr("Dump full map state to log"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.dumpState(map);
        });

    define(this.CMD_FUNDS_ADD, this.CAT_PLAYER,
        function(map)
        {
            return qsTr("Add ") + ACTION_DEBUG_MENU.FUNDS_ADD_AMOUNT +
                   qsTr(" funds");
        },
        [],
        function(map, args)
        {
            map.getCurrentPlayer().addFunds(
                ACTION_DEBUG_MENU.FUNDS_ADD_AMOUNT);
        });
    define(this.CMD_FUNDS_ZERO, this.CAT_PLAYER,
        function(map) { return qsTr("Set funds to 0"); },
        [],
        function(map, args)
        {
            map.getCurrentPlayer().setFunds(0);
        });
    define(this.CMD_FUNDS_RICH, this.CAT_PLAYER,
        function(map)
        {
            return qsTr("Set funds to ") +
                   ACTION_DEBUG_MENU.FUNDS_RICH_AMOUNT;
        },
        [],
        function(map, args)
        {
            map.getCurrentPlayer().setFunds(
                ACTION_DEBUG_MENU.FUNDS_RICH_AMOUNT);
        });
    define(this.CMD_CHARGE, this.CAT_PLAYER,
        function(map) { return qsTr("Set CO charge..."); },
        [{type: this.STEP_MENU, options: function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            return [
                {id: self.CHARGE_PRESET_EMPTY, text: qsTr("Empty")},
                {id: self.CHARGE_PRESET_ALMOST,
                 text: qsTr("Just below power")},
                {id: self.CHARGE_PRESET_COP, text: qsTr("Power threshold")},
                {id: self.CHARGE_PRESET_FULL,
                 text: qsTr("Full (superpower)")}];
        }}],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performCharge(map, args);
        });
    define(this.CMD_DEFEAT, this.CAT_PLAYER,
        function(map) { return qsTr("Defeat player..."); },
        [stepOtherPlayers],
        function(map, args)
        {
            var player = map.getPlayer(parseInt(args[0]));
            player.defeatPlayer(null, false);
            ACTION_DEBUG_MENU.print("defeated player " + args[0]);
        });

    define(this.CMD_HEAL, this.CAT_UNIT,
        function(map) { return qsTr("Heal + resupply my units"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.healAndResupply(map);
        });
    define(this.CMD_REFRESH_ALL, this.CAT_UNIT,
        function(map) { return qsTr("Refresh my units (move again)"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.refreshAllUnits(map);
        });
    define(this.CMD_UNIT_SET_HP, this.CAT_UNIT,
        function(map) { return qsTr("Set unit HP..."); },
        [stepAllUnits(undefined),
         {type: this.STEP_MENU, options: function(map, args)
         {
             var self = ACTION_DEBUG_MENU;
             var options = [];
             for (var i = 0; i < self.HP_PRESETS.length; ++i)
             {
                 options.push({id: String(self.HP_PRESETS[i]),
                               text: qsTr("HP ") + self.HP_PRESETS[i]});
             }
             return options;
         }}],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.setHp(parseInt(args[1]));
            }
        });
    define(this.CMD_UNIT_REFRESH, this.CAT_UNIT,
        function(map) { return qsTr("Refresh one unit"); },
        [stepAllUnits(undefined)],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.setHasMoved(false);
            }
        });
    define(this.CMD_UNIT_PROMOTE, this.CAT_UNIT,
        function(map) { return qsTr("Promote unit"); },
        [stepAllUnits(undefined)],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.setUnitRank(unit.getUnitRank() + 1, true);
            }
        });
    define(this.CMD_UNIT_KILL, this.CAT_UNIT,
        function(map) { return qsTr("Kill unit (with triggers)"); },
        [stepAllUnits(this.FIELD_COLOR_DESTROY)],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.killUnit();
            }
        });
    define(this.CMD_UNIT_DELETE, this.CAT_UNIT,
        function(map) { return qsTr("Delete unit (silent)"); },
        [stepAllUnits(this.FIELD_COLOR_DESTROY)],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.removeUnit(false);
            }
        });
    define(this.CMD_UNIT_OWNER, this.CAT_UNIT,
        function(map) { return qsTr("Change unit owner..."); },
        [stepAllUnits(undefined), stepAllPlayers],
        function(map, args)
        {
            var unit = ACTION_DEBUG_MENU.unitAt(map, args[0]);
            if (unit !== null)
            {
                unit.setOwner(map.getPlayer(parseInt(args[1])));
            }
        });
    define(this.CMD_UNIT_SPAWN, this.CAT_UNIT,
        function(map) { return qsTr("Spawn unit..."); },
        [{type: this.STEP_MENU, options: function(map, args)
         {
             var self = ACTION_DEBUG_MENU;
             var options = [];
             for (var i = 0; i < self.SPAWN_UNITS.length; ++i)
             {
                 options.push({id: self.SPAWN_UNITS[i],
                               text: self.SPAWN_UNITS[i]});
             }
             return options;
         }},
         stepAllFields],
        function(map, args)
        {
            var unit = map.spawnUnit(args[1].x, args[1].y, args[0],
                                     map.getCurrentPlayer());
            ACTION_DEBUG_MENU.print("spawn " + args[0] + " at " +
                                    args[1].x + "," + args[1].y + " -> " +
                                    (unit !== null ? "ok" : "failed"));
        });

    define(this.CMD_FOG, this.CAT_MATCH,
        function(map)
        {
            var fogOn = (map.getGameRules().getFogMode() !==
                         GameEnums.Fog_Off);
            return fogOn ? qsTr("Fog: turn off") : qsTr("Fog: turn on");
        },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.toggleFog(map);
        });
    define(this.CMD_WEATHER, this.CAT_MATCH,
        function(map) { return qsTr("Set weather..."); },
        [{type: this.STEP_MENU, options: function(map, args)
        {
            var rules = map.getGameRules();
            var options = [];
            for (var i = 0; i < rules.getWeatherCount(); ++i)
            {
                options.push({id: String(i),
                              text: rules.getWeather(i).getWeatherId()});
            }
            return options;
        }}],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performWeather(map, args);
        });

    define(this.CMD_PERK_LIST, this.CAT_PERKS,
        function(map) { return qsTr("List attached perks"); },
        [],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performPerkList(map, args);
        });
    define(this.CMD_PERK_GRANT, this.CAT_PERKS,
        function(map) { return qsTr("Grant perk..."); },
        [stepCoSlots,
         {type: this.STEP_MENU, options: function(map, args)
         {
             var options = [];
             if (typeof coPerkSpriteManager !== "undefined")
             {
                 var ids = coPerkSpriteManager.getLoadedPerks();
                 for (var i = 0; i < ids.length; ++i)
                 {
                     options.push({id: ids[i], text: ids[i]});
                 }
             }
             return options;
         }}],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performPerkChange(map, args, true);
        });
    define(this.CMD_PERK_REVOKE, this.CAT_PERKS,
        function(map) { return qsTr("Revoke perk..."); },
        [stepCoSlots,
         {type: this.STEP_MENU, options: function(map, args)
         {
             var options = [];
             var co = map.getCurrentPlayer().getCO(parseInt(args[0]));
             if (co !== null)
             {
                 var perks = co.getPerkList();
                 for (var i = 0; i < perks.length; ++i)
                 {
                     options.push({id: perks[i], text: perks[i]});
                 }
             }
             return options;
         }}],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performPerkChange(map, args, false);
        });
    define(this.CMD_PERK_BUDGET, this.CAT_PERKS,
        function(map) { return qsTr("Lift perk budget caps"); },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            var rules = map.getGameRules();
            rules.setMaxPerkCost(self.PERK_COST_LIFTED);
            rules.setMaxPerkCount(self.PERK_COUNT_LIFTED);
            self.print("perk caps lifted");
        });

    define(this.CMD_LOG_LEVEL, this.CAT_LOGGING,
        function(map) { return qsTr("Set log level..."); },
        [{type: this.STEP_MENU, options: function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            var options = [];
            for (var i = 0; i < self.LOG_LEVEL_NAMES.length; ++i)
            {
                options.push({id: String(i),
                              text: i + " " + self.LOG_LEVEL_NAMES[i]});
            }
            return options;
        }}],
        function(map, args)
        {
            // confirmation first: a raised level would swallow it
            ACTION_DEBUG_MENU.print("log level " + args[0]);
            GameConsole.setLogLevel(parseInt(args[0]));
        });
    define(this.CMD_LOG_MODULE_AI, this.CAT_LOGGING,
        function(map) { return qsTr("Toggle AI log module"); },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            var enabled = !GameConsole.isActiveModule(self.MODULE_AI_BIT);
            self.print("eAI module " + (enabled ? "ON" : "OFF"));
            GameConsole.setModuleMode(self.MODULE_AI_BIT, enabled);
        });
    define(this.CMD_LOG_MODULE_JS, this.CAT_LOGGING,
        function(map) { return qsTr("Toggle JS log module"); },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            var enabled = !GameConsole.isActiveModule(self.MODULE_JS_BIT);
            // confirmation first: disabling eJavaScript swallows print
            self.print("eJavaScript module " + (enabled ? "ON" : "OFF"));
            GameConsole.setModuleMode(self.MODULE_JS_BIT, enabled);
        });
    define(this.CMD_MOD_FLAGS, this.CAT_LOGGING,
        function(map) { return qsTr("Toggle mod debug flag..."); },
        [{type: this.STEP_MENU, options: function(map, args)
        {
            return ACTION_DEBUG_MENU.modFlagOptions(map);
        }}],
        function(map, args)
        {
            ACTION_DEBUG_MENU.performModFlag(map, args);
        });

    define(this.CMD_KEEP_POWER, this.CAT_TOGGLES,
        function(map)
        {
            var self = ACTION_DEBUG_MENU;
            return self.keepToggleText(self.STATE_KEEP_POWER,
                qsTr("Keep power full: turn off"),
                qsTr("Keep power full: turn on"));
        },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            self.performKeepToggle(map, self.STATE_KEEP_POWER);
        });
    define(this.CMD_KEEP_FUNDS, this.CAT_TOGGLES,
        function(map)
        {
            var self = ACTION_DEBUG_MENU;
            return self.keepToggleText(self.STATE_KEEP_FUNDS,
                qsTr("Keep funds topped up: turn off"),
                qsTr("Keep funds topped up: turn on"));
        },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            self.performKeepToggle(map, self.STATE_KEEP_FUNDS);
        });
    define(this.CMD_ICONS, this.CAT_TOGGLES,
        function(map)
        {
            return ACTION_DEBUG_MENU.iconsHidden ?
                qsTr("AI-mode icons: show") : qsTr("AI-mode icons: hide");
        },
        [],
        function(map, args)
        {
            var self = ACTION_DEBUG_MENU;
            self.setAiIconsHidden(map, !self.iconsHidden);
        });
    define(this.CMD_DEVELOPER_MODE, this.CAT_TOGGLES,
        function(map)
        {
            return GameConsole.getDeveloperMode() ?
                qsTr("Developer mode: turn off") :
                qsTr("Developer mode: turn on");
        },
        [],
        function(map, args)
        {
            GameConsole.setDeveloperMode(!GameConsole.getDeveloperMode());
        });

    this.getName = function()
    {
        return qsTr("Debug menu");
    };
    this.getDescription = function()
    {
        return qsTr("Debug tools for modders and map makers: inspect hidden script variables, set CO charge and funds, manipulate units, control weather and fog, grant perks, and flip logging switches at runtime. Other mods can extend the menu through the DEBUG_MENU registry. Single-player debug use only.");
    };
};

// list join helper shared by the dump paths, QStringList and JS arrays both
function self_joinList(list)
{
    var parts = [];
    if (list !== null && list !== undefined)
    {
        if (typeof list.length === "number")
        {
            for (var i = 0; i < list.length; ++i)
            {
                parts.push(String(list[i]));
            }
        }
        else if (typeof list.size === "function")
        {
            for (var i2 = 0; i2 < list.size(); ++i2)
            {
                parts.push(String(list.at(i2)));
            }
        }
    }
    return parts.join(",");
}

Constructor.prototype = ACTION;
var ACTION_DEBUG_MENU = new Constructor();
// action scripts load after base actions exist, so the turn-change hook can
// attach here even when a later mod has replaced the PLAYER object
if (typeof DEBUG_MENU_INSTALL !== "undefined")
{
    DEBUG_MENU_INSTALL.installNextPlayerHook();
}
