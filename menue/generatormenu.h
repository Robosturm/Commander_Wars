#ifndef GENERATORMENU_H
#define GENERATORMENU_H

#include "menue/basemenu.h"

class GeneratorMenu;
using spGeneratorMenu = std::shared_ptr<GeneratorMenu>;

class GeneratorMenu final : public Basemenu
{
public:
    explicit GeneratorMenu(const QString & generatorUi);
   virtual ~GeneratorMenu() = default;

public slots:
    void exitMenue(const QString & mainMenuXml = "ui/menu/playermenu.xml");
    virtual void onEnter() override;
    /**
     * @brief loadVariablesFromFile
     * @param file
     * @return
     */
    bool loadVariablesFromFile(const QString & file);
    /**
     * @brief writeVariablesFromFile
     * @param file
     */
    void writeVariablesToFile(const QString & file) const;
    /**
     * @brief writeDataToFile
     * @param file
     * @param data
     */
    void writeDataToFile(const QString & file, const QString & data) const;
    /**
     * @brief createDir
     * @param dir
     */
    void createDir(const QString & path) const;
    /**
     * @brief copySourceToTarget
     * @param source
     * @param target
     */
    void copySourceToTarget(const QString & source, const QString & target);
private:
};

Q_DECLARE_INTERFACE(GeneratorMenu, "GeneratorMenu");

#endif // GENERATORMENU_H
