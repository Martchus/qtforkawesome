# qtforkawesome
Library that bundles ForkAwesome for use within Qt applications

## Build instructions
The library depends on the following Qt modules (version 5.6 or higher):
`core`, `gui`

At build time `qtutilities` and `c++utilities` are required. This library is
built in the same way as these libraries so checkout the `c++utilities`
repository for detailed instructions.

To generate the header with icon definitions, Perl and the module `YAML::XS` (or
`YAML`) are required.

Of course the font file and icon definitions for ForkAwesome also need to be
supplied as well.
Just download it from https://github.com/ForkAwesome/Fork-Awesome/releases
and specify its path via the CMake variables `FORK_AWESOME_FONT_FILE`
and `FORK_AWESOME_ICON_DEFINITIONS`, e.g. add
`-DFORK_AWESOME_FONT_FILE=/path/to/Fork-Awesome-1.2.0/fonts/forkawesome-webfont.woff2`
and
`-DFORK_AWESOME_ICON_DEFINITIONS=/path/to/Fork-Awesome-1.2.0/src/icons/icons.yml`
to the CMake invocation. The font file will be built into the library and
is hence only required at build time. If none of the variables are specified,
the build system will attempt to download the files from GitHub.

Note that Web Open Font Format 2 support might require FreeType2 to be
configured with Brotli support.

## Usage
The installation provides a CMake find module and a pkg-config file. Once
configured, the library is fairly simple to use:

```
#include <qtforkawesome/icon.h>
#include <qtforkawesome/renderer.h>

auto renderer = QtForkAwesome::Renderer();
renderer.pixmap(QtForkAwesome::Icon::Globe, QSize(64, 64), Qt::black);
```

There's also `renderer.render(…)` which takes a `QPainter` directly.
