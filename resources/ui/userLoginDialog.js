var UserLoginDialog =
{
    primaryPassword : function(input)
    {
        var variables = userLogin.getVariables();
        var passwordVariable = variables.createVariable("password");
        passwordVariable.writeDataString(input);
    },
    login : function()
    {
        var variables = userLogin.getVariables();
        var passwordVariable = variables.createVariable("password");
        var password =  passwordVariable.readDataString();
        var menu = userLogin.getBaseMenu();
        UserLoginDialog.changeEnableForItems(false);
        menu.loginToServerAccount(password);
        settings.setServerPassword(password);
    },
    onAccountMessage : function(errorCode)
    {
        var menu = userLogin.getBaseMenu();
        if (errorCode === GameEnums.LoginError_None)
        {
            userLogin.showMessageBox(qsTr("Logged onto the server."));
            menu.enableServerButtons(true);
            userLogin.exit();
        }
        else if (errorCode === GameEnums.LoginError_WrongPassword)
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("Your password isn't correct."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_AccountDoesntExist)
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("No account with your username was found."));
            UserLoginDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_PasswordOutdated)
        {
            settings.setServerPassword("");
            menu.setServerRequestNewPassword(true);
            userLogin.createDialog("changePassword", "ui/changePasswordDialog.xml", menu);
            userLogin.exit();
        }
        else
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("Unknown error happened."));
            UserLoginDialog.changeEnableForItems(true);
        }
    },
    changeEnableForItems : function(value)
    {
        userLogin.setObjectEnabled("PasswordBox", value);
        userLogin.setObjectEnabled("CreateAccountButton", value);
        userLogin.setObjectEnabled("LoginButton", value);
        userLogin.setObjectEnabled("ForgotPasswordButton", value);
        userLogin.setObjectEnabled("LocalButton", value);
    },
    exit : function()
    {
        userLogin.getBaseMenu().leaveServer();
        userLogin.exit();
    },
};
