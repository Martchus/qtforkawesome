# Qt ForkAwesome
Library that bundles [ForkAwesome](https://forkaweso.me) for use within Qt
applications

## Build instructions

### Dependencies
The library depends on the following Qt modules (version 5.6 or higher):
`core`, `gui`

The additional library for Qt Quick integration (currently only providing a
`QQuickImageProvider`) depends on the following further Qt modules: `quick`

At build time `qtutilities` and `c++utilities` are required. This library is
built in the same way as these libraries so checkout the `c++utilities`
repository for detailed instructions.

To generate the header with icon definitions, Perl and the module `YAML::XS` (or
`YAML` or `YAML::Tiny`) are required. To use a specific Perl binary, one can set
the CMake variable `PERL_BIN`. Under Windows, one can simply install
`perl-YAML-Tiny` via MSYS2 and set `PERL_BIN` to the path of `perl.exe` from the
MSYS2 installation.

### Providing the font file
Of course the font file and icon definitions for ForkAwesome need to be
supplied as well. If none of the variables mentioned in the next paragraph are
specified, the build system will attempt to download the files from GitHub
automatically.

To supply the files manually, just download the `ttf`, `woff` or `woff2`
file from
[ForkAwesome's fonts directory](https://github.com/ForkAwesome/Fork-Awesome/tree/master/fonts)
and the
[icon definitions file](https://github.com/ForkAwesome/Fork-Awesome/blob/master/src/icons/icons.yml).
Then specify the path of the downloaded files via the CMake variables
`FORK_AWESOME_FONT_FILE` and `FORK_AWESOME_ICON_DEFINITIONS`, e.g. add
`-DFORK_AWESOME_FONT_FILE=/path/to/Fork-Awesome/fonts/forkawesome-webfont.woff2`
and
`-DFORK_AWESOME_ICON_DEFINITIONS=/path/to/Fork-Awesome/src/icons/icons.yml`
to the CMake invocation. The font file will be built into the library and
is hence only required at build time.

The Web Open Font Format (the `.woff`/`.woff2` file) might not be supported by
the font renderer. Notably, Window's native font rendering which Qt uses by
default does *not* support it. Then it makes sense to use the True Type Font
instead. Alternatively, one can force the platform plugin to use FreeType2 (e.g.
by setting `QT_QPA_PLATFORM=windows:fontengine=freetype`). Recent versions of
FreeType2 even support Web Open Font Format 2 (the `.woff2` file) but this
requires FreeType2 to be configured with Brotli support.

### Examples
Checkout https://github.com/Martchus/PKGBUILDs/tree/master/qtforkawesome for
examples to build for GNU/Linux and Windows against Qt 5 and Qt 6.

### Further remarks
As usual with CMake projects, it is possible to control whether to build as
static or shared library via the `BUILD_SHARED_LIBS` variable. If you build
qtforkawesome as part of another project and only want to affect qtforkawesome,
you can also use the variable `QT_FORK_AWESOME_BUILD_SHARED_LIBS`.

## Usage
The installation provides a CMake find module called `qtforkawesome` which
provides the imported target `qtforkawesome`. A pkg-config file is provided as
well. Once configured, the library is fairly simple to use:

```
#include <qtforkawesome/icon.h>
#include <qtforkawesome/renderer.h>

auto renderer = QtForkAwesome::Renderer();
renderer.pixmap(QtForkAwesome::Icon::Globe, QSize(64, 64), Qt::black);
```

There's also `renderer.render(…)` which takes a `QPainter` directly.

### Icon engine
A `QIconEnginePlugin` is provided as well. When it is loaded one can
create a `QIcon` using a file name with `.fa` extension, e.g.:

```
const auto icon = QIcon(QStringLiteral("qrcode:blue.fa"));
```

The color will be deduced from the applications default color palette unless it
is specified explicitly like in the example above.

To link against the plugin statically, find the CMake module
`qtforkawesomeiconengine`, link against the imported target
`qtforkawesomeiconengine` and add `Q_IMPORT_PLUGIN(ForkAwesomeIconEnginePlugin)`
to one of your source files.

### QQuickImageProvider
A `QQuickImageProvider` is provided as well in form of the additional library
`qtquickforkawesome`.

Then just include the header:

```
#include <qtquickforkawesome/imageprovider.h>
```

Create an instance and add it to your `QQmlEngine`:

```
engine->addImageProvider(QStringLiteral("fa"), new QtForkAwesome::QuickImageProvider(renderer));
```

And use it like this:
```
Image {
    source: "image://fa/syncthing"
}
```

### Bundling
It is also possible to build the library as part of your project. Simply add
it via `add_subdirectory`. Checkout the
[Syncthing Tray's project file](https://github.com/Martchus/syncthingtray/blob/master/CMakeLists.txt)
for an example.

## Copyright notice and license
Copyright © 2021-2025 Marius Kittler

All code is licensed under [GPL-2-or-later](LICENSE).
