let pkgs = import <nixos> {};
in
  pkgs.gcc10Stdenv.mkDerivation {
    name = "pratt-parser-env";
    hardeningDisable = [ "all" ];
    buildInputs = with pkgs; [ cmake gdb clang-tools ];
  }
