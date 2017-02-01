#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThread>

#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

void ClickCallbackFunction(vtkObject *caller, long unsigned eventID, void* clientData, void* callData);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void runVtkWindow();

	void initVTK();

protected:
	void mouseMoveEvent(QMouseEvent *eventMove);

private:
    Ui::MainWindow *ui;

	
};

#endif // MAINWINDOW_H
