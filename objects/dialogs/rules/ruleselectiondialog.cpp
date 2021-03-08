#include "ruleselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

#include "objects/dialogs/filedialog.h"

RuleSelectionDialog::RuleSelectionDialog(RuleSelection::Mode mode, bool enabled)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigOk();
    });
    connect(this, &RuleSelectionDialog::sigOk, this, &RuleSelectionDialog::pressedOk, Qt::QueuedConnection);

    if (enabled)
    {
        m_pButtonLoadRules = ObjectManager::createButton(tr("Load"));
        m_pButtonLoadRules->setPosition(Settings::getWidth() / 2 + 20 + m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
        m_pButtonLoadRules->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigShowLoadRules();
        });
        pSpriteBox->addChild(m_pButtonLoadRules);
        connect(this, &RuleSelectionDialog::sigShowLoadRules, this, &RuleSelectionDialog::showLoadRules, Qt::QueuedConnection);

        m_pButtonSaveRules = ObjectManager::createButton(tr("Save"));
        m_pButtonSaveRules->setPosition(Settings::getWidth() / 2 - m_pButtonSaveRules->getWidth() - 20 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
        m_pButtonSaveRules->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
        {
            emit sigShowSaveRules();
        });
        pSpriteBox->addChild(m_pButtonSaveRules);
        connect(this, &RuleSelectionDialog::sigShowSaveRules, this, &RuleSelectionDialog::showSaveRules, Qt::QueuedConnection);
    }
    m_pRuleSelection = new RuleSelection(Settings::getWidth() - 80, mode, enabled);
    QSize size(Settings::getWidth() - 20, Settings::getHeight() - 40 * 2 - m_OkButton->getHeight());
    m_pPanel = new  Panel(true,  size, size);
    m_pPanel->setPosition(10, 20);
    m_pPanel->addItem(m_pRuleSelection);
    m_pPanel->setContentHeigth(m_pRuleSelection->getHeight() + 40);
    m_pPanel->setContentWidth(m_pRuleSelection->getWidth());
    pSpriteBox->addChild(m_pPanel);
}


void RuleSelectionDialog::showLoadRules()
{    
    QVector<QString> wildcards;
    wildcards.append("*.grl");
    QString path = QCoreApplication::applicationDirPath() + "/data/gamerules";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelectionDialog::loadRules, Qt::QueuedConnection);
}

void RuleSelectionDialog::showSaveRules()
{
    QVector<QString> wildcards;
    wildcards.append("*.grl");
    QString path = QCoreApplication::applicationDirPath() + "/data/gamerules";
    spFileDialog fileDialog = new FileDialog(path, wildcards);
    this->addChild(fileDialog);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &RuleSelectionDialog::saveRules, Qt::QueuedConnection);
}

void RuleSelectionDialog::loadRules(QString filename)
{
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        if (file.exists())
        {
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QDataStream stream(&file);
            GameMap::getInstance()->getGameRules()->deserializeObject(stream);
            file.close();
            auto mode = m_pRuleSelection->getMode();
            m_pRuleSelection->detach();
            m_pRuleSelection = new RuleSelection(Settings::getWidth() - 80, mode);
            m_pPanel->addItem(m_pRuleSelection);
            m_pPanel->setContentHeigth(m_pRuleSelection->getHeight() + 40);
            m_pPanel->setContentWidth(m_pRuleSelection->getWidth());
        }
    }
}

void RuleSelectionDialog::saveRules(QString filename)
{
    if (filename.endsWith(".grl"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        spGameMap pMap = GameMap::getInstance();
        pMap->getGameRules()->serializeObject(stream);
        file.close();
    }    
}

void RuleSelectionDialog::pressedOk()
{    
    m_pRuleSelection->confirmRuleSelectionSetup();
    emit sigRulesChanged();
    detach();    
}
