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
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkRendererCollection.h>

#include "mainwindow.h"
#include <QApplication>

#include <iostream>

using namespace std;
void planeWidget();
void distanceWidget();
void mouseEvents();
void moveCubeIntoSphere();
void UserEvent();

class MouseInteractorStyle6 : public vtkInteractorStyleTrackballActor
{
public:
	static MouseInteractorStyle6 *New();
	vtkTypeMacro(MouseInteractorStyle6, vtkInteractorStyleTrackballActor);


	virtual void OnLeftButtonDown()
	{
		std::cout << "Pressed left mouse button." << std::endl;

		vtkInteractorStyleTrackballActor::OnLeftButtonDown();
	}

	virtual void OnMiddleButtonDown()
	{
		std::cout << "Pressed middle mouse button." << std::endl;
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		this->FindPokedRenderer(x, y);
		this->FindPickedActor(x, y);

		if (this->CurrentRenderer == NULL || this->InteractionProp == NULL)
		{
			std::cout << "Nothing picked." << std::endl;
			return;
		}

		vtkSmartPointer<vtkPropCollection> actors = vtkSmartPointer<vtkPropCollection>::New();

		this->InteractionProp->GetActors(actors);
		actors->InitTraversal();
		vtkActor *actor = vtkActor::SafeDownCast(actors->GetNextProp());

		vtkPolyData *polydata = vtkPolyData::SafeDownCast(actor->GetMapper()->GetInputAsDataSet());

		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		transform->SetMatrix(actor->GetMatrix());

		vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformPolyData->SetInputData(polydata);
		transformPolyData->SetTransform(transform);
		transformPolyData->Update();

		vtkSmartPointer<vtkSelectEnclosedPoints> selectEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
		selectEnclosedPoints->SetInputConnection(transformPolyData->GetOutputPort());
		selectEnclosedPoints->SetSurfaceData(this->Sphere);
		selectEnclosedPoints->Update();

		vtkDataArray *insideArray = vtkDataArray::SafeDownCast(selectEnclosedPoints->GetOutput()->GetPointData()->GetArray("SelectedPoints"));

		if (insideArray)
		{
			bool inside = false;
			for (vtkIdType i = 0; i < insideArray->GetNumberOfTuples(); i++)
			{
				if (insideArray->GetComponent(i, 0) == 1)
				{
					inside = true;
					break;
				}
			}

			if (inside)
			{
				std::cout << "a point of the cube is inside the sphere!" << std::endl;
				//this->CubeActor->SetPosition(0, 0, 0);
				//this->CubeActor->SetOrientation(0, 0, 0);
				this->CubeActor->GetMatrix()->Identity();

				this->Interactor->GetRenderWindow()->GetRenderers()->Render();
				this->Interactor->GetRenderWindow()->Render();
			}
			this->StopState();
		}
		vtkInteractorStyleTrackballActor::OnMiddleButtonDown();
	}

	virtual void OnRightButtonDown()
	{
		std::cout << "Pressed right mouse button." << std::endl;
		vtkInteractorStyleTrackballActor::OnRightButtonDown();
	}	

	vtkPolyData *Sphere;
	vtkActor *CubeActor;
};
vtkStandardNewMacro(MouseInteractorStyle6);




int main(int argc, char ** argv)
{
	//UserEvent();
	
	
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);	
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	a.exec();
}

void UserEvent()
{
	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renderWindow = vtkRenderWindow::New();
	renderWindow->AddRenderer(renderer);
	vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();


}

void moveCubeIntoSphere()
{
	vtkSphereSource *sphereSource = vtkSphereSource::New();
	sphereSource->SetRadius(2.0);
	sphereSource->Update();

	vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkActor *sphereActor = vtkActor::New();
	sphereActor->SetMapper(sphereMapper);

	vtkCubeSource *cubeSource = vtkCubeSource::New();
	cubeSource->SetCenter(5.0, 0.0, 0.0);
	cubeSource->Update();

	vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
	cubeMapper->SetInputConnection(cubeSource->GetOutputPort());

	vtkActor *cubeActor = vtkActor::New();
	cubeActor->SetMapper(cubeMapper);


	// visualize
	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renderWindow = vtkRenderWindow::New();
	renderWindow->SetSize(400, 400);
	renderWindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->AddActor(sphereActor);
	renderer->AddActor(cubeActor);
	renderWindow->Render();

	MouseInteractorStyle6 *style = MouseInteractorStyle6::New();
	style->Sphere = sphereSource->GetOutput();
	style->CubeActor = cubeActor;

	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->Start();
}

void mouseEvents()
{
	vtkSphereSource *sphereSource = vtkSphereSource::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5.0);
	sphereSource->Update();

	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkActor *sphereActor = vtkActor::New();
	sphereActor->SetMapper(mapper);

	vtkRenderer *renderer = vtkRenderer::New();
	renderer->SetBackground(1, 1, 1);
	renderer->AddActor(sphereActor);

	vtkRenderWindow *renderWindow = vtkRenderWindow::New();
	renderWindow->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	vtkCallbackCommand *clickCallBack = vtkCallbackCommand::New();
	//clickCallBack->SetCallback(ClickCallbackFunction);

	renderWindowInteractor->AddObserver(vtkCommand::MouseMoveEvent, clickCallBack);

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}

void distanceWidget()
{
	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renWin);

	vtkDistanceWidget *distanceWidget = vtkDistanceWidget::New();
	distanceWidget->SetInteractor(renderWindowInteractor);
	distanceWidget->CreateDefaultRepresentation();
	static_cast<vtkDistanceRepresentation*>(distanceWidget->GetRepresentation())
		->SetLabelFormat("%-#6.3g mm");

	renWin->Render();

	renderWindowInteractor->Initialize();
	renWin->Render();
	distanceWidget->On();

	renderWindowInteractor->Start();
}

void planeWidget()
{
	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *renderWindowInteractor = vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renWin);

	vtkPlaneWidget *planeWidget = vtkPlaneWidget::New();
	planeWidget->SetInteractor(renderWindowInteractor);

	planeWidget->On();

	renderWindowInteractor->Initialize();

	renderer->Render();
	renderWindowInteractor->Start();
}