# CuteGit

![alt text](https://github.com/Jango73/CuteGit/blob/master/Media/Screenshot01.jpg)

This is a simple UI for the GIT executable (and other versioning systems in a near future).
I decided to make my own because the only efficient GIT UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal GIT functionality.

## Building

- Make sure to update submodules
- Open /CuteGit.pro in QtCreator
- Build and run

## Caution

Do not use this software on very large repositories, where you can have hundreds of modified files at once.
It is not *yet* able to handle massive repositories.

## Things it does

* Open a GIT repository
* Show repository status (clean, merge, rebase, ...)
* Show repository files as a tree, colored according to status
* Show the branches of the repository
* Show the branch log
* Show the selected file log
* Show the unstaged diffs in the selected item (folder or file)
* Show the output of the executed GIT commands
* Mark files as staged / unstaged
* Change current branch
* Revert files
* Commit / amend (auto on interactive rebase)
* Simple pull
* Simple push
* Refresh view
* Modify a commit message
* Begin rebase on a commit, continue and abort rebase

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
* Implements GIT versioning commands

### CSVNCommands

* Inherits CCommands
* Will implements SVN versioning commands

### CHGCommands

* Inherits CCommands
* Will implements Mercurial versioning commands

### CLogModel

* A collection of log lines

### CLogLine

* A line of log : commit id, date, author, message

### CDiffModel

* A collection of CDiffLine

### CDiffLine

* A line of diff between two files

## Code summary

As of 2019-07-30:

```
      94 text files.                                                                                                       
      94 unique files.                                                                                                     
      15 files ignored.                                                                                                    

http://cloc.sourceforge.net v 1.64  T=0.22 s (364.8 files/s, 40039.0 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C++                             17            910            474           2624
QML                             39            449             47           2162
C/C++ Header                    21            579            625            772
Qt Project                       2              8              4             17
-------------------------------------------------------------------------------
SUM:                            79           1946           1150           5575
-------------------------------------------------------------------------------
```
