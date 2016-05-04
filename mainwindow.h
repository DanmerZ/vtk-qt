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

namespace Ui {
class MainWindow;
}

class VtkThread;

class vtkMyCallback : public vtkCommand
{
public:
    static vtkMyCallback* New()
    {
        return new vtkMyCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        vtkRenderer *ren = reinterpret_cast<vtkRenderer*>(caller);
        qDebug() << "x: " << ren->GetActiveCamera()->GetPosition()[0]
                 << ",y: " << ren->GetActiveCamera()->GetPosition()[1]
                 << ",z: " << ren->GetActiveCamera()->GetPosition()[2];
    }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void runVtkWindow();
private:
    Ui::MainWindow *ui;

    VtkThread *vtkThread;

    vtkConeSource *cone;
    vtkPolyDataMapper *coneMapper;
    vtkActor *coneActor;
    vtkRenderer *ren1;
    vtkRenderWindow *renWin;
    vtkMyCallback *mo1;
    vtkRenderWindowInteractor *iren;
    vtkInteractorStyleTrackballCamera *style;

    double rotX;
    double rotY;
    double rotZ;
signals:

public slots:

private slots:
    void on_btnStartVtkWindow_clicked();
    void on_btnCloseVTKWindow_clicked();
    void on_xSlider_sliderMoved(int position);
    void on_ySlider_sliderMoved(int position);
    void on_zSlider_sliderMoved(int position);
    void on_xSlider_rangeChanged(int min, int max);
    void on_xSlider_valueChanged(int value);
    void on_ySlider_valueChanged(int value);
    void on_zSlider_valueChanged(int value);
};

class VtkThread : public QThread
{
    MainWindow *mainWindow;
public:
    VtkThread(VtkThread* newThread);
    VtkThread(MainWindow *w);
    void run();
};


#endif // MAINWINDOW_H
