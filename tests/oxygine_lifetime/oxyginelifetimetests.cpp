#include <cstdio>

#include <QMetaObject>

#include "tests/oxygine_lifetime/oxyginelifetimetests.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "coreengine/memorymanagement.h"

namespace OxygineLifetimeTests
{
    static qint32 s_failures = 0;

    static void check(bool ok, const char* what)
    {
        std::printf("[oxygine-selftest] %s %s\n", ok ? "PASS" : "FAIL", what);
        std::fflush(stdout);
        if (!ok)
        {
            ++s_failures;
        }
    }

    static void drainOnMain()
    {
        QMetaObject::invokeMethod(oxygine::GameWindow::getWindow(), []()
        {
            oxygine::Stage::getStage()->updateStage();
        }, Qt::BlockingQueuedConnection);
    }

    static void dispatchOnMain(oxygine::spActor & target, qint32 type)
    {
        QMetaObject::invokeMethod(oxygine::GameWindow::getWindow(), [target, type]()
        {
            oxygine::Event event(type);
            target->dispatchEvent(&event);
        }, Qt::BlockingQueuedConnection);
    }

    static constexpr qint32 STACK_CLOBBER_BYTES = 4096;
    static constexpr QSize TEST_FRAME_SIZE{31, 17};

    static void clobberStack()
    {
        volatile char buffer[STACK_CLOBBER_BYTES];
        for (qint32 i = 0; i < STACK_CLOBBER_BYTES; ++i)
        {
            buffer[i] = static_cast<char>(i);
        }
    }

    class Owner final : public oxygine::IClosureOwner
    {
    public:
        qint32 m_hits{0};
        void onEvent(oxygine::Event*)
        {
            ++m_hits;
        }
    };

    static constexpr qint32 TEST_EVENT = oxygine::eventID('T', 'S', 'T', 'E');

    qint32 runAll()
    {
        s_failures = 0;
        auto stage = oxygine::Stage::getStage();

        oxygine::spActor detached = MemoryManagement::create<oxygine::Actor>();
        check(!detached->requiresThreadChange(), "detached actor permits direct construction");

        oxygine::spActor subtreeRoot = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor subtreeBranch = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor subtreeLeaf = MemoryManagement::create<oxygine::Actor>();
        subtreeRoot->addChild(subtreeBranch);
        check(subtreeBranch->getParent() == subtreeRoot.get(), "detached subtree construction remains direct");
        stage->addChild(subtreeRoot);
        check(subtreeBranch->requiresThreadChange(), "pending subtree descendants defer updates");
        subtreeBranch->addChild(subtreeLeaf);
        check(subtreeLeaf->getParent() == nullptr, "pending subtree relationship is queued");
        drainOnMain();
        check(subtreeRoot->getParent() == stage.get(), "pending subtree root attaches after drain");
        check(subtreeBranch->getParent() == subtreeRoot.get(), "pending subtree branch remains attached");
        check(subtreeLeaf->getParent() == subtreeBranch.get(), "queued subtree relationship attaches after drain");
        subtreeRoot->detach();
        drainOnMain();

        oxygine::spActor pendingReparent = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor detachedParent = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor destinationBranch = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor destinationLeaf = MemoryManagement::create<oxygine::Actor>();
        detachedParent->addChild(destinationBranch);
        stage->addChild(pendingReparent);
        detachedParent->addChild(pendingReparent);
        check(pendingReparent->requiresThreadChange(), "detached reparent preserves pending attachment");
        check(detachedParent->requiresThreadChange(), "queued relationship protects detached destination");
        check(destinationBranch->requiresThreadChange(), "destination descendants inherit protection");
        destinationBranch->addChild(destinationLeaf);
        check(pendingReparent->getParent() == nullptr, "pending child relationship remains queued");
        check(destinationLeaf->getParent() == nullptr, "destination subtree mutation remains queued");
        drainOnMain();
        check(pendingReparent->getParent() == detachedParent.get(), "queued reparent order is preserved");
        check(destinationLeaf->getParent() == destinationBranch.get(), "queued destination mutation completes");
        check(!pendingReparent->requiresThreadChange(), "detached child is direct after queued transitions finish");

        oxygine::spActor sourceParent = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor sourceChild = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor sourceSibling = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor attachedDestination = MemoryManagement::create<oxygine::Actor>();
        sourceParent->addChild(sourceChild);
        stage->addChild(attachedDestination);
        drainOnMain();
        attachedDestination->addChild(sourceChild);
        check(sourceParent->requiresThreadChange(), "queued reparent protects detached source");
        sourceParent->addChild(sourceSibling);
        check(sourceSibling->getParent() == nullptr, "source subtree mutation remains queued");
        drainOnMain();
        check(sourceChild->getParent() == attachedDestination.get(), "queued child leaves detached source");
        check(sourceSibling->getParent() == sourceParent.get(), "queued source mutation completes");

        oxygine::spActor removalRoot = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor removalBranch = MemoryManagement::create<oxygine::Actor>();
        oxygine::spActor removalLeaf = MemoryManagement::create<oxygine::Actor>();
        removalRoot->addChild(removalBranch);
        stage->addChild(removalRoot);
        drainOnMain();
        stage->removeChild(removalRoot);
        check(removalBranch->requiresThreadChange(), "queued removal protects descendants");
        removalBranch->addChild(removalLeaf);
        check(removalLeaf->getParent() == nullptr, "removal subtree mutation remains queued");
        drainOnMain();
        check(removalRoot->getParent() == nullptr, "queued root removal completes");
        check(removalLeaf->getParent() == removalBranch.get(), "post-removal subtree mutation completes");

        oxygine::spActor pendingDetach = MemoryManagement::create<oxygine::Actor>();
        stage->addChild(pendingDetach);
        pendingDetach->detach();
        drainOnMain();
        check(pendingDetach->getParent() == nullptr, "plain detach cancels pending attachment");

        oxygine::spActor pending = MemoryManagement::create<oxygine::Actor>();
        stage->addChild(pending);
        check(pending->requiresThreadChange(), "pending attach defers off-main updates");
        drainOnMain();
        check(pending->getParent() == stage.get(), "pending actor attaches after drain");
        pending->detach();
        drainOnMain();

        oxygine::spActor parent = MemoryManagement::create<oxygine::Actor>();
        stage->addChild(parent);
        drainOnMain();
        check(parent->getParent() == stage.get(), "addChild attaches after drain");

        oxygine::spActor child = MemoryManagement::create<oxygine::Actor>();
        parent->addChild(child);
        drainOnMain();
        check(child->getParent() == parent.get(), "child attached to parent");
        child->detach();
        child->detach();
        drainOnMain();
        check(child->getParent() == nullptr, "double detach leaves child detached");
        check(parent->getParent() == stage.get(), "parent survives duplicate remove");

        oxygine::spActor ghost = MemoryManagement::create<oxygine::Actor>();
        stage->addChild(ghost);
        ghost->detachAndRemove();
        drainOnMain();
        check(ghost->getParent() == nullptr, "detachAndRemove cancels pending attach");
        std::weak_ptr<oxygine::Actor> watch = ghost;
        ghost.reset();
        drainOnMain();
        check(watch.expired(), "no owner retains the removed actor");

#ifdef GRAPHICSUPPORT
        oxygine::spSprite sprite = MemoryManagement::create<oxygine::Sprite>();
        stage->addChild(sprite);
        drainOnMain();
        {
            oxygine::AnimationFrame frame;
            frame.setSize(TEST_FRAME_SIZE);
            sprite->changeAnimFrame(frame);
        }
        clobberStack();
        drainOnMain();
        check(sprite->getSize() == TEST_FRAME_SIZE, "queued anim frame survives caller stack frame");
        sprite->detach();
#endif

        Owner owner;
        oxygine::spActor target = MemoryManagement::create<oxygine::Actor>();
        stage->addChild(target);
        drainOnMain();
        target->addEventListener(TEST_EVENT, oxygine::EventCallback(&owner, &Owner::onEvent));
        drainOnMain();
        dispatchOnMain(target, TEST_EVENT);
        check(owner.m_hits == 1, "queued addEventListener registered");
        target->removeEventListeners(&owner);
        drainOnMain();
        dispatchOnMain(target, TEST_EVENT);
        check(owner.m_hits == 1, "queued removeEventListeners removes the owner's listeners");

        target->detach();
        parent->detach();
        attachedDestination->detach();
        drainOnMain();

        std::printf("[oxygine-selftest] finished with %d failure(s)\n", s_failures);
        std::fflush(stdout);
        return s_failures;
    }
}
