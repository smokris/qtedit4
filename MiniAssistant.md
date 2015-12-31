# Introduction #
The MiniAssistant is a small widget, which can be embbeded
within your application to provide a similar experience
as the full bloat Qt4-Assitant, with some constrains.


This is a stand alone dockable widget which is completely
self contained, and has constructed mainly of a tab widget which
separates the GUI to the index tab and the Mini-Browser.

The Mini Assistant has the following features:
  * Can load DCF files used by Qt4 documentation
  * Can display the Qt4 documentation
  * Can display a list of references available in the loaded DCF file
  * Can filter the list of pages "on the fly"
  * The Mini Browser has a "URL" edit for quick acess to pages, which supports completion
  * Tracks the history of visited help pages, and lists them as popular pages
  * Can scan a QTextEdit a give suggestions to help pages
  * Uses the model view controller support provided by Qt4
  * All internal classes are contained in an MiniAssistantInt internal namespace

![![](http://qt-apps.org/CONTENT/content-m1/m67392-1.png)](http://qtedit4.googlecode.com/svn/tags/miniassistant_0_0_1/doc/miniassistant-index.png)
![![](http://qt-apps.org/CONTENT/content-m2/m67392-2.png)](http://qtedit4.googlecode.com/svn/tags/miniassistant_0_0_1/doc/miniassistant-minibrowser-linux.png)

QtApps home page:
http://qt-apps.org/content/show.php?content=67392

# Get the code #

You can get the released tarballs at the qtedit4 downloads page: http://code.google.com/p/qtedit4/downloads/list


SVN/Last (stable) release checkout:
```
svn co http://qtedit4.googlecode.com/svn/tags/miniassistant_0_0_1
```

SVN/Trunk (uunstable) checkout:
```
svn co http://qtedit4.googlecode.com/svn/tools/miniassistant/
```


# License #
The project is licensed under the terms of the LGPL 2, which is available in the same
directory as this readme.