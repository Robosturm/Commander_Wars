#include "objects/dialogs/rules/coselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"

#include "objects/dialogs/ingame/coinfodialog.h"

#include "game/co.h"

COSelectionDialog::COSelectionDialog(GameMap* pMap, QString coid, QColor color, qint32 player, QStringList coids)
    : m_player(player),
      m_pMap(pMap)

{
#ifdef GRAPHICSUPPORT
    setObjectName("COSelectionDialog");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    m_COSelection = spCOSelection::create(QPoint(30, 30), QSize(Settings::getWidth() - 60, Settings::getHeight() - 100), coids);
    m_COSelection->colorChanged(color);
    pSpriteBox->addChild(m_COSelection);
    selectedCOIDChanged(coid);

    connect(m_COSelection.get(), &COSelection::coSelected, this, &COSelectionDialog::selectedCOIDChanged);
    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getScaledWidth() - 30,
                            Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit editFinished(m_currentCOID, m_player);
        emit sigFinished();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit canceled();
    });

    // show co info button
    m_ShowCOInfoButton = pObjectManager->createButton(tr("Show CO Info"), 250);
    m_ShowCOInfoButton->setPosition(Settings::getWidth() / 2 - m_ShowCOInfoButton->getScaledWidth() / 2,
                                    Settings::getHeight() - 30 - m_ShowCOInfoButton->getScaledHeight());
    pSpriteBox->addChild(m_ShowCOInfoButton);
    m_ShowCOInfoButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigShowCOInfo();
    });
    connect(this, &COSelectionDialog::sigShowCOInfo, this, &COSelectionDialog::showCOInfo, Qt::QueuedConnection);
    connect(this, &COSelectionDialog::canceled, this, &COSelectionDialog::remove, Qt::QueuedConnection);
    connect(this, &COSelectionDialog::sigFinished, this, &COSelectionDialog::remove, Qt::QueuedConnection);
    filterAndSortCoIds(coids);
}

void COSelectionDialog::filterAndSortCoIds(const QStringList & coids)
{
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QStringList armies = pCOSpriteManager->getArmyList(coids);
    for (const auto & army : armies)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue ret = pInterpreter->doFunction("PLAYER", "getArmyCOs" + army);
        QStringList preSetCOOrder = ret.toVariant().toStringList();
        qint32 index = 0;
        if (coids.size() > 0)
        {
            // remove all unallowed co's
            while (index < preSetCOOrder.size())
            {
                if (coids.contains(preSetCOOrder[index]))
                {
                    index++;
                }
                else
                {
                    preSetCOOrder.removeAt(index);
                }
            }
        }
        for (qint32 i = 0; i < pCOSpriteManager->getCount(); ++i)
        {
            QString coid = pCOSpriteManager->getID(i);
            if ((coids.isEmpty() || coids.contains(coid)) &&
                !m_coids.contains(coid) && !preSetCOOrder.contains(coid))
            {
                QString function1 = "getCOArmy";
                QJSValue ret = pInterpreter->doFunction(coid, function1);
                if (ret.isString())
                {
                    QString COArmy = ret.toString();
                    if (COArmy == army && !preSetCOOrder.contains(coid))
                    {
                        preSetCOOrder.append(coid);
                        break;
                    }
                }
            }
        }
        m_coids.append(preSetCOOrder);
    }
    m_coids.removeAll(CO::CO_RANDOM);
    m_coids.removeDuplicates();
}

void COSelectionDialog::remove()
{
    detach();
}

void COSelectionDialog::showCOInfo()
{
    QString coid = m_currentCOID;
    if (coid.isEmpty())
    {
        coid = m_coids[0];
    }
    Player* pPlayer = m_pMap->getPlayer(m_player);
    spCO co = spCO::create(coid, pPlayer, m_pMap);
    addChild(spCOInfoDialog::create(co, spPlayer(pPlayer), [this, pPlayer](spCO& pCurrentCO, spPlayer&, qint32 direction)
    {        
        qint32 index = m_coids.indexOf(pCurrentCO->getCoID());
        index += direction;
        QString coid;
        if (index < 0)
        {
            coid = m_coids[m_coids.size() - 1];
        }
        else if (index >= m_coids.size())
        {
            coid = m_coids[0];
        }
        else
        {
            coid = m_coids[index];
        }
        pCurrentCO = spCO::create(coid, pPlayer, m_pMap);
    }, false));
}

void COSelectionDialog::selectedCOIDChanged(QString coid)
{
    
    m_currentCOID = coid;
    
}
