{
    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    };

    outputs = { systems, nixpkgs, ... }@inputs:
        let
            eachSystem = f: nixpkgs.lib.genAttrs systems (system: f nixpkgs.legacyPackages.${system});
            systems = [
                "aarch64-darwin"
                "aarch64-linux"
                "x86_64-darwin"
                "x86_64-linux"
            ];
        in {
            packages = eachSystem (pkgs: rec {
                CommanderWars = pkgs.kdePackages.callPackage ./CommanderWars.nix { };
            });

            devShells = eachSystem (pkgs: {
                default = pkgs.mkShell (with pkgs; {
                    buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qttools qt6.qtmultimedia libressl ];
                    nativeBuildInputs = [ cmake pkg-config ];
                });
            });
        };
}