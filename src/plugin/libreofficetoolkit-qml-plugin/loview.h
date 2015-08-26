/*
 * Copyright (C) 2015 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOVIEW_H
#define LOVIEW_H

#include <QQuickPaintedItem>
#include <QTimer>

class LODocument;
class TileItem;

class LOView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* parentFlickable READ parentFlickable WRITE setParentFlickable NOTIFY parentFlickableChanged)
    Q_PROPERTY(LODocument* document        READ document        WRITE setDocument        NOTIFY documentChanged)

    // TODO: Implement zoom!
    Q_PROPERTY(qreal       zoomFactor      READ zoomFactor      WRITE setZoomFactor      NOTIFY zoomFactorChanged)
    Q_PROPERTY(int         cacheBuffer     READ cacheBuffer     WRITE setCacheBuffer     NOTIFY cacheBufferChanged)

public:
    LOView(QQuickItem *parent = 0);
    ~LOView();

    void        paint(QPainter *painter);

    QQuickItem* parentFlickable() const;
    void        setParentFlickable(QQuickItem* flickable);

    LODocument* document() const;
    void        setDocument(LODocument* doc);

    qreal       zoomFactor() const;
    void        setZoomFactor(qreal zoom);

    int         cacheBuffer() const;
    void        setCacheBuffer(int cacheBuffer);

Q_SIGNALS:
    void parentFlickableChanged();
    void documentChanged();
    void zoomFactorChanged();
    void cacheBufferChanged();

private Q_SLOTS:
    void updateViewSize();
    void updateVisibleRect();
    void scheduleVisibleRectUpdate();

private:
    QQuickItem*             m_parentFlickable;
    LODocument*             m_document;

    qreal                   m_zoomFactor;
    int                     m_cacheBuffer;

    QRect                   m_visibleArea;
    QRect                   m_bufferArea;

    QTimer                  m_updateTimer;

    QMap<int, TileItem*>    m_tiles;

    void                    generateTiles(int x1, int y1, int x2, int y2, int tilesPerWidth);
    void                    createTile(int index, QRect rect);
};

#endif // LOVIEW_H