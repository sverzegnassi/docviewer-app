/*
  Copyright (C) 2015 Canonical, Ltd.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License 3 as published by
  the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/

#ifndef DOCVIEWERUTILS_H
#define DOCVIEWERUTILS_H

#include <QObject>

class DocviewerUtils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool desktopMode READ desktopMode CONSTANT)

public:
    bool desktopMode() const;

    Q_INVOKABLE static bool exists(const QString &path);
    Q_INVOKABLE static bool copy(const QString &source, const QString &destination);

    Q_INVOKABLE static bool isFileSupported(const QString &path);
    Q_INVOKABLE static QString getXdgDocumentsLocation();

    Q_INVOKABLE static QString buildDestinationPath(const QString &destinationDir, const QString &sourcePath);

    Q_INVOKABLE static QString checkIfFileAlreadyImported(const QString &filePath, const QStringList &storageLocationList);

    Q_INVOKABLE static QString getFileBaseNameFromPath(const QString &filePath);
    Q_INVOKABLE static QString getFileNameFromPath(const QString &filePath);
    Q_INVOKABLE static QString getCanonicalPath(const QString &filePath);
};

#endif // DOCVIEWERUTILS_H
