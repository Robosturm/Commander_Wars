#ifndef SCRIPTFUNCTIONSOURCE_H
#define SCRIPTFUNCTIONSOURCE_H

#include <memory>

#include <QHash>
#include <QJSValue>
#include <QObject>
#include <QString>
#include <QVariantMap>

class QJSEngine;
class ScriptFunctionSource;
using spScriptFunctionSource = std::shared_ptr<ScriptFunctionSource>;

// read-only access to the text of functions the interpreter evaluated
class ScriptFunctionSource final : public QObject
{
    Q_OBJECT
public:
    enum class FunctionKind
    {
        Unavailable,
        Script,
        Arrow,
        Eval,
        Native,
    };

    enum class SourceStatus
    {
        Ok,
        Unsupported,
        NotCallable,
        NativeFunction,
        UnsupportedKind,
        NoScriptText,
        NotAtFunctionKeyword,
        NoFunctionEnd,
    };

    explicit ScriptFunctionSource(QJSEngine* pEngine);
    // the per line transform the replay runtime data uses, so live and replay text match
    static QString simplifyLines(const QString & text);
    void storeScriptText(const QString & script, const QString & contents);
public slots:
    QVariantMap getScriptFunctionInfo(const QJSValue & function);
    QString getScriptFunctionSource(const QJSValue & function);
    QString getScriptText(const QString & script, qint32 offset, qint32 length);
private:
    struct Location
    {
        QString script;
        qint32 line{0};
        qint32 column{0};
        qint32 formalCount{0};
        FunctionKind kind{FunctionKind::Unavailable};
        SourceStatus status{SourceStatus::Unsupported};
    };
    Location locate(const QJSValue & function) const;
    QString extract(const Location & location, SourceStatus & status) const;
    QString findScriptText(const QString & script) const;
private:
    QJSEngine* m_pEngine{nullptr};
    QHash<QString, QString> m_scriptTexts;
};

#endif // SCRIPTFUNCTIONSOURCE_H
