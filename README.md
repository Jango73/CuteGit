# CuteGit

![alt text](https://github.com/Jango73/CuteGit/blob/master/Media/Screenshot01.jpg)

This is a simple multi-document UI for the Git executable (and other versioning systems in a near future).
I decided to make my own because the only efficient Git UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal versioning functionality.

## Building

- Make sure to update submodules
- Open /CuteGit.pro in QtCreator
- Build and run

## Things it does

* Open one or more git repositories
* Show repository status (clean, merge, rebase, ...)
* Show repository files as a tree, colored according to status
* Show the branches and tags of the repository
* Show the branch log
* Show the graph as output by Git (might be graphical some day)
* Show the selected file's log
* Show the unstaged diffs in the selected item (folder or file)
* Show the output of the executed commands
* Change current branch
* Mark files as staged / unstaged
* Revert files
* Commit / amend (auto on interactive rebase)
* Basic fetch, pull and push
* Refresh view
* Modify a commit message
* Begin rebase on a commit, continue and abort rebase
* Reset on a commit
* Save and pop stash (no selectable stash yet)
* Branch from a commit
* Tag a commit
* View diff with previous commit
* View diff between any two commits

## Classes

### CuteGit

* Registers types
* Creates application controller
* Creates QML engine

### CController

* Creates and provides access to
  * the list of known repositories
  * the list of open repositories
  * the current repository
  * the Git interactive rebase editing

### CRepository

* Represents a repository
* Creates and provides access to
  * the corresponding versioning system commands
  * the file system model
  * the filter for the file system model
  * the model for lines of output when executing a process
  * the name of the current branch
  * the repository status (normal, rebasing, merging, ...)
  * the list of branches
  * the current branch log
  * the diff of the selected folder / file
  * the log of the selected folder / file
* Exposes to QML / JS the versioning methods: stage, unstage, commit, push, ...

### CTreeFileModel

* Inherits QFileSystemModel
* Shows changed files as a tree
* Provides access to
  * the repository files

### CTreeFileModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CTreeFileModel
* Enables showing/hiding the files using their GIT status property

### CFlatFileModel

* Inherits QAbstractListModel
* Shows changed files as a list
* Provides access to
  * the repository files

### CFlatFileModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CFlatFileModel

### CRepoFile

* The properties of a repository file
  * status
  * staged / unstaged flag

### CCommands

* A (kind of) interface for a versioning system
* Makes the versioning system abstract

### CGitCommands

* Inherits CCommands
* Implements Git versioning commands

### CSVNCommands

* Inherits CCommands
* Will implement SVN versioning commands

### CHGCommands

* Inherits CCommands
* Will implement Mercurial versioning commands

### CLogModel

* A collection of log lines

### CLogLine

* A line of log : commit id, date, author, message

### CDiffModel

* A collection of CDiffLine

### CDiffLine

* A line of diff between two files

## Code summary

As of 2019-09-02:

```
     126 text files.
     126 unique files.                                          
      28 files ignored.

http://cloc.sourceforge.net v 1.64  T=0.32 s (308.8 files/s, 37276.7 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             21           1196            558           3567
QML                             49            655             93           3196
C/C++ Header                    26            724            758           1054
Qt Project                       1              7              4             13
Prolog                           1              1              0              5
-------------------------------------------------------------------------------
SUM:                            98           2583           1413           7835
-------------------------------------------------------------------------------
```
