#ifndef TABBEDBOX_H
#define TABBEDBOX_H

#include <QObject>
#include <vector>
#include <QString>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "objects/base/panel.h"
#include "objects/base/label.h"

struct TabItem
{
    oxygine::spBox9Sprite m_TabButton;       // The tab header button
    spLabel m_TabLabel;         // The text label on the tab
    spPanel m_TabPanel;                      // The content panel for this tab
    QString m_TabName;                       // Identifier for the tab
    QString m_InactiveResAnim;               // Resource animation for inactive state
    QString m_ActiveResAnim;                 // Resource animation for active state
};

class TabbedBox;
using spTabbedBox = std::shared_ptr<TabbedBox>;

/**
 * @brief The TabbedBox class provides a tabbed interface widget
 *
 * A TabbedBox displays multiple panels with tab headers that allow switching
 * between different content. Only one tab is active at a time. Tab buttons
 * display different visual states (sprites) depending on whether they are active or inactive.
 */
class TabbedBox final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief TabbedBox constructor
     * @param size The size of the tabbed box (width, height)
     * @param tabBarHeight Height of the tab header bar
     * @param tabButtonWidth Width of each tab button
     */
    explicit TabbedBox(QSize size, qint32 tabBarHeight = 80, qint32 tabButtonHeight = 50, qint32 tabButtonWidth = 100, qint32 tapHeight = 10);
    virtual ~TabbedBox() = default;

    /**
     * @brief addTab Adds a new tab to the tabbed box
     * @param tabName Identifier for the tab
     * @param tabLabel Display text for the tab
     * @param inactiveResAnim Resource animation for inactive tab button state
     * @param activeResAnim Resource animation for active tab button state
     */
    spPanel addTab(QString tabName, QString tabLabel,
                   QString inactiveResAnim = "tab_inactive", QString activeResAnim = "tab_active");

    /**
     * @brief setActiveTab Sets which tab is currently active
     * @param tabIndex Index of the tab to activate
     */
    void setActiveTab(qint32 tabIndex);

    /**
     * @brief setActiveTabByName Sets the active tab by its name
     * @param tabName Name identifier of the tab to activate
     */
    void setActiveTabByName(QString tabName);

    /**
     * @brief getActiveTabIndex Returns the index of the currently active tab
     * @return Index of active tab, or -1 if no tab is active
     */
    qint32 getActiveTabIndex() const;

    /**
     * @brief getTabCount Returns the total number of tabs
     * @return Number of tabs
     */
    qint32 getTabCount() const;

    /**
     * @brief getTabPanel Gets a panel by tab index
     * @param tabIndex Index of the tab
     * @return Pointer to the Panel, or nullptr if index is invalid
     */
    spPanel getTabPanel(qint32 tabIndex) const;

signals:
    /**
     * @brief tabChanged Signal emitted when active tab changes
     * @param tabIndex Index of the newly active tab
     */
    void tabChanged(qint32 tabIndex);

private:
    /**
     * @brief updateTabButtonStates Updates the visual appearance of all tab buttons
     */
    void updateTabButtonStates();

    /**
     * @brief updateTabButtonState Updates the visual state of a single tab button
     * @param tabIndex Index of the tab button to update
     * @param isActive Whether this tab button should appear active
     */
    void updateTabButtonState(qint32 tabIndex, bool isActive);

private:
    std::vector<TabItem> m_Tabs;             // All tabs in this tabbed box
    qint32 m_ActiveTabIndex = -1;            // Index of currently active tab
    qint32 m_TabBarHeight = 40;              // Height of the tab bar
    qint32 m_TabButtonWidth = 100;           // Width of each tab button
    qint32 m_tapHeight = 10;
    QSize m_ContentSize;                     // Size of content area
    spPanel m_TabBarContainer;      // Container for tab buttons
    oxygine::spActor m_ContentContainer;     // Container for tab panels
};

#endif // TABBEDBOX_H
