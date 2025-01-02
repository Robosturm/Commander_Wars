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
                commander_wars = pkgs.kdePackages.callPackage ./distribution/commander_wars.nix {
                    pname = "commander_wars";
                };

                default = commander_wars;
            });

            devShells = eachSystem (pkgs: {
                default = pkgs.mkShell (with pkgs; {
                    buildInputs = [ qt6.qtbase qt6.qtdeclarative qt6.qtmultimedia libressl ];
                    nativeBuildInputs = [ cmake pkg-config qt6.qttools ];
                });
            });
        };
}