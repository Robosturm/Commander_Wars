#ifndef WIKIPAGE_H
#define WIKIPAGE_H

#include <QObject>

#include "objects/panel.h"

class Wikipage;
typedef oxygine::intrusive_ptr<Wikipage> spWikipage;

class Wikipage : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Wikipage();
    /**
     * @brief getPanel
     * @return
     */
    inline Panel* getPanel()
    {
        return m_pPanel.get();
    }
signals:
    void sigFinished();
public slots:
    void keyInput(SDL_Event event);
    /**
     * @brief loadText
     * @param text
     */
    void loadText(QString text);
    /**
     * @brief loadImage
     * @param file
     */
    void loadImage(QString file, float scale = 1.0f);
    /**
     * @brief loadHeadline
     * @param text
     */
    void loadHeadline(QString text);
protected:
    spPanel m_pPanel;
    oxygine::spButton m_OkButton;
    qint32 y{10};
};

#endif // WIKIPAGE_H
