var ForgotPasswordDialog =
{
    showCodeInput : function()
    {
        var variables = forgotPassword.getVariables();
        var showVariable = variables.createVariable("showCodeInput");
        var show = showVariable.readDataBool();
        return show === true;
    },
    emailAdress : function(input)
    {
        var variables = forgotPassword.getVariables();
        var emailVariable = variables.createVariable("email");
        emailVariable.writeDataString(input);
    },
    codeInput : function(input)
    {
        var variables = forgotPassword.getVariables();
        var codeVariable = variables.createVariable("code");
        codeVariable.writeDataString(input);
    },
    resetPassword : function()
    {
        var variables = forgotPassword.getVariables();
        var email = variables.createVariable("email").readDataString();
        var menu = forgotPassword.getBaseMenu();
        if (menu.isValidEmailAdress(email) === false)
        {
            forgotPassword.showMessageBox(qsTr("You entered an invalid email address."));
        }
        else
        {
            ForgotPasswordDialog.changeEnableForItems(false);
            menu.resetPasswordOnServerAccount(email);
        }
    },
    submitCode : function()
    {
        var variables = forgotPassword.getVariables();
        var code = variables.createVariable("code").readDataString();
        if (code.length !== 6)
        {
            forgotPassword.showMessageBox(qsTr("Please enter the 6 digit code shown by your authenticator app."));
        }
        else
        {
            ForgotPasswordDialog.changeEnableForItems(false);
            var menu = forgotPassword.getBaseMenu();
            menu.submitPasswordResetCode(code);
        }
    },
    abort : function()
    {
        var menu = forgotPassword.getBaseMenu();
        menu.cancelServer2fa();
        forgotPassword.exit();
    },
    onAccountMessage : function(errorCode)
    {
        if (errorCode === GameEnums.LoginError_None)
        {
            forgotPassword.showMessageBox(qsTr("An e-mail with a new password was sent to your e-mail account."));
            forgotPassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_InvalidPasswordReset)
        {
            forgotPassword.showMessageBox(qsTr("Invalid e-mail address which is linked to your account."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
        else if (errorCode === GameEnums.LoginError_SendingMailFailed)
        {
            forgotPassword.showMessageBox(qsTr("Password has been reset, but the server failed to send a mail with the new password."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
        else
        {
            forgotPassword.showMessageBox(qsTr("Unknown error happened."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
    },
    on2faMessage : function(errorCode, secret, url, newPassword)
    {
        if (errorCode === GameEnums.LoginError_None && newPassword.length === 0)
        {
            // server requests the totp code: reveal the code input
            var variables = forgotPassword.getVariables();
            variables.createVariable("showCodeInput").writeDataBool(true);
            forgotPassword.refreshUi();
            forgotPassword.showMessageBox(qsTr("Enter the current 6 digit code shown by your authenticator app."));
            forgotPassword.setObjectEnabled("CodeBox", true);
            forgotPassword.setObjectEnabled("SubmitCodeButton", true);
            forgotPassword.setObjectEnabled("AbortButton", true);
        }
        else if (errorCode === GameEnums.LoginError_None && newPassword.length > 0)
        {
            forgotPassword.showMessageBox(qsTr("Your password has been reset. Your new password is: ") + newPassword +
                                          qsTr(" Log in and change your password immediately."));
            forgotPassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_Invalid2faCode)
        {
            forgotPassword.showMessageBox(qsTr("The entered code is invalid. Please check the time of your device and try again."));
            forgotPassword.setObjectEnabled("CodeBox", true);
            forgotPassword.setObjectEnabled("SubmitCodeButton", true);
            forgotPassword.setObjectEnabled("AbortButton", true);
        }
        else if (errorCode === GameEnums.LoginError_2faResetTimeout)
        {
            forgotPassword.showMessageBox(qsTr("The password reset timed out. Please start it again."));
            forgotPassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_TooMany2faAttempts)
        {
            forgotPassword.showMessageBox(qsTr("Too many invalid codes entered. The password reset was canceled."));
            forgotPassword.exit();
        }
        else if (errorCode === GameEnums.LoginError_No2faConfigured)
        {
            forgotPassword.showMessageBox(qsTr("This account has no 2 factor authentication configured and the server cannot send e-mails. The password can not be reset. Please contact a server admin."));
            forgotPassword.exit();
        }
        else
        {
            forgotPassword.showMessageBox(qsTr("Unknown error happened."));
            ForgotPasswordDialog.changeEnableForItems(true);
        }
    },
    changeEnableForItems : function(value)
    {
        forgotPassword.setObjectEnabled("EmailBox", value);
        forgotPassword.setObjectEnabled("AbortButton", value);
        forgotPassword.setObjectEnabled("ResetButton", value);
    },
};
