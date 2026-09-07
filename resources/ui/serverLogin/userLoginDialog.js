var UserLoginDialog =
{
    showCodeInput : function()
    {
        var variables = forgotPassword.getVariables();
        var showVariable = variables.createVariable("showCodeInput");
        var show = showVariable.readDataBool();
        return show === true;
    },
    createAccountText : function()
    {
        return qsTr("Create account");
    },
    createAccountTooltipText : function()
    {
        return qsTr("Creates a new account on the server.");
    },

    loginText : function()
    {
        return qsTr("Login");
    },
    loginTooltipText : function()
    {
        return qsTr("Logs you onto the server with the given userdata.");
    },
    forgotPassword : function()
    {
        return qsTr("Reset password");
    },
    forgotPasswordTooltip : function()
    {
        return qsTr("Allows you to reset your password.");
    },
    back : function()
    {
        return qsTr("Back");
    },
    backTooltip : function()
    {
        return qsTr("Goes back to the main menu.");
    },
    local : function()
    {
        return qsTr("Local");
    },
    localTooltip : function()
    {
        return qsTr("Play a game in your local network.");
    },
    changePassword : function()
    {
        return qsTr("Change password");
    },
    changePasswordTooltip : function()
    {
        return qsTr("Allows you to change your password.");
    },
    deleteText : function()
    {
        return qsTr("Delete Account");
    },
    deleteTooltip : function()
    {
        return qsTr("Let's you delete your account on the servers.");
    },
    setup2fa : function()
    {
        return qsTr("Setup 2FA");
    },
    setup2faTooltip : function()
    {
        return qsTr("Sets up an optional 2 factor authentication for your account which allows you to reset your password.");
    },
    openSetup2fa : function()
    {
        var menu = userLogin.getBaseMenu();
        userLogin.createDialog("setup2fa", "ui/serverLogin/setup2faDialog.xml", menu);
        menu.requestServer2faSetup();
    },
    loginOnServerText : function()
    {
        return qsTr("Login on Server");
    },
    usernameText : function()
    {
        return qsTr("Username:");
    },
    passwordText : function()
    {
        return qsTr("Password:");
    },
    passwordHelpText : function()
    {
        return qsTr("Password of your account.");
    },
    continueText : function()
    {
        return qsTr("Continue");
    },
    continueTooltip : function()
    {
        return qsTr("Continue and closing this dialog.");
    },

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
    onAccountMessage : function(errorCode, has2fa)
    {
        var menu = userLogin.getBaseMenu();
        if (errorCode === GameEnums.LoginError_None)
        {
            if (has2fa === false)
            {
                userLogin.showMessageBox(qsTr("Logged onto the server.\nYour account has no 2 factor authentication. Without it your password can not be reset in case you forget it. You can set it up with the 'Setup 2FA' button."));
            }
            else
            {
                userLogin.showMessageBox(qsTr("Logged onto the server."));
            }
            menu.onLogin();
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
            userLogin.createDialog("changePassword", "ui/serverLogin/changePasswordDialog.xml", menu);
            userLogin.exit();
        }
        else if (errorCode === GameEnums.LoginError_LoginLockedDueToTooManyFailedAttempts)
        {
            settings.setServerPassword("");
            userLogin.showMessageBox(qsTr("Your account is locked due to too many failed login attempts. Please try again later."));
            UserLoginDialog.changeEnableForItems(true);
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
        userLogin.setObjectEnabled("LocalButton", value);
        userLogin.setObjectEnabled("Setup2faButton", !value);
        userLogin.setObjectEnabled("ContinueButton", !value);
    },
    exit : function()
    {
        userLogin.getBaseMenu().leaveServer();
        userLogin.exit();
    },
};
