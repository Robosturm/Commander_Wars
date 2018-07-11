This will enable native build and debug in eclipse, plus code navigation and completion.
I assume you already set-up eclipse, cdt, adt (with ndk support).
The procedure has been tested on linux, with the game/part5 example.

**1) Import app project into eclipse workspace**

- In eclipse, go to "File -> Import -> Import Android Code Into Workspace"
- Navigate to the project dir and add it to the workspace

**2) import "lib" and "extension" library projects**

Oxygine projects have some dependencies. Command line build scripts take care of this (through ant). In eclipse you will have to import the these library projects by hand (in the same way you did for the app project):

- oxygine-framework/oxigine/SDL/android/lib
- oxygine-framework/oxigine/SDL/android/extension

Now try building the app. If you get errors about lib.jar and extension.jar not existing, try unchecking and re-checking the "is Library" toggle for both the library projects (right click on the project's directory and select Properties -> Android"). Try running the library projects before launching the app project again (ignore the library project's errors).

**3) symlink data directory to assets directory**

The command line ant-based system uses the asset.dir variable to set a custom asset directory for the android app project. Apparently, this is not possible with eclipse. The solution for linux and mac is to symlink the "data" dir to "proj.android/assets". A similar trick may work for windows. Or you can manually copy the "data" dir contents to proj.android/assets. 

**4) add c++ nature to the app project**

Right click on the app's workspace folder, and select "Android Tools -> Add native support..."
You will be asked for a custom library name, you can input a dummy name, since the android.mk is already present and it will not be substituted by a default one.

**5) setup native build**

in [project]/jni/src/Android.mk, add: 


```
add $(call import-add-path,$(OXYGINE_ROOT))

```

before the import-module line, where OXYGINE_ROOT is the framework's root dir (the one containing oxigine-framework, SDL and oxygine-sound). The path must be relative to the project directory (proj.android)

For example, for the Game/part5 example, it's:


```
add $(call import-add-path,../../../../../)

```

If you run the app now, native libs will be built as well.

**6) setup debug configuration**

- In project's properties (right-click on project's dir in workspace -> properties), go to "C/C++ build" -> "Manage configurations"
- in the "manage configuration" dialogs, create a new configuration named "Debug" (or whatever). Copy settings from default config.
- from the same dialog, set the debug configuration as the active one
- close the dialog, in the "C/C++" panel, select the "debug" configuration (it should be marked as [active])
- uncheck "Use default build command" and set "ndk-build NDK_DEBUG=1" as the buil command
- right-click on the project and select "Debug as -> Android Native Application"

The app should now be launched in debug mode and the debugger should attach to it and automatically switch eclipse to the Debug perspective. This won't be of great use without being able to set breakpoints. For this we will need to browse our sources in eclipse.

**7) Add app sources to eclipse project**

This is both useful for setting breakpoints and for browsing the sources and autocompletion.

- go to project's properties, and select "C/C++ General -> Paths and Symbols"
- in the "Source location" panel, click on "Link folder"
- indicate the path to the game's "src" dir. you can insert relative paths using the ${PARENT-n-PROJECT_LOC} eclipse variable. Substitute "n" with the desired parent level. For example, for the game/part5 example, you could indicate ${PARENT-1-PROJECT_LOC}/src 
- give the linked directory a name (eg: app_src). Note that "src" is already taken by the java sources dir!
- Close properties window with OK
- Right-click on project dir in workspace, select "Index->Rebuild" and "Index->Freshen All Files" (the latter being proably superflous)

Now you should be able to navigate the sources and set breakpoints with a double click on the left margin of a source. If you see errors in the source it means the indexing has failed (if you try building the code without opening the sources it would in fact build ok).

In that case go to project's properties and:

- Go to "C/C++ general" -> "Indexer"
- Check "Enable project specific settings", "Enable Indexer", "Index source files not included...", "Index unused headers as c++ files", "Index unused headers as C files", "Index source and header files opened in editor", "Allow heuristic resolution of includes", "Use active build configuration"(some of them may be not needed, but just in case...).  Leave the rest unchecked.
- Rebuild the index.
- If you still have issues, close and restart eclipse.