let pkgs = import ~/nix/nixpkgs {};
in
  pkgs.gcc11Stdenv.mkDerivation {
    name = "pratt-parser-env";
    hardeningDisable = [ "all" ];
    buildInputs = with pkgs; [ cmake gdb clang_12 ];
  }
