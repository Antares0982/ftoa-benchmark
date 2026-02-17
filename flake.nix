{
  description = "A simple flake for a simple rust project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      forAllSystems =
        function:
        nixpkgs.lib.genAttrs
          [
            "x86_64-linux"
            "aarch64-linux"
            "x86_64-darwin"
            "aarch64-darwin"
          ]
          (
            system:
            function (
              import nixpkgs {
                inherit system;
              }
            )
          );
    in
    {
      devShells = forAllSystems (pkgs: rec {
        default = pkgs.mkShell {
          buildInputs = with pkgs; [
            python3
            clang
            cmake
            rustc
            cargo
            cmake-format
            ruff
          ];
        };
      });
    };
}
