# CuteGit

This is a simple UI for the GIT executable.
I decided to make my own because the only efficient GIT UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal GIT functionality.

## Building

Please do not use shadow build because of the submoduled qt-plus library.

## Things it does

* Open a GIT repository
* Show repository status (clean, merge, rebase, ...)
* Show repository files as a tree, colored according to status
* Show the branches of the repository
* Show the graph of the repository
* Show the diffs in the selected item (folder or file)
* Show the output of the executed GIT commands
* Mark files as staged / unstaged
* Change current branch
* Revert files
* Commit
* Amend on interactive rebase
* Simple pull
* Simple push
* Refresh view
* File log

## Things to fix

* 'Expand all' and 'collapse all' buttons

## Things to do

* Make a list view for a flat listing of changed files
  * With a button to switch between tree and flat views
* File sorting
* Make the file system model refresh file status when changes occur outside the app
* Show deleted files in the file system model (somehow)
* Rename a commit
* Reset HEAD to a commit
* Rebase
* Blame
* Stash save and pop

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

### CFileModel

* Inherits QFileSystemModel
* Provides access to the repository files
* Exposes to C++ the versioning methods: stage, unstage, commit, push, ...

### CFileModelProxy

* Inherits QSortFilterProxyModel
* Enables showing/hiding the files using their GIT status property
* Exposes to QML/JS the versioning methods: stage, unstage, commit, push, ...

### CRepoFile

* Represents the status of a repository file

### CCommands

* A (kind of) interface for a versioning system
* Makes the versioning system abstract

### CGitCommands

* Inherits CCommands
* Implements GIT versioning commands
