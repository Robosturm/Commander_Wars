#include "objects/base/tabbedbox.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"
#include "coreengine/mainapp.h"

TabbedBox::TabbedBox(QSize size, qint32 tabBarHeight, qint32 tabButtonHeight, qint32 tabButtonWidth, qint32 tapHeight)
    : m_TabBarHeight(tabButtonHeight), m_TabButtonWidth(tabButtonWidth), m_tapHeight(tapHeight)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TabbedBox");
#endif

    // Calculate content area size
    m_ContentSize = QSize(size.width(), size.height() - tabBarHeight);

    // Set overall size
    setSize(size.width(), size.height());

    // Set priority
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    // Create tab bar container
    m_TabBarContainer = MemoryManagement::create<Panel>(true, QSize(size.width(), tabBarHeight), QSize(size.width(), tabBarHeight));
    addChild(m_TabBarContainer);
    m_TabBarContainer->setPosition(0, 0);
    m_TabBarContainer->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 1);

    // Create content container
    m_ContentContainer = MemoryManagement::create<oxygine::Actor>();
    addChild(m_ContentContainer);
    m_ContentContainer->setPosition(0, tabBarHeight);
    m_ContentContainer->setSize(m_ContentSize.width(), m_ContentSize.height());
    m_ContentContainer->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
}

spPanel TabbedBox::addTab(QString tabName, QString tabLabel,
                          QString inactiveResAnim, QString activeResAnim)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    QSize size = QSize(m_ContentSize.width(), m_ContentSize.height());
    TabItem newTab;
    newTab.m_TabName = tabName;
    newTab.m_InactiveResAnim = inactiveResAnim;
    newTab.m_ActiveResAnim = activeResAnim;
    newTab.m_TabPanel = MemoryManagement::create<Panel>(true, size, size);


    // Create tab button sprite
    newTab.m_TabButton = MemoryManagement::create<oxygine::Box9Sprite>();
    oxygine::ResAnim* pInactiveAnim = pObjectManager->getResAnim(inactiveResAnim);
    newTab.m_TabButton->setResAnim(pInactiveAnim);
    newTab.m_TabButton->setSize(m_TabButtonWidth, m_TabBarHeight - m_tapHeight);

    qint32 xPos = m_Tabs.size() * m_TabButtonWidth;
    newTab.m_TabButton->setPosition(xPos, m_tapHeight);
    newTab.m_TabButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 1);

    m_TabBarContainer->addItem(newTab.m_TabButton);

    // Create tab label text field
    newTab.m_TabLabel = MemoryManagement::create<Label>(m_TabButtonWidth - 20);
    newTab.m_TabLabel->setText(tabLabel);
    newTab.m_TabLabel->setPosition(8, 3); // Adjust Y offset as needed
    newTab.m_TabLabel->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 2);

    newTab.m_TabButton->addChild(newTab.m_TabLabel);

    newTab.m_TabButton->addClickListener([this, tabIndex = static_cast<qint32>(m_Tabs.size())](oxygine::Event*)
                                         {
                                             setActiveTab(tabIndex);
                                         });

    // Add the content panel to the content container
    m_ContentContainer->addChild(newTab.m_TabPanel);
    newTab.m_TabPanel->setPosition(0, 0);
    newTab.m_TabPanel->setVisible(false); // Hidden by default
    newTab.m_TabPanel->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    // Store the tab
    m_Tabs.push_back(newTab);

    m_TabBarContainer->setContentWidth(m_Tabs.size() * m_TabButtonWidth + 10);

    // Set first tab as active by default
    if (m_ActiveTabIndex == -1)
    {
        setActiveTab(0);
    }
    return newTab.m_TabPanel;
}

void TabbedBox::setActiveTab(qint32 tabIndex)
{
    if (tabIndex < 0 || tabIndex >= static_cast<qint32>(m_Tabs.size()))
    {
        return;
    }

    if (m_ActiveTabIndex == tabIndex)
    {
        return; // Already active
    }

    // Hide the previously active panel
    if (m_ActiveTabIndex >= 0 && m_ActiveTabIndex < static_cast<qint32>(m_Tabs.size()))
    {
        m_Tabs[m_ActiveTabIndex].m_TabPanel->setVisible(false);
    }

    m_ActiveTabIndex = tabIndex;

    // Show the new active panel
    m_Tabs[m_ActiveTabIndex].m_TabPanel->setVisible(true);

    // Update button visuals
    updateTabButtonStates();

    // Emit signal
    emit tabChanged(tabIndex);
}

void TabbedBox::setActiveTabByName(QString tabName)
{
    for (qint32 i = 0; i < static_cast<qint32>(m_Tabs.size()); ++i)
    {
        if (m_Tabs[i].m_TabName == tabName)
        {
            setActiveTab(i);
            return;
        }
    }
}

qint32 TabbedBox::getActiveTabIndex() const
{
    return m_ActiveTabIndex;
}

qint32 TabbedBox::getTabCount() const
{
    return static_cast<qint32>(m_Tabs.size());
}

spPanel TabbedBox::getTabPanel(qint32 tabIndex) const
{
    if (tabIndex < 0 || tabIndex >= static_cast<qint32>(m_Tabs.size()))
    {
        return nullptr;
    }
    return m_Tabs[tabIndex].m_TabPanel;
}

void TabbedBox::updateTabButtonStates()
{
    for (qint32 i = 0; i < static_cast<qint32>(m_Tabs.size()); ++i)
    {
        updateTabButtonState(i, i == m_ActiveTabIndex);
    }
}

void TabbedBox::updateTabButtonState(qint32 tabIndex, bool isActive)
{
    if (tabIndex < 0 || tabIndex >= static_cast<qint32>(m_Tabs.size()))
    {
        return;
    }

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    TabItem& tab = m_Tabs[tabIndex];

    QString resAnimName = isActive ? tab.m_ActiveResAnim : tab.m_InactiveResAnim;
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim(resAnimName);

    if (pAnim)
    {
        tab.m_TabButton->setResAnim(pAnim);
    }

    // Optionally adjust visual properties for active state
    if (isActive)
    {
        tab.m_TabButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 2);
        tab.m_TabLabel->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 3);

        tab.m_TabButton->setY(0);
        tab.m_TabButton->setHeight(m_TabBarHeight);
    }
    else
    {
        tab.m_TabButton->setY(m_tapHeight);
        tab.m_TabButton->setHeight(m_TabBarHeight - m_tapHeight);
        tab.m_TabButton->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 1);
        tab.m_TabLabel->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects) + 2);
    }
}
