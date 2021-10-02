#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include "qtextstream.h"

#include "coreengine/console.h"
#include "coreengine/settings.h"

namespace oxygine
{
    spResource ResFontBM::create(CreateResourceContext& context)
    {
        spResFontBM font = spResFontBM();
        font = spResFontBM::create();
        font->_createFont(&context);
        setNode(font, context.m_walker.getNode());
        context.m_resources->add(font);
        return font;
    }

    ResFontBM::~ResFontBM()
    {
        cleanup();
    }

    const oxygine::Font* ResFontBM::getClosestFont(float, qint32 styleFontSize, float& resScale) const
    {
        if (!styleFontSize)
        {
            styleFontSize = m_size;
        }

        float scale = m_size / float(styleFontSize) * m_font->getScale();
        resScale = scale;
        return m_font.get();
    }

    void ResFontBM::init(QString path, bool premultipliedAlpha)
    {
        m_premultipliedAlpha = premultipliedAlpha;
        m_file = path;
        _createFont(nullptr);
    }

    void ResFontBM::cleanup()
    {
        for (pages::iterator i = m_pages.begin(); i != m_pages.end(); ++i)
        {
            const page& p = *i;
            p.texture->release();
        }
        m_pages.clear();
        m_font = nullptr;
    }

    void ResFontBM::_loadPage(const page& p)
    {
        spImage mt = spImage::create();
        QImage img;
        if (QFile::exists(Settings::getUserPath() + p.file))
        {
           img = QImage(Settings::getUserPath() + p.file);
        }
        else if (QFile::exists(RCC_PREFIX_PATH + p.file))
        {
           img = QImage(RCC_PREFIX_PATH + p.file);
        }
        mt->init(img, !m_premultipliedAlpha);
        p.texture->init(mt->lock());
        p.texture->setLinearFilter(m_linearFilter);
        p.texture->setClamp2Edge(m_clamp2edge);
    }

    void ResFontBM::_load()
    {
        if (m_pages.empty())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "ResFontBM::_load loading empty pages");
        }
        if (m_pages.empty())
        {
            return;
        }

        for (pages::iterator i = m_pages.begin(); i != m_pages.end(); ++i)
        {
            const page& p = *i;
            _loadPage(p);
        }
    }

    void ResFontBM::addPage(qint32 tw, qint32 th, QString head, QString textureFile)
    {
        page p;
        if (!head.isEmpty())
        {
            p.file = head;
            p.file += "//";
        }
        p.file += textureFile;
        p.texture = VideoDriver::instance->createTexture();

        if (tw)
        {
            p.texture->init(0, tw, th, ImageData::TF_UNDEFINED);
        }

        m_pages.push_back(p);
    }

    void ResFontBM::loadBase(QDomElement node)
    {
        QVariant value = QVariant(node.attribute("linearFilter"));
        if (value.typeId() == QMetaType::QString &&
            !value.isNull() &&
            node.hasAttribute("linearFilter"))
        {
            if (value.toBool())
            {
                m_linearFilter = GL_LINEAR;
            }
            else
            {
                m_linearFilter = GL_NEAREST;
            }
        }
        else
        {
            m_linearFilter = GL_LINEAR;
        }
        value = QVariant(node.attribute("clamp2edge"));
        if (value.typeId() == QMetaType::QString &&
            !value.isNull() &&
            node.hasAttribute("clamp2edge"))
        {
            m_clamp2edge = value.toBool();
        }
        else
        {
            m_clamp2edge = true;
        }
    }

    void ResFontBM::_finalize()
    {
        const glyph* g = m_font->getGlyph(0xA0);
        if (g)
        {
            return;
        }

        g = m_font->getGlyph(' ');
        if (!g)
        {
            return;
        }

        glyph p = *g;
        p.ch = 0xA0;
        m_font->addGlyph(p);

    }

    void ResFontBM::_createFont(CreateResourceContext* context)
    {
        qint32 downsample = 1;
        if (context)
        {
            QDomElement node = context->m_walker.getNode();
            bool ok = false;
            downsample = node.attribute("downsample").toInt(&ok);
            if (!ok)
            {
                downsample = 1;
            }
            QVariant value(node.attribute("premultiplied_alpha"));
            if (value.typeId() == QMetaType::Bool)
            {
                m_premultipliedAlpha = value.toBool();
            }

            m_file = context->m_walker.getPath("file");
            setName(Resource::extractID(node, m_file, ""));
        }

        QString path = m_file;
        QFile file(Settings::getUserPath() + path);
        if (!file.exists())
        {
            file.setFileName(QString(RCC_PREFIX_PATH) + path);
            if (!file.exists())
            {
                CONSOLE_PRINT("Unable to load font " + m_file, Console::eFATAL);
                return;
            }
        }
        file.open(QIODevice::ReadOnly);

        QDomDocument doc;
        QString error;
        qint32 line;
        qint32 column;
        bool isXml = doc.setContent(&file, &error, &line, &column);

        if (!isXml)
        {
            CONSOLE_PRINT("Error: " + error + " at line " + QString::number(line) + " at column " + QString::number(column), Console::eERROR);
            return;
        }
        /////////////////////////////////////////////////

        QDomElement root = doc.documentElement();
        QDomElement info = root.firstChildElement("info");

        //<info face="Century Gothic" size="-24" bold="0" italic="0" charset="" unicode="1" stretchH="100" smooth="1" aa="1" padding="0,0,0,0" spacing="1,1" outline="0"/>
        qint32 fontSize = info.attribute("size").toInt() / downsample;

        QDomElement common = root.firstChildElement("common");
        qint32 base = common.attribute("base").toInt() / downsample;
        qint32 tw = common.attribute("scaleW").toInt();
        qint32 th = common.attribute("scaleH").toInt();

        QDomElement pages = root.firstChildElement("pages");
        tw /= downsample;
        th /= downsample;
        loadBase(pages);

        QDir dir = QFileInfo(path).dir();
        for (QDomElement page_node = pages.firstChildElement("page"); !page_node.isNull(); page_node = page_node.nextSiblingElement("page"))
        {
            QString textureFile = page_node.attribute("file");
            addPage(tw, th, dir.path().toStdString().c_str(), textureFile);
        }

        if (!tw)
        {
            load();
        }

        fontSize = qAbs(fontSize);
        spFont font = spFont::create();
        font->init(fontSize, fontSize);
        m_size = fontSize;
        m_font = font;

        if (context)
        {
            float scale = 1.0f / context->m_walker.getScaleFactor();
            m_font->setScale(scale);
        }

        QDomElement chars = root.firstChildElement("chars");
        QDomElement child = chars.firstChildElement();
        while (!child.isNull())
        {
            qint32 charID = 0;
            qint32 xpos = 0;
            qint32 ypos = 0;
            qint32 width = 0;
            qint32 height = 0;
            qint32 xoffset = 0;
            qint32 yoffset = 0;
            qint32 xadvance = 0;
            qint32 page = 0;

            auto attr = child.attributes();
            for (qint32 i = 0; i < attr.size(); i++)
            {
                QDomNode attribute = attr.item(i);
                QString attr_name = attribute.nodeName();
                qint32 value = attribute.nodeValue().toInt();

                if (attr_name == "id")
                {
                    charID = value;
                }
                else if (attr_name == "x")
                {
                    xpos = value;
                }
                else if (attr_name == "y")
                {
                    ypos = value;
                }
                else if (attr_name == "width")
                {
                    width = value;
                }
                else if (attr_name == "height")
                {
                    height = value;
                }
                else if (attr_name == "xoffset")
                {
                    xoffset = value;
                }
                else if (attr_name == "yoffset")
                {
                    yoffset = value;
                }
                else if (attr_name == "xadvance")
                {
                    xadvance = value;
                }
                else if (attr_name == "page")
                {
                    page = value;
                }
            }

            spTexture t = m_pages[page].texture;
            float iw = 1.0f / t->getWidth() / downsample;
            float ih = 1.0f / t->getHeight() / downsample;

            glyph gl;
            gl.src = RectF(xpos * iw, ypos * ih, width * iw, height * ih);
            gl.sw = width / downsample;
            gl.sh = height / downsample;
            gl.offset_x = xoffset / downsample;
            gl.offset_y = yoffset / downsample - base;
            gl.advance_x = xadvance / downsample;
            gl.advance_y = 0;
            gl.ch = charID;
            gl.texture = m_pages[page].texture;

            font->addGlyph(gl);

            child = child.nextSiblingElement();
        }

        font->sortGlyphs();
        _finalize();
    }

    void ResFontBM::_unload()
    {
        if (m_pages.empty())
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "ResFontBM::_unload unloading empty pages");
        }
        for (pages::iterator i = m_pages.begin(); i != m_pages.end(); ++i)
        {
            const page& p = *i;
            p.texture->release();
        }
    }

    const Font* ResFontBM::getFont(QString, int) const
    {
        return m_font.get();
    }
}
