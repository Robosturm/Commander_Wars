#pragma once

#include <QObject>
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class CustomSpriteManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance
     * @return
     */
    static CustomSpriteManager* getInstance();

    oxygine::ResAnim* getResAnim(QString id);

    void release();
private:
    explicit CustomSpriteManager() = default;
    static CustomSpriteManager* m_pInstance;
    using resourcesMap = QMap<QString, oxygine::spSingleResAnim>;
    resourcesMap m_resources;
};

