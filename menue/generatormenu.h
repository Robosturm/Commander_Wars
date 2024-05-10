#ifndef GENERATORMENU_H
#define GENERATORMENU_H

#include "menue/basemenu.h"

class GeneratorMenu;
using spGeneratorMenu = std::shared_ptr<GeneratorMenu>;

class GeneratorMenu : public Basemenu
{
    Q_OBJECT
public:
    explicit GeneratorMenu(const QString & generatorUi, bool load = true);
    virtual ~GeneratorMenu() = default;

    Q_INVOKABLE void exitMenue(const QString & mainMenuXml = "ui/menu/playermenu.xml");
    /**
     * @brief loadVariablesFromFile
     * @param file
     * @return
     */
    Q_INVOKABLE bool loadVariablesFromFile(const QString & file);
    /**
     * @brief writeVariablesFromFile
     * @param file
     */
    Q_INVOKABLE void writeVariablesToFile(const QString & file) const;
    /**
     * @brief writeDataToFile
     * @param file
     * @param data
     */
    Q_INVOKABLE void writeDataToFile(const QString & file, const QString & data) const;
    /**
     * @brief createDir
     * @param dir
     */
    Q_INVOKABLE void createDir(const QString & path) const;
    /**
     * @brief copySourceToTarget
     * @param source
     * @param target
     */
    Q_INVOKABLE void copySourceToTarget(const QString & source, const QString & target);

public slots:
    virtual void onEnter() override;
private:
};

Q_DECLARE_INTERFACE(GeneratorMenu, "GeneratorMenu");

#endif // GENERATORMENU_H
