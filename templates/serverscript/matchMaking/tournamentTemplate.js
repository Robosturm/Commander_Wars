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
                tournament.players.forEach(function(player)
                {
                    if (processedPlayers.indexOf(player) < 0)
                    {
                        processedPlayers.push(player);
                    }
                });
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
        if (!bracket.setupSeededOrder(names, ratings, !!config.thirdPlaceMatch))
        {
            throw new Error("Unable to create tournament bracket: " + bracket.getLastError());
        }
        var tournamentId = autoMatchMaker.getMatchId() + "-" + state.nextId++;
        state.tournaments.push({
            id: tournamentId,
            players: names,
            pending: [],
            bracket: bracket.save(),
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
            if (!bracket.load(tournament.bracket))
            {
                throw new Error("Unable to load tournament " + tournament.id + ": " + bracket.getLastError());
            }
            var ready = JSON.parse(bracket.readyMatchesJson());
            ready.forEach(function(match)
            {
                var alreadyPending = tournament.pending.some(function(item)
                {
                    return item.matchId === match.matchId;
                });
                if (alreadyPending)
                {
                    return;
                }
                AUTOMATCHMAKER.currentRound = match.round;
                var players = [match.player1, match.player2];
                if (autoMatchMaker.createNewGame(players, config.mods || []))
                {
                    tournament.pending.push({ matchId: match.matchId, players: players });
                }
            });
            tournament.bracket = bracket.save();
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
                return item.players.length === 2 &&
                       usernames.indexOf(item.players[0]) >= 0 &&
                       usernames.indexOf(item.players[1]) >= 0;
            });
            if (pendingIndex < 0)
            {
                return;
            }
            var pending = tournament.pending[pendingIndex];
            if (!bracket.load(tournament.bracket))
            {
                throw new Error("Unable to load tournament " + tournament.id + ": " + bracket.getLastError());
            }
            var firstResult = results[usernames.indexOf(pending.players[0])];
            var secondResult = results[usernames.indexOf(pending.players[1])];
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
            if (usernames.length === 2 && results.length === 2)
            {
                autoMatchMaker.updateMmr(pending.players[0], pending.players[1],
                                         config.maxMmrChange,
                                         results[usernames.indexOf(pending.players[0])]);
            }
            if (!bracket.reportResult(pending.matchId, winner))
            {
                throw new Error("Unable to apply result to tournament " + tournament.id +
                                ": " + bracket.getLastError());
            }
            tournament.pending.splice(pendingIndex, 1);
            tournament.bracket = bracket.save();
            if (bracket.isComplete())
            {
                tournament.complete = true;
                if (!autoMatchMaker.recordTournamentResults(tournament.id, bracket.placementsJson()))
                {
                    throw new Error("Unable to persist final placements for tournament " + tournament.id);
                }
            }
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
                    bracket: JSON.parse(tournament.bracket)
                };
            })
        });
    }
};
