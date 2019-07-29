#include "selectkey.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

SelectKey::SelectKey(SDL_Keycode code)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    m_Button = ObjectManager::createButton("");
    m_Button->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        oxygine::TextField* pText = dynamic_cast<oxygine::TextField*>(m_Button->getFirstChild().get());
        pText->setHtmlText(tr("Press Key").toStdString().c_str());
        active = true;
    });
    addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event * )->void
    {
        setKeycode(currentCode);
    });
    addChild(m_Button);
    setKeycode(code);
    connect(pApp, &Mainapp::sigKeyDown, this, &SelectKey::keyInput, Qt::QueuedConnection);
}

void SelectKey::keyInput(SDL_Event event)
{
    if (active)
    {
        SDL_Keycode cur = event.key.keysym.sym;
        setKeycode(cur);
    }
}

void SelectKey::setKeycode(SDL_Keycode code)
{
    QString codeText = tr("Unknown");
    const oxygine::Font* pFont = FontManager::getMainFont()->getFont();
    if (code == SDLK_SPACE)
    {
        codeText = tr("Space");
    }
    else if (code == SDLK_F2)
    {
        codeText = tr("F2");
    }
    else if (code == SDLK_F3)
    {
        codeText = tr("F3");
    }
    else if (code == SDLK_F4)
    {
        codeText = tr("F4");
    }
    else if (code == SDLK_F5)
    {
        codeText = tr("F5");
    }
    else if (code == SDLK_F6)
    {
        codeText = tr("F6");
    }
    else if (code == SDLK_F7)
    {
        codeText = tr("F7");
    }
    else if (code == SDLK_F8)
    {
        codeText = tr("F8");
    }
    else if (code == SDLK_F9)
    {
        codeText = tr("F9");
    }
    else if (code == SDLK_F10)
    {
        codeText = tr("F10");
    }
    else if (code == SDLK_F11)
    {
        codeText = tr("F11");
    }
    else if (code == SDLK_F12)
    {
        codeText = tr("F12");
    }
    else if (code == SDLK_UP)
    {
        codeText = tr("Up");
    }
    else if (code == SDLK_DOWN)
    {
        codeText = tr("Down");
    }
    else if (code == SDLK_LEFT)
    {
        codeText = tr("Left");
    }
    else if (code == SDLK_RIGHT)
    {
        codeText = tr("Right");
    }
    else if (code == SDLK_RETURN)
    {
        codeText = tr("Return");
    }
    else if (code == SDLK_TAB)
    {
        codeText = tr("Tab");
    }
    else if (code <= 255)
    {
        const oxygine::glyph* pGlyph = pFont->getGlyph(code, oxygine::glyphOptions());
        if (pGlyph != nullptr)
        {
            codeText = static_cast<char>(code);
        }
    }
    if (codeText != tr("Unknown"))
    {
        currentCode = code;
        oxygine::TextField* pText = dynamic_cast<oxygine::TextField*>(m_Button->getFirstChild().get());
        pText->setHtmlText((tr("Key ") + codeText).toStdString().c_str());
        active = false;
        emit sigKeyChanged(currentCode);
    }
    else
    {
        setKeycode(currentCode);
    }
}
