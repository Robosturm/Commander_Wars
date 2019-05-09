#ifndef SELECTKEY_H
#define SELECTKEY_H

#include <QObject>
#include "oxygine-framework.h"

class SelectKey;
typedef oxygine::intrusive_ptr<SelectKey> spSelectKey;

class SelectKey : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit SelectKey(SDL_Keycode code);
    virtual ~SelectKey() = default;

signals:
    void sigKeyChanged(SDL_Keycode key);

public slots:
    void keyInput(SDL_Event event);
protected:
    void setKeycode(SDL_Keycode code);
private:
    oxygine::spButton m_Button;
    SDL_Keycode currentCode{SDLK_SPACE};
    bool active{false};

};

#endif // SELECTKEY_H
