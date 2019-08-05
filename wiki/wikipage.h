#ifndef WIKIPAGE_H
#define WIKIPAGE_H

#include <QObject>

#include "oxygine-flow.h"

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
protected:
    spPanel m_pPanel;
    oxygine::spButton m_OkButton;
};

#endif // WIKIPAGE_H
