# CuteGit

![alt text](https://github.com/Jango73/CuteGit/blob/master/Media/Screenshot01.jpg)

This is a simple UI for the GIT executable.
I decided to make my own because the only efficient GIT UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal GIT functionality.

## Building

- Make sure to update submodules
- Open /CuteGit.pro in QtCreator
- Build and run

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
  * the interface to the versioning system
  * the file system model
  * the filter for the file system model
  * the model for the list of repositories
  * the model for lines of output when executing a process

### CTreeFileModel

* Inherits QFileSystemModel
* Provides access to
  * the name of the current branch
  * the repository status (normal, rebasing, merging, ...)
  * the repository files
  * the model for the list of branches
  * the model for the current branch log
  * the model for the diff of the selected folder / file
  * the model for the log of the selected folder / file
* Exposes to C++ the versioning methods: stage, unstage, commit, push, ...

### CFileModelProxy

* Inherits QSortFilterProxyModel
* Encapsulates CTreeFileModel
* Enables showing/hiding the files using their GIT status property
* Exposes to QML / JS the versioning methods: stage, unstage, commit, push, ...

### CFlatFileModel

* Inherits QAbstractListModel
* Shows changed files as a list

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

* Will inherit CCommands
* Will implements SVN versioning commands

### CLogModel

* A collection of log lines

### CLogLine

* A line of log : commit id, date, author, message

### CDiffModel

* A collection of CDiffLine

### CDiffLine

* A line of diff between two files
