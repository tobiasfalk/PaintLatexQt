#include "testpaint.h"

#include "paintlatexqt.h"

#include <QApplication>
#include <QPainter>
#include <QSvgGenerator>
#include <QPainterPath>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    testPaint w;
    // w.show();

    QSvgGenerator generatorSVG;
    generatorSVG.setFileName("./test.svg");
    generatorSVG.setSize(QSize(200, 200));
    generatorSVG.setViewBox(QRect(0, 0, 200, 200));
    generatorSVG.setTitle("SVG Generator Example Drawing");
    generatorSVG.setDescription("An SVG drawing created by the SVG Generator "
                                "Example provided with Qt.");

    LatexDevice deviceLatex;
    deviceLatex.setFileName("./test.tex");
    // deviceLatex.setPageSize(QSizeF(42.42, 200.200));
    deviceLatex.setCaption("Latex TikZ Generator Example Drawing");
    deviceLatex.setDescription("An Latex TikZ drawing created by the PaintLatex Generator "
                               "Example provided with Qt.");
    // deviceLatex.setIntegrationStyle(LatexIntegrationStyle::StandalonPackage);

    // qDebug() << "latex Size: " << deviceLatex.size();
    // qDebug() << "latex Pagesize: " << deviceLatex.pageSize();

    QPainter painter;
    // painter.begin(&generatorSVG);
    painter.begin(&deviceLatex);
    QColor color = Qt::blue;
    color.setAlphaF(0.9);
    painter.setPen(color);
    // painter.setPen(Qt::blue);
    painter.setBrush(Qt::magenta);
    //      painter.setFont(QFont("Arial", 30));
    painter.drawText(50, 50, "Qt");

    color = Qt::green;
    color.setAlphaF(.25);
    //  painter.setPen(color);
    //  painter.setFont(QFont("osifont", 42));

    QRectF rectangle(10.0, 20.0, 80.0, 60.0);
    int startAngle = 30 * 16;
    int spanAngle = 120 * 16;
    //    painter.drawArc(rectangle, startAngle, spanAngle); // #1

    //    painter.drawChord(rectangle, startAngle, spanAngle); // #2

    static const QPointF points[4] = { QPointF(10.0, 80.0), QPointF(20.0, 10.0),
                                       QPointF(80.0, 30.0), QPointF(90.0, 70.0) };
    //    painter.drawConvexPolygon(points, 4); // #3

    //    painter.drawEllipse(rectangle); // #4
    //    painter.drawEllipse(QRectF(0.0, 0.0, 80.0, 60.0)); // #5

    QLineF line(10.0, 80.0, 90.0, 20.0);
    //    painter.drawLine(line); // #6

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    //                   c1x c1y c2x c2y enx eny
    path.cubicTo(QPointF(80, 00), QPointF(50, 50), QPointF(80, 80));
    path.cubicTo(QPointF(1, 2), QPointF(3, 4), QPointF(5, 6));
    //    painter.drawPath(path); // #7

    painter.drawPie(rectangle, startAngle, spanAngle); // #8

    //    painter.drawPoint(0, 42); // #9

    //    painter.drawPolygon(points, 4); // #10

    static const QPointF points2[3] = {
        QPointF(10.0, 80.0),
        QPointF(20.0, 10.0),
        QPointF(80.0, 30.0),
    };
    painter.drawPolyline(points, 4); // #11

    painter.drawRect(rectangle); // #12
    //    painter.drawRect(QRectF(0.0, 0.0, 80.0, 60.0)); // #13

    //    painter.drawRoundedRect(rectangle, 20.0, 15.0); // #14

    // a.exit();

    qInfo() << "Done";
    a.exit();
    // return a.exec();
}
