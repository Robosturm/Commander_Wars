#ifndef COREAI_H
#define COREAI_H

#include "gameinput/basegameinputif.h"

class GameAction;

class CoreAI : public BaseGameInputIF
{
    Q_OBJECT
public:
    explicit CoreAI();
    virtual ~CoreAI();
    /**
     * @brief init
     */
    virtual void init() override;
    /**
     * @brief process
     */
    virtual void process() = 0;
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(GameAction* pAction);
public slots:
    void nextAction();

protected:
    virtual void run()  override;

private:
    bool finish{false};
};

#endif // COREAI_H
