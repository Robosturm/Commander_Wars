#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include <qvariant.h>

#include "coreengine/console.h"

namespace oxygine
{
    static load_texture_hook _hook = nullptr;

    void load_texture_internal(QString file, spNativeTexture nt, quint32 linearFilter, bool clamp2edge, LoadResourcesContext* load_context)
    {
        ImageData im;
        spImage mt = spImage::create();

        Console::print("loading atlas: " + file, Console::eDEBUG);
        QImage img (file);
        Console::print("atlas file loaded: " + file, Console::eDEBUG);
        mt->init(img, true);
        im = mt->lock();
        Console::print("atlas size: " + QString::number(im.m_w) + " " + QString::number(im.m_h), Console::eDEBUG);

        CreateTextureTask opt;
        opt.src = mt;
        opt.dest = nt;
        opt.linearFilter = linearFilter;
        opt.clamp2edge = clamp2edge;
        load_context->createTexture(opt);
    }

    void load_texture(QString file, spNativeTexture nt, quint32 linearFilter, bool clamp2edge, LoadResourcesContext* load_context)
    {
        if (_hook)
        {
            _hook(file, nt, linearFilter, clamp2edge, load_context);
            return;
        }

        load_texture_internal(file, nt, linearFilter, clamp2edge, load_context);
    }


    void ResAtlas::init_resAnim(spResAnim rs, QString file, QDomElement node)
    {
        rs->setName(Resource::extractID(node, file, ""));
        setNode(rs, node);
    }

    quint32 ResAtlas::getLinearFilter() const
    {
        return m_linearFilter;
    }

    void ResAtlas::setLinearFilter(quint32 linearFilter)
    {
        m_linearFilter = linearFilter;
        // apply filter
        for (auto & atlas : m_atlasses)
        {
            if (atlas.alpha.get() != nullptr)
            {
                atlas.alpha->setLinearFilter(linearFilter);
            }
            if (atlas.base.get() != nullptr)
            {
                atlas.base->setLinearFilter(linearFilter);
            }
        }
    }

    void ResAtlas::addAtlas(ImageData::TextureFormat tf, QString base, QString alpha, qint32 w, qint32 h)
    {
        atlas atl;
        atl.base = IVideoDriver::instance->createTexture();
        atl.base_path = base;
        atl.base->init(0, w, h, tf);

        if (!alpha.isEmpty())
        {
            atl.alpha = IVideoDriver::instance->createTexture();
            atl.alpha_path = alpha;
            atl.alpha->init(0, w, h, tf);
        }

        m_atlasses.push_back(atl);
    }

    spResource ResAtlas::create(CreateResourceContext& context)
    {
        context.m_walker.checkSetAttributes();
        spResAtlas ra = nullptr;
        spResAtlasGeneric rs = spResAtlasGeneric::create();
        rs->loadAtlas(context);
        ra = rs;

        ra->setName(Resource::extractID(context.m_walker.getNode(), "", QString("!atlas:") + context.m_xml_name));
        context.m_resources->add(ra);
        setNode(ra, context.m_walker.getNode());

        return ra;
    }

    ResAtlas::ResAtlas()
        : m_linearFilter(GL_LINEAR),
          m_clamp2edge(true)
    {

    }

    ResAtlas::~ResAtlas()
    {
        for (atlasses::iterator i = m_atlasses.begin(); i != m_atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (atl.base)
            {
                atl.base->release();
            }
            if (atl.alpha)
            {
                atl.alpha->release();
            }
        }
    }


    void ResAtlas::loadBase(QDomElement node)
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

    void ResAtlas::_restore(Restorable* r)
    {
        NativeTexture* texture = dynamic_cast<NativeTexture*>(r->_getRestorableObject());

        for (atlasses::iterator i = m_atlasses.begin(); i != m_atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (atl.base.get() == texture)
            {
                load_texture(atl.base_path, atl.base, m_linearFilter, m_clamp2edge, &RestoreResourcesContext::m_instance);
                atl.base->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore));
                break;
            }

            if (atl.alpha.get() == texture)
            {
                load_texture(atl.alpha_path, atl.alpha, m_linearFilter, m_clamp2edge, &RestoreResourcesContext::m_instance);
                atl.alpha->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore));
                break;
            }
        }
    }

    void ResAtlas::_load(LoadResourcesContext* load_context)
    {
        for (atlasses::iterator i = m_atlasses.begin(); i != m_atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (!load_context->isNeedProceed(atl.base))
            {
                continue;
            }
            load_texture(atl.base_path, atl.base, m_linearFilter, m_clamp2edge, load_context);
            atl.base->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore));

            if (atl.alpha)
            {
                load_texture(atl.alpha_path, atl.alpha, m_linearFilter, m_clamp2edge, load_context);
                atl.alpha->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore));
            }
        }
    }

    void ResAtlas::_unload()
    {
        for (atlasses::iterator i = m_atlasses.begin(); i != m_atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (atl.base)
            {
                atl.base->release();
            }
            if (atl.alpha)
            {
                atl.alpha->release();
            }
        }
    }

    spResAnim ResAtlas::createEmpty(const XmlWalker& walker, CreateResourceContext& context)
    {
        spResAnim ra = spResAnim::create(this);
        ra->init(nullptr, 0, 0, walker.getScaleFactor());
        init_resAnim(ra, "", walker.getNode());
        ra->setParent(this);
        context.m_resources->add(ra);

        return ra;
    }
}
