#ifndef SCRIPTFUNCTIONSOURCE_H
#define SCRIPTFUNCTIONSOURCE_H

#include <QJSEngine>
#include <QJSValue>
#include <QString>

namespace ScriptFunctionSource
{

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

struct Location
{
    QString script;
    qint32 line{0};
    qint32 column{0};
    qint32 formalCount{0};
    FunctionKind kind{FunctionKind::Unavailable};
    SourceStatus status{SourceStatus::Unsupported};
};

Location locate(QJSEngine & engine, const QJSValue & function);

QString extract(const QString & scriptText, const Location & location, SourceStatus & status);

}

#endif // SCRIPTFUNCTIONSOURCE_H
