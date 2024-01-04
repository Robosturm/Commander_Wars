#pragma once

class GameAction;
using spGameAction = std::shared_ptr<GameAction>;

class IAwbwAction
{
public:
    virtual void readAction(QTextStream & stream, bool & valid) = 0;
    virtual spGameAction getAction() = 0;
};
