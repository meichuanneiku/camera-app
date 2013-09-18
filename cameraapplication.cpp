/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Ugo Riboni <ugo.riboni@canonical.com>
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

#include "cameraapplication.h"

#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QLibrary>
#include <QDate>
#include <QQmlContext>
#include <QQmlEngine>
#include <QScreen>

#include "config.h"

const QString APP_ID = QString("camera-app");
const QString PHOTO_STATISTICS_ID = QString("camera-photos");
const QString VIDEO_STATISTICS_ID = QString("camera-videos");

using namespace UserMetricsInput;

static void printUsage(const QStringList& arguments)
{
    qDebug() << "usage:"
             << arguments.at(0).toUtf8().constData()
             << "[-testability]";
}

CameraApplication::CameraApplication(int &argc, char **argv)
    : QGuiApplication(argc, argv), m_metricManager(MetricManager::getInstance())
{

    // The testability driver is only loaded by QApplication but not by QGuiApplication.
    // However, QApplication depends on QWidget which would add some unneeded overhead => Let's load the testability driver on our own.
    if (arguments().contains(QLatin1String("-testability"))) {
        QLibrary testLib(QLatin1String("qttestability"));
        if (testLib.load()) {
            typedef void (*TasInitialize)(void);
            TasInitialize initFunction = (TasInitialize)testLib.resolve("qt_testability_init");
            if (initFunction) {
                initFunction();
            } else {
                qCritical("Library qttestability resolve failed!");
            }
        } else {
            qCritical("Library qttestability load failed!");
        }
    }

    m_photoMetric = m_metricManager->add(MetricParameters(PHOTO_STATISTICS_ID).formatString("<b>%1</b> photos taken today").emptyDataString(
                    "No photos taken today").textDomain(APP_ID).minimum(0.0));
    m_videoMetric = m_metricManager->add(MetricParameters(VIDEO_STATISTICS_ID).formatString("<b>%1</b> videos recorded today").emptyDataString(
                    "No videos recorded today").textDomain(APP_ID).minimum(0.0));
}

CameraApplication::~CameraApplication()
{
}

bool CameraApplication::setup()
{
    QGuiApplication::primaryScreen()->setOrientationUpdateMask(Qt::PortraitOrientation |
                Qt::LandscapeOrientation |
                Qt::InvertedPortraitOrientation |
                Qt::InvertedLandscapeOrientation);

    m_view.reset(new QQuickView());
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->setTitle("Camera");
    m_view->rootContext()->setContextProperty("application", this);
    m_view->engine()->setBaseUrl(QUrl::fromLocalFile(cameraAppDirectory()));
    QObject::connect(m_view->engine(), SIGNAL(quit()), this, SLOT(quit()));
    m_view->setSource(QUrl::fromLocalFile("camera-app.qml"));
    if (arguments().contains(QLatin1String("--fullscreen"))) m_view->showFullScreen();
    else m_view->show();

    return true;
}

/*!
 * \brief CameraApplication::increaseTodaysPhotoMetrics increase the number of
 * Photos taken pictures today in the MetricManager
 */
void CameraApplication::increaseTodaysPhotoMetrics()
{
    m_photoMetric->increment();
}

/*!
 * \brief CameraApplication::increaseTodaysVideoMetrics increase the number of
 * Videos taken pictures today in the MetricManager
 */
void CameraApplication::increaseTodaysVideoMetrics()
{
    m_videoMetric->increment();
}
