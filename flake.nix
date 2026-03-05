{
  description = "A simple flake for a simple project";

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
      mkMyEnv =
        _name: _getPkgs: _pkgs:
        _pkgs.buildEnv {
          name = _name;
          paths = _getPkgs _pkgs;
        };
      mkMyShell =
        _symlinkName: _getPkgs: _pkgs:
        let
          myEnv = mkMyEnv _symlinkName _getPkgs _pkgs;
          myPkgs = _getPkgs _pkgs;
          shellHook = ''
            export CC=clang
            export CXX=clang++
            ${_pkgs.nix}/bin/nix-store --add-root ./${_symlinkName} --realise ${myEnv}
          '';
          rawShell = _pkgs.mkShell {
            buildInputs = myPkgs;
          };
        in
        rawShell.overrideAttrs { inherit shellHook; };
      # The list of packages to be included in the development shell.
      getPkgs =
        _pkgs: with _pkgs; [
          python3
          clang
          cmake
          rustc
          cargo
          cmake-format
          ruff
        ];
    in
    {
      devShells = forAllSystems (pkgs: {
        default = mkMyShell ".nix-env" getPkgs pkgs;
      });
    };
}
