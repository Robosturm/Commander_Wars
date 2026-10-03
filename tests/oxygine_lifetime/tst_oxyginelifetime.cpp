#include <memory>

#include <QtTest>

#include <QCoreApplication>
#include <QMetaObject>
#include <QThread>

#include "tests/testgamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/closure.h"
#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"
#include "3rd_party/oxygine-framework/oxygine/Event.h"
#include "3rd_party/oxygine-framework/oxygine/EventDispatcher.h"

#include "coreengine/gameconsole.h"
#include "coreengine/memorymanagement.h"
#include "coreengine/metatyperegister.h"
#include "coreengine/settings.h"

namespace
{
    constexpr qint32 STACK_CLOBBER_BYTES = 4096;
    constexpr QSize TEST_FRAME_SIZE{31, 17};
    constexpr qint32 TEST_EVENT = oxygine::eventID('T', 'S', 'T', 'E');

    void clobberStack()
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
}

class OxygineLifetimeTests final : public QObject
{
    Q_OBJECT
public:
    OxygineLifetimeTests() = default;
    ~OxygineLifetimeTests() override = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
    void test6();
    void test7();
    void test8();
    void test9();
    void test10();
    void test11();

private:
    // The production render thread is the thread draining queued actor updates
    // (Renderer::onPaint -> Stage::updateStage). The harness reproduces that by
    // running the drain on the renderer which lives on the render thread.
    static void drainOnRenderThread();
    static void dispatchOnRenderThread(oxygine::spActor & target, qint32 type);

    std::unique_ptr<TestGameWindow> m_window;
};

void OxygineLifetimeTests::initTestCase()
{
    QCoreApplication::setApplicationName("Commander Wars Tests");
    GameConsole::getInstance();
    MemoryManagement::getInstance().moveToThread(QThread::currentThread());
    MetaTypeRegister::registerInterfaceData();
    // default settings (1024x800, scale 1.0, ui enabled) are sufficient for the tests;
    // Settings::setup() is intentionally not called since it requires the full Mainapp.
    Settings::getInstance();
    // the tests validate the queued actor mutation path drained by Stage::updateStage()
    // (same behavior as the production "SyncActorEvents=false" setting); the engine
    // default true applies mutations synchronously via the render thread instead.
    oxygine::EventDispatcher::setSyncEvents(false);
    m_window = std::make_unique<TestGameWindow>();
    m_window->startRenderThread();
    m_window->launch();
    QVERIFY(oxygine::Stage::getStage().get() != nullptr);
}

void OxygineLifetimeTests::cleanupTestCase()
{
    if (m_window)
    {
        m_window->shutdownTest();
        m_window.reset();
    }
}

void OxygineLifetimeTests::drainOnRenderThread()
{
    auto* window = oxygine::GameWindow::getWindow();
    QMetaObject::invokeMethod(&window->getRenderer(), []()
    {
        oxygine::Stage::getStage()->updateStage();
    }, Qt::BlockingQueuedConnection);
}

void OxygineLifetimeTests::dispatchOnRenderThread(oxygine::spActor & target, qint32 type)
{
    auto* window = oxygine::GameWindow::getWindow();
    QMetaObject::invokeMethod(&window->getRenderer(), [target, type]()
    {
        oxygine::Event event(type);
        target->dispatchEvent(&event);
    }, Qt::BlockingQueuedConnection);
}

void OxygineLifetimeTests::test1()
{
    oxygine::spActor detached = MemoryManagement::create<oxygine::Actor>();
    QVERIFY2(!detached->requiresThreadChange(), "detached actor permits direct construction");
}

void OxygineLifetimeTests::test2()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor subtreeRoot = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor subtreeBranch = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor subtreeLeaf = MemoryManagement::create<oxygine::Actor>();
    subtreeRoot->addChild(subtreeBranch);
    QVERIFY2(subtreeBranch->getParent() == subtreeRoot.get(), "detached subtree construction remains direct");
    stage->addChild(subtreeRoot);
    QVERIFY2(subtreeBranch->requiresThreadChange(), "pending subtree descendants defer updates");
    subtreeBranch->addChild(subtreeLeaf);
    QVERIFY2(subtreeLeaf->getParent() == nullptr, "pending subtree relationship is queued");
    drainOnRenderThread();
    QVERIFY2(subtreeRoot->getParent() == stage.get(), "pending subtree root attaches after drain");
    QVERIFY2(subtreeBranch->getParent() == subtreeRoot.get(), "pending subtree branch remains attached");
    QVERIFY2(subtreeLeaf->getParent() == subtreeBranch.get(), "queued subtree relationship attaches after drain");
    subtreeRoot->detach();
    drainOnRenderThread();
}

void OxygineLifetimeTests::test3()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor pendingReparent = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor detachedParent = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor destinationBranch = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor destinationLeaf = MemoryManagement::create<oxygine::Actor>();
    detachedParent->addChild(destinationBranch);
    stage->addChild(pendingReparent);
    detachedParent->addChild(pendingReparent);
    QVERIFY2(pendingReparent->requiresThreadChange(), "detached reparent preserves pending attachment");
    QVERIFY2(detachedParent->requiresThreadChange(), "queued relationship protects detached destination");
    QVERIFY2(destinationBranch->requiresThreadChange(), "destination descendants inherit protection");
    destinationBranch->addChild(destinationLeaf);
    QVERIFY2(pendingReparent->getParent() == nullptr, "pending child relationship remains queued");
    QVERIFY2(destinationLeaf->getParent() == nullptr, "destination subtree mutation remains queued");
    drainOnRenderThread();
    QVERIFY2(pendingReparent->getParent() == detachedParent.get(), "queued reparent order is preserved");
    QVERIFY2(destinationLeaf->getParent() == destinationBranch.get(), "queued destination mutation completes");
    QVERIFY2(!pendingReparent->requiresThreadChange(), "detached child is direct after queued transitions finish");
}

void OxygineLifetimeTests::test4()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor sourceParent = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor sourceChild = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor sourceSibling = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor attachedDestination = MemoryManagement::create<oxygine::Actor>();
    sourceParent->addChild(sourceChild);
    stage->addChild(attachedDestination);
    drainOnRenderThread();
    attachedDestination->addChild(sourceChild);
    QVERIFY2(sourceParent->requiresThreadChange(), "queued reparent protects detached source");
    sourceParent->addChild(sourceSibling);
    QVERIFY2(sourceSibling->getParent() == nullptr, "source subtree mutation remains queued");
    drainOnRenderThread();
    QVERIFY2(sourceChild->getParent() == attachedDestination.get(), "queued child leaves detached source");
    QVERIFY2(sourceSibling->getParent() == sourceParent.get(), "queued source mutation completes");
    attachedDestination->detach();
}

void OxygineLifetimeTests::test5()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor removalRoot = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor removalBranch = MemoryManagement::create<oxygine::Actor>();
    oxygine::spActor removalLeaf = MemoryManagement::create<oxygine::Actor>();
    removalRoot->addChild(removalBranch);
    stage->addChild(removalRoot);
    drainOnRenderThread();
    stage->removeChild(removalRoot);
    QVERIFY2(removalBranch->requiresThreadChange(), "queued removal protects descendants");
    removalBranch->addChild(removalLeaf);
    QVERIFY2(removalLeaf->getParent() == nullptr, "removal subtree mutation remains queued");
    drainOnRenderThread();
    QVERIFY2(removalRoot->getParent() == nullptr, "queued root removal completes");
    QVERIFY2(removalLeaf->getParent() == removalBranch.get(), "post-removal subtree mutation completes");
}

void OxygineLifetimeTests::test6()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor pendingDetach = MemoryManagement::create<oxygine::Actor>();
    stage->addChild(pendingDetach);
    pendingDetach->detach();
    drainOnRenderThread();
    QVERIFY2(pendingDetach->getParent() == nullptr, "plain detach cancels pending attachment");
}

void OxygineLifetimeTests::test7()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor pending = MemoryManagement::create<oxygine::Actor>();
    stage->addChild(pending);
    QVERIFY2(pending->requiresThreadChange(), "pending attach defers off-main updates");
    drainOnRenderThread();
    QVERIFY2(pending->getParent() == stage.get(), "pending actor attaches after drain");
    pending->detach();
    drainOnRenderThread();
}

void OxygineLifetimeTests::test8()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor parent = MemoryManagement::create<oxygine::Actor>();
    stage->addChild(parent);
    drainOnRenderThread();
    QVERIFY2(parent->getParent() == stage.get(), "addChild attaches after drain");

    oxygine::spActor child = MemoryManagement::create<oxygine::Actor>();
    parent->addChild(child);
    drainOnRenderThread();
    QVERIFY2(child->getParent() == parent.get(), "child attached to parent");
    child->detach();
    child->detach();
    drainOnRenderThread();
    QVERIFY2(child->getParent() == nullptr, "double detach leaves child detached");
    QVERIFY2(parent->getParent() == stage.get(), "parent survives duplicate remove");
    parent->detach();
}

void OxygineLifetimeTests::test9()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spActor ghost = MemoryManagement::create<oxygine::Actor>();
    stage->addChild(ghost);
    ghost->detachAndRemove();
    drainOnRenderThread();
    QVERIFY2(ghost->getParent() == nullptr, "detachAndRemove cancels pending attach");
    std::weak_ptr<oxygine::Actor> watch = ghost;
    ghost.reset();
    drainOnRenderThread();
    QVERIFY2(watch.expired(), "no owner retains the removed actor");
}

void OxygineLifetimeTests::test10()
{
    auto stage = oxygine::Stage::getStage();
    oxygine::spSprite sprite = MemoryManagement::create<oxygine::Sprite>();
    stage->addChild(sprite);
    drainOnRenderThread();
    {
        oxygine::AnimationFrame frame;
        frame.setSize(TEST_FRAME_SIZE);
        sprite->changeAnimFrame(frame);
    }
    clobberStack();
    drainOnRenderThread();
    QVERIFY2(sprite->getSize() == TEST_FRAME_SIZE, "queued anim frame survives caller stack frame");
    sprite->detach();
}

void OxygineLifetimeTests::test11()
{
    auto stage = oxygine::Stage::getStage();
    Owner owner;
    oxygine::spActor target = MemoryManagement::create<oxygine::Actor>();
    stage->addChild(target);
    drainOnRenderThread();
    target->addEventListener(TEST_EVENT, oxygine::EventCallback(&owner, &Owner::onEvent));
    drainOnRenderThread();
    dispatchOnRenderThread(target, TEST_EVENT);
    QVERIFY2(owner.m_hits == 1, "queued addEventListener registered");
    target->removeEventListeners(&owner);
    drainOnRenderThread();
    dispatchOnRenderThread(target, TEST_EVENT);
    QVERIFY2(owner.m_hits == 1, "queued removeEventListeners removes the owner's listeners");

    target->detach();
    drainOnRenderThread();
}

QTEST_MAIN(OxygineLifetimeTests)

#include "tst_oxyginelifetime.moc"
