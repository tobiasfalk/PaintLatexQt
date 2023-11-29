#include "paintlatexqt.h"

LatexDevice::LatexDevice() : QPaintDevice()
{
    latexPaintEngine = new LatexEngine();
}

LatexDevice::~LatexDevice()
{
    free(latexPaintEngine);
}

LatexEngine *LatexDevice::paintEngine() const
{
    return latexPaintEngine;
}

int LatexDevice::metric(PaintDeviceMetric metric) const
{
    qDebug() << "metric: " << metric;
    switch (metric) {
    case QPaintDevice::PdmDepth:
        return 32; // copyed from SVG because I did not find a value for LaTex
    case QPaintDevice::PdmWidth:
        return latexPaintEngine->getWidth(); // 42; // d->engine->size().width();
    case QPaintDevice::PdmHeight:
        return latexPaintEngine->getHight(); // 42; // d->engine->size().height();
    case QPaintDevice::PdmDpiX:
        return 72; // d->engine->resolution();
    case QPaintDevice::PdmDpiY:
        return 72; // d->engine->resolution();
    case QPaintDevice::PdmHeightMM:
        return qRound(latexPaintEngine->getHight() * 25.4 / 72.27);
    case QPaintDevice::PdmWidthMM:
        return qRound(latexPaintEngine->getWidth() * 25.4 / 72.27);
    case QPaintDevice::PdmNumColors:
        return 0xffffffff;
    case QPaintDevice::PdmPhysicalDpiX:
        return 72; // d->engine->resolution();
    case QPaintDevice::PdmPhysicalDpiY:
        return 72; // d->engine->resolution();
    case QPaintDevice::PdmDevicePixelRatio:
        return 1;
    case QPaintDevice::PdmDevicePixelRatioScaled:
        return 1 * QPaintDevice::devicePixelRatioFScale();
    default:
        qWarning("QSvgGenerator::metric(), unhandled metric %d\n", metric);
        break;
    }
}

void LatexEngine::checkSB(QPointF point)
{
    if (point.x() < sX) {
        sX = point.x();
    }
    if (point.y() * -1 < sY) {
        sY = point.y() * -1;
    }

    if (point.x() > bX) {
        bX = point.x();
    }
    if (point.y() * -1 > bY) {
        bY = point.y() * -1;
    }
}

void LatexEngine::top()
{
    //*fStream << "create top" << Qt::endl;
    // docClass
    // usepackages
    // beginDoc

    switch (integrationStyle) {
    case LatexIntegrationStyle::StandalonPackage:
        *fStream << "\\documentclass[class=article, crop=false]{standalone}" << Qt::endl;
        *fStream << "% "
                    "https://www.overleaf.com/learn/latex/"
                    "Multi-file_LaTeX_projects#The_standalone_package"
                 << Qt::endl;
        *fStream << "\\usepackage[subpreambles=true]{standalone}" << Qt::endl;
        *fStream << "\\usepackage{tikz}" << Qt::endl;
        *fStream << "\\begin{document}" << Qt::endl;
        break;
    case LatexIntegrationStyle::SubfilesPackage:
        *fStream << "\\documentclass[path/to/main.tex]{subfiles}" << Qt::endl;
        *fStream << "% https://www.overleaf.com/learn/latex/"
                    "Multi-file_LaTeX_projects#The_subfiles_package"
                 << Qt::endl;
        *fStream << "\\usepackage{tikz}" << Qt::endl;
        *fStream << "\\usetikzlibrary{patterns}" << Qt::endl;
        *fStream << "\\usetikzlibrary{shadings}" << Qt::endl;
        *fStream << "\\begin{document}" << Qt::endl;
        break;
    default:
        break;
    }

    *fStream << "\\begin{figure}[h]" << Qt::endl;
    if (topOptions.size() > 0) {
        *fStream << topOptions << Qt::endl;
    }
    *fStream << "\\begin{tikzpicture}" << Qt::endl;
}

void LatexEngine::bottom()
{
    *fStream << "\\end{tikzpicture}" << Qt::endl;
    if (caption.size() > 0) {
        *fStream << "\\caption{" << caption << "}" << Qt::endl;
    }
    if (description.size() > 0) {
        *fStream << "\\medskip\\small\\centering " << description << Qt::endl;
    }
    if (lable.size() > 0) {
        *fStream << "\\label{" << lable << "}" << Qt::endl;
    }
    *fStream << "\\end{figure}" << Qt::endl;
    // endDoc
    switch (integrationStyle) {
    case LatexIntegrationStyle::StandalonPackage:
        *fStream << "\\end{document}" << Qt::endl;
        break;
    case LatexIntegrationStyle::SubfilesPackage:
        *fStream << "\\end{document}" << Qt::endl;
        break;
    default:
        break;
    }
}

void LatexEngine::translate_color(const QColor &color, QString *color_string,
                                  QString *opacity_string)
{
    Q_ASSERT(color_string);
    Q_ASSERT(opacity_string);
    *color_string = QString::fromLatin1("{rgb,255:red,%1; green,%2; blue,%3}")
                            .arg(color.red(), 3, 10, QLatin1Char('0'))
                            .arg(color.green(), 3, 10, QLatin1Char('0'))
                            .arg(color.blue(), 3, 10, QLatin1Char('0'));
    *opacity_string = QString::number(color.alphaF());
}

void LatexEngine::translate_color(const QColor &color, QString *color_string)
{
    Q_ASSERT(color_string);
    *color_string = QString::fromLatin1("{rgb,255:red,%1; green,%2; blue,%3}")
                            .arg(color.red(), 3, 10, QLatin1Char('0'))
                            .arg(color.green(), 3, 10, QLatin1Char('0'))
                            .arg(color.blue(), 3, 10, QLatin1Char('0'));
}

void LatexEngine::translate_PenAndBrush(QPen pen, QBrush brush)
{
    QColor drawColor = pen.color();
    QString strDrawColor;
    QString strDrawOpac;
    translate_color(drawColor, &strDrawColor, &strDrawOpac);

    QColor fillColor = brush.color();
    QString strFillColor;
    QString strFillOpac;
    translate_color(fillColor, &strFillColor, &strFillOpac);

    // line

    lineStyle = "color=" + strDrawColor + ", draw opacity=" + strDrawOpac
            + ", line width=" + QString::number(pen.widthF()) + "pt";

    switch (pen.capStyle()) {
    case Qt::FlatCap:
        lineStyle += ", line cap=butt";
        break;
    case Qt::SquareCap:
        lineStyle += ", line cap=rect";
        break;
    case Qt::RoundCap:
        lineStyle += ", line cap=round";
        break;
    default:
        break;
    }

    switch (pen.style()) {
    case Qt::SolidLine:
        break;
    case Qt::DashLine:
        lineStyle += ", dashed";
        break;
    case Qt::DotLine:
        lineStyle += ", dotted";
        break;
    case Qt::DashDotLine:
        lineStyle += ", dash dot";
        break;
    case Qt::DashDotDotLine:
        lineStyle += ", dash dot dot";
        break;
    case Qt::CustomDashLine:
        QList<qreal> patternList = pen.dashPattern();
        QString patternStr = ", dash pattern={";
        bool onOff = true; // true = on
        for (auto width : patternList) {
            if (onOff) {
                patternStr += "on ";
                onOff = false;
            } else {
                patternStr += "off ";
                onOff = true;
            }
            patternStr += QString::number(width) + "pt ";
        }
        lineStyle += patternStr + "}";
        break;
        //    case Qt::NoPen:
        //    case Qt::MPenStyle:
        //        break;
    }

    // fill

    fillStyle = "";

    QGradient graid;
    QString strGraidColorA;
    QString strGraidColorB;
    if (brush.gradient() != NULL) {
        graid = *brush.gradient();
        translate_color(graid.stops().first().second, &strGraidColorA);
        translate_color(graid.stops().last().second, &strGraidColorB);
    }

    switch (brush.style()) {
    case Qt::NoBrush:
        break;
    case Qt::SolidPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac;
        break;
    case Qt::Dense1Pattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense2Pattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=crosshatch dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense3Pattern: // not denser
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense4Pattern: // not denser
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense5Pattern: // not denser
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense6Pattern: // not denser
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::Dense7Pattern: // not denser
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::HorPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=horizontal lines, pattern color=" + strFillColor;
        break;
    case Qt::VerPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=vertical lines, pattern color=" + strFillColor;
        break;
    case Qt::CrossPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=grid, pattern color=" + strFillColor;
        break;
    case Qt::BDiagPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=north east lines, pattern color=" + strFillColor;
        break;
    case Qt::FDiagPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=north west lines, pattern color=" + strFillColor;
        break;
    case Qt::DiagCrossPattern:
        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
                + ", pattern=crosshatch, pattern color=" + strFillColor;
        break;
    case Qt::LinearGradientPattern:
        fillStyle += ", left color=" + strGraidColorA + ", rigth color=" + strGraidColorB
                + ", fill opacity=" + strFillOpac + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::ConicalGradientPattern:
        // fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
        //         + ", pattern=dots, pattern color=" + strFillColor;
        qInfo() << "Qt::ConicalGradientPattern dous not work with latex";
        break;
    case Qt::RadialGradientPattern:
        fillStyle += ", outer color=" + strGraidColorA + ", inner color=" + strGraidColorB
                + ", fill opacity=" + strFillOpac + ", pattern=dots, pattern color=" + strFillColor;
        break;
    case Qt::TexturePattern:
        //        fillStyle += ", fill=" + strFillColor + ", fill opacity=" + strFillOpac
        //                + ", pattern=dots, pattern color=" + strFillColor;
        qInfo() << "Qt::TexturePattern dous not work with latex";
        break;
    }
}

LatexEngine::LatexEngine() { }

LatexEngine::~LatexEngine() { }

bool LatexEngine::begin(QPaintDevice *pdev)
{
    file = new QFile(fileName);
    if (file->open(QIODeviceBase::WriteOnly)) {
        fStream = new QTextStream(file);
        //*fStream << "something" << Qt::endl;
        qDebug() << "Engine Start";
        top();
        return true;
    }
    return false;
}

bool LatexEngine::end()
{
    bottom();
    bool tmpSt = false;
    if (fStream != NULL) {
        fStream->flush();
        delete fStream;
        tmpSt = true;
    }
    bool tmpFl = false;
    if (file != NULL) {
        file->flush();
        file->close();
        delete file;
        tmpFl = true;
    }
    qDebug() << "end";
    return tmpSt && tmpFl;
}

qreal LatexEngine::getWidth()
{
    return std::abs(bX - sX);
}

qreal LatexEngine::getHight()
{
    return std::abs(bY - sY);
}

void LatexEngine::drawEllipse(const QRectF &rect)
{
    checkSB(rect.topLeft());
    checkSB(rect.topRight());
    checkSB(rect.bottomLeft());
    checkSB(rect.bottomRight());
    *fStream << "\\draw"
             << "[" << lineStyle << fillStyle << "]"
             << " (" << rect.left() + rect.width() / 2 << "pt, "
             << (rect.top() + rect.height() / 2) * -1 << "pt) ellipse (" << rect.width() / 2
             << "pt and " << rect.height() / 2 << "pt);" << Qt::endl;
}

void LatexEngine::drawEllipse(const QRect &rect)
{
    checkSB(rect.topLeft());
    checkSB(rect.topRight());
    checkSB(rect.bottomLeft());
    checkSB(rect.bottomRight());
    *fStream << "\\draw"
             << "[" << lineStyle << fillStyle << "]"
             << " (" << rect.left() + rect.width() / 2 << "pt, "
             << (rect.top() + rect.height() / 2) * -1 << "pt) ellipse (" << rect.width() / 2
             << "pt and " << rect.height() / 2 << "pt);" << Qt::endl;
}

void LatexEngine::drawImage(const QRectF &rectangle, const QImage &image, const QRectF &sr,
                            Qt::ImageConversionFlags flags)
{
    checkSB(rectangle.topLeft());
    checkSB(rectangle.topRight());
    checkSB(rectangle.bottomLeft());
    checkSB(rectangle.bottomRight());
    qDebug() << "drawImage";
    *fStream << "drawImage" << Qt::endl;
}

void LatexEngine::drawLines(const QLineF *lines, int lineCount)
{
    for (int i = 0; i < lineCount; i++) {
        QLineF line = lines[i];
        checkSB(line.p1());
        checkSB(line.p2());

        *fStream << "\\draw"
                 << "[" << lineStyle << "]"
                 << " (" << line.x1() << "pt, " << line.y1() * -1 << "pt) -- (" << line.x2()
                 << "pt, " << line.y2() * -1 << "pt);" << Qt::endl;
    }
}

void LatexEngine::drawLines(const QLine *lines, int lineCount)
{
    for (int i = 0; i < lineCount; i++) {
        QLine line = lines[i];
        checkSB(line.p1());
        checkSB(line.p2());

        *fStream << "\\draw"
                 << "[" << lineStyle << "]"
                 << " (" << line.x1() << "pt, " << line.y1() * -1 << "pt) -- (" << line.x2()
                 << "pt, " << line.y2() * -1 << "pt);" << Qt::endl;
    }
}

// enum ElementType {
//     MoveToElement,       0
//     LineToElement,       1
//     CurveToElement,      2
//     CurveToDataElement   3
// };

void LatexEngine::drawPath(const QPainterPath &path)
{
    *fStream << "\\draw"
             << "[" << lineStyle;

    if (path.elementAt(0).x == path.elementAt(path.elementCount() - 1).x
        && path.elementAt(0).y == path.elementAt(path.elementCount() - 1).y) {
        *fStream << fillStyle;
    }

    *fStream << "]";
    QPointF last(0, 0);
    QPointF first = path.elementAt(0);
    QPointF nullP;
    QPointF pA = nullP;
    QPointF pB = nullP;
    QPointF pC = nullP;
    for (int i = 0; i < path.elementCount(); i++) {
        checkSB(path.elementAt(i));
        QLineF line(last, path.elementAt(i));
        switch (path.elementAt(i).type) {
        case QPainterPath::MoveToElement:
            if (i != 0) {
                *fStream << " .. ";
            }
            *fStream << "(" << path.elementAt(i).x << "pt, " << path.elementAt(i).y * -1 << "pt)";
            last = path.elementAt(i);
            break;
        case QPainterPath::LineToElement:
            if (i != 0) {
                *fStream << " -- ";
            }
            *fStream << "(" << path.elementAt(i).x << "pt, " << path.elementAt(i).y * -1 << "pt)";
            last = path.elementAt(i);
            break;
        case QPainterPath::CurveToElement:
            // \draw (-2,2) .. controls (-1,0) and (1,0) .. (2,2); Bézier curve

            if (pC.isNull()) {
                pC = path.elementAt(i);
            }
            break;
        case QPainterPath::CurveToDataElement:
            if (pA.isNull()) {
                pA = path.elementAt(i);
            } else if (pB.isNull()) {
                pB = path.elementAt(i);
            }

            if (!pA.isNull() && !pB.isNull() && !pC.isNull()) {
                //                if (i != path.elementCount() - 1 || i == 0) {
                //                    *fStream << " -- ";
                //                }
                *fStream << " .. controls (" << pC.x() << "pt, " << pC.y() * -1 << "pt) and ("
                         << pA.x() << "pt, " << pA.y() * -1 << "pt) .. (" << pB.x() << "pt, "
                         << pB.y() * -1 << "pt)";
                pA = nullP;
                pB = nullP;
                pC = nullP;
                last = pC;
            }
            break;
        }
        //
    }
    if (first == last) {
        *fStream << " -- cycle;" << Qt::endl;
    } else {
        *fStream << ";" << Qt::endl;
    }
}

void LatexEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    checkSB(r.topLeft());
    checkSB(r.topRight());
    checkSB(r.bottomLeft());
    checkSB(r.bottomRight());

    qDebug() << "drawPixmap";
    *fStream << "drawPixmap" << Qt::endl;
}

void LatexEngine::drawPoints(const QPointF *points, int pointCount)
{
    for (int i = 0; i < pointCount; i++) {
        QPointF point = points[i];
        checkSB(point);

        *fStream << "\\filldraw[" << lineStyle << fillStyle << "] (" << point.x() << "pt, "
                 << point.y() * -1 << "pt) circle (2pt);" << Qt::endl;
    }
}

void LatexEngine::drawPoints(const QPoint *points, int pointCount)
{
    for (int i = 0; i < pointCount; i++) {
        QPoint point = points[i];
        checkSB(point);

        *fStream << "\\filldraw[" << lineStyle << fillStyle << "] (" << point.x() << "pt, "
                 << point.y() * -1 << "pt) circle (2pt);" << Qt::endl;
    }
}

void LatexEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    *fStream << "\\draw[" << lineStyle;
    if (mode != QPaintEngine::PolylineMode) {
        *fStream << fillStyle;
        //        qDebug() << "fillStyle: " << fillStyle;
    }
    *fStream << "]";
    for (int i = 0; i < pointCount; i++) {
        QPointF point = points[i];
        checkSB(point);

        *fStream << " (" << point.x() << "pt, " << point.y() * -1 << "pt)";
        if (!(i == pointCount - 1 && mode == QPaintEngine::PolylineMode)) {
            *fStream << " --";
        }
    }
    if (mode != QPaintEngine::PolylineMode) {
        *fStream << " cycle;" << Qt::endl;
    } else {
        // fStream->seek(fStream->pos() - 3);
        *fStream << ";" << Qt::endl;
    }
}

void LatexEngine::drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
{
    *fStream << "\\draw[" << lineStyle;
    if (mode != QPaintEngine::PolylineMode) {
        *fStream << fillStyle;
        qDebug() << "fillStyle";
    }
    *fStream << "]";

    for (int i = 0; i < pointCount; i++) {
        QPointF point = points[i];
        checkSB(point);

        *fStream << " (" << point.x() << "pt, " << point.y() * -1 << "pt)";
        if (!(i == pointCount - 1 && mode == QPaintEngine::PolylineMode)) {
            *fStream << " --";
        }
    }
    if (mode != QPaintEngine::PolylineMode) {
        *fStream << " cycle;" << Qt::endl;
    } else {
        // fStream->seek(fStream->pos() - 3);
        *fStream << ";" << Qt::endl;
    }
}

void LatexEngine::drawRects(const QRectF *rects, int rectCount)
{
    for (int i = 0; i < rectCount; i++) {
        QRectF rect = rects[i];
        checkSB(rect.topLeft());
        checkSB(rect.topRight());
        checkSB(rect.bottomLeft());
        checkSB(rect.bottomRight());

        *fStream << "\\draw"
                 << "[" << lineStyle << fillStyle << "]"
                 << " (" << rect.left() << "pt, " << rect.top() * -1 << "pt) rectangle ("
                 << rect.right() << "pt, " << rect.bottom() * -1 << "pt);" << Qt::endl;
    }
}

void LatexEngine::drawRects(const QRect *rects, int rectCount)
{
    for (int i = 0; i < rectCount; i++) {
        QRect rect = rects[i];
        checkSB(rect.topLeft());
        checkSB(rect.topRight());
        checkSB(rect.bottomLeft());
        checkSB(rect.bottomRight());

        *fStream << "\\draw"
                 << "[" << lineStyle << fillStyle << "]"
                 << " (" << rect.left() << "pt, " << rect.top() * -1 << "pt) rectangle ("
                 << rect.right() << "pt, " << rect.bottom() * -1 << "pt);" << Qt::endl;
    }
}

void LatexEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    checkSB(p); // Text length not included
    // Font and co ar missing
    *fStream << "\\draw"
             << "[" << lineStyle << "]"
             << " (" << p.x() << "pt ," << p.y() * -1 << "pt) node {" << textItem.text() << "};"
             << Qt::endl;
}

void LatexEngine::drawTiledPixmap(const QRectF &rect, const QPixmap &pixmap, const QPointF &p)
{
    checkSB(rect.topLeft());
    checkSB(rect.topRight());
    checkSB(rect.bottomLeft());
    checkSB(rect.bottomRight());

    qDebug() << "drawTiledPixmap";
    *fStream << "drawTiledPixmap" << Qt::endl;
}

QPaintEngine::Type LatexEngine::type() const
{
    return QPaintEngine::User;
}

void LatexEngine::updateState(const QPaintEngineState &state)
{ // pen is done

    translate_PenAndBrush(state.pen(), state.brush());

    qDebug() << "updateState";
}
