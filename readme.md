# exqudens-conan-libusb

## how-to-create-conan-package

1. `mkdir tmp`
1. download zip file from `https://github.com/libusb/libusb/archive/refs/tags/v1.0.26.zip`
1. extract zip file to `tmp/libusb-1.0.26`
1. check file `tmp/libusb-1.0.26/README.md` exists
1. create file `tmp/conanfile.py` with content:

```
from pathlib import Path

required_conan_version = ">=2.0"

from conan import ConanFile
from conan.tools.files import copy

class ConanConfiguration(ConanFile):
    name: str = 'github-libusb'
    version: str = '1.0.26'

    def package(self):
        try:
            copy(self, pattern="*", src=Path(self.build_folder).as_posix(), dst=Path(self.package_folder).as_posix())
        except Exception as e:
            self.output.error(e)
            raise e
```

1. `conan export-pkg --output-folder tmp/libusb-1.0.26 tmp/conanfile.py`
1. check `conan list 'github-libusb/1.0.26:*'`
1. check `conan cache path 'github-libusb/1.0.26:${conan list-output-packages[0]}'`
1. check `ls -1a ${conan-cache-path-output}`
1. *(optional)* `conan upload github-libusb/1.0.26 --remote gitlab`

## how-to-test-all-presets

1. `git clean -xdf`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target cmake-test || exit 255"`

## how-to-build-all-presets

1. `git clean -xdf`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target cmake-install || exit 255"`

## how-to-export-all-presets

1. `conan list 'libusb/*'`
1. `conan remove -c 'libusb'`
1. `git clean -xdf`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --preset {} || exit 255"`
1. `cmake --list-presets | cut -d ':' -f2 | xargs -I '{}' echo '{}' | xargs -I '{}' bash -c "cmake --build --preset {} --target conan-export || exit 255"`

## vscode

1. `git clean -xdf`
1. `cmake --preset ${preset}`
1. `cmake --build --preset ${preset} --target vscode`
1. use vscode debugger launch configurations: `test-app`

### extensions

For `command-variable-launch.json`
use [Command Variable](https://marketplace.visualstudio.com/items?itemName=rioj7.command-variable#pickstringremember) `version >= v1.69.0`
