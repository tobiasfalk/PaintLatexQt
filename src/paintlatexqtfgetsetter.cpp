#include "paintlatexqt.h"

// QSizeF LatexEngine::getPageSize() const
//{
//     return pageSize;
// }

// void LatexEngine::setPageSize(const QSizeF &newPageSize)
//{
//     pageSize = newPageSize;
// }

QString LatexEngine::getCaption() const
{
    return caption;
}

void LatexEngine::setCaption(const QString &newCaption)
{
    caption = newCaption;
}

QString LatexEngine::getDescription() const
{
    return description;
}

void LatexEngine::setDescription(const QString &newDescription)
{
    description = newDescription;
}

QString LatexEngine::getFileName() const
{
    return fileName;
}

void LatexEngine::setFileName(const QString &newFileName)
{
    fileName = newFileName;
}

// QSizeF LatexDevice::pageSize() const
//{
//     return latexPaintEngine->getPageSize();
// }

// void LatexDevice::setPageSize(const QSizeF &newPageSize)
//{
//     latexPaintEngine->setPageSize(newPageSize);
// }

QString LatexDevice::caption() const
{
    return latexPaintEngine->getCaption();
}

void LatexDevice::setCaption(const QString &newCaption)
{
    latexPaintEngine->setCaption(newCaption);
}

QString LatexDevice::description() const
{
    return latexPaintEngine->getDescription();
}

void LatexDevice::setDescription(const QString &newDescription)
{
    latexPaintEngine->setDescription(newDescription);
}

QString LatexDevice::fileName() const
{
    return latexPaintEngine->getFileName();
}

void LatexDevice::setFileName(const QString &newFileName)
{
    latexPaintEngine->setFileName(newFileName);
}

// bool LatexEngine::getOwnpage() const
//{
//     return ownpage;
// }

// void LatexEngine::setOwnpage(bool newOwnpage)
//{
//     ownpage = newOwnpage;
// }

// bool LatexDevice::ownpage() const
//{
//     return latexPaintEngine->getOwnpage();
// }

// void LatexDevice::setOwnpage(bool newOwnpage)
//{
//     latexPaintEngine->setOwnpage(newOwnpage);
// }

QString LatexEngine::getLable() const
{
    return lable;
}

void LatexEngine::setLable(const QString &newLable)
{
    lable = newLable;
}

QString LatexDevice::lable() const
{
    return latexPaintEngine->getLable();
}

void LatexDevice::setLable(const QString &newLable)
{
    latexPaintEngine->setLable(newLable);
}

QString LatexDevice::figPositioning() const
{
    return latexPaintEngine->getFigPositioning();
}

void LatexDevice::setFigPositioning(const QString &newFigPositioning)
{
    latexPaintEngine->setFigPositioning(newFigPositioning);
}

QString LatexEngine::getFigPositioning() const
{
    return figPositioning;
}

void LatexEngine::setFigPositioning(const QString &newFigPositioning)
{
    figPositioning = newFigPositioning;
}

QString LatexEngine::getTopOptions() const
{
    return topOptions;
}

void LatexEngine::setTopOptions(const QString &newTopOptions)
{
    topOptions = newTopOptions;
}

QString LatexDevice::topOptions() const
{
    return latexPaintEngine->getTopOptions();
}

void LatexDevice::setTopOptions(const QString &newTopOptions)
{
    latexPaintEngine->setTopOptions(newTopOptions);
}

LatexIntegrationStyle LatexEngine::getIntegrationStyle() const
{
    return integrationStyle;
}

void LatexEngine::setIntegrationStyle(LatexIntegrationStyle newIntegrationStyle)
{
    integrationStyle = newIntegrationStyle;
}

LatexIntegrationStyle LatexDevice::integrationStyle() const
{
    return latexPaintEngine->getIntegrationStyle();
}

void LatexDevice::setIntegrationStyle(LatexIntegrationStyle newIntegrationStyle)
{
    latexPaintEngine->setIntegrationStyle(newIntegrationStyle);
}
