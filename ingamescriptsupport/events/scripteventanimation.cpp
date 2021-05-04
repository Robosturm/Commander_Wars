#include "qcoreapplication.h"

#include "ingamescriptsupport/events/scripteventanimation.h"

#include "coreengine/settings.h"
#include "coreengine/mainapp.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/gameanimationmanager.h"

#include "objects/base/spinbox.h"
#include "objects/base/dropdownmenucolor.h"
#include "objects/dialogs/filedialog.h"
#include "objects/base/label.h"

ScriptEventAnimation::ScriptEventAnimation()
    : ScriptEvent(EventType::animation)
{
    connect(this, &ScriptEventAnimation::sigShowLoadDialog, this, &ScriptEventAnimation::showLoadDialog, Qt::QueuedConnection);
}

void ScriptEventAnimation::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("var animation = GameAnimationFactory.createAnimation(", "")
                        .replace("\", ", ",")
                        .replace(", \"", ",")
                        .replace(", ", ",")
                        .replace("); animation.addSprite3(\"", ",")
                        .replace("); // ", ",").split(",");
    if (items.size() >= 11)
    {
        x = items[0].toInt();
        y = items[1].toInt();
        sprite = items[2];
        offsetX = items[3].toInt();
        offsetY = items[4].toInt();
        color = items[5];
        sleepAfterFinish = items[6].toInt();
        scaleX = items[7].toFloat();
        scaleY = items[8].toFloat();
        delay = items[9].toInt();
        frames = items[10].toInt();
    }
}

void ScriptEventAnimation::writeEvent(QTextStream& rStream)
{
     rStream <<  "            var animation = GameAnimationFactory.createAnimation("
             << QString::number(x) << ", "
             << QString::number(y) << "); animation.addSprite3(\""
             << sprite << "\", "
             << QString::number(offsetX) << ", "
             << QString::number(offsetY) << ", \""
             << color.name() << "\", "
             << QString::number(sleepAfterFinish) << ", "
             << QString::number(scaleX) << ", "
             << QString::number(scaleY) << ", "
             << QString::number(delay) << ", "
             << QString::number(frames) << "); // "
             << QString::number(getVersion()) << " " << EventAnimation << "\n";
}

void ScriptEventAnimation::showEditEvent(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    qint32 y = 30;

    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("X: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Field X on which the animation will be shown."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(x);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Y: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setTooltipText(tr("Field Y on which the animation will be shown."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(getY());
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Offset X: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -9999, 9999);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("X-Offset of the animation. Moving it away from the center."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(offsetX);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setOffsetX(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Offset Y: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, -9999, 9999);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("Y-Offset of the animation. Moving it away from the center."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(offsetY);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setOffsetY(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Color: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    QVector<QColor> colors = {Qt::white};
    spDropDownmenuColor colorBox = spDropDownmenuColor::create(150, colors);
    colorBox->setTooltipText(tr("Recoloring color for the animation sprite."));
    colorBox->setPosition(width, y);
    colorBox->setCurrentItem(color);
    connect(colorBox.get(), &DropDownmenuColor::sigItemChanged, [=](QColor value)
    {
        setColor(value);
    });
    pBox->addItem(colorBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Sleep after finish: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("Time in ms the animation remains visible after all frames were played."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(sleepAfterFinish);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setSleepAfterFinish(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Scale X: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999, SpinBox::Mode::Float);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("X-Scaling of the actual sprite."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(scaleX);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setScaleX(static_cast<float>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Scale Y: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999, SpinBox::Mode::Float);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("Y-Scaling of the actual sprite."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(scaleY);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setScaleY(static_cast<float>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Delay: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 99999);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("Time in ms before the animation gets played."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(delay);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setDelay(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Sprite: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    m_pTextbox = spTextbox::create(Settings::getWidth() - 200 - width);
    m_pTextbox->setTooltipText(tr("Current select animation."));
    m_pTextbox->setPosition(width, y);
    m_pTextbox->setCurrentText(sprite);
    connect(m_pTextbox.get(), &Textbox::sigTextChanged, [=](QString text)
    {
        setSprite(text);
    });
    pBox->addItem(m_pTextbox);
    oxygine::spButton pButtonSelect = ObjectManager::getInstance()->createButton(tr("Select Image"), 150);
    pButtonSelect->setPosition(width + m_pTextbox->getWidth() + 10, y);
    pBox->addItem(pButtonSelect);
    pButtonSelect->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
         emit sigShowLoadDialog();
    });
    y += 40;

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Frames: "));
    pText->setPosition(30, y);
    pBox->addItem(pText);
    spinBox = spSpinBox::create(300, 0, 9999);
    spinBox->setInfinityValue(std::numeric_limits<qint32>::min());
    spinBox->setTooltipText(tr("Amount of frames for the animation. Only has an effect on custom animations."));
    spinBox->setPosition(width, y);
    spinBox->setCurrentValue(frames);
    connect(spinBox.get(), &SpinBox::sigValueChanged, [=](qreal value)
    {
        setFrames(static_cast<qint32>(value));
    });
    pBox->addItem(spinBox);
    y += 40;

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}

void ScriptEventAnimation::showLoadDialog()
{
    
    QVector<QString> wildcards;
    wildcards.append("*.png");
    QString path = QCoreApplication::applicationDirPath() + "/resources/images/animations/";
    spFileDialog fileDialog = spFileDialog::create(path, wildcards, sprite, true);
    connect(fileDialog.get(),  &FileDialog::sigFileSelected, this, &ScriptEventAnimation::selectAnimation, Qt::QueuedConnection);
    oxygine::getStage()->addChild(fileDialog);
    
}

void ScriptEventAnimation::selectAnimation(QString id)
{
    QString imageId = id.replace(QCoreApplication::applicationDirPath() + "/", "");
    QString animationId = id.replace("resources/images/animations/", "").replace("/", "").replace(".png", "");
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(animationId, oxygine::error_policy::ep_ignore_error);
    if (pAnim != nullptr)
    {
        m_pTextbox->setCurrentText(animationId);
    }
    else
    {
        m_pTextbox->setCurrentText(imageId);
    }
    sprite = m_pTextbox->getCurrentText();
}

QString ScriptEventAnimation::getSprite() const
{
    return sprite;
}

void ScriptEventAnimation::setSprite(const QString &value)
{
    sprite = value;
}

qint32 ScriptEventAnimation::getFrames() const
{
    return frames;
}

void ScriptEventAnimation::setFrames(const qint32 &value)
{
    frames = value;
}

qint32 ScriptEventAnimation::getX() const
{
    return x;
}

void ScriptEventAnimation::setX(const qint32 &value)
{
    x = value;
}

qint32 ScriptEventAnimation::getY() const
{
    return y;
}

void ScriptEventAnimation::setY(const qint32 &value)
{
    y = value;
}

qint32 ScriptEventAnimation::getOffsetX() const
{
    return offsetX;
}

void ScriptEventAnimation::setOffsetX(const qint32 &value)
{
    offsetX = value;
}

qint32 ScriptEventAnimation::getOffsetY() const
{
    return offsetY;
}

void ScriptEventAnimation::setOffsetY(const qint32 &value)
{
    offsetY = value;
}

QColor ScriptEventAnimation::getColor() const
{
    return color;
}

void ScriptEventAnimation::setColor(const QColor &value)
{
    color = value;
}

qint32 ScriptEventAnimation::getSleepAfterFinish() const
{
    return sleepAfterFinish;
}

void ScriptEventAnimation::setSleepAfterFinish(const qint32 &value)
{
    sleepAfterFinish = value;
}

float ScriptEventAnimation::getScaleX() const
{
    return scaleX;
}

void ScriptEventAnimation::setScaleX(float value)
{
    scaleX = value;
}

float ScriptEventAnimation::getScaleY() const
{
    return scaleY;
}

void ScriptEventAnimation::setScaleY(float value)
{
    scaleY = value;
}

qint32 ScriptEventAnimation::getDelay() const
{
    return delay;
}

void ScriptEventAnimation::setDelay(const qint32 &value)
{
    delay = value;
}
