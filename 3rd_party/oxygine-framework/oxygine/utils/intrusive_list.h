#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

namespace oxygine
{
    template<class T>
    class intrusive_list
    {
    public:
        intrusive_ptr<T> _first;
        intrusive_ptr<T> _last;

        ~intrusive_list()
        {
            clear();
        }

        bool empty() const
        {
            return _first == nullptr;
        }

        void clear()
        {
            intrusive_ptr<T> item = _first;
            while (item)
            {
                intrusive_ptr<T> copy = item;
                item = item->_next;
                if (copy)
                {
                    copy->_list = nullptr;
                    copy->_next = nullptr;
                    copy->_prev = nullptr;
                }
            }
            _first = nullptr;
            _last = nullptr;
        }

        void insert_after(intrusive_ptr<T>& child, intrusive_ptr<T>& after)
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
        }

        void insert_before(intrusive_ptr<T>& child, intrusive_ptr<T>& before)
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
        }

        void append(intrusive_ptr<T>& child)
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
                child->_prev = nullptr;
            }

            _last = child;
            child->_next = nullptr;
        }

        void prepend(intrusive_ptr<T>& child)
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
                child->_next = nullptr;
            }

            _first = child;
            child->_prev = nullptr;
        }

        void removeItem(intrusive_ptr<T>& child)
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
        }
    };


    template<class T>
    class intrusive_list_item
    {
    public:

        intrusive_list<T>* getParentList() {return _list;}

    protected:
        intrusive_ptr<T>& getNextSibling() {return _next;}
        intrusive_ptr<T>& getPrevSibling() {return _prev;}


        intrusive_list_item()
            : _prev(nullptr),
              _next(nullptr),
              _list(nullptr)
        {
        }
        intrusive_ptr<T> _prev;
        intrusive_ptr<T> _next;

        intrusive_list<T>* _list;

        template<class U>
        friend class intrusive_list;
    };
}
