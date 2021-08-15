#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include <QDomDocument>

namespace oxygine
{
    class Image;
    using spImage = intrusive_ptr<Image>;

    /**internal class*/
    class XmlWalker
    {
    public:
        explicit XmlWalker(QString path,
                  float scaleFactor,
                  bool load, bool alpha,
                  QDomElement xml);
        virtual ~XmlWalker() = default;

        bool empty() const {return m_root.isNull();}

        QString  getCurrentFolder() const { return m_path; }
        QString     getPath(QString attrName) const;
        QDomElement getNode() const {return m_root;}
        float       getScaleFactor() const {return m_scaleFactor;}
        bool        getLoad() const {return m_load;}
        bool        getAlphaHitTest() const { return m_alphaHitTest; }
        QString     getType() const {return m_root.nodeName();}

        void                checkSetAttributes();

        XmlWalker       next();

    private:
        void _checkSetAttributes(QDomElement node);

    private:
        QString m_path;
        QDomElement m_root;
        QDomElement m_last;
        bool m_notStarted;
        float m_scaleFactor;
        bool m_load;
        bool m_alphaHitTest;
    };

    class CreateResourceContext
    {
    public:
        explicit CreateResourceContext()
            : m_resources(0),
              m_xml_name(""),
              m_prebuilt_folder(""),
              m_options(0),
              m_walker("", 1.0f, true, false, QDomElement())
        {
        }
        virtual ~CreateResourceContext() = default;

        Resources* m_resources;
        QString m_xml_name;
        QString m_prebuilt_folder;
        const ResourcesLoadOptions* m_options;
        XmlWalker m_walker;
    };

    class CreateTextureTask
    {
    public:
        explicit CreateTextureTask();
        virtual ~CreateTextureTask() = default;
        spImage src;
        spTexture dest;
        quint32 linearFilter;
        bool clamp2edge;

        void ready() const;
    };

    /**internal class*/
    class LoadResourcesContext
    {
    public:
        static LoadResourcesContext* get();
        explicit LoadResourcesContext() = default;
        virtual ~LoadResourcesContext() = default;


        virtual void createTexture(const CreateTextureTask& opt) = 0;
        virtual bool isNeedProceed(spTexture t) = 0;
    };

    class SingleThreadResourcesContext: public LoadResourcesContext
    {
    public:
        explicit SingleThreadResourcesContext() = default;
        virtual ~SingleThreadResourcesContext() = default;
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spTexture t) override;
    public:
        static SingleThreadResourcesContext m_instance;
    };

    class RestoreResourcesContext: public LoadResourcesContext
    {
    public:
        explicit RestoreResourcesContext() = default;
        virtual ~RestoreResourcesContext() = default;
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spTexture t) override;
    public:
        static RestoreResourcesContext m_instance;
    };
}
