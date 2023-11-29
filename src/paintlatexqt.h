#ifndef PAINTLATEXQT_H
#define PAINTLATEXQT_H

#include "PaintLatexQt_global.h"

#include <QPaintDevice>
#include <QPaintEngine>

#include <QPainterPath>

#include <QFile>
#include <QTextStream>

#include <QDebug>

// standart of QPaint is pixel wich is pt in latex
// https://www.overleaf.com/learn/latex/Errors/Illegal_unit_of_measure_(pt_inserted)
// https://doc.qt.io/qt-6/coordsys.html

// rgb color: {rgb,255:red,21; green,66; blue,128}
// https://tex.stackexchange.com/questions/24434/how-to-specify-a-fill-color-in-rgb-format-in-a-node-in-tikzpicture

///
/// \brief The LatexIntegrationStyle enum defines the different integration styles that can be
/// chosen(https://www.overleaf.com/learn/latex/Multi-file_LaTeX_projects)
enum class LatexIntegrationStyle {
    PureLatex /// only the picture code can be found in the file
              /// \b STANDARD \b
    ,
    SubfilesPackage /// uses the style of the package
                    /// subfiles(https://www.overleaf.com/learn/latex/Multi-file_LaTeX_projects#The_subfiles_package)\n
                    /// main: \code{.tex}\usepackage{subfiles}\endcode\n
                    /// sub: \code{.tex}\documentclass[<path to main>/main.tex]{subfiles}\endcode
    ,
    StandalonPackage /// uses the style od the package
                     /// standalone(https://www.overleaf.com/learn/latex/Multi-file_LaTeX_projects#The_standalone_package)\n
                     /// main:
                     /// \code{.tex}
                     ///    \usepackage[subpreambles=true]{standalone}
                     ///    \usepackage{import}
                     /// \endcode
                     /// sub:
                     /// \code{.tex}
                     ///    \documentclass[class=article, crop=false]{standalone}
                     ///    \usepackage[subpreambles=true]{standalone}
                     /// \endcode
    ,
};

///
/// \brief The LatexPageSizePackage enum is the package thet controles the page size, this is only
/// used if it is set as its own page(ownpage is true)
///
enum class LatexPageSizePackage {
    Typearea /// sets the pagesize with the typearea(https://www.ctan.org/pkg/typearea) package \n
             /// \code{.tex}\usepackage[paper=A4]{typearea}\endcode
             /// \b STANDARD \b
};

class PAINTLATEXQT_EXPORT LatexEngine : public QPaintEngine
{
private:
    QFile *file;
    QTextStream *fStream;

    // QSizeF pageSize; make own clase for whole page documents
    QString caption;
    QString lable;
    QString description;
    QString fileName;
    // bool ownpage;
    ///
    /// \brief figPositioning is the string for the positoning of the
    /// figure(https://www.overleaf.com/learn/latex/Inserting_Images#Positioning)
    ///
    QString figPositioning = "h";
    ///
    /// \brief topOptions is for things like "\\centering", witch can chage the apeareans and
    /// placemant of the \bold figure \endbold
    ///
    QString topOptions;
    ///
    /// \brief integrationStyle defines the style of integration of the document
    ///
    LatexIntegrationStyle integrationStyle = LatexIntegrationStyle::PureLatex;

    ///
    /// \brief sX is the smales X value given in a point
    ///
    qreal sX = 0;
    ///
    /// \brief sY is the smales Y value given in a point
    ///
    qreal sY = 0;

    ///
    /// \brief bX is the bigest X value given in a point
    ///
    qreal bX = 0;
    ///
    /// \brief bY is the bigest Y value given in a point
    ///
    qreal bY = 0;

    ///
    /// \brief checkSB chacks if a point expaneds the size of the picture
    /// \param point the point to be checked
    ///
    void checkSB(QPointF point);

    // QString styleStr;

    QString lineStyle;
    QString fillStyle;

    ///
    /// \brief top creates/writes the begining/top of the file, like the
    /// "\\begin{tikzpicture}"
    ///
    void top();
    ///
    /// \brief bottom creates/writes the end/bottom of the file, like the "\\end{tikzpicture}"
    ///
    void bottom();

    static void translate_color(const QColor &color, QString *color_string,
                                QString *opacity_string);
    static void translate_color(const QColor &color, QString *color_string);

    void translate_PenAndBrush(QPen pen, QBrush brush);

public:
    LatexEngine();
    ~LatexEngine();

    bool begin(QPaintDevice *pdev) override;
    bool end() override;

    qreal getWidth();
    qreal getHight();

    void drawEllipse(const QRectF &rect) override;
    void drawEllipse(const QRect &rect) override;
    void drawImage(const QRectF &rectangle, const QImage &image, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor) override;
    void drawLines(const QLineF *lines, int lineCount) override;
    void drawLines(const QLine *lines, int lineCount) override;
    void drawPath(const QPainterPath &path) override;
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override;
    void drawPoints(const QPointF *points, int pointCount) override;
    void drawPoints(const QPoint *points, int pointCount) override;
    void drawPolygon(const QPointF *points, int pointCount,
                     QPaintEngine::PolygonDrawMode mode) override;
    void drawPolygon(const QPoint *points, int pointCount,
                     QPaintEngine::PolygonDrawMode mode) override;
    void drawRects(const QRectF *rects, int rectCount) override;
    void drawRects(const QRect *rects, int rectCount) override;
    void drawTextItem(const QPointF &p, const QTextItem &textItem) override;
    void drawTiledPixmap(const QRectF &rect, const QPixmap &pixmap, const QPointF &p) override;

    QPaintEngine::Type type() const;
    void updateState(const QPaintEngineState &state) override;

    QSizeF getPageSize() const;
    void setPageSize(const QSizeF &newPageSize);

    QString getCaption() const;
    void setCaption(const QString &newCaption);

    QString getDescription() const;
    void setDescription(const QString &newDescription);

    QString getFileName() const;
    void setFileName(const QString &newFileName);

    //    bool getOwnpage() const;
    //    void setOwnpage(bool newOwnpage);

    QString getLable() const;
    void setLable(const QString &newLable);

    QString getFigPositioning() const;
    void setFigPositioning(const QString &newFigPositioning);

    QString getTopOptions() const;
    void setTopOptions(const QString &newTopOptions);

    LatexIntegrationStyle getIntegrationStyle() const;
    void setIntegrationStyle(LatexIntegrationStyle newIntegrationStyle);
};

class PAINTLATEXQT_EXPORT LatexDevice : public QPaintDevice
{
private:
    LatexEngine *latexPaintEngine;

    ///
    /// \brief pageSize
    ///
    // Q_PROPERTY(QSizeF pageSize READ pageSize WRITE setPageSize)
    Q_PROPERTY(QString caption READ caption WRITE setCaption)
    Q_PROPERTY(QString lable READ lable WRITE setLable)
    Q_PROPERTY(QString description READ description WRITE
                       setDescription) //\medskip (\n) \small (\n) Here in a new line a long
                                       // description about the figure, in a smaller text
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    // Q_PROPERTY(bool ownpage READ ownpage WRITE setOwnpage)
    ///
    /// \brief figPositioning is the string for the positoning of the
    /// figure(https://www.overleaf.com/learn/latex/Inserting_Images#Positioning)
    ///
    Q_PROPERTY(QString figPositioning READ figPositioning WRITE setFigPositioning)
    ///
    /// \brief topOptions is for things like "\\centering", witch can chage the apeareans and
    /// placemant of the \bold figure \endbold
    ///
    Q_PROPERTY(QString topOptions READ topOptions WRITE setTopOptions)
    ///
    /// \brief integrationStyle defines the style of integration of the document
    ///
    Q_PROPERTY(
            LatexIntegrationStyle integrationStyle READ integrationStyle WRITE setIntegrationStyle)

public:
    LatexDevice();
    ~LatexDevice();

    //    QSizeF pageSize() const;
    //    void setPageSize(const QSizeF &newPageSize);

    QString caption() const;
    void setCaption(const QString &newCaption);

    QString description() const;
    void setDescription(const QString &newDescription);

    QString fileName() const;
    void setFileName(const QString &newFileName);

    //    bool ownpage() const;
    //    void setOwnpage(bool newOwnpage);

    QString lable() const;
    void setLable(const QString &newLable);

    QString figPositioning() const;
    void setFigPositioning(const QString &newFigPositioning);

    QString topOptions() const;
    void setTopOptions(const QString &newTopOptions);

    LatexIntegrationStyle integrationStyle() const;
    void setIntegrationStyle(LatexIntegrationStyle newIntegrationStyle);

protected:
    ///
    /// \brief paintEngine
    /// \return
    ///
    LatexEngine *paintEngine() const;
    int metric(PaintDeviceMetric m) const;
};

#endif // PAINTLATEXQT_H
