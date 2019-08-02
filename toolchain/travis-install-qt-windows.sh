set -eu -o pipefail

if [ -e "$QT5_BASE_DIR/bin/moc.exe" ]; then
    echo "Found an existing Qt installation at $QT5_BASE_DIR"
    exit
fi

echo "Downloading the installer..."
# https is of no use if it redirects to a http mirror...
curl -vLo ~/qt-unified-windows-x86-online.exe http://download.qt.io/official_releases/online_installers/qt-unified-windows-x86-online.exe

echo "Installing..."
# Run installer and save the installer output. To avoid hitting the timeout,
# periodically print some progress. On error, show the full log and abort.
~/qt-unified-windows-x86-online.exe --verbose --script toolchain/qt-installer-windows.qs |
    tee ~/qt-installer-output.txt |
    toolchain/report-progress.sh ||
    (cat ~/qt-installer-output.txt; exit 1)

printf 'Installation size: '
du -sm "$QT5_BASE_DIR" 2>&1 ||
    (cat ~/qt-installer-output.txt; exit 1)