{ stdenv, lib, cmake, qt6, libressl, wrapQtAppsHook, pkg-config }:

stdenv.mkDerivation {
    pname = "commander_wars";
    version = "1.0";

    src = ./.;

    cmakeFlags = [
        "-DOPENSSL_USE_STATIC_LIBS:BOOL=OFF"
    ];

    buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qttools qt6.qtmultimedia libressl ];
    nativeBuildInputs = [ cmake wrapQtAppsHook pkg-config ];
}