# Introduction #

This is a demo of a syntax highlighter done in C++/Qt.
The code is extraeamly non optimized, but it does work most of the time.
The class is very slow, so please don't use it on large files.

The syntax highlighter definitions are the same ones used by the gtksourceview library
for gtk. The difinitions are passed to a class which does the highligting. This means
that the engine is not aware of what he is coloring, and adding new languages is as
trivial as adding new XML to the langs directory.

See the html documentation inside the source tarball for more information

![https://qtedit4.googlecode.com/svn/tools/qtsourceview/demos/demo5/demo5-explain.png](https://qtedit4.googlecode.com/svn/tools/qtsourceview/demos/demo5/demo5-explain.png)

# License #
All code is licensed under the terms of the LGPL.

The tests available in this release are taken from the Kate project.

The color definitions are self made, and released to the public domain (the syntax is part of the kate syntax definitions)

The highlight definitions are part of the gtksourceview library:
> http://gtksourceview.sourceforge.net/

The parenthesis mathing is based on the example on QtQuarterly #31, http://doc.trolltech.com/qq/QtQuarterly31.pdf

# Get the code #

You can get the released tarballs at the qtedit4 downloads page: http://code.google.com/p/qtedit4/downloads/list


SVN/Last (stable) release checkout:
```
svn co http://qtedit4.googlecode.com/svn/tags/qtsourceview_0_0_3/
```

SVN/Trunk (uunstable) checkout:
```
svn co http://qtedit4.googlecode.com/svn/tools/qtsourceview/
```

## Changelog ##

QtSourceView - 30 Jul 2011 - 0.0.3
  * Hilight engine: Support for non case-sensitve languages
  * new demos: a text editor control (demo4, demo5)
  * Qate: a new highlighter based on the highlighter from QtCreator (uses qate language definitions)
  * About bloody time


QtSourceView - 18 Aug 06 - 0.0.2
  * Second release
  * Added 3 new language definitions from GtkSourceView: scheme, octave, changelog
  * Added 1 new original language: Qt project file
  * Several bug fixes
  * All examples are made with the designer now
  * Minimal doxygen documentation

QtSourceView - 22 Jul 06 - 0.0.1
  * First release