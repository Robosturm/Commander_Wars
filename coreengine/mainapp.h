#ifndef MAINAPP_H
#define MAINAPP_H

#include "oxygine-framework.h"

#include <QTimer>
#include <QTranslator>
#include <QCoreApplication>

#include "coreengine/interpreter.h"

class Mainapp : public QCoreApplication
{
    Q_OBJECT

public slots:
    void update();

    static qint32 randInt(qint32 low, qint32 high);
    static bool isEven(qint32 value);
signals:
    void sigText(SDL_Event* event);
    void sigKeyDown(SDL_Event* event);
    void sigKeyUp(SDL_Event* event);
public:
    explicit Mainapp(int argc, char* argv[]);
    virtual ~Mainapp() = default;

    static Mainapp* getInstance();

    inline QTranslator* getTranslator()
    {
        return &m_Translator;
    }

    inline Interpreter* getInterpreter()
    {
        return &m_Interpreter;
    }

    void setup();
protected:
    void onEvent(oxygine::Event* ev);
private:
    QTranslator m_Translator;
    QTimer m_Timer;
    static Mainapp* m_pMainapp;

    Interpreter m_Interpreter;

};

#endif // MAINAPP_H
