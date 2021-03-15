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

        bool empty() const {return _root.isNull();}

        QString  getCurrentFolder() const { return _path; }
        QString         getPath(QString attrName) const;
        QDomElement        getNode() const {return _root;}
        float               getScaleFactor() const {return _scaleFactor;}
        bool                getLoad() const {return _load;}
        bool                getAlphaHitTest() const { return _alphaHitTest; }
        QString         getType() const {return _root.nodeName();}

        void                checkSetAttributes();

        XmlWalker       next();

    private:
        void _checkSetAttributes(QDomElement node);
        QString _path;

        QDomElement _root;
        QDomElement _last;

        bool _notStarted;

        float _scaleFactor;
        bool _load;
        bool _alphaHitTest;
    };


    class CreateResourceContext
    {
    public:
        CreateResourceContext() : resources(0), xml_name(""), prebuilt_folder(""), options(0),
            walker("", 1.0f, true, false, QDomElement())
        {
        }

        Resources* resources;
        QString xml_name;
        QString prebuilt_folder;
        const ResourcesLoadOptions* options;
        XmlWalker walker;
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
        static SingleThreadResourcesContext instance;
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spNativeTexture t) override;
    };

    class RestoreResourcesContext: public LoadResourcesContext
    {
    public:
        static RestoreResourcesContext instance;
        void createTexture(const CreateTextureTask& opt) override;
        bool isNeedProceed(spNativeTexture t) override;
    };
}
