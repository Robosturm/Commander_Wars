#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"

namespace oxygine
{
    template<class T>
    class intrusive_list
    {
    public:
        T _first;
        T _last;

        ~intrusive_list()
        {
            clear();
        }

        bool empty() const
        {
            return _first == 0;
        }

        void clear()
        {
            T item = _first;
            while (item)
            {
                T copy = item;
                item = item->_next;
                if (copy)
                {
                    copy->_list = 0;
                    copy->_next = 0;
                    copy->_prev = 0;
                }
            }
            _first = 0;
            _last = 0;
        }

        void insert_after(T& child, T& after)
        {
            Q_ASSERT(after->_list == this);
            child->_list = this;

            if (after->_next)
            {
                after->_next->_prev = child;
            }
            else
            {
                _last = child;
            }
            child->_next = after->_next;
            child->_prev = after;
            after->_next = child;


            checkList();
        }

        void insert_before(T& child, T& before)
        {
            child->_list = this;

            if (before->_prev)
            {
                before->_prev->_next = child;
            }
            else
            {
                _first = child;
            }
            child->_prev = before->_prev;
            child->_next = before;
            before->_prev = child;


            checkList();
        }

        void append(T& child)
        {
            child->_list = this;

            if (_first)
            {
                _last->_next = child;
                child->_prev = _last;
            }
            else
            {
                _first = child;
                child->_prev = 0;
            }

            _last = child;
            child->_next = 0;

            checkList();
        }

        void prepend(T& child)
        {
            child->_list = this;

            if (_first)
            {
                _first->_prev = child;
                child->_next = _first;
            }
            else
            {
                _last = child;
                child->_next = 0;
            }

            _first = child;
            child->_prev = 0;

            checkList();
        }

        void checkList()
        {
            return;
            if (!_first)
            {
                return;
            }
            T v = _first;
            do
            {
                T p = v;
                v = v->_next;

                if (v)
                {
                    Q_ASSERT(v->_prev == p);
                }
                else
                {
                    Q_ASSERT(p == _last);
                }
            }
            while (v);

        }



        void removeItem(T& child)
        {
            Q_ASSERT(child->_list == this);
            child->_list = nullptr;

            if (child->_next)
            {
                Q_ASSERT(child->_next->_prev == child);
                child->_next->_prev = child->_prev;
            }
            else
            {
                Q_ASSERT(child == _last);
                _last = child->_prev;
            }


            if (child->_prev)
            {
                Q_ASSERT(child->_prev->_next == child);
                child->_prev->_next = child->_next;
            }
            else
            {
                Q_ASSERT(child == _first);
                _first = child->_next;
            }

            child->_prev = nullptr;
            child->_next = nullptr;

            checkList();
        }
    };


    template<class T>
    class intrusive_list_item
    {
    public:

        intrusive_list<T>* getParentList() {return _list;}

    protected:
        T& getNextSibling() {return _next;}
        T& getPrevSibling() {return _prev;}


        intrusive_list_item(): _prev(T(0)), _next(T(0)), _list(0) {}
        T _prev;
        T _next;

        intrusive_list<T>* _list;

        template<class U>
        friend class intrusive_list;
    };
}
