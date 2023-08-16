#ifndef MemoryManagement_H
#define MemoryManagement_H

#include <QObject>

class MemoryManagement final : public QObject
{
    Q_OBJECT
public:

    template <class T, typename ...TArgs>
   static std::shared_ptr<T> create(TArgs... args)
    {
        std::shared_ptr<T> pRet(new T(args...), &MemoryManagement::deleter<T>);
        return pRet;
    }

    template <class T>
    static void deleter(T * pObj)
    {
        if constexpr (std::is_base_of_v<T, QObject>)
        {
            pObj->deleteLater();
        }
        else
        {
            delete pObj;
        }
    }
};

#endif // TIMER_H
