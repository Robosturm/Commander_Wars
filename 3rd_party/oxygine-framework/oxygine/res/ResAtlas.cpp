#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlasGeneric.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"

#include <qvariant.h>

namespace oxygine
{
    static load_texture_hook _hook = nullptr;
    void set_load_texture_hook(load_texture_hook hook)
    {
        _hook = hook;
    }

    void load_texture_internal(QString file, spNativeTexture nt, quint32 linearFilter, bool clamp2edge, LoadResourcesContext* load_context)
    {
        ImageData im;
        spImage mt = spImage::create();

        qDebug("loading atlas: %s", file.toStdString().c_str());
        QImage img (file);
        qDebug("atlas file loaded: %s", file.toStdString().c_str());
        mt->init(img, true);
        im = mt->lock();
        qDebug("atlas size: %d %d", im.w, im.h);

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
        return _linearFilter;
    }

    void ResAtlas::setLinearFilter(quint32 linearFilter)
    {
        _linearFilter = linearFilter;
        // apply filter
        for (auto & atlas : _atlasses)
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

        _atlasses.push_back(atl);
    }

    spResource ResAtlas::create(CreateResourceContext& context)
    {
        context.walker.checkSetAttributes();
        spResAtlas ra = nullptr;
        spResAtlasGeneric rs = spResAtlasGeneric::create();
        rs->loadAtlas(context);
        ra = rs;

        ra->setName(Resource::extractID(context.walker.getNode(), "", QString("!atlas:") + context.xml_name));
        context.resources->add(ra);
        setNode(ra, context.walker.getNode());

        return ra;
    }

    ResAtlas::ResAtlas()
        : _linearFilter(GL_LINEAR),
          _clamp2edge(true)
    {

    }

    ResAtlas::~ResAtlas()
    {
        for (atlasses::iterator i = _atlasses.begin(); i != _atlasses.end(); ++i)
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
                _linearFilter = GL_LINEAR;
            }
            else
            {
                _linearFilter = GL_NEAREST;
            }
        }
        else
        {
            _linearFilter = GL_LINEAR;
        }
        value = QVariant(node.attribute("clamp2edge"));
        if (value.type() == QVariant::Type::String &&
            !value.isNull())
        {
            _clamp2edge = value.toBool();
        }
        else
        {
            _clamp2edge = true;
        }
    }

    void ResAtlas::_restore(Restorable* r, void*)
    {
        NativeTexture* texture = (NativeTexture*)r->_getRestorableObject();

        for (atlasses::iterator i = _atlasses.begin(); i != _atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (atl.base.get() == texture)
            {
                load_texture(atl.base_path, atl.base, _linearFilter, _clamp2edge, &RestoreResourcesContext::instance);
                atl.base->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore), nullptr);
                break;
            }

            if (atl.alpha.get() == texture)
            {
                load_texture(atl.alpha_path, atl.alpha, _linearFilter, _clamp2edge, &RestoreResourcesContext::instance);
                atl.alpha->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore), nullptr);
                break;
            }
        }
    }

    void ResAtlas::_load(LoadResourcesContext* load_context)
    {
        for (atlasses::iterator i = _atlasses.begin(); i != _atlasses.end(); ++i)
        {
            atlas& atl = *i;
            if (!load_context->isNeedProceed(atl.base))
            {
                continue;
            }
            load_texture(atl.base_path, atl.base, _linearFilter, _clamp2edge, load_context);
            atl.base->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore), nullptr);

            if (atl.alpha)
            {
                load_texture(atl.alpha_path, atl.alpha, _linearFilter, _clamp2edge, load_context);
                atl.alpha->reg(Restorable::RestoreCallback(this, &ResAtlas::_restore), nullptr);
            }
        }
    }

    void ResAtlas::_unload()
    {
        for (atlasses::iterator i = _atlasses.begin(); i != _atlasses.end(); ++i)
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
        context.resources->add(ra);

        return ra;
    }
}
