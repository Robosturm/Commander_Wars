var Setup2faDialog =
{
    titleText : function()
    {
        return qsTr("Setup 2 factor authentication");
    },
    infoText : function()
    {
        return qsTr("Optional: Scan the qr code with an authenticator app (e.g. Aegis, 2FAS, FreeOTP+, Google Authenticator, Microsoft Authenticator, Bitwarden or KeePassXC) or enter the shown secret manually in the app. Afterwards enter the 6 digit code shown by the app to activate the 2 factor authentication. The 2 factor authentication allows you to reset your password without an e-mail account.");
    },
    secretText : function()
    {
        return qsTr("Secret:");
    },
    secretTooltip : function()
    {
        return qsTr("The secret key for the manual entry in your authenticator app.");
    },
    codeText : function()
    {
        return qsTr("Code:");
    },
    codeTooltip : function()
    {
        return qsTr("The current 6 digit code shown by your authenticator app.");
    },
    abortText : function()
    {
        return qsTr("Abort");
    },
    abortTooltip : function()
    {
        return qsTr("Aborts the 2 factor authentication setup.");
    },
    confirmText : function()
    {
        return qsTr("Activate");
    },
    confirmTooltip : function()
    {
        return qsTr("Activates the 2 factor authentication for your account.");
    },

    loaded : function()
    {
        Setup2faDialog.changeEnableForItems(true);
    },
    codeInput : function(input)
    {
        var variables = setup2fa.getVariables();
        var codeVariable = variables.createVariable("code");
        codeVariable.writeDataString(input);
    },
    confirm : function()
    {
        var variables = setup2fa.getVariables();
        var code = variables.createVariable("code").readDataString();
        if (code.length !== 6)
        {
            setup2fa.showMessageBox(qsTr("Please enter the 6 digit code shown by your authenticator app."));
        }
        else
        {
            Setup2faDialog.changeEnableForItems(false);
            var menu = setup2fa.getBaseMenu();
            menu.confirmServer2fa(code);
        }
    },
    abort : function()
    {
        var menu = setup2fa.getBaseMenu();
        menu.cancelServer2fa();
        setup2fa.exit();
    },
    on2faMessage : function(errorCode, secret, url, newPassword)
    {
        if (url.length > 0)
        {
            // server sent the enrollment data: show the qr code and the secret
            setup2fa.getObject("QrCodeBox").setQrData(url);
            setup2fa.getObject("SecretBox").setCurrentText(secret);
        }
        else if (errorCode === GameEnums.LoginError_None)
        {
            setup2fa.showMessageBox(qsTr("The 2 factor authentication is now active for your account."));
            setup2fa.exit();
        }
        else if (errorCode === GameEnums.LoginError_Invalid2faCode)
        {
            setup2fa.showMessageBox(qsTr("The entered code is invalid. Please check the time of your device and try again."));
            Setup2faDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_2faAlreadyConfigured)
        {
            setup2fa.showMessageBox(qsTr("A 2 factor authentication is already active for your account."));
            setup2fa.exit();
        }
        else if (errorCode === GameEnums.LoginError_2faSetupExpired)
        {
            setup2fa.showMessageBox(qsTr("The 2 factor authentication setup timed out. Please start it again."));
            setup2fa.exit();
        }
        else
        {
            setup2fa.showMessageBox(qsTr("Unknown error happened."));
            Setup2faDialog.changeEnableForItems(true);
        }
    },
    changeEnableForItems : function(value)
    {
        setup2fa.setObjectEnabled("CodeBox", value);
        setup2fa.setObjectEnabled("AbortButton", value);
        setup2fa.setObjectEnabled("ConfirmButton", value);
    },
};
