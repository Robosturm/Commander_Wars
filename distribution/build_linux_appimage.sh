#!/usr/bin/env bash

set -eu

# Set the directory to the repository root
dir="$(cd -P -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
cd "$dir/.."

# Download linuxdeploy
LINUXDEPLOY="distribution/linuxdeploy-x86_64-2.0.0-alpha-1-20241106.AppImage"
LINUXDEPLOY_URL="https://github.com/linuxdeploy/linuxdeploy/releases/download/2.0.0-alpha-1-20241106/linuxdeploy-x86_64.AppImage"

LINUXDEPLOY_QT="distribution/linuxdeploy-plugin-qt-x86_64-2.0.0-alpha-1-20241106.AppImage"
LINUXDEPLOY_QT_URL="https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/2.0.0-alpha-1-20241106/linuxdeploy-plugin-qt-x86_64.AppImage"

if [ ! -f $LINUXDEPLOY ]; then
    wget -O $LINUXDEPLOY $LINUXDEPLOY_URL
    chmod +x $LINUXDEPLOY
fi

if [ ! -f $LINUXDEPLOY_QT ]; then
    wget -O $LINUXDEPLOY_QT $LINUXDEPLOY_QT_URL
    chmod +x $LINUXDEPLOY_QT
fi

# Build the appdir for the client
rm -rfv distribution/AppDir ||:
cmake . -DCMAKE_BUILD_TYPE=Release -DOPENSSL_USE_STATIC_LIBS:BOOL=OFF "$@"
make -j"$(nproc)"

export QMAKE
QMAKE="$(which qmake6)"
$LINUXDEPLOY --appdir=distribution/AppDir \
    -e commander_wars \
    -i distribution/res/commander_wars_32.png --icon-filename=commander_wars \
    -i distribution/res/commander_wars_64.png --icon-filename=commander_wars \
    -i distribution/res/commander_wars_128.png --icon-filename=commander_wars \
    -i distribution/res/commander_wars_256.png --icon-filename=commander_wars \
    -i distribution/res/commander_wars.svg --icon-filename=commander_wars \
    -d distribution/res/Commander_Wars.desktop \
    --plugin qt
rm -v distribution/AppDir/usr/lib/{libgallium*,libLLVM*} # Unneeded libraries that are excessively large.
$LINUXDEPLOY --appdir=distribution/AppDir --output appimage \
    --exclude-library "*libgallium*" --exclude-library "*libLLVM*"