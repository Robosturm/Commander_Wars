#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QObject>
#include <QSemaphore>

#include "oxygine-framework.h"

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
    /**
     * @brief doUpdate
     * @param us
     */
    virtual void doUpdate(const oxygine::UpdateState& us) override;
signals:
private:
    explicit LoadingScreen();
    void triggerUpdate();
private:
    bool loaded = false;

    oxygine::spTextField m_workText;
    oxygine::spTextField m_loadingProgress;
    QSemaphore m_semaphore;

    static spLoadingScreen m_pLoadingScreen;
};

#endif // LOADINGSCREEN_H
