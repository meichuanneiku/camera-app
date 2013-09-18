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

#ifndef CAMERAAPPLICATION_H
#define CAMERAAPPLICATION_H

#include <QtQuick/QQuickView>
#include <QGuiApplication>
#include <libusermetricsinput/MetricManager.h>

class QDate;

class CameraApplication : public QGuiApplication
{
    Q_OBJECT

public:
    CameraApplication(int &argc, char **argv);
    virtual ~CameraApplication();

    bool setup();

    Q_INVOKABLE void increaseTodaysPhotoMetrics();
    Q_INVOKABLE void increaseTodaysVideoMetrics();

private:
    QScopedPointer<QQuickView> m_view;

    UserMetricsInput::MetricManagerPtr m_metricManager;
    UserMetricsInput::MetricPtr m_photoMetric;
    UserMetricsInput::MetricPtr m_videoMetric;
};

#endif // CAMERAAPPLICATION_H
