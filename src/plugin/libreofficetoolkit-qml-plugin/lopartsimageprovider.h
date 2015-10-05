/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LOPARTSIMAGEPROVIDER_H
#define LOPARTSIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QSharedPointer>
#include <QDebug>

class LODocument;

class LOPartsImageProvider : public QQuickImageProvider
{
public:
    LOPartsImageProvider(LODocument *document);
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

private:
    QSharedPointer<LODocument> m_document;
    // LODocument* m_document;
};

#endif // LOPARTSIMAGEPROVIDER_H
