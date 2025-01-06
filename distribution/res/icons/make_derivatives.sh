#!/usr/bin/env bash

set -eu

rm -rfv v0.3.tar.gz blocky-0.3

# Download blocky
wget https://github.com/starulli/blocky/archive/refs/tags/v0.3.tar.gz
tar xvf v0.3.tar.gz
cd blocky-0.3
make build
cd ..

# Convert icons
convert_ico() {
    # Icon size recommendations from https://blog.icons8.com/articles/choosing-the-right-size-and-format-for-icons/

    IN="$1"
    OUT="$2"

    # Convert to SVG and render the master icon.
    blocky-0.3/blocky "$IN" | sed "s/svg /svg width=\"512px\" height=\"512px\" /" > ${OUT}_linux_scalable.svg
    inkscape ${OUT}_linux_scalable.svg -o ${OUT}_linux_512.png

    # Convert the large icon down into smaller icons for Linux
    convert ${OUT}_linux_512.png -filter Point -resize 256x256 ${OUT}_linux_256.png
    convert ${OUT}_linux_512.png -filter Point -resize 128x128 ${OUT}_linux_128.png
    convert ${OUT}_linux_512.png -filter Point -resize 64x64 ${OUT}_linux_64.png
    convert ${OUT}_linux_512.png -filter Point -resize 32x32 ${OUT}_linux_32.png
    convert ${OUT}_linux_512.png -filter Point -resize 16x16 ${OUT}_linux_16.png

    # Convert the large icon into specifically sized icons for Windows
    convert ${OUT}_linux_512.png -filter Point -resize 20x20 ${OUT}_temp_20.png
    convert ${OUT}_linux_512.png -filter Point -resize 24x24 ${OUT}_temp_24.png
    convert ${OUT}_linux_512.png -filter Point -resize 30x30 ${OUT}_temp_30.png
    convert ${OUT}_linux_512.png -filter Point -resize 36x36 ${OUT}_temp_36.png
    convert ${OUT}_linux_512.png -filter Point -resize 40x40 ${OUT}_temp_40.png
    convert ${OUT}_linux_512.png -filter Point -resize 48x48 ${OUT}_temp_48.png
    convert ${OUT}_linux_512.png -filter Point -resize 60x60 ${OUT}_temp_60.png
    convert ${OUT}_linux_512.png -filter Point -resize 72x72 ${OUT}_temp_72.png
    convert ${OUT}_linux_512.png -filter Point -resize 80x80 ${OUT}_temp_80.png
    convert ${OUT}_linux_512.png -filter Point -resize 96x96 ${OUT}_temp_96.png

    # Combine into the Windows icon
    convert ${OUT}_*.png -colors 256 ${OUT}_windows.ico
}
convert_ico orig_client_icon.png icoclient
convert_ico orig_server_icon.png icoserver

# Cleanup
rm -rfv v0.3.tar.gz blocky-0.3 *_temp_*
