#ifndef RENDERTASK_H
#define RENDERTASK_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QHash>
#include <QQueue>
#include <QAtomicInt>

#include "lodocument.h"

// TODO replace with class.

enum RenderTaskType
{
    RttUnknown = 0x0,
    RttTile = 0x1,
    RttImpressThumbnail = 0x2,
    RttPdfPage = 0x3 // TODO
};

// TODO Abstract.
class AbstractRenderTask
{

public:
    virtual RenderTaskType type() { return RttUnknown; }
    virtual QImage doWork() = 0 ; // { return QImage(); }
    virtual ~AbstractRenderTask() { }

    int id() { return m_id; }
    void setId(int i) { m_id = i; }
protected:
    int m_id;
};

Q_DECLARE_METATYPE(AbstractRenderTask*)

class LoRenderTask : public AbstractRenderTask
{
public:
    int part() { return m_part; }
    void setPart(int p) { m_part = p; }

    QSharedPointer<LODocument> document() { return m_document; }
    void setDocument(QSharedPointer<LODocument> d) { m_document = d; }
protected:
    int m_part;
    QSharedPointer<LODocument> m_document;
};

class TileRenderTask : public LoRenderTask
{
public:
    virtual RenderTaskType type() { return RttTile; }
    virtual QImage doWork()
    {
        return m_document->paintTile(m_area.size(), m_area, m_zoom);
    }
    QRect area() { return m_area; }
    void setArea(const QRect& a) { m_area = a; }
    qreal zoom() { return m_zoom; }
    void setZoom(qreal z) { m_zoom = z; }
protected:
    QRect m_area;
    qreal m_zoom;
};

class ThumbnailRenderTask : public LoRenderTask
{
public:
    virtual RenderTaskType type() { return RttImpressThumbnail; }
    virtual QImage doWork()
    {
        return m_document->paintThumbnail(m_size);
    }
    qreal size() { return m_size; }
    void setSize(qreal s) { m_size = s; }
protected:
    qreal m_size;
};

#endif // RENDERTASK_H
