#include "coreengine/scriptfunctionsource.h"

#include <QVector>

#ifdef SCRIPTSOURCESUPPORT
#include <private/qjsvalue_p.h>
#include <private/qv4compileddata_p.h>
#include <private/qv4engine_p.h>
#include <private/qv4function_p.h>
#include <private/qv4functionobject_p.h>
#include <private/qv4scopedvalue_p.h>
#endif

enum class ScanState
{
    Code,
    LineComment,
    BlockComment,
    SingleQuote,
    DoubleQuote,
    Template,
    TemplateSubstitution,
    RegExp,
    RegExpClass,
};

static constexpr QChar CHAR_CARRIAGE_RETURN{u'\r'};
static constexpr QChar CHAR_LINE_FEED{u'\n'};
static constexpr QChar CHAR_SLASH{u'/'};
static constexpr QChar CHAR_STAR{u'*'};
static constexpr QChar CHAR_BACKSLASH{u'\\'};
static constexpr QChar CHAR_SINGLE_QUOTE{u'\''};
static constexpr QChar CHAR_DOUBLE_QUOTE{u'"'};
static constexpr QChar CHAR_BACK_QUOTE{u'`'};
static constexpr QChar CHAR_DOLLAR{u'$'};
static constexpr QChar CHAR_BRACE_OPEN{u'{'};
static constexpr QChar CHAR_BRACE_CLOSE{u'}'};
static constexpr QChar CHAR_PAREN_OPEN{u'('};
static constexpr QChar CHAR_PAREN_CLOSE{u')'};
static constexpr QChar CHAR_BRACKET_OPEN{u'['};
static constexpr QChar CHAR_BRACKET_CLOSE{u']'};
static constexpr qint32 FIRST_LINE = 1;
static constexpr qint32 FIRST_COLUMN = 1;
static const QString FUNCTION_KEYWORD = QStringLiteral("function");
static const QString REGEXP_PRECEDERS = QStringLiteral("(,=:[!&|?{};+-*%~^<>");

static ScanState popState(QVector<ScanState> & resumeStates)
{
    if (resumeStates.isEmpty())
    {
        return ScanState::Code;
    }
    return resumeStates.takeLast();
}

static qint32 getOffsetForPosition(const QString & text, qint32 line, qint32 column)
{
    if (line < FIRST_LINE || column < FIRST_COLUMN)
    {
        return -1;
    }
    const qint32 size = static_cast<qint32>(text.size());
    qint32 currentLine = FIRST_LINE;
    qint32 pos = 0;
    while (currentLine < line && pos < size)
    {
        const QChar current = text.at(pos);
        ++pos;
        if (current == CHAR_CARRIAGE_RETURN)
        {
            if (pos < size && text.at(pos) == CHAR_LINE_FEED)
            {
                ++pos;
            }
            ++currentLine;
        }
        else if (current == CHAR_LINE_FEED)
        {
            ++currentLine;
        }
    }
    if (currentLine != line)
    {
        return -1;
    }
    const qint32 offset = pos + column - FIRST_COLUMN;
    if (offset >= size)
    {
        return -1;
    }
    return offset;
}

static qint32 findFunctionKeyword(const QString & text, qint32 offset)
{
    const qint32 size = static_cast<qint32>(text.size());
    const qint32 keywordLength = static_cast<qint32>(FUNCTION_KEYWORD.size());
    qint32 pos = offset;
    while (pos < size && text.at(pos).isSpace())
    {
        ++pos;
    }
    if (pos + keywordLength > size)
    {
        return -1;
    }
    if (QStringView(text).mid(pos, keywordLength) != FUNCTION_KEYWORD)
    {
        return -1;
    }
    return pos;
}

static qint32 findFunctionEnd(const QString & text, qint32 start)
{
    const qint32 size = static_cast<qint32>(text.size());
    QVector<ScanState> resumeStates;
    QVector<qint32> substitutionDepths;
    ScanState state = ScanState::Code;
    QChar lastSignificant;
    qint32 braceDepth = 0;
    qint32 parenDepth = 0;
    qint32 pos = start;
    bool bodyOpened = false;
    while (pos < size)
    {
        const ScanState stateBefore = state;
        const QChar current = text.at(pos);
        const QChar next = pos + 1 < size ? text.at(pos + 1) : QChar();
        switch (state)
        {
            case ScanState::Code:
            case ScanState::TemplateSubstitution:
            {
                if (current == CHAR_SLASH && next == CHAR_SLASH)
                {
                    resumeStates.append(state);
                    state = ScanState::LineComment;
                    ++pos;
                }
                else if (current == CHAR_SLASH && next == CHAR_STAR)
                {
                    resumeStates.append(state);
                    state = ScanState::BlockComment;
                    ++pos;
                }
                else if (current == CHAR_SLASH &&
                         (lastSignificant.isNull() || REGEXP_PRECEDERS.contains(lastSignificant)))
                {
                    resumeStates.append(state);
                    state = ScanState::RegExp;
                }
                else if (current == CHAR_SINGLE_QUOTE)
                {
                    resumeStates.append(state);
                    state = ScanState::SingleQuote;
                }
                else if (current == CHAR_DOUBLE_QUOTE)
                {
                    resumeStates.append(state);
                    state = ScanState::DoubleQuote;
                }
                else if (current == CHAR_BACK_QUOTE)
                {
                    resumeStates.append(state);
                    state = ScanState::Template;
                }
                else if (current == CHAR_PAREN_OPEN)
                {
                    ++parenDepth;
                }
                else if (current == CHAR_PAREN_CLOSE)
                {
                    --parenDepth;
                }
                else if (current == CHAR_BRACE_CLOSE &&
                         state == ScanState::TemplateSubstitution &&
                         !substitutionDepths.isEmpty() &&
                         braceDepth == substitutionDepths.last())
                {
                    substitutionDepths.removeLast();
                    state = popState(resumeStates);
                }
                // parentheses always balance their own braces
                else if (parenDepth == 0)
                {
                    if (current == CHAR_BRACE_OPEN)
                    {
                        ++braceDepth;
                        bodyOpened = true;
                    }
                    else if (current == CHAR_BRACE_CLOSE)
                    {
                        --braceDepth;
                        if (bodyOpened && braceDepth == 0)
                        {
                            return pos;
                        }
                    }
                }
                break;
            }
            case ScanState::LineComment:
            {
                if (current == CHAR_LINE_FEED || current == CHAR_CARRIAGE_RETURN)
                {
                    state = popState(resumeStates);
                }
                break;
            }
            case ScanState::BlockComment:
            {
                if (current == CHAR_STAR && next == CHAR_SLASH)
                {
                    state = popState(resumeStates);
                    ++pos;
                }
                break;
            }
            case ScanState::SingleQuote:
            {
                if (current == CHAR_BACKSLASH)
                {
                    ++pos;
                }
                else if (current == CHAR_SINGLE_QUOTE)
                {
                    state = popState(resumeStates);
                }
                break;
            }
            case ScanState::DoubleQuote:
            {
                if (current == CHAR_BACKSLASH)
                {
                    ++pos;
                }
                else if (current == CHAR_DOUBLE_QUOTE)
                {
                    state = popState(resumeStates);
                }
                break;
            }
            case ScanState::Template:
            {
                if (current == CHAR_BACKSLASH)
                {
                    ++pos;
                }
                else if (current == CHAR_DOLLAR && next == CHAR_BRACE_OPEN)
                {
                    resumeStates.append(state);
                    substitutionDepths.append(braceDepth);
                    state = ScanState::TemplateSubstitution;
                    ++pos;
                }
                else if (current == CHAR_BACK_QUOTE)
                {
                    state = popState(resumeStates);
                }
                break;
            }
            case ScanState::RegExp:
            {
                if (current == CHAR_BACKSLASH)
                {
                    ++pos;
                }
                else if (current == CHAR_BRACKET_OPEN)
                {
                    state = ScanState::RegExpClass;
                }
                else if (current == CHAR_SLASH || current == CHAR_LINE_FEED || current == CHAR_CARRIAGE_RETURN)
                {
                    state = popState(resumeStates);
                }
                break;
            }
            case ScanState::RegExpClass:
            {
                if (current == CHAR_BACKSLASH)
                {
                    ++pos;
                }
                else if (current == CHAR_BRACKET_CLOSE)
                {
                    state = ScanState::RegExp;
                }
                break;
            }
        }
        const bool codeReached = state == ScanState::Code || state == ScanState::TemplateSubstitution;
        const bool leftComment = stateBefore == ScanState::LineComment || stateBefore == ScanState::BlockComment;
        if (codeReached && !leftComment && !current.isSpace())
        {
            lastSignificant = current;
        }
        ++pos;
    }
    return -1;
}

namespace ScriptFunctionSource
{

Location locate(QJSEngine & engine, const QJSValue & function)
{
    Location location;
#ifdef SCRIPTSOURCESUPPORT
    QV4::ExecutionEngine * pV4Engine = engine.handle();
    if (pV4Engine == nullptr)
    {
        return location;
    }
    QV4::Scope scope(pV4Engine);
    QV4::ScopedValue scoped(scope, QJSValuePrivate::convertToReturnedValue(pV4Engine, function));
    if (scoped->as<QV4::FunctionObject>() == nullptr)
    {
        location.status = SourceStatus::NotCallable;
        return location;
    }
    const QV4::JavaScriptFunctionObject * pFunctionObject = scoped->as<QV4::JavaScriptFunctionObject>();
    if (pFunctionObject == nullptr || pFunctionObject->function() == nullptr)
    {
        location.kind = FunctionKind::Native;
        location.status = SourceStatus::NativeFunction;
        return location;
    }
    const QV4::Function * pFunction = pFunctionObject->function();
    if (pFunction->compiledFunction == nullptr)
    {
        location.status = SourceStatus::UnsupportedKind;
        return location;
    }
    location.script = pFunction->sourceFile();
    // not sourceLocation(): it truncates the line to quint16
    location.line = static_cast<qint32>(pFunction->compiledFunction->location.line());
    location.column = static_cast<qint32>(pFunction->compiledFunction->location.column());
    location.formalCount = static_cast<qint32>(pFunction->nFormals);
    if (pFunction->kind == QV4::Function::Kind::Eval)
    {
        location.kind = FunctionKind::Eval;
    }
    else if (pFunction->isArrowFunction())
    {
        location.kind = FunctionKind::Arrow;
    }
    else
    {
        location.kind = FunctionKind::Script;
    }
    location.status = SourceStatus::Ok;
#else
    Q_UNUSED(engine)
    Q_UNUSED(function)
#endif
    return location;
}

QString extract(const QString & scriptText, const Location & location, SourceStatus & status)
{
    if (location.status != SourceStatus::Ok)
    {
        status = location.status;
        return QString();
    }
    if (scriptText.isEmpty())
    {
        status = SourceStatus::NoScriptText;
        return QString();
    }
    const qint32 offset = getOffsetForPosition(scriptText, location.line, location.column);
    if (offset < 0)
    {
        status = SourceStatus::NotAtFunctionKeyword;
        return QString();
    }
    const qint32 start = findFunctionKeyword(scriptText, offset);
    if (start < 0)
    {
        status = SourceStatus::NotAtFunctionKeyword;
        return QString();
    }
    const qint32 end = findFunctionEnd(scriptText, start);
    if (end < 0)
    {
        status = SourceStatus::NoFunctionEnd;
        return QString();
    }
    status = SourceStatus::Ok;
    return scriptText.mid(start, end - start + 1);
}

}
