/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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

#include "lodocument.h"
#include "twips.h"
#include "config.h"

#include <QImage>
#include <QDebug>

#define LOK_USE_UNSTABLE_API
#include <LibreOfficeKit/LibreOfficeKitInit.h>
#include <LibreOfficeKit/LibreOfficeKit.hxx>

// TODO: Error management

#ifdef DEBUG_TILE_BENCHMARK
#include <QElapsedTimer>
#endif

lok::Office *LODocument::s_office = nullptr;

LODocument::LODocument()
  : m_path("")
  , m_currentPart(-1)
  , m_document(nullptr)
{
    // This space is intentionally empty.
}

// Return the path of the loaded document
QString LODocument::path() const
{
    return m_path;
}

// Set the path of the document, then it tries to load it.
void LODocument::setPath(const QString& pathName)
{
    if (pathName.isEmpty())
        return;

    m_path = pathName;
    Q_EMIT pathChanged();

    // Load the new document
    this->loadDocument(m_path);
}

int LODocument::currentPart() {
    return m_currentPart;
}

void LODocument::setCurrentPart(int index)
{
    if (!m_document)
        return;

    if (m_currentPart == index || index < 0 || index > partsCount() - 1)
        return;

    m_currentPart = index;
    Q_EMIT currentPartChanged();
}

// Load the document
bool LODocument::loadDocument(const QString &pathName)
{
    qDebug() << "Loading document...";

    if (pathName.isEmpty()) {
        qDebug() << "Can't load the document, path is empty.";
        return false;
    }

    if (!s_office)
        s_office = lok::lok_cpp_init(LO_PATH);

    m_document = s_office->documentLoad(m_path.toUtf8().constData());

    m_docType = DocumentType(m_document->getDocumentType());
    Q_EMIT documentTypeChanged();

    setCurrentPart(m_document->getPart());

    m_document->initializeForRendering();
    qDebug() << "Document loaded successfully !";

    return true;
}

// Return the type of the loaded document (e.g. text document,
// spreadsheet or presentation).
LODocument::DocumentType LODocument::documentType() const
{
    return m_docType;
}

int LODocument::documentPart() const
{
    return m_document->getPart();
}

void LODocument::setDocumentPart(int p)
{
    if (documentPart() != p)
        m_document->setPart(p);
}

// Return the size of the document, in TWIPs
QSize LODocument::documentSize() const
{
    if (!m_document)
        return QSize(0, 0);

    long pWidth(0);
    long pHeight(0);
    m_document->getDocumentSize(&pWidth, &pHeight);

    return QSize(pWidth, pHeight);
}

// Paint a tile, with size=canvasSize, of the part of the document defined by
// the rect tileSize.
QImage LODocument::paintTile(const QSize& canvasSize, const QRect& tileSize, const qreal &zoom)
{
    if (!m_document)
        return QImage();

    if (m_currentPart != m_document->getPart())
        m_document->setPart(m_currentPart);

    QImage result = QImage(canvasSize.width(), canvasSize.height(),  QImage::Format_RGB32);

#ifdef DEBUG_TILE_BENCHMARK
    QElapsedTimer renderTimer;
    renderTimer.start();
#endif

    m_document->paintTile(result.bits(),
                          canvasSize.width(), canvasSize.height(),
                          Twips::convertPixelsToTwips(tileSize.x(), zoom),
                          Twips::convertPixelsToTwips(tileSize.y(), zoom),
                          Twips::convertPixelsToTwips(tileSize.width(), zoom),
                          Twips::convertPixelsToTwips(tileSize.height(), zoom));

#ifdef DEBUG_TILE_BENCHMARK
    qDebug() << "Time to render the tile:" << renderTimer.elapsed() << "ms";
#endif

    return result.rgbSwapped();
}

QImage LODocument::paintThumbnail(qreal size)
{
    if (!m_document)
        return QImage();

#ifdef DEBUG_TILE_BENCHMARK
    QElapsedTimer renderTimer;
    renderTimer.start();
#endif

    qreal tWidth = this->documentSize().width();
    qreal tHeight = this->documentSize().height();

    QSize resultSize;

    if (tWidth > tHeight) {
        resultSize.setWidth(size);
        resultSize.setHeight(size * tHeight / tWidth);
    } else {
        resultSize.setHeight(size);
        resultSize.setWidth(size * tWidth / tHeight);
    }

    QImage result = QImage(resultSize.width(), resultSize.height(), QImage::Format_RGB32);
    m_document->paintTile(result.bits(), resultSize.width(), resultSize.height(),
                          0, 0, tWidth, tHeight);

#ifdef DEBUG_TILE_BENCHMARK
    qDebug() << "Time to render the thumbnail:" << renderTimer.elapsed() << "ms";
#endif

    return result.rgbSwapped();
}

int LODocument::partsCount()
{
    if (!m_document)
        return int(0);
 
    return m_document->getParts();
}
 
QString LODocument::getPartName(int index) const
{
    if (!m_document)
        return QString();
 
    return QString::fromUtf8(m_document->getPartName(index));
}
 
/* Export the file in a given format:
 *  - url is a mandatory argument.
 *  - format is optional. If not specified, lok will try to get it from the file
 *    extension given at the 'url' argument.
 *  - filerOptions is also optional.
 */
// TODO: Is there some documentation on safe formats or filterOptions that can
// be used?
bool LODocument::saveAs(QString url, QString format = QString(), QString filterOptions = QString())
{
    if (!m_document) {
        qDebug() << "No loaded document. It's not possible to save this file.";
        return false;
    }

    return m_document->saveAs(url.toLatin1().constData(),
                              format.toLatin1().constData(),
                              filterOptions.toLatin1().constData());
}

LODocument::~LODocument()
{
    delete m_document;
#ifndef DEBUG_VERBOSE
    qDebug() << " ---- ~LODocument";
    m_document->setPart(1);
#endif
}
