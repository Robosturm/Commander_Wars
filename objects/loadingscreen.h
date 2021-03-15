#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QObject>
#include <QSemaphore>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class LoadingScreen;
typedef oxygine::intrusive_ptr<LoadingScreen> spLoadingScreen;

class LoadingScreen : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static LoadingScreen* getInstance();
    /**
     * @brief show
     */
    void show();
    /**
     * @brief setProgress
     * @param workText
     * @param value
     */
    void setProgress(QString workText, qint32 value);
    /**
     * @brief setWorktext
     * @param workText
     */
    void setWorktext(QString workText);
    /**
     * @brief hide
     */
    void hide();

private:
    explicit LoadingScreen();
private:
    bool loaded = false;

    oxygine::spTextField m_workText;
    oxygine::spTextField m_loadingProgress;
    oxygine::spColorRectSprite m_BackgroundBar;
    oxygine::spColorRectSprite m_LoadingBar;


    static spLoadingScreen m_pLoadingScreen;
};

#endif // LOADINGSCREEN_H
