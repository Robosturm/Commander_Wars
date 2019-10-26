#pragma once
#include "../oxygine-include.h"
#include "ref_counter.h"
#include <string>
#include <vector>

namespace oxygine
{
    /**Base class for each oxygine object. Each object has unique internal ID and name. Debug build is tracking all created and deleted objects. Using memory pools*/
    class ObjectBase
    {
    public:
        ObjectBase(const ObjectBase& src);
        ObjectBase(bool assignID = true);
        virtual ~ObjectBase();

        const std::string&  getName() const;
        const void*         getUserData() const {return __userData;}
        uint64              getUserData64() const { return __userData64; }
        int                 getUserData32() const { return __userData32; }
        int                 getObjectID()const {return __id;}
        bool                isName(const std::string& name) const;
        bool                isName(const char* name) const;



        void setName(const std::string& name);

        /**void*, uin64 and int userData is UNION!*/
        void setUserData(const void* data) { __userData64 = 0; __userData = data; }
        /**void*, uin64 and int userData is UNION!*/
        void setUserData64(uint64 data) { __userData64 = data; }
        /**void*, uin64 and int userData is UNION!*/
        void setUserData32(int data) { __userData32 = data; }

        /**Shows assert when object with this unique ID will be created.*/
        static void showAssertInCtor(int id);
        /**Shows assert when object with this unique ID will be destroyed.*/
        static void showAssertInDtor(int id);

        //debug functions
        typedef std::vector<ObjectBase*> __createdObjects;
        static __createdObjects&    __getCreatedObjects();

    protected:
        void __generateID();
        std::string __name;
        int __id;

        union
        {
            const void* __userData;
            uint64 __userData64;
            int    __userData32;
        };

        static int _lastID;
        static int _assertCtorID;
        static int _assertDtorID;
    };

    DECLARE_SMART(Object, spObject);
    class Object: public ref_counter, public ObjectBase
    {
    public:
        Object(const Object& src);
        Object(bool assignUniqueID = true);

    protected:
    };


    template <class dest, class src>
    dest safeCast(src ptr)
    {
        if (!ptr)
            return 0;
#ifdef OXYGINE_DEBUG_SAFECAST
        dest cast = dynamic_cast<dest>(ptr);
        Q_ASSERT(cast && "can't cast pointer");
        return cast;
#else
        return static_cast<dest>(ptr);
#endif
    }

    template<class T, class U>
    intrusive_ptr<T> safeSpCast(intrusive_ptr<U> const& p)
    {
        if (!p)
            return 0;
#ifdef OXYGINE_DEBUG_SAFECAST
        intrusive_ptr<T> t = dynamic_cast<T*>(p.get());
        Q_ASSERT(t && "can't cast pointer");
#endif
        return static_cast<T*>(p.get());
    }
}

EDITOR_INCLUDE(Object);
