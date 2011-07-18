#include "NaViewer.h"

void NaViewer::synchronizeWithCameraModel(CameraModel* externalCamera)
{
    // Two-way connection for all data members
    QObject::connect(&cameraModel, SIGNAL(scaleChanged(qreal)),
            externalCamera, SLOT(setScale(qreal)));
    QObject::connect(externalCamera, SIGNAL(scaleChanged(qreal)),
            &cameraModel, SLOT(setScale(qreal)));
    QObject::connect(&cameraModel, SIGNAL(focusChanged(const Vector3D&)),
            externalCamera, SLOT(setFocus(const Vector3D&)));
    QObject::connect(externalCamera, SIGNAL(focusChanged(const Vector3D&)),
            &cameraModel, SLOT(setFocus(const Vector3D&)));
    QObject::connect(&cameraModel, SIGNAL(rotationChanged(const Rotation3D&)),
            externalCamera, SLOT(setRotation(const Rotation3D&)));
    QObject::connect(externalCamera, SIGNAL(rotationChanged(const Rotation3D&)),
            &cameraModel, SLOT(setRotation(const Rotation3D&)));
}


void NaViewer::annotationModelUpdate(QString updateType)
{
    QList<QString> list=updateType.split(QRegExp("\\s+"));

    if (updateType.startsWith("NEURONMASK_UPDATE")) {
        QString indexString=list.at(1);
        QString checkedString=list.at(2);
        int index=indexString.toInt();
        bool checked=(checkedString.toInt()==1);
        toggleNeuronDisplay(index, checked);
    }
    else if (updateType.startsWith("FULL_UPDATE")) {
        updateFullVolume();
    }
}


void NaViewer::decoupleCameraModel(CameraModel* externalCamera)
{
    cameraModel.disconnect(externalCamera);
    externalCamera->disconnect(&cameraModel);
}

void NaViewer::setAnnotationSession(AnnotationSession* annotationSession) {
       this->annotationSession=annotationSession;
}

void NaViewer::wheelZoom(double delta)
{
    bMouseIsDragging = false;
    double numDegrees = delta/8.0;
    double numTicks = numDegrees/15.0;
    if (numTicks == 0) return;
    // Though internet map services generally zoom IN when scrolling UP,
    // 3D viewers generally zoom OUT when scrolling UP.  So we zoom out.
    double factor = std::pow(1.10, -numTicks);
    cameraModel.setScale(cameraModel.scale() * factor);
}

