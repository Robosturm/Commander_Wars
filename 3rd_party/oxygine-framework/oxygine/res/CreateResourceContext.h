#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/Object.h"
#include <QDomDocument>

namespace oxygine
{
    /**internal class*/
    class XmlWalker
    {
    public:
        XmlWalker(QString path,
                  float scaleFactor,
                  bool load, bool alpha,
                  QDomElement xml);

        bool empty() const {return m_root.isNull();}

        QString  getCurrentFolder() const { return m_path; }
        QString         getPath(QString attrName) const;
        QDomElement        getNode() const {return m_root;}
        float               getScaleFactor() const {return m_scaleFactor;}
        bool                getLoad() const {return m_load;}
        bool                getAlphaHitTest() const { return m_alphaHitTest; }
        QString         getType() const {return m_root.nodeName();}

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
        CreateResourceContext() : m_resources(0), m_xml_name(""), m_prebuilt_folder(""), m_options(0),
            m_walker("", 1.0f, true, false, QDomElement())
        {
        }

        Resources* m_resources;
        QString m_xml_name;
        QString m_prebuilt_folder;
        const ResourcesLoadOptions* m_options;
        XmlWalker m_walker;
    };

    DECLARE_SMART(Image, spImage);
    DECLARE_SMART(NativeTexture, spNativeTexture);


    class CreateTextureTask
    {
    public:
        CreateTextureTask();

        spImage src;
        spNativeTexture dest;
        quint32 linearFilter;
        bool clamp2edge;

        void ready() const;
    };

    /**internal class*/
    class LoadResourcesContext
    {
    public:
        static LoadResourcesContext* get();

        virtual ~LoadResourcesContext() {}


        virtual void createTexture(const CreateTextureTask& opt) = 0;
        virtual bool isNeedProceed(spNativeTexture t) = 0;
    };

    class SingleThreadResourcesContext: public LoadResourcesContext
    {
    public:
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spNativeTexture t) override;
    public:
        static SingleThreadResourcesContext m_instance;
    };

    class RestoreResourcesContext: public LoadResourcesContext
    {
    public:
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spNativeTexture t) override;
    public:
        static RestoreResourcesContext m_instance;
    };
}
