var LobbyMenu =
{
    SmallContentWidth : 1024,
    ModeButtonWidth : 200,
    ActionButtonWidth : 220,
    TableY : 60,
    PaginationHeight : 50,
    ChatHeight : 140,
    BottomAreaHeight : 110,

    getTableHeaders : function()
    {
        return [qsTr("Map"), qsTr("Players"), qsTr("Description"), qsTr("Mods"), qsTr("Locked")];
    },

    getTableWidths : function(totalWidth)
    {
        var first = Math.floor(totalWidth * 0.25);
        var second = Math.floor(totalWidth * 0.125);
        var third = Math.floor(totalWidth * 0.25);
        var fourth = Math.floor(totalWidth * 0.25);
        var fifth = totalWidth - first - second - third - fourth;
        return [first, second, third, fourth, fifth];
    },

    getSmallModeX : function(column)
    {
        return 10 + column * (LobbyMenu.ModeButtonWidth + 10);
    },

    getSmallActionX : function(column)
    {
        return 10 + column * (LobbyMenu.ActionButtonWidth + 20);
    },

    getSmallTableWidth : function()
    {
        return LobbyMenu.SmallContentWidth - 58;
    },

    getSmallChatWidth : function()
    {
        return LobbyMenu.SmallContentWidth - 50;
    },

    getDesktopTableHeight : function()
    {
        return Math.max(160, settings.getStageHeight() - LobbyMenu.TableY - LobbyMenu.PaginationHeight - LobbyMenu.ChatHeight - LobbyMenu.BottomAreaHeight);
    },

    getDesktopPaginationY : function()
    {
        return LobbyMenu.TableY + LobbyMenu.getDesktopTableHeight() + 10;
    },

    getDesktopChatY : function()
    {
        return LobbyMenu.getDesktopPaginationY() + LobbyMenu.PaginationHeight;
    },

    getDesktopChatHeight : function()
    {
        return Math.max(100, settings.getStageHeight() - LobbyMenu.getDesktopChatY() - LobbyMenu.BottomAreaHeight);
    },

    getChatItem : function(width, height)
    {
        return "CHAT:" + Math.floor(width) + ":" + Math.floor(height);
    }
};
