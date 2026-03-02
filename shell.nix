{ pkgs ? import <nixpkgs> {} }:

(pkgs.buildFHSEnv {
  name = "pio-env";
  targetPkgs = pkgs: (with pkgs; [
    platformio
    gcc
    gnumake
    zlib
  ]);
  runScript = "bash";
}).env
