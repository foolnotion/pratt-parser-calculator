{
  description = "pratt-parser dev";
  nixConfig.bash-prompt = "\n\\[\\e[93m\\e[1m\\][pratt-parser-dev:\\[\\e[92m\\e[1m\\]\\w]$\\[\\e[0m\\] ";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nur.url = "github:nix-community/NUR";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/master";

  outputs = { self, flake-utils, nixpkgs, nur }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ nur.overlay ];
          };
        in rec
        {
          defaultPackage = pkgs.gcc11Stdenv.mkDerivation {
            name = "pratt-parser";
            src = self;
            nativeBuildInputs = with pkgs; [ cmake ];
            buildInputs = with pkgs; [
                pkgs.nur.repos.foolnotion.fast_float
                doctest
              ];
          };

          devShell = pkgs.gcc11Stdenv.mkDerivation {
            name = "pratt-parser-dev";
          };
        }
      );
}
