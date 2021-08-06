#pragma once

#include <QString>
#include <atomic>

/**main oxygine namespace*/
namespace oxygine
{
    namespace core
    {
        /**destroy and reset any GPU allocated memory and handles. Call it to free memory if app was minimized (lost focus)*/
        void reset();
    }
    enum error_policy
    {
        ep_show_error,//shows assert and prints error to log
        ep_show_warning,//prints warning to log
        ep_ignore_error//doesn't show any errors
    };
    void handleErrorPolicy(error_policy ep, QString message);
}
