let pkgs = import <nixos-unstable> {};
in
  pkgs.gcc10Stdenv.mkDerivation {
    name = "pratt-parser-env";
    hardeningDisable = [ "all" ];
    buildInputs = with pkgs; [ cmake gdb clang_11 ];
  }
