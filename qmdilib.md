# Introduction #

This is a simple MDI library for Qt 4. The goal of this project,
is to provide an easy way to modifying menus and toolbars of windows
on demand. This can be used to present different menus and toolbars to
widgets inside a QTabWidget, hiding or showing a different set of menus
on demand or even enabling a small plugin system.

The library is fully documented, and contains some demonstrations. It should compile
flawlessly (no warnings) with a single "qmake; make" on the root directory of the
project. It is known to work on Windows and Linux, and there is no reason why it
should not work on other platforms supported by Qt4. Currently only gcc is tested,
but other compilers should be able to compile this library

![![](http://qt-apps.org/CONTENT/content-m1/m61365-1.png)](http://qtedit4.googlecode.com/svn/tools/qmdilib/doc/mdi-tab-2.jpg)
![![](http://qt-apps.org/CONTENT/content-m2/m61365-2.png)](http://qtedit4.googlecode.com/svn/tools/qmdilib/doc/mdi-tab-3.jpg)
![![](http://qt-apps.org/CONTENT/content-m3/m61365-3.png)](http://qtedit4.googlecode.com/svn/tools/qmdilib/doc/qmdilib_basic_view.png)

More images are available on the [SVN ](http://qtedit4.googlecode.com/svn/tools/qmdilib/doc/)

QtApps home page:
http://qt-apps.org/content/show.php/qmdilib?content=61365


# Get the code #

You can get the released tarballs at the qtedit4 downloads page: http://code.google.com/p/qtedit4/downloads/list


SVN/Last (stable) release checkout:
```
svn co http://qtedit4.googlecode.com/svn/tags/qmdilib_0_0_4
```

SVN/Trunk (uunstable) checkout:
```
svn co http://qtedit4.googlecode.com/svn/tools/qmdilib/
```

## Changelog ##
0.0.5 - SOMEDAY - Diego Iastrubni <diegoiast@gmail.com> (not officialy relseased yet, only a developer snapshot based on [revision 304](https://code.google.com/p/qtedit4/source/detail?r=304))
  * code is released now under a dual license: (L)GPL 2 or 3
  * code refactoring: qmdiClient::mdiClientFileName() should be used instead of old "filename"
  * code refactoring: qmdiClient::name has been renamed to qmdiClient::mdiClientName
  * code refactoring: include files for qmdiActionGroup and qmdiActionGroupList changed from
> > "actiongroup.h" and "actiongrouplist.h" into "qmdiactiongroup.h" and qmdiactiongrouplist.h"
> > Transitional includes have been added, to be removed next version, please update your code
  * new feature: qmdiClients are notified of (un)merged from a host
  * new feature: qmdiTabWidget will hide the tabbar if the number of tabs is less then 2
  * new feature: when qmdiHost adds a qmdiClient if that client is also a widget, add it's menus+toolbars actions to the widget.
  * new feature: you can tell qmdiHost not to update it's GUI by changing qmdiHost::updateMenusAndToolBars

0.0.4 - 29 Jun 2007 - Diego Iastrubni
  * Documentation re-created using doxygen 1.4.7
  * code refactoring: qmdiClient - qmdiServer is now a public member
  * new feature: qmdiActionGroup - added support for submenus
  * new feature: qmdiActionGroup - added support for adding QActionGroup
  * new feature: qmdiActionGroup - merge points are computed at run time from list of added groups
  * code refactoring: qmdiHost - updateGUI can has been updated, see documentation
  * code refactoring: qmdiServer - a lot of functions moved from qmdiTabWidget
  * code refactoring: qmdiTabWidget contains less implementation details (see qmdiServer)
  * code refactoring: qmdiTabBar has been removed, qmdiTabWidget uses an event filter now
  * new demo: plugin demo is working now, soon to be ported to the main library

0.0.3 - 7 Oct 2006 - Diego Iastrubni
  * new feature: QWorkspace children are also checked for qmdiClient interface, feature coded by Nox PasNox
  * new feature: merge point for qmdiActionGroup
  * new class: qmdiWorkspace, a qmdiServer based on QWorkspace
  * core refactoring: qmdiClient does not contain QObject self any more
  * code refactoring: qmdiServer now contains a pointer to the host (was in qmdiTabWidget)
  * code refactoring: demos should be easier to read, and hopefully maintain
  * removed most internal frames from all widgets in demos, the demos will look nicer
  * all the documentation has been spell checked

0.0.2 - 11 Jul 06 - Diego Iastrubni <elcuco@kde.org>
  * fixed many flickering issues, now it only flicker when you add QWidgets to qmdiActionGroup
  * added demos (4 and 5) which demonstrate how to build the code into dynamic and static libraries.
  * Documentation updates.
  * Compilation problems on mingw fixed since 0.0.2-pre

0.0.2-pre  - 19 Jul 06 - Diego Iastrubni <elcuco@kde.org>
  * fixed many flickering issues, now it only flickers when you add QWidgets to qmdiActionGroup
  * added demos (4 and 5) which demonstrate how to build the code into dynamic and static libraries

0.0.1 - 11 Jun 06 - Diego Iastrubni <elcuco@kde.org>
  * initial release