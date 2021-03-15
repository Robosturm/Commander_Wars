#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/closure/closure.h"

#include <qvector.h>

namespace oxygine
{
    class Restorable
    {
    public:
        Restorable();
        virtual ~Restorable();

        typedef QVector<Restorable*> restorable;
        static const restorable& getObjects();
        static void restoreAll();
        static void releaseAll();
        static bool isRestored();

        virtual void* _getRestorableObject() = 0;
        virtual void release() = 0;

        void restore();
        using RestoreCallback = Closure<void,Restorable*, void*>;

        void reg(RestoreCallback cb, void* user);
        void unreg();

    protected:

    private:
        //non copyable
        Restorable(const Restorable&);
        const Restorable& operator=(const Restorable&);

        RestoreCallback _cb;
        void* _userData;
        bool _registered;
    };
}
