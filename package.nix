{
  pkgs,
  rustPlatform,
  lib,
  ...
}:
rustPlatform.buildRustPackage rec {
  pname = "my-rust-project";
  version = "1.0.0";
  src = builtins.path {
    name = pname;
    path = ./.;
  };
  cargoHash = "";
}
