{
  description = "pratt-parser dev";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.foolnotion.url = "github:foolnotion/nur-pkg";
  inputs.nixpkgs.url = "github:nixos/nixpkgs/master";

  outputs = { self, flake-utils, nixpkgs, foolnotion }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ foolnotion.overlay ];
          };
        in rec
        {
          packages.default = pkgs.stdenv.mkDerivation {
            name = "pratt-parser";
            src = self;
            nativeBuildInputs = with pkgs; [ cmake ];

            buildInputs = with pkgs; [
                doctest
                fast_float
              ];
          };

          devShells.default = pkgs.gcc11Stdenv.mkDerivation {
            name = "pratt-parser-dev";

            nativeBuildInputs = packages.default.nativeBuildInputs;
            buildInputs = packages.default.buildInputs;
          };
        }
      );
}
