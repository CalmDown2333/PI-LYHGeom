#include "stdafx.h"
#include "MoveToScreen.h"

void UserSolidToScreen(ProSolid solid,ProPoint3d solid_point,ProPoint3d screen_point)
{
	ProMatrix view_matrix;
	ProError status;
	status = ProViewMatrixGet(solid,NULL,view_matrix);
	status = ProPntTrfEval(solid_point,view_matrix,screen_point);
}
void MoveToScreenCenter(ProMdl p_mdl, ProPoint3d sp)
{
	ProError status;
	ProPoint3d screen_p;
	//ProPoint3d win_center;
	//ProUIRectangle size;
	//int x,y;

	ProMatrix matrix, zoom_matrix;
	int window_id;
	int j, k;
	double scale;
	double window_outline [2][3] = {{0.0, 0.0, 0.0}, {1000.0/*800.0*/ , 843.75/*300.0*/, 0.0}};

	UserSolidToScreen(ProSolid(p_mdl),sp,screen_p);

	/*---------------------------------------------------------------------*\
	Get the window pan-zoom transformation matrix
	\*---------------------------------------------------------------------*/
	ProWindowCurrentMatrixGet (matrix);

	/*---------------------------------------------------------------------*\
	Zoom in on the created point
	\*---------------------------------------------------------------------*/
	for (j=0; j<4; j++)
	{
		for (k=0; k<4; k++)
		zoom_matrix [j][k] = 0.0;
	}

	/*---------------------------------------------------------------------*\
	Double the existing window scale
	\*---------------------------------------------------------------------*/
	//scale = 2.0 * matrix [0][0];
	if (matrix[0][0]<2)
	{
		scale=2.0;
	}
	else
	{
		scale=matrix[0][0];
	}


	for (j=0; j<3; j++)
	{
		zoom_matrix [j][j] = scale;
		zoom_matrix [3][j] = 
		(window_outline [1][j] - window_outline [0][j])/2.0 -
		screen_p[j]*scale;
	}

	zoom_matrix [3][3] = 1.0;

	ProMdlWindowGet (p_mdl, &window_id);

	status = ProWindowPanZoomMatrixSet (window_id, zoom_matrix);

	/*---------------------------------------------------------------------*\
	Repaint the window
	\*---------------------------------------------------------------------*/
	ProWindowRepaint (window_id);

	//CHwMath::ProUtilVectorCopy(screen_p,circle_center);
	//circle_rad=10;
	//ShowSolidCircle();
}