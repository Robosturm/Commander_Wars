#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QObject>
#include <QSemaphore>


#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "objects/base/label.h"

class LoadingScreen;
using spLoadingScreen = std::shared_ptr<LoadingScreen>;

class LoadingScreen final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static spLoadingScreen getInstance();
    virtual ~LoadingScreen();
    /**w
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
    friend class MemoryManagement;
    explicit LoadingScreen();
private:
    spLabel m_workText;
    spLabel m_loadingProgress;
    oxygine::spColorRectSprite m_BackgroundBar;
    oxygine::spColorRectSprite m_LoadingBar;
    static spLoadingScreen m_pLoadingScreen;
};

#endif // LOADINGSCREEN_H
