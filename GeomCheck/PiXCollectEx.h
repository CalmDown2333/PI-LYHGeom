/************************************************************************/
/*           Copyright (C) www.pisx.com, cad@pisx.com, 2017             */
/*                                                                      */
/*             FILE_NAME         :  PiXCollectEx.h                      */
/*             PRINCIPAL AUTHOR  :  Xuhaoran                            */
/*             SUBSYSTEM NAME    :  Independent                         */
/*             MODULE NAME       :  StringOperate                       */
/*             LANGUAGE          :  C++                                 */
/*             TARGET ENVIRONMENT:  Win7 x64 Creo 3.0                   */
/*             DATE OF CREATED   :  2017/12/07                          */
/*             DESCRIPTION       :  Visit & collect C style             */
/************************************************************************/

/*
 *Revision log:
 *
 *Created by Xuhaoran, 2017/12/07
 *
 */

#ifndef PI_X_COLLECT_EX_H
#define PI_X_COLLECT_EX_H
#include "stdafx.h"
#include "ProSurface.h"

namespace PISX_xhr{

template<typename T>
ProError PiCollectVisitAction(T obj, ProError status, ProAppData app_data);

template<typename T>
ProError PiCollectVisitAction(T* p_obj, ProError status, ProAppData app_data);

ProError PiOneLevelAsmcomppathCollect(ProAssembly p_asm, ProAsmcomppath& r_path, int level, std::vector<ProAsmcomppath>& v_pathes);
ProError PiAsmcomppathCollect(ProAssembly p_asm, std::vector<ProAsmcomppath>& v_pathes);

ProError PiAsmcompCollect(ProAssembly p_asm, std::vector<ProAsmcomp>& v_asmcomps);

ProError PiAsmcompFilterAction(ProFeature *feature, ProAppData app_data);

ProError PiFeatureFilterAction(ProFeature* p_feature, ProAppData app_data);

//template<class Container>
//ProError PiSolidFeatCollect(ProSolid p_solid, Container<ProFeature>& v_p_feats);

ProError PiSolidFeatCollect(ProSolid p_solid, std::vector<ProFeature>& v_p_feats);
ProError PiSolidCsysCollect(ProSolid p_solid, std::vector<ProCsys>& v_csys);

ProError PiSolidSurfacesCollectWithFilter(ProSolid p_solid, std::vector<ProSurface>& v_srfs, ProSurfaceFilterAction filter);
ProError PiSolidSurfacesCollect(ProSolid p_solid, std::vector<ProSurface>& v_srfs);

ProError PiSurfaceContoursCollectWithFilter(ProSurface p_surface, std::vector<ProContour>& v_contours, ProSurfaceContourFilterAction filter);
ProError PiSurfaceContoursCollect(ProSurface p_surface, std::vector<ProContour>& v_contours);
ProError PiContourEdgesCollect(ProSurface p_srf, ProContour p_contour, std::vector<ProEdge>& v_edges);
ProError PiModelParamsCollect(ProModelitem* p_modelitem, std::vector<ProParameter>& v_p_parameters);

}
#endif