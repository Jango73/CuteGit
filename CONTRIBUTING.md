# General

This file defines the coding rules for my projects.

## Spaces
Put spaces between operators.

**Do**
```cpp
QFileInfo fileInfo(sFullPath);
return fileInfo.baseName() + "." + fi.suffix();
```

**Don't**
```cpp
QFileInfo fileInfo(sFullPath);
return fileInfo.baseName()+"."+fi.suffix();
```

## Comments and dead code
Every comment should provide a meaningful message that explains the code below or to the left of it.<br>
Do not write/keep useless comments. It hurts the code readability and makes coders unsure of what to keep. Your previous code is in the repository...

**Do**
```javascript
// Check loaders and such
pInstanciated = pFoundItem->property("item").value<QQuickItem*>();
```

**Don't**
```javascript
pInstanciated = nullptr; // pFoundItem->property("item").value<QQuickItem*>();
```

## Instruction blocks
Don't build methods with compressed code lines, instead smartly place an empty line between logically grouped statements and before a block statement (if, while, etc...).<br>
Compressed text is not easily readable.

**Do**
```cpp
void CMyClass::onItemClicked(QModelIndex index)
{
    QModelIndex realIndex = m_tErrorsProxy.mapToSource(index);

    if (realIndex.isValid())
    {
        QString sFileName = m_mErrors.data(realIndex, CErrorListModel::File).toString();
        int pX = m_mErrors.data(realIndex, CErrorListModel::Column).toInt() - 1;
        int pY = m_mErrors.data(realIndex, CErrorListModel::Line).toInt() - 1;
        QFile fFile(sFileName);

        if (fFile.open(QFile::ReadOnly))
        {
            ui->textEditTitle->setText(sFileName);

            QString sText = fFile.readAll();
            fFile.close();

            ui->textEdit->setPlainText(sText);

            QTextCursor tmpCursor = ui->textEdit->textCursor();
            tmpCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
            tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, pY);
            tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pX);
            tmpCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor, 1);

            ui->textEdit->setTextCursor(tmpCursor);
        }
    }
}
```

**Don't**
```cpp
void CMyClass::onItemClicked(QModelIndex index)
{
    QModelIndex realIndex = m_tErrorsProxy.mapToSource(index);
    if (realIndex.isValid())
    {
        QString sFileName = m_mErrors.data(realIndex, CErrorListModel::File).toString();
        int pX = m_mErrors.data(realIndex, CErrorListModel::Column).toInt() - 1;
        int pY = m_mErrors.data(realIndex, CErrorListModel::Line).toInt() - 1;
        QFile fFile(sFileName);
        if (fFile.open(QFile::ReadOnly))
        {
            ui->textEditTitle->setText(sFileName);
            QString sText = fFile.readAll();
            fFile.close();
            ui->textEdit->setPlainText(sText);
            QTextCursor tmpCursor = ui->textEdit->textCursor();
            tmpCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
            tmpCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, pY);
            tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, pX);
            tmpCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor, 1);
            ui->textEdit->setTextCursor(tmpCursor);
        }
    }
}
```

## Pointer and reference
The & and * suffixes refer to the type of a symbol, not its name. So make them stick to the type, not the name.

**Do**
```cpp
void CMyClass::store(const QString& sText)
```

**Don't**
```cpp
void CMyClass::store(const QString &sText)
```

# Classes

## Class naming
A class begins with the 'C' character and uses upper camel casing.

**Do**
```cpp
class CMyClass: public QObject
```

**Don't**
```cpp
class myClass: public QObject
```

## Class property (variable) naming
A class property begins with 'm_' which stands for member.<br>
It is followed by one or more lower case characters that designate its type:
- p for a pointer
- b for a boolean
- i for an int
- ui for for an unsigned int
- c for a char
- s for a string
- m for a map or a mutex
- v for a vector
- l for a list
- ba for a byte array
- other letters you can imagine

After that comes the name of the class using camel casing.

**Do**
```cpp
protected:

    QMutex                          m_mContextMutex;
    QString                         m_sFolder;
    QString                         m_sFile;
    QMLTreeContext*                 m_pContext;
    QVector<CMyClass>               m_vErrors;
    CXMLNode                        m_xGrammar;
```

**Don't**
```cpp
protected:
    QMutex                          mutex;
    QString                         folder;
    QString                         File;
    QMLTreeContext*                 context;
    QVector<CMyClass>               Errors;
    CXMLNode                        grammar;
```

## Class static property (variable) naming
The same rules as above apply here, except that m_ is replaced with s_ (for static).

## Class declaration
In the header of a class, declare its contents in the following order:
- Q_OBJECT macro (if class inherits a QObject)
- Enumerators
- QML Properties
- Constructors and destructor
- Setters
- Getters
- Operators
- Public control methods
- Invokable control methods
- Protected control methods
- Private control methods
- Signals
- Slots
- Protected properties
- Private Properties

## Method implementation
In the cpp file of your class, implement the methods in the same order they are declared in the header.

## Method size
A method will not exceed 120 lines (including empty lines and comments).<br>
Large methods make it hard to understand what they do.

## Constructor implementation
When a constructor has many initializers, put each one of them on a seperate line, with the operator (':' and ',') at the start of the line.

**Do**
```cpp
CMyClass::CMyClass(const QString& sFrom, const QString& sTo)
    : QObject(nullptr)
    , m_sFrom(sFrom)
    , m_sTo(sTo)
{
}
```

**Don't**
```cpp
CMyClass::CMyClass(const QString& sFrom, const QString& sTo) : QObject(nullptr), m_sFrom(sFrom), m_sTo(sTo) {
}
```

## Header format
Use the following format for headers.

```cpp
#pragma once

#include "global.h"

//-------------------------------------------------------------------------------------------------
// Includes

// Qt
#include <QObject>

// Application / library
#include "SomeOtherClass.h"

//-------------------------------------------------------------------------------------------------

//! Defines a ...
class xxx_EXPORT SomeClass : public QObject
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Declare QML properties here...

public:

    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    SomeClass();

    //! Copy constructor
    SomeClass(const SomeClass& sTarget);

    //! Destructor
    virtual ~SomeClass();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    Declare setters here...

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    Declare getters here...

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    Declare control methods here...

    //-------------------------------------------------------------------------------------------------
    // Invokable control methods
    //-------------------------------------------------------------------------------------------------

    Declare invokables here
    Q_INVOKABLE ...

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    Declare protected control methods here...

    //-------------------------------------------------------------------------------------------------
    // Private control methods
    //-------------------------------------------------------------------------------------------------

private:

    Declare private control methods here...

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    Declare signals here...

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    Declare slots here...

    //-------------------------------------------------------------------------------------------------
    // Static properties
    //-------------------------------------------------------------------------------------------------

public / protected:

    Declare static properties here...

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    Declare protected member variables here...
};
```
