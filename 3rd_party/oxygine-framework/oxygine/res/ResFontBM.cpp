#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Font.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include "qtextstream.h"

namespace oxygine
{
    Resource* ResFontBM::create(CreateResourceContext& context)
    {
        ResFontBM* font = nullptr;

        font = new ResFontBM();
        font->_createFont(&context, false, false, 1);
        setNode(font, context.walker.getNode());
        context.resources->add(font);

        //context.meta = context.meta.next_sibling();

        return font;
    }

    Resource* ResFontBM::createBM(CreateResourceContext& context)
    {
        ResFontBM* font = nullptr;

        font = new ResFontBM();
        font->_createFont(&context, false, true, 1);
        setNode(font, context.walker.getNode());
        context.resources->add(font);

        return font;
    }

    Resource* ResFontBM::createSD(CreateResourceContext& context)
    {
        ResFontBM* font = nullptr;

        font = new ResFontBM();
        font->_createFont(&context, true, false, 1);
        setNode(font, context.walker.getNode());
        context.resources->add(font);

        //context.meta = context.meta.next_sibling();

        return font;
    }


    ResFontBM::ResFontBM()
        : _font(nullptr),
          _sdf(false),
          _format(ImageData::TF_R8G8B8A8),
          _premultipliedAlpha(false)
    {

    }

    ResFontBM::~ResFontBM()
    {
        cleanup();
    }

    const oxygine::Font* ResFontBM::getClosestFont(float, int styleFontSize, float& resScale) const
    {
        if (!styleFontSize)
        {
            styleFontSize = _size;
        }

        float scale = _size / float(styleFontSize) * _font->getScale();
        resScale = scale;
        return _font.get();
    }

    void ResFontBM::init(QString path, bool premultipliedAlpha)
    {
        _premultipliedAlpha = premultipliedAlpha;
        _file = path;
        _createFont(nullptr, false, false, 1);
    }

    void ResFontBM::cleanup()
    {
        for (pages::iterator i = _pages.begin(); i != _pages.end(); ++i)
        {
            const page& p = *i;
            p.texture->release();
        }
        _pages.clear();
        _font = nullptr;
        _loadCounter = 0;
    }

    void ResFontBM::_restore(Restorable* r, void*)
    {
        void* object = r->_getRestorableObject();
        for (pages::iterator i = _pages.begin(); i != _pages.end(); ++i)
        {
            const page& p = *i;
            if (p.texture.get() == object)
            {
                _loadPage(p, &RestoreResourcesContext::instance);
            }
        }
    }

    void ResFontBM::_loadPage(const page& p, LoadResourcesContext* load_context)
    {
        if (!load_context->isNeedProceed(p.texture))
        {
            return;
        }

        spImage mt = new Image;

        QImage img(p.file);
        mt->init(img, !_premultipliedAlpha);
        CreateTextureTask opt;
        opt.src = mt;
        opt.dest = p.texture;
        opt.linearFilter = m_linearFilter;
        opt.clamp2edge = m_clamp2edge;
        load_context->createTexture(opt);
        p.texture->reg(Restorable::RestoreCallback(this, &ResFontBM::_restore), nullptr);
    }

    void ResFontBM::_load(LoadResourcesContext* load_context)
    {
        Q_ASSERT(!_pages.empty());
        if (_pages.empty())
        {
            return;
        }

        for (pages::iterator i = _pages.begin(); i != _pages.end(); ++i)
        {
            const page& p = *i;
            _loadPage(p, load_context);
        }
    }

    void ResFontBM::addPage(int tw, int th, QString head, QString textureFile)
    {
        page p;
        if (!head.isEmpty())
        {
            p.file = head;
            p.file += "//";
        }
        p.file += textureFile;
        p.texture = IVideoDriver::instance->createTexture();
        p.texture->setName(p.file);

        if (tw)
        {
            p.texture->init(0, tw, th, ImageData::TF_UNDEFINED);
        }

        _pages.push_back(p);
    }

    void ResFontBM::loadBase(QDomElement node)
    {
        QVariant value = QVariant(node.attribute("linearFilter"));
        if (value.type() == QVariant::Type::String &&
            !value.isNull())
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
        if (value.type() == QVariant::Type::String &&
            !value.isNull())
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
        glyphOptions opt = 0;
        const glyph* g = _font->getGlyph(0xA0, opt);
        if (g)
        {
            return;
        }

        g = _font->getGlyph(' ', opt);
        if (!g)
        {
            return;
        }

        glyph p = *g;
        p.ch = 0xA0;
        _font->addGlyph(p);

    }

    void ResFontBM::_createFont(CreateResourceContext* context, bool, bool bmc, int downsample)
    {
        _sdf = false;

        if (context)
        {
            QDomElement node = context->walker.getNode();
            bool ok = false;
            downsample = node.attribute("downsample").toInt(&ok);
            if (!ok)
            {
                downsample = 1;
            }
            QVariant value(node.attribute("premultiplied_alpha"));
            if (value.type() == QVariant::Type::Bool)
            {
                _premultipliedAlpha = value.toBool();
            }

            _file = context->walker.getPath("file");
            setName(Resource::extractID(node, _file, ""));

            if (bmc)
            {
                _file = context->prebuilt_folder + getName() + ".fnt";
            }
        }

        QString path = _file;
        QFile file(path);
        if (!file.exists())
        {
            return;
        }
        else
        {
            file.open(QIODevice::ReadOnly);
        }

        QDomDocument doc;
        bool isXml = doc.setContent(&file);

        if (!isXml)
        {
            return;
        }
        /////////////////////////////////////////////////

        QDomElement root = doc.documentElement();
        QDomElement info = root.firstChildElement("info");

        //<info face="Century Gothic" size="-24" bold="0" italic="0" charset="" unicode="1" stretchH="100" smooth="1" aa="1" padding="0,0,0,0" spacing="1,1" outline="0"/>
        int fontSize = info.attribute("size").toInt() / downsample;

        QDomElement common = root.firstChildElement("common");
        int lineHeight = common.attribute("lineHeight").toInt() / downsample;
        int base = common.attribute("base").toInt() / downsample;
        int tw = common.attribute("scaleW").toInt();
        int th = common.attribute("scaleH").toInt();

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
            load(nullptr);
        }

        fontSize = qAbs(fontSize);
        Font* font = new Font();
        font->init(getName(), fontSize, fontSize, lineHeight + fontSize - base, _sdf);
        _size = fontSize;
        _font = font;

        if (context)
        {
            float scale = 1.0f / context->walker.getScaleFactor();
            _font->setScale(scale);
        }

        QDomElement chars = root.firstChildElement("chars");
        QDomElement child = chars.firstChildElement();
        while (!child.isNull())
        {
            int charID = 0;
            int xpos = 0;
            int ypos = 0;
            int width = 0;
            int height = 0;
            int xoffset = 0;
            int yoffset = 0;
            int xadvance = 0;
            int page = 0;

            auto attr = child.attributes();
            for (qint32 i = 0; i < attr.size(); i++)
            {
                QDomNode attribute = attr.item(i);
                QString attr_name = attribute.nodeName();
                int value = attribute.nodeValue().toInt();

                if (attr_name == "id")
                    charID = value;
                else if (attr_name == "x")
                    xpos = value;
                else if (attr_name == "y")
                    ypos = value;
                else if (attr_name == "width")
                    width = value;
                else if (attr_name == "height")
                    height = value;
                else if (attr_name == "xoffset")
                    xoffset = value;
                else if (attr_name == "yoffset")
                    yoffset = value;
                else if (attr_name == "xadvance")
                    xadvance = value;
                else if (attr_name == "page")
                    page = value;
            }

            spTexture t = _pages[page].texture;
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
            gl.opt = 0;
            gl.texture = _pages[page].texture;

            font->addGlyph(gl);

            child = child.nextSiblingElement();
        }

        font->sortGlyphs();
        _finalize();
    }

    void ResFontBM::_unload()
    {
        Q_ASSERT(!_pages.empty());
        for (pages::iterator i = _pages.begin(); i != _pages.end(); ++i)
        {
            const page& p = *i;
            p.texture->release();
        }
    }

    const Font* ResFontBM::getFont(QString, int) const
    {
        return _font.get();
    }
}
