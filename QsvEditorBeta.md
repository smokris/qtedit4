# Introduction #

The QtSourceView project currently consists of a framework for syntax highlighters.
The project needs also a text editor control, more suitable for code editing.

http://qtedit4.googlecode.com/svn/wiki/Visual-Features.PNG

## Planned features ##
  1. Line numbers, with current line marked as bold
  1. File modification panel
  1. Search/replace/goto line controls (no popups, but inline)
  1. Bookmarks
  1. Display white spaces (space, tabs etc)
  1. Display current line with different color
  1. Lines should have different states, represented by different colors, and an icon representing it's state.
  1. Better text navigation (smart home, more control over scroll bars)
  1. Printer margins
  1. Tab width should be set to different number of spaces
  1. Auto brackets
  1. Display lines which have been modified since the file has been opened
  1. Central way to control all the options available to the editor
  1. Tab width and other options can be set per file (like vim modeline)
  1. Printing support

The editor should also have these characteristics:
  * Highly configurable.
  * Syntax definitions, color schemes definitions (and more) should be loaded on runtime, from files
  * Modern look and feel, but still compatible with the operating system visual guidelines
  * No popups, or as little as possible
  * Fully documented
  * Easy API
  * Should not depend on 3rd party libraries (only Qt4)
  * Derive directly from QTextEdit and not include it (using layouts and other tricks)
  * Easy for 3rd party users to use.

## Get the code ##

Please remember that this project is still being developed. The development phase of the QtSourceView
library can be described as:
  * Develop a feature in a demo
  * Clean up the code of the demo
  * Write a final set of features
  * Continue the implementation of those features in the demo
  * Code cleanup + documentation
  * Move the code to the library, and clean up the demo
  * Optimizations for the code

The status of the editor control is that the final set of features is been written, some have been implemented
and some not. When the code is stable enough it will get cleaned up and documented and it will move to the library.

If you still want to review the code, here is the SVN to get it from:
```
svn co http://qtedit4.googlecode.com/svn/tools/qtsourceview/ qtsourceview
```

The editor control is available as demos/demo4/demo4-debug.pro.

## Status/TODO ##
  * Replace functionally missing: no option to replace within selected text. (started working on revision [r226](https://code.google.com/p/qtedit4/source/detail?r=226))
  * ~~Whitespace support is done via images, which does not scale right. Need to find similar Unicode chars.~~ done since revision [r227](https://code.google.com/p/qtedit4/source/detail?r=227).
  * ~~Better text navigation (8)~~ working since revision [r227](https://code.google.com/p/qtedit4/source/detail?r=227) [r229](https://code.google.com/p/qtedit4/source/detail?r=229)
  * ~~Auto brackets (11) is not fully implemented. Deleting does not always work.~~ workign since revsision 233
  * ~~Modified lines (12) does not work as expected, when pasting lines, they are not marked as modified.~~ working since [revision 234](https://code.google.com/p/qtedit4/source/detail?r=234)
  * No central way of controlling editors (13), the EditorConfig class does not emit a usable signal.
  * VIM modelines not implemented yet.
  * No special printing support is coded yet.


## Feature walk trough ##
  1. **Line numbers, with current line marked as bold**
> > Line numbers should be displayed as a side panel, which can be optionally hidden. Then panel should also
> > display information about the line: an icon representing it's state, or has it been modified since
> > the file has been loaded.
  1. **File modification panel**
> > No popups, a file modification panel should be displayed if the file has been modified outside
> > the editor (edited, or deleted). If the file has been modified, place a link to reload the file.
  1. **Search/replace/goto line controls (no popups, but inline)**
> > When text has been found the search control should change it's color to blue. If not found, the color should
> > change to green. Search control should have a button for clearing the text, which should also set the background
> > to white. When searching, you can press _escape_ to close the control, and then pressing it once more
> > will clear the selection.
> > The replace widget will start changing text only when the _enter_ key or "Replace" button have been pressed,
> > however, the "Text to find" input line will change colors, as the find control, to help the user understand
> > what will happen when he presses the replace button.
> > The Goto line control will also display the length of the file being edited.
  1. **Bookmarks**
> > The user should be able to press _control+b_ (shortcut taken from Kate) to toggle mark a line as a bookmark
> > and to navigate between bookmarks in the same file, he should press control+page up, page down to navigate
> > between them. Since this is done using a QAction, shortcuts can be modified.
> > Lines marked as bookmark should have a different background.
  1. **Display white spaces (space, tabs etc)**
> > The editor should display spaces as "dots" and tabs as ">>" arrows. This is to aid people coding
> > in languages like python in which blocks are defined as indentations. This also helpes developers
> > to enforce the coding styles of their projects.
  1. **Display current line with different color**
> > The most important line of the text editor is the one being edited at the moment. It should
> > be easy to identify it. The line should have a different background and the line on the side panel
> > should be marked with bold.
  1. **Lines should have different states, represented by different colors, and an icon representing it's state.**
> > In code editors, line can have different states, for example bookmark, brakpoint, current line in the debugger
> > and more. Lines with such state should have a different background. If a line has 2 different status (both
> > bookmark and breakpoint) the effective background color should be a composition of all the colors of the status
> > of that line.
  1. **Better text navigation (smart home, more control over scroll bars)**
> > The editor will have a smart home/end, this means that pressing _home_ will navigate not to the first char of the
> > line, but to the first non white-space character. Same for pressing _end_. Pressing _control+up arrow_ should
> > scroll the page up without moving the text cursor, same for _control+down arrow_.
  1. **Printer margins**
> > Printer margins display where the document will cut off if it would be printed. Some projects (like Debian)
> > demand that their documentation **must** be trimmed at 80 rows, so this will help people editing Debian
> > documentation.
  1. **Tab width should be set to different number of spaces**
> > The tab size should be modified using, by specifying a number of spaces, not pixels.
  1. **Auto brackets**
> > When the user presses _(_ the editor will also automatically add _)_. If some text is selected, and the
> > user presses _"_, the selected text will be surrounded by _"_. If a user presses _[_ and text editor adds _]_,
> > upon deletion of _[_ or _]_ both chars will be deleted. The bracket list should be easy to modify by the user.
  1. **Display lines which have been modified since the file has been opened**
> > When the user loads a file, keep note of all modified lines, so he can know what did he change, even without
> > asking the version control system about it.
  1. **Central way to control all the options available to the editor**
> > The editor needs a configuration dialog. It should be able to modify most aspects of the control, while
> > still not beeing too complicated to use. Also, since in some applications developers will have several controls
> > in their applications, the configuration dialog shuold emit a signal _new configuration is available_ which all
> > editors should connect to. This way, pressing _Ok_ on the configuration dialog will modify _n_ controls.
  1. **Tab width and other options can be set per file (like vim modeline)**
> > VIM (also emacs and kate) have support for setting the tab width per file, this is called modeline magic,
> > and is described in the following links:
      * Vim: http://vim.wikia.com/wiki/VimTip331 http://www.vim.org/htmldoc/options.html#modeline
      * emacs: http://www.emacswiki.org/cgi-bin/wiki/FileLocalVariables
      * kate: http://kate-editor.org/article/katepart_modelines
> > The editor should have support for this.
  1. **Printing support**
> > The editor should be able to print it's content. Need to create a dialog for printing part of the document,
> > having a banner etc.