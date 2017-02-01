#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPlaneWidget.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkCallbackCommand.h>
#include <vtkPicker.h>
#include <vtk3DWidget.h>
#include <QMouseEvent>
#include <QScreen>

void ClickCallbackFunction(vtkObject *caller, long unsigned eventID, void* clientData, void* callData);

QLabel *vtkLabel;
QLabel *qtLabel;
QLabel *globalLabel;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setMouseTracking(true);

	vtkLabel = ui->label;
	qtLabel = ui->label_2;
	globalLabel = ui->label_3;	
}

void MainWindow::initVTK()
{
	//QTimer::singleShot(0, [=]() {
	vtkSphereSource *sphereSource = vtkSphereSource::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5.0);
	sphereSource->Update();

	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkActor *sphereActor = vtkActor::New();
	sphereActor->SetMapper(mapper);

	vtkRenderer *renderer = vtkRenderer::New();
	renderer->SetBackground(0.3, 0.4, 0.5);
	renderer->AddActor(sphereActor);

	vtkRenderWindow *renderWindow = ui->VTKWidget->GetRenderWindow();
	renderWindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = renderWindow->GetInteractor();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	vtkCallbackCommand *clickCallBack = vtkCallbackCommand::New();
	clickCallBack->SetCallback(ClickCallbackFunction);

	renderWindowInteractor->AddObserver(vtkCommand::MouseMoveEvent, clickCallBack);

	qDebug() << QApplication::screens().at(0)->logicalDotsPerInch();
	qDebug() << QApplication::screens().at(0)->physicalDotsPerInch();
	qDebug() << renderWindow->GetDPI();
	qDebug() << renderWindow->DetectDPI();
	//});
}

MainWindow::~MainWindow()
{
    delete ui;   
}

void MainWindow::mouseMoveEvent(QMouseEvent * eventMove)
{
	QString qtTxt = QString("Qt: %1, %2").arg(eventMove->x()).arg(eventMove->y());
	ui->label_2->setText(qtTxt);

	QString globalStr = QString("global: %1, %2").arg(QCursor::pos().x()).arg(QCursor::pos().y());
	ui->label_3->setText(globalStr);
}

void ClickCallbackFunction(vtkObject *caller, long unsigned eventID, void* clientData, void* callData)
{
	vtkRenderWindowInteractor *iren = static_cast<vtkRenderWindowInteractor*>(caller);
	QString vtkStr = QString("VTK: %1,%2").arg(iren->GetEventPosition()[0]).arg(iren->GetEventPosition()[1]);
	/*vtkStr.append(iren->GetEventPosition()[0]).append(",").append(iren->GetEventPosition()[1])
		.append(",").append(iren->GetEventPosition()[2]);*/
	vtkLabel->setText(vtkStr);
	
	double picked[3];
	iren->GetPicker()->GetPickPosition(picked);
	//cout << picked[0] << "," << picked[1] << "," << picked[2] << endl;

	QString globalStr = QString("global: %1, %2").arg(QCursor::pos().x()).arg(QCursor::pos().y());
	globalLabel->setText(globalStr);
}