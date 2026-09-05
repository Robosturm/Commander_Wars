#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/rules/ruleselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

#include "objects/dialogs/filedialog.h"

RuleSelectionDialog::RuleSelectionDialog(GameMap* pMap, RuleSelection::Mode mode, bool enabled)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("RuleSelectionDialog");
#endif
    Mainapp::getInstance()->pauseRendering();
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    m_pSpriteBox = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(m_pSpriteBox);
    m_pSpriteBox->setPosition(0, 0);
    m_pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    m_pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigOk();
    });
    connect(this, &RuleSelectionDialog::sigOk, this, &RuleSelectionDialog::pressedOk, Qt::QueuedConnection);

    if (enabled)
    {
        m_pButtonLoadRules = ObjectManager::createButton(tr("Load"));
        m_pButtonLoadRules->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 20 + m_OkButton->getScaledWidth() / 2, oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
        m_pButtonLoadRules->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
        {
            emit sigShowLoadRules();
        });
        m_pSpriteBox->addChild(m_pButtonLoadRules);
        connect(this, &RuleSelectionDialog::sigShowLoadRules, this, &RuleSelectionDialog::showLoadRules, Qt::QueuedConnection);

        m_pButtonSaveRules = ObjectManager::createButton(tr("Save"));
        m_pButtonSaveRules->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pButtonSaveRules->getScaledWidth() - 20 - m_OkButton->getScaledWidth() / 2,
                                        oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
        m_pButtonSaveRules->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event * )->void
        {
            emit sigShowSaveRules();
        });
        m_pSpriteBox->addChild(m_pButtonSaveRules);
        connect(this, &RuleSelectionDialog::sigShowSaveRules, this, &RuleSelectionDialog::showSaveRules, Qt::QueuedConnection);
    }
    m_pRuleSelection = MemoryManagement::create<RuleSelection>(m_pMap, oxygine::Stage::getStage()->getWidth() - 80, mode, enabled);
    connect(m_pRuleSelection.get(), &RuleSelection::sigSizeChanged, this, &RuleSelectionDialog::ruleSelectionSizeChanged, Qt::QueuedConnection);
    m_pSpriteBox->addChild(m_pRuleSelection);
    Mainapp::getInstance()->continueRendering();
}

void RuleSelectionDialog::ruleSelectionSizeChanged()
{    
}

void RuleSelectionDialog::showLoadRules()
{    
    QStringList wildcards;
    wildcards.append("*.grl");
    QString path = Settings::getInstance()->getUserPath() + "data/gamerules";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, false, "", false, tr("Load"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelectionDialog::loadRules, Qt::QueuedConnection);
}

void RuleSelectionDialog::showSaveRules()
{
    QStringList wildcards;
    wildcards.append("*.grl");
    QString path = Settings::getInstance()->getUserPath() + "data/gamerules";
    spFileDialog fileDialog = MemoryManagement::create<FileDialog>(path, wildcards, true, "", false, tr("Save"));
    addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelectionDialog::saveRules, Qt::QueuedConnection);
}

void RuleSelectionDialog::loadRules(QString filename)
{
    Mainapp::getInstance()->pauseRendering();
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        if (file.exists())
        {
            QFile file(filename);
            if (!file.open(QIODevice::ReadOnly))
            {
                CONSOLE_PRINT("Failed to open file " + filename, GameConsole::eERROR);
            }
            else
            {
                QDataStream stream(&file);
                stream.setVersion(QDataStream::Version::Qt_6_5);
                m_pMap->getGameRules()->deserializeObject(stream);
                file.close();
                auto mode = m_pRuleSelection->getMode();
                m_pRuleSelection->detachAndRemove();
                m_pRuleSelection = MemoryManagement::create<RuleSelection>(m_pMap, oxygine::Stage::getStage()->getWidth() - 80, mode);
                m_pSpriteBox->addChild(m_pRuleSelection);
            }
        }
    }
    Mainapp::getInstance()->continueRendering();
}

void RuleSelectionDialog::saveRules(QString filename)
{
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Version::Qt_6_5);

            m_pMap->getGameRules()->serializeObject(stream);
            file.close();
        }
        else
        {
            CONSOLE_PRINT("Failed to open file " + filename, GameConsole::eERROR);
        }
    }    
}

void RuleSelectionDialog::pressedOk()
{
    emit sigRulesChanged();
    detachAndRemove();
}
