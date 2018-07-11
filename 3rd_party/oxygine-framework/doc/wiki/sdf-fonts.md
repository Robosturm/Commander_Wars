Oxygine natively supports SDF (Signed Distance Field) fonts.
SDF is a type of Bitmap fonts. SDF allows the fonts to be resized without any noticeable artifacts.

# SDF font creation
SDF font is created based on a regular black-and-white bitmap font (you may create it using [BMFont] (http://www.angelcode.com/products/bmfont/)).
It is recommended to generate a font with the size 400pt, without smoothing, indents of 45x45x45x45 and image size of 4096x4096. 
Bit depth: 32.
Export the font in png format.

![](img/sdf1.png)

Next, we create the SDF font itself using [ImageMagick](http://www.imagemagick.org/script/index.php) 

    convert font.png -filter Jinc ( +clone -negate -morphology Distance Euclidean -level 50%,-50% ) -morphology Distance Euclidean -compose Plus -composite -level 43%,57% -resize 12.5% font.png
On Ubuntu we had to screen the brackets (bash).
    
    convert font.png -filter Jinc \( +clone -negate -morphology Distance Euclidean -level 50%,-50% \) -morphology Distance Euclidean -compose Plus -composite -level 43%,57% -resize 12.5% font.png

The size of the image decreased by a factor of 8 (from 4096 to 512). In order for it to work in Oxygine correctly, the image must be in 32-bit-png format.

![](img/sdf3.png)

# Loading the font
The font may be declared in resources.xml:

    <sdfont file="font.fnt" downsample="8" /> 

or loaded at runtime:

    sdfont.initSD("sdf/font.fnt", 8);
    sdfont.load();

# Usage
SDF font allows for outline, scale and weight adjustment without noticeable artifacts.

    txt->setOutline(myOutline);
    txt->setScale(myScale);
    txt->setWeight(myWeight);

![](img/sdf2.png)

SDF also supports any styling options that are supported by a regular bitmap font.
    
    TextStyle st;
    st.vAlign = TextStyle::VALIGN_MIDDLE;
    st.hAlign = TextStyle::HALIGN_MIDDLE;
    st.color = Color::CornflowerBlue;

    st.multiline = true;

    st.outlineColor = Color::White;

    txt->setStyle(st);
