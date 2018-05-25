/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2018             */
/*                                                                      */
/*             FILE_NAME         :  PiHoleRecognize.h                   */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  GeomCheck                           */
/*             MODULE NAME       :  Check                               */
/*             LANGUAGE          :  C++                                 */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2018/03/19                          */
/*             DESCRIPTION       :  Compute the geom_info in the hole   */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2018/03/10
 *
 */

#ifndef PI_HOLE_RECOGNIZE_H
#define PI_HOLE_RECOGNIZE_H

#include "BaseGeom.h"
#include "BaseHole.h"
#include "BaseSlot.h"
#include "PiXCollectEx.h"
#include "ProEdge.h"

//Collect all holes (ignoring all combinations)
vector<BaseHole*> PiBaseHoleCollect(ProMdl p_mdl);

ProError HoleDepthCompute(IN BaseHole* hole, OUT double& depth);
ProError HoleDiameterCompute(IN BaseHole* hole, OUT double& diameter);
ProError ScrewDiameterCompute(IN BaseHole* hole, OUT double& base_diameter, OUT double& diameter);
ProError HoleDiameterCompute(IN BaseHole* hole_1s, IN BaseHole* hole_2nd, OUT double& diameter);
//ProError HoleDiameterCompute(IN BaseSlot* slot, OUT double& )

ProError HoleDepthCompute(IN CombHole* hole, OUT double& up_depth, OUT double& down_depth, OUT double& total_depth);
ProError HoleDiameterCompute(IN CombHole* hole, OUT double& up_diameter, OUT double& down_diameter);


#endif