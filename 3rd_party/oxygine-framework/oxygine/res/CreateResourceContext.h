#pragma once
#include "../oxygine-include.h"
#include "../core/Object.h"
#include "../pugixml/pugixml.hpp"
#include <string>

namespace oxygine
{
    /**internal class*/
    class XmlWalker
    {
    public:
        XmlWalker(const std::string* xmlFolder,
                  const std::string& path,
                  float scaleFactor,
                  bool load, bool alpha,
                  pugi::xml_node xml);

        bool empty() const {return _root.empty();}

        const std::string&  getCurrentFolder() const { return _path; }
        const std::string&  getXmlFolder() const { return *_xmlFolder; }
        std::string         getPath(const char* attrName) const;
        pugi::xml_node      getNode() const {return _root;}
        float               getScaleFactor() const {return _scaleFactor;}
        bool                getLoad() const {return _load;}
        bool                getAlphaHitTest() const { return _alphaHitTest; }
        const char*         getType() const {return _root.name();}

        void                checkSetAttributes();
        void                setXmlFolder(const std::string* ptr) { _xmlFolder = ptr; }


        XmlWalker       next();

    private:
        void            _checkSetAttributes(pugi::xml_node node);
        std::string connectPath(const char* currentPath, const char* str);

        const std::string* _xmlFolder;
        std::string _path;

        pugi::xml_node _root;
        pugi::xml_node _last;

        bool _notStarted;

        float _scaleFactor;
        bool _load;
        bool _alphaHitTest;
    };


    class CreateResourceContext
    {
    public:
        CreateResourceContext() : resources(0), xml_name(0), prebuilt_folder(0), options(0),
            walker(0, "", 1.0f, true, false, pugi::xml_node())
        {
        }

        Resources* resources;
        const std::string* xml_name;
        const std::string* prebuilt_folder;
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
        bool linearFilter;
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
