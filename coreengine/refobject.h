#pragma once

#include <concepts>
#include <memory>

template <class Base>
class RefObject : public std::enable_shared_from_this<Base>
{
public:
    std::weak_ptr<Base> getWeakPtr()
    {
        return std::enable_shared_from_this<Base>::weak_from_this();
    }
    template <class Derived>
    std::shared_ptr<Derived> getSharedPtrFromWeak()
    {
        return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::weak_from_this().lock());
    }

    bool notInSharedUse() const
    {
        auto count = std::enable_shared_from_this<Base>::weak_from_this().use_count();
        return  count == 0;
    }
protected:
    template <class Derived>
    std::shared_ptr<Derived> getSharedPtr()
    {
        return std::static_pointer_cast<Derived>(std::enable_shared_from_this<Base>::shared_from_this());
    }
};
