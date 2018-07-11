##FreeType
repository https://github.com/oxygine/oxygine-freetype

#1. include header
```cpp
#include "ResFontFT.h"
```

#2. initialize
```cpp
ResFontFT::initLibrary();
```

#3. declare your font in resources.xml
```xml
<ftfont file="verdana.ttf" />
```

#4. use
```cpp
ResFont *font = resources.getResFont("verdana");

spTextField text = new TextField();
text->attachTo(getStage());
text->setFont(font);
text->setFontSize(30);
text->setText("Hello World!");
```

#5. release
```cpp
ResFontFT::freeLibrary();
```