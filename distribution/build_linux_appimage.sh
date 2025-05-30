#!/usr/bin/env bash

set -eu

# Set the directory to the repository root
dir="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
cd "$dir/.."

# Download linuxdeploy
LINUXDEPLOY="distribution/linuxdeploy-x86_64-2.0.0-alpha-1-20241106.AppImage"
LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20250213-2/linuxdeploy-x86_64.AppImage"                 

LINUXDEPLOY_QT="distribution/linuxdeploy-plugin-qt-x86_64-2.0.0-patched.AppImage"
LINUXDEPLOY_QT_URL="https://github.com/Lymia/linuxdeploy-plugin-qt/releases/download/patch-exclude-library/linuxdeploy-plugin-qt-static-x86_64.AppImage"

if [ ! -f $LINUXDEPLOY ]; then
    wget -O $LINUXDEPLOY $LINUXDEPLOY_URL
    chmod +x $LINUXDEPLOY
fi

if [ ! -f $LINUXDEPLOY_QT ]; then
    wget -O $LINUXDEPLOY_QT $LINUXDEPLOY_QT_URL
    chmod +x $LINUXDEPLOY_QT
fi

# Check for the server flag
SIDE="client"
SIDE_DESKTOP="Client"

if [ "${1-}" = "-server" ]; then
    SIDE="server"
    SIDE_DESKTOP="Server"
    shift
fi

# Build the appdir for the client
export CC="clang" CXX="clang++" QMAKE
QMAKE="$(which qmake6)"

rm -rfv distribution/AppDir ||:
cmake . -DCMAKE_BUILD_TYPE=Release "$@" \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DOPENSSL_USE_STATIC_LIBS:BOOL=OFF
    
make -j"$(nproc)" install DESTDIR=distribution/AppDir

EXCLUDES_LIST="
    --exclude-library *libqcertonlybackend*
    --exclude-library *libqsqlmysql*
    --exclude-library *libqsqlodbc*
    --exclude-library *libqsqlmimer*
    --exclude-library *libqsqlpsql*
    --exclude-library *libgallium*
    --exclude-library *libLLVM*
    --exclude-library *libqicns*
    --exclude-library *libqmng*
    --exclude-library *libqtga*
    --exclude-library *libqtiff*
    --exclude-library *libqwbmp*
    --exclude-library *libqwebp*
    --exclude-library *libgstreamermediaplugin*
"
$LINUXDEPLOY --appdir=distribution/AppDir \
    -i distribution/res/icons/ico${SIDE}_linux_16.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_32.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_64.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_128.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_256.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_512.png --icon-filename=commander_wars_ico${SIDE} \
    -i distribution/res/icons/ico${SIDE}_linux_scalable.svg --icon-filename=commander_wars_ico${SIDE} \
    -d distribution/res/Commander_Wars_${SIDE_DESKTOP}.desktop
$LINUXDEPLOY_QT --appdir=distribution/AppDir $EXCLUDES_LIST
$LINUXDEPLOY --appdir=distribution/AppDir $EXCLUDES_LIST --output appimage
