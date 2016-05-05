#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    vtkThread = new VtkThread(this);

    cone = vtkConeSource::New();
    coneMapper = vtkPolyDataMapper::New();
    coneActor = vtkActor::New();
    ren1 = vtkRenderer::New();
    renWin = vtkRenderWindow::New();
    mo1 = vtkMyCallback::New();
    iren = vtkRenderWindowInteractor::New();
    style = vtkInteractorStyleTrackballCamera::New();

    rotX = 0.0;
    rotY = 0.0;
    rotZ = 0.0;
}

MainWindow::~MainWindow()
{
    delete ui;

    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
    ren1->Delete();
    renWin->Delete();
    mo1->Delete();
    iren->Delete();
    style->Delete();

    delete vtkThread;
}

void MainWindow::runVtkWindow()
{
    //cone = vtkConeSource::New();
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);

    //coneMapper = vtkPolyDataMapper::New();
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    //coneActor = vtkActor::New();
    coneActor->SetMapper( coneMapper );
    coneActor->GetProperty()->SetColor(0.3, 0.63, 0.79);
    coneActor->GetProperty()->SetDiffuse(0.7);
    coneActor->GetProperty()->SetSpecular(0.4);
    coneActor->GetProperty()->SetSpecularPower(20);

    coneActor->RotateX(rotX);
    coneActor->RotateY(rotY);
    coneActor->RotateZ(rotZ);

    //ren1 = vtkRenderer::New();
    ren1->AddActor(coneActor);
    ren1->SetBackground(0.1, 0.2, 0.4);
    ren1->SetViewport(0.0, 0.0, 1.0, 1.0);

    //renWin = vtkRenderWindow::New();
    renWin->AddRenderer(ren1);
    renWin->SetSize(300, 300);

    //mo1 = vtkMyCallback::New();
    ren1->AddObserver(vtkCommand::StartEvent, mo1);

    //iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);

    //style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren->SetInteractorStyle(style);

    iren->Initialize();
    iren->Start();
}

void MainWindow::on_btnStartVtkWindow_clicked()
{
    vtkThread->start();

    ui->xSlider->setEnabled(true);
    ui->ySlider->setEnabled(true);
    ui->zSlider->setEnabled(true);
}

void MainWindow::on_btnCloseVTKWindow_clicked()
{
   // renWin->FastDelete();
}

VtkThread::VtkThread(VtkThread *newThread)
{

}

VtkThread::VtkThread(MainWindow *w) : mainWindow(w) { }

void VtkThread::run()
{
    mainWindow->runVtkWindow();
}

void MainWindow::on_xSlider_sliderMoved(int position)
{

}

void MainWindow::on_ySlider_sliderMoved(int position)
{

}

void MainWindow::on_zSlider_sliderMoved(int position)
{

}

void MainWindow::on_xSlider_rangeChanged(int min, int max)
{

}

void MainWindow::on_xSlider_valueChanged(int value)
{
    //qDebug() << "x: " << ui->xSlider->value();
    rotX = ui->xSlider->value();
    coneActor->RotateX(rotX);
    iren->Render();
    ui->xLabel->setText(QString("Rotation around Y: %1").arg(coneActor->GetOrientation()[0]));
}

void MainWindow::on_ySlider_valueChanged(int value)
{
    //qDebug() << "y: " << ui->ySlider->value();
    rotY = ui->ySlider->value();
    coneActor->RotateY(rotY);
    iren->Render();
    ui->yLabel->setText(QString("Rotation around X: %1").arg(coneActor->GetOrientation()[1]));
}

void MainWindow::on_zSlider_valueChanged(int value)
{
    //qDebug() << "z: " << ui->zSlider->value();
    rotZ = ui->zSlider->value();
    coneActor->RotateZ(rotZ);
    iren->Render();
    ui->zLabel->setText(QString("Rotation around Z: %1").arg(coneActor->GetOrientation()[2]));
}
