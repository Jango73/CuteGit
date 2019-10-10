# CuteGit

![alt text](https://github.com/Jango73/CuteGit/blob/master/Media/Screenshot01.jpg)

This is a simple multi-document UI for the Git executable (and other versioning systems in a near future).
I decided to make my own because the only efficient Git UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal versioning functionality.

## Dependencies

* Qt 5.13
* QtCreator 4.9.1

## Building

* Make sure to update submodules
```
    git submodule init
    git submodule update
```
* Open /CuteGit.pro in QtCreator
* Build and run

## Things it does

* Open one or more repositories
* Refresh view
* For repositories of type Git and Gerrit:
  * Show repository status (clean, merge, rebase, ...)
  * Show repository files as a flat list, colored according to status
  * Show the branches and tags
  * Show the branch log
  * Show the graph as output by Git (might be graphical some day)
  * Show the selected file's log
  * Show the unstaged diffs in the selected item (folder or file)
  * Show the output of the executed commands
  * Change current branch
  * Mark files as staged / unstaged
  * Revert files
  * Commit
  * Amend (on interactive rebase, commit is always amend)
  * Fetch, pull and push
  * Modify a commit message
  * Begin rebase on a commit, continue and abort rebase
  * Reset on a commit
  * Save and pop stash (no selectable stash yet)
  * Branch from a commit
  * Tag a commit
  * View diff with previous commit
  * View diff between any two commits
* For repositories of type Mercurial:
  * Show the branches
  * Show repository files as a flat list, colored according to status
  * Mark files as staged / unstaged
  * Show the branch log
  * Show the selected file's log
  * Commit
  * Branch from a commit
* For repositories of type SVN:
  * Nothing yet

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
  * the log of the current branch
  * the diff of the selected file
  * the log of the selected file
* Exposes to QML / JS the versioning methods: stage, unstage, commit, push, ...

### CTreeFileModel

* Inherits QFileSystemModel
* Shows changed files as a tree
* Provides access to the repository files

### CTreeFileModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CTreeFileModel
* Enables filtering files on
  * file name
  * GIT status property

### CFlatFileModel

* Inherits QAbstractListModel
* Shows changed files as a list
* Provides access to the repository files

### CFlatFileModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CFlatFileModel
* Enables filtering files on
  * file name
  * GIT status property

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
* Partially implements Mercurial versioning commands

### CLogModel

* A collection of log lines

### CLogModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CLogModel
* Enables filtering a log on
  * commit author
  * commit message

### CLogLine

* A line of log : commit id, date, author, message

### CDiffModel

* A collection of CDiffLine

### CDiffModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CDiffModel
* Enables filtering a diff

### CDiffLine

* A line of diff between two files

## Code summary

As of 2019-09-20:

```
     135 text files.
     135 unique files.                                          
      22 files ignored.

http://cloc.sourceforge.net v 1.64  T=0.32 s (352.5 files/s, 47022.5 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             26           1339            616           3978
QML                             52            746            108           3608
TypeScript                       2             18              0           1633
C/C++ Header                    32            869            908           1227
Qt Project                       1              7              4             13
-------------------------------------------------------------------------------
SUM:                           113           2979           1636          10459
-------------------------------------------------------------------------------
```
