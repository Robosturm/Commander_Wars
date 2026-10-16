var AUTOMATCHMAKER =
{
    jsonFileName: "server/tournament.json",
    stateVariableName: "TournamentState",
    currentRound: 0,

    getConfiguration: function(autoMatchMaker)
    {
        var data = autoMatchMaker.readDataFromJson(AUTOMATCHMAKER.jsonFileName);
        if (!data)
        {
            throw new Error("Unable to read " + AUTOMATCHMAKER.jsonFileName);
        }
        return JSON.parse(data);
    },

    getStateData: function(autoMatchMaker)
    {
        var variable = autoMatchMaker.getVariables().createVariable(AUTOMATCHMAKER.stateVariableName);
        var data = variable.readDataString();
        if (!data)
        {
            var persisted = JSON.parse(autoMatchMaker.getTournamentRecords());
            if (!persisted.ok)
            {
                throw new Error("Unable to restore tournaments from the database: " + persisted.error);
            }
            var tournaments = persisted.records;
            var processedPlayers = [];
            tournaments.forEach(function(tournament)
            {
                if (!tournament || !Array.isArray(tournament.players) ||
                    !Array.isArray(tournament.pending) || typeof tournament.bracket !== "string")
                {
                    throw new Error("Persisted tournament record is incomplete");
                }
                if (!tournament.complete)
                {
                    tournament.players.forEach(function(player)
                    {
                        if (processedPlayers.indexOf(player) < 0)
                        {
                            processedPlayers.push(player);
                        }
                    });
                }
            });
            return {
                nextId: tournaments.length + 1,
                processedPlayers: processedPlayers,
                tournaments: tournaments
            };
        }
        try
        {
            var state = JSON.parse(data);
            if (!state || !Array.isArray(state.processedPlayers) || !Array.isArray(state.tournaments))
            {
                throw new Error("Invalid persisted tournament state");
            }
            state.processedPlayers = [];
            state.tournaments.forEach(function(tournament)
            {
                if (!tournament || !Array.isArray(tournament.players) ||
                    !Array.isArray(tournament.pending) || typeof tournament.complete !== "boolean")
                {
                    throw new Error("Persisted tournament state contains an incomplete tournament");
                }
                if (!tournament.complete)
                {
                    tournament.players.forEach(function(player)
                    {
                        if (state.processedPlayers.indexOf(player) < 0)
                        {
                            state.processedPlayers.push(player);
                        }
                    });
                }
            });
            return state;
        }
        catch (error)
        {
            throw new Error("Unable to restore tournament state: " + error);
        }
    },

    saveStateData: function(autoMatchMaker, state)
    {
        state.tournaments.forEach(function(tournament)
        {
            if (!autoMatchMaker.saveTournamentRecord(tournament.id, JSON.stringify(tournament),
                                                     tournament.complete))
            {
                throw new Error("Unable to persist tournament state for " + tournament.id);
            }
        });
        autoMatchMaker.getVariables().createVariable(AUTOMATCHMAKER.stateVariableName)
            .writeDataString(JSON.stringify(state));
    },

    getName: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).name;
    },

    getDescription: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).description;
    },

    getStartMmr: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).startMmr;
    },

    getState: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).state || "SignUp";
    },

    getIsSignUpChangeAllowed: function(autoMatchMaker)
    {
        return AUTOMATCHMAKER.getConfiguration(autoMatchMaker).isSignUpChangeAllowed;
    },

    onCreateNewGame: function(autoMatchMaker, multiplayerMenu, players)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        var pools = config.mapPoolsByRound || [];
        var pool = pools[Math.min(AUTOMATCHMAKER.currentRound, pools.length - 1)];
        if (!pool || pool.length === 0)
        {
            throw new Error("No map pool is configured for tournament round " + AUTOMATCHMAKER.currentRound);
        }
        var map = pool[Math.floor(Math.random() * pool.length)];
        multiplayerMenu.selectMap(map.folder, map.filename);
        if (config.ruleFile)
        {
            multiplayerMenu.loadRules(config.ruleFile);
        }
        var currentMap = multiplayerMenu.getCurrentMap();
        if (!currentMap || currentMap.getPlayerCount() < 2 || players.length !== 2)
        {
            throw new Error("Tournament matches require a valid map with at least two slots and two players");
        }
        var playerSelection = multiplayerMenu.getPlayerSelection();
        playerSelection.setMap(currentMap);
        for (var i = 0; i < currentMap.getPlayerCount(); ++i)
        {
            if (i < players.length)
            {
                var team = config.teams && config.teams.length > i ? config.teams[i] : i;
                if (!multiplayerMenu.assignPlayerToUser(i, players[i], team))
                {
                    throw new Error("Unable to assign tournament player " + players[i]);
                }
            }
            else
            {
                playerSelection.forcePlayerAi(i, config.aiType === undefined ? 2 : config.aiType);
                currentMap.getPlayer(i).setTeam(i);
            }
        }
    },

    createTournament: function(autoMatchMaker, state, players, config)
    {
        var rankedPlayers = players.map(function(name)
        {
            return { name: name, rating: autoMatchMaker.getMmr(name) };
        });
        if (rankedPlayers.some(function(player) { return player.rating < 0; }))
        {
            throw new Error("Could not load MMR for every tournament entrant");
        }
        rankedPlayers.sort(function(left, right) { return right.rating - left.rating; });
        var seededCount = Math.ceil(rankedPlayers.length / 2);
        var seeded = rankedPlayers.slice(0, seededCount);
        var unseeded = rankedPlayers.slice(seededCount);
        for (var i = unseeded.length - 1; i > 0; --i)
        {
            var j = Math.floor(Math.random() * (i + 1));
            var swap = unseeded[i];
            unseeded[i] = unseeded[j];
            unseeded[j] = swap;
        }
        var ordered = seeded.concat(unseeded);
        var names = ordered.map(function(player) { return player.name; });
        var ratings = ordered.map(function(player) { return player.rating; });
        var bracket = autoMatchMaker.createTournamentBracket();
        var useGroups = config.groupStage && config.groupStage.enabled;
        var groupStage = "";
        if (useGroups)
        {
            if (!bracket.setupGroupStage(names, ratings,
                                         Math.max(2, config.groupStage.groupSize || 4),
                                         Math.max(1, config.groupStage.advanceCount || 2)))
            {
                throw new Error("Unable to create tournament groups: " + bracket.getLastError());
            }
            groupStage = bracket.saveGroupStage();
        }
        else if (!bracket.setupSeededOrder(names, ratings, !!config.thirdPlaceMatch))
        {
            throw new Error("Unable to create tournament bracket: " + bracket.getLastError());
        }
        var tournamentId = autoMatchMaker.getMatchId() + "-" + state.nextId++;
        state.tournaments.push({
            id: tournamentId,
            players: names,
            pending: [],
            stage: useGroups ? "groups" : "knockout",
            groupStage: groupStage,
            bracket: useGroups ? "" : bracket.save(),
            complete: false
        });
        for (var p = 0; p < names.length; ++p)
        {
            if (state.processedPlayers.indexOf(names[p]) < 0)
            {
                state.processedPlayers.push(names[p]);
            }
        }
    },

    onCreateNewGames: function(autoMatchMaker)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        var state = AUTOMATCHMAKER.getStateData(autoMatchMaker);
        var activeTournaments = state.tournaments.filter(function(tournament)
        {
            return !tournament.complete;
        }).length;
        var maximumConcurrent = Math.max(1, config.maxConcurrentTournaments || 1);
        var maximumEntrants = Math.max(2, config.maxEntrantsPerTournament || 16);
        var minimumEntrants = Math.max(2, Math.min(maximumEntrants, config.minimumEntrants || 2));
        var available = autoMatchMaker.getSignedUpPlayers().filter(function(player)
        {
            return state.processedPlayers.indexOf(player) < 0;
        });

        while (activeTournaments < maximumConcurrent && available.length >= minimumEntrants)
        {
            var entrants = available.splice(0, Math.min(maximumEntrants, available.length));
            AUTOMATCHMAKER.createTournament(autoMatchMaker, state, entrants, config);
            ++activeTournaments;
        }

        var bracket = autoMatchMaker.createTournamentBracket();
        state.tournaments.forEach(function(tournament)
        {
            if (tournament.complete)
            {
                return;
            }
            var inGroups = tournament.stage === "groups";
            var ready;
            if (inGroups)
            {
                if (!bracket.loadGroupStage(tournament.groupStage))
                {
                    throw new Error("Unable to load groups for tournament " + tournament.id +
                                    ": " + bracket.getLastError());
                }
                ready = JSON.parse(bracket.readyGroupMatchesJson());
            }
            else
            {
                if (!bracket.load(tournament.bracket))
                {
                    throw new Error("Unable to load tournament " + tournament.id + ": " + bracket.getLastError());
                }
                ready = JSON.parse(bracket.readyMatchesJson());
            }
            ready.forEach(function(match)
            {
                var alreadyPending = tournament.pending.some(function(item)
                {
                    return (item.type || "knockout") === (inGroups ? "group" : "knockout") &&
                           item.matchId === match.matchId;
                });
                if (alreadyPending)
                {
                    return;
                }
                AUTOMATCHMAKER.currentRound = match.round;
                var players = [match.player1, match.player2];
                if (autoMatchMaker.createNewGame(players, config.mods || []))
                {
                    tournament.pending.push({
                        type: inGroups ? "group" : "knockout",
                        matchId: match.matchId,
                        players: players
                    });
                }
            });
            if (inGroups)
            {
                tournament.groupStage = bracket.saveGroupStage();
            }
            else
            {
                tournament.bracket = bracket.save();
            }
        });
        AUTOMATCHMAKER.saveStateData(autoMatchMaker, state);
    },

    onNewMatchResultData: function(autoMatchMaker, usernames, results)
    {
        var config = AUTOMATCHMAKER.getConfiguration(autoMatchMaker);
        var state = AUTOMATCHMAKER.getStateData(autoMatchMaker);
        var bracket = autoMatchMaker.createTournamentBracket();
        state.tournaments.forEach(function(tournament)
        {
            if (tournament.complete)
            {
                return;
            }
            var pendingIndex = tournament.pending.findIndex(function(item)
            {
                return (item.type || "knockout") === (tournament.stage === "groups" ? "group" : "knockout") &&
                       item.players.length === 2 &&
                       usernames.indexOf(item.players[0]) >= 0 &&
                       usernames.indexOf(item.players[1]) >= 0;
            });
            if (pendingIndex < 0)
            {
                return;
            }
            var pending = tournament.pending[pendingIndex];
            var firstResult = results[usernames.indexOf(pending.players[0])];
            var secondResult = results[usernames.indexOf(pending.players[1])];
            if (pending.type === "group")
            {
                if (!bracket.loadGroupStage(tournament.groupStage))
                {
                    throw new Error("Unable to load groups for tournament " + tournament.id +
                                    ": " + bracket.getLastError());
                }
                if (!bracket.reportGroupResult(pending.matchId, firstResult, secondResult))
                {
                    throw new Error("Unable to apply group result to tournament " + tournament.id +
                                    ": " + bracket.getLastError());
                }
                tournament.groupStage = bracket.saveGroupStage();
                if (bracket.isGroupStageComplete())
                {
                    var qualifiers = JSON.parse(bracket.groupStageAdvancersJson());
                    qualifiers.sort(function(left, right) { return right.rating - left.rating; });
                    var seededCount = Math.ceil(qualifiers.length / 2);
                    var seededQualifiers = qualifiers.slice(0, seededCount);
                    var unseededQualifiers = qualifiers.slice(seededCount);
                    for (var i = unseededQualifiers.length - 1; i > 0; --i)
                    {
                        var j = Math.floor(Math.random() * (i + 1));
                        var swap = unseededQualifiers[i];
                        unseededQualifiers[i] = unseededQualifiers[j];
                        unseededQualifiers[j] = swap;
                    }
                    qualifiers = seededQualifiers.concat(unseededQualifiers);
                    if (!bracket.setupSeededOrder(
                        qualifiers.map(function(player) { return player.name; }),
                        qualifiers.map(function(player) { return player.rating; }),
                        !!config.thirdPlaceMatch))
                    {
                        throw new Error("Unable to create knockout bracket for tournament " + tournament.id +
                                        ": " + bracket.getLastError());
                    }
                    tournament.bracket = bracket.save();
                    tournament.stage = "knockout";
                }
            }
            else
            {
                if (!bracket.load(tournament.bracket))
                {
                    throw new Error("Unable to load tournament " + tournament.id + ": " + bracket.getLastError());
                }
                var winner;
                if (firstResult === 2)
                {
                    winner = pending.players[0];
                }
                else if (secondResult === 2)
                {
                    winner = pending.players[1];
                }
                else
                {
                    winner = autoMatchMaker.getMmr(pending.players[0]) >=
                             autoMatchMaker.getMmr(pending.players[1]) ?
                             pending.players[0] : pending.players[1];
                }
                if (!bracket.reportResult(pending.matchId, winner))
                {
                    throw new Error("Unable to apply result to tournament " + tournament.id +
                                    ": " + bracket.getLastError());
                }
                tournament.bracket = bracket.save();
                if (bracket.isComplete())
                {
                    tournament.complete = true;
                    var placements = JSON.parse(bracket.placementsJson());
                    if (tournament.groupStage)
                    {
                        if (!bracket.loadGroupStage(tournament.groupStage))
                        {
                            throw new Error("Unable to load completed groups for tournament " + tournament.id +
                                            ": " + bracket.getLastError());
                        }
                        var advancementCount = Math.max(1, config.groupStage.advanceCount || 2);
                        var placedPlayers = placements.map(function(placement) { return placement.name; });
                        var lastKnockoutPlace = placements.reduce(function(lastPlace, placement)
                        {
                            return Math.max(lastPlace, placement.place);
                        }, 0);
                        JSON.parse(bracket.groupStageStandingsJson()).forEach(function(standing)
                        {
                            if (placedPlayers.indexOf(standing.name) < 0)
                            {
                                placements.push({
                                    name: standing.name,
                                    rating: standing.rating,
                                    place: lastKnockoutPlace + standing.place - advancementCount
                                });
                            }
                        });
                    }
                    if (!autoMatchMaker.recordTournamentResults(tournament.id, JSON.stringify(placements)))
                    {
                        throw new Error("Unable to persist final placements for tournament " + tournament.id);
                    }
                }
            }
            if (usernames.length === 2 && results.length === 2)
            {
                autoMatchMaker.updateMmr(pending.players[0], pending.players[1],
                                         config.maxMmrChange, firstResult);
            }
            tournament.pending.splice(pendingIndex, 1);
        });
        AUTOMATCHMAKER.saveStateData(autoMatchMaker, state);
    },

    onNewPlayerData: function(autoMatchMaker, player)
    {
        var state = AUTOMATCHMAKER.getStateData(autoMatchMaker);
        var isStillPlaying = state.tournaments.some(function(tournament)
        {
            return !tournament.complete && tournament.players.indexOf(player) >= 0;
        });
        if (!isStillPlaying)
        {
            state.processedPlayers = state.processedPlayers.filter(function(name)
            {
                return name !== player;
            });
            AUTOMATCHMAKER.saveStateData(autoMatchMaker, state);
        }
    },

    getBracketGraphInfo: function(autoMatchMaker)
    {
        var state = AUTOMATCHMAKER.getStateData(autoMatchMaker);
        return JSON.stringify({
            tournaments: state.tournaments.map(function(tournament)
            {
                return {
                    id: tournament.id,
                    complete: tournament.complete,
                    stage: tournament.stage || "knockout",
                    groupStage: tournament.groupStage ? JSON.parse(tournament.groupStage) : null,
                    bracket: tournament.bracket ? JSON.parse(tournament.bracket) : null
                };
            })
        });
    }
};
