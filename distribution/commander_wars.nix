{ stdenv, lib, cmake, qt6, libressl, wrapQtAppsHook, pkg-config, pname }:

stdenv.mkDerivation {
    inherit pname;
    version = let
        buildScript = builtins.readFile ../CMakeLists.txt;
        lines = lib.strings.splitString "\r\n" buildScript;
        versionLine = lib.lists.findFirst (lib.strings.hasInfix "COW_ANDROID_VERSION_NAME") "\"0.0.0\"" lines;
        calculatedVersion = builtins.elemAt (lib.strings.splitString "\"" versionLine) 1;
    in
        calculatedVersion;

    src = ./.;
    meta.mainProgram = "commander_wars";

    cmakeFlags = [
        "-DOPENSSL_USE_STATIC_LIBS:BOOL=OFF"
    ];

    buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qttools qt6.qtmultimedia libressl ];
    nativeBuildInputs = [ cmake wrapQtAppsHook pkg-config ];
}
