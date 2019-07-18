# CuteGit

This is a simple UI for the GIT executable.
I decided to make my own because the only efficient GIT UIs on Linux are paywares.
It is however not meant to compete with paywares, just provide minimal GIT functionality.

## Building

Please do not use shadow build because of the submoduled qt-plus library.

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

## Things to fix

* 'Expand all' and 'collapse all' buttons

## Things to do

* Add file sorting
* Make the file system model refresh file status when changes occur outside the app
* Show deleted files in the file system model (somehow)
* Make a list view for a flat listing of changed files
  * With a button to switch between tree and flat views
* Rename a commit (that has not been pushed)
* Reset HEAD to a commmit
* Amend
* Rebase
