/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Guenter Schwann <guenter.schwann@canonical.com>
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

#ifndef ADVANCEDCAMERASETTINGS_H
#define ADVANCEDCAMERASETTINGS_H

#include <QObject>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoDeviceSelectorControl>
#include <QtMultimedia/QCameraViewfinderSettingsControl>
#include <QtMultimedia/QCameraExposureControl>
#include <QtMultimedia/QMediaControl>

class QCameraControl;
class QCameraFlashControl;

class AdvancedCameraSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QObject* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY (int activeCameraIndex READ activeCameraIndex WRITE setActiveCameraIndex
                NOTIFY activeCameraIndexChanged)
    Q_PROPERTY (QSize resolution READ resolution NOTIFY resolutionChanged)
    Q_PROPERTY (bool hasFlash READ hasFlash NOTIFY hasFlashChanged)
    Q_PROPERTY (bool hdrEnabled READ hdrEnabled WRITE setHdrEnabled NOTIFY hdrEnabledChanged)
    Q_PROPERTY (bool hasHdr READ hasHdr NOTIFY hasHdrChanged)

public:
    explicit AdvancedCameraSettings(QObject *parent = 0);
    QObject* camera() const;
    int activeCameraIndex() const;
    void setCamera(QObject* camera);
    void setActiveCameraIndex(int index);
    QSize resolution() const;
    bool hasFlash() const;
    bool hasHdr() const;
    bool hdrEnabled() const;
    void setHdrEnabled(bool enabled);
    void readCapabilities();

Q_SIGNALS:
    void cameraChanged();
    void activeCameraIndexChanged();
    void resolutionChanged();
    void hasFlashChanged();
    void hasHdrChanged();
    void hdrEnabledChanged();

private Q_SLOTS:
    void onCameraStateChanged();
    void onExposureValueChanged(int parameter);

private:
    QVideoDeviceSelectorControl* selectorFromCamera(QCamera *camera) const;
    QCameraViewfinderSettingsControl* viewfinderFromCamera(QCamera *camera) const;
    QCameraControl *camcontrolFromCamera(QCamera *camera) const;
    QCameraFlashControl* flashControlFromCamera(QCamera* camera) const;
    QCameraExposureControl* exposureControlFromCamera(QCamera *camera) const;
    QCamera* cameraFromCameraObject(QObject* cameraObject) const;
    QMediaControl* mediaControlFromCamera(QCamera *camera, const char* iid) const;

    QObject* m_cameraObject;
    QCamera* m_camera;
    QVideoDeviceSelectorControl* m_deviceSelector;
    int m_activeCameraIndex;
    QCameraViewfinderSettingsControl* m_viewFinderControl;
    QCameraControl* m_cameraControl;
    QCameraFlashControl* m_cameraFlashControl;
    QCameraExposureControl* m_cameraExposureControl;

};

#endif // ADVANCEDCAMERASETTINGS_H
